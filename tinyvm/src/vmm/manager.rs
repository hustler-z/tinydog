// @Hustler
//
// Self-Education Only

use alloc::sync::Arc;
use alloc::vec::Vec;

use crate::arch::{
    is_boot_core, power_arch_vm_shutdown_secondary_cores, IntCtrl, InterruptController,
};
use crate::config::{vm_id_list, vm_num};
// use crate::config::{init_tmp_config_for_bma1, init_tmp_config_for_bma2, init_tmp_config_for_vm1, init_tmp_config_for_vm2};
use crate::config::vm_cfg_entry;
use crate::config::vm_type;
use crate::config::NAME_MAX_LEN;
use crate::kernel::HVC_CONFIG;
use crate::kernel::HVC_CONFIG_UPLOAD_KERNEL_IMAGE;
use crate::kernel::HVC_VMM;
use crate::kernel::HVC_VMM_REBOOT_VM;
use crate::kernel::{
    active_vcpu_id, active_vm, current_cpu, push_vm, vm, vm_if_get_state, vm_if_set_ivc_arg,
    vm_if_set_ivc_arg_ptr, vm_if_set_type, vm_ipa2pa, Scheduler, Vm, VM_NUM_MAX,
};
use crate::kernel::{active_vm_id, vm_if_get_cpu_id};
use crate::kernel::{hvc_send_msg_to_vm, HvcGuestMsg, HvcManageMsg};
use crate::kernel::{ipi_send_msg, IpiInnerMsg, IpiMessage, IpiType, IpiVmmMsg};
use crate::utils::{bit_extract, memcpy, memset};
use crate::vmm::{
    vmm_assign_vcpu_percore, vmm_boot, vmm_cpu_remove_vcpu, vmm_init_image, vmm_setup_config,
};

#[derive(Copy, Clone)]
pub enum VmmEvent {
    VmmBoot,
    VmmReboot,
    VmmShutdown,
}

#[derive(Copy, Clone)]
pub enum VmmPercoreEvent {
    VmmAssignCpu,
    VmmRemoveCpu,
}

pub fn vmm_shutdown_secondary_vm() {
    info!("Shutting down all Vms...");
}

/* Generate VM structure and push it to VM.
 *
 * @param[in]  vm_id: new added VM id.
 */
pub fn vmm_push_vm(vm_id: usize) -> Result<Arc<Vm>, ()> {
    info!("Push Vm[{}] On Cpu {}", vm_id, current_cpu().id);
    let vm_cfg = match vm_cfg_entry(vm_id) {
        Some(vm_cfg) => vm_cfg,
        None => {
            error!("Failed To Find Config For Vm[{}]", vm_id);
            return Err(());
        }
    };
    vm_if_set_type(vm_id, vm_cfg.os_type);
    push_vm(vm_id, vm_cfg)
}

/* Init VM before boot.
 * Only VM0 will call this function.
 *
 * @param[in] vm_id: target VM id to boot.
 */
pub fn vmm_init_gvm(vm_id: usize) {
    // Before boot, we need to set up the VM config.
    if is_boot_core(current_cpu().id) || (active_vm_id() == 0 && active_vm_id() != vm_id) {
        if let Ok(vm) = vmm_push_vm(vm_id) {
            vmm_setup_config(vm);
        } else {
            error!("Failed To Push Vm[{}]", vm_id);
        }
    } else {
        error!(
            "Vm[{}] Core {} Shall Not Init Vm[{}]",
            active_vm_id(),
            current_cpu().id,
            vm_id
        );
    }
}

/* Boot Guest VM.
 *
 * @param[in] vm_id: target VM id to boot.
 */
pub fn vmm_boot_vm(vm_id: usize) {
    if let Some(phys_id) = vm_if_get_cpu_id(vm_id) {
        if phys_id != current_cpu().id {
            use crate::arch::psci_vm_maincpu_on;
            use crate::kernel::{CpuState, CPU_IF_LIST};
            let state = CPU_IF_LIST.lock().get(phys_id).unwrap().state_for_start;
            if state == CpuState::CpuInv {
                let vmpidr = vm(vm_id).unwrap().pcpuid_to_vcpuid(phys_id).unwrap();
                info!("Now Start Cpu On, VMPIDR={vmpidr}");
                psci_vm_maincpu_on(vmpidr, vmm_boot_vm as usize, 0, vm_id);
            } else {
                let m = IpiVmmMsg {
                    vmid: vm_id,
                    event: VmmEvent::VmmBoot,
                };
                if !ipi_send_msg(phys_id, IpiType::IpiTVMM, IpiInnerMsg::VmmMsg(m)) {
                    error!("Failed To Send IPI To Core {}", phys_id);
                }
            }
        } else {
            match current_cpu().vcpu_array.pop_vcpu_through_vmid(vm_id) {
                None => {
                    error!("Failed To Get vCpu For Vm[{}], It's Not Configured", vm_id);
                }
                Some(vcpu) => {
                    IntCtrl::clear_current_irq(true);
                    current_cpu().scheduler().yield_to(vcpu.clone());
                    vmm_boot();
                }
            }
        }
    } else {
        error!(
            "Failed To Get Cpu Id For Vm[{}], It's Not Configured",
            vm_id
        );
    }
}

/**
 * Reboot target vm according to arguments
 *
 * @param arg force ~ (31, 16) ~ [soft shutdown or hard shutdown]
 *            vmid ~ (15, 0) ~ [target vm id]
 */
pub fn vmm_reboot_vm(arg: usize) {
    let vm_id = bit_extract(arg, 0, 16);
    let force = bit_extract(arg, 16, 16) != 0;
    let cur_vm = active_vm().unwrap();

    info!("Vm[{}] Force:{}", vm_id, force);

    if force {
        if cur_vm.id() == vm_id {
            vmm_reboot();
        } else if let Some(cpu_trgt) = vm_if_get_cpu_id(vm_id) {
            let m = IpiVmmMsg {
                vmid: vm_id,
                event: VmmEvent::VmmReboot,
            };
            if !ipi_send_msg(cpu_trgt, IpiType::IpiTVMM, IpiInnerMsg::VmmMsg(m)) {
                error!("Failed To Send IPI To Core {}", cpu_trgt);
            }
        } else {
            error!("Failed To Get Cpu Id For Vm[{}]", vm_id);
        }
        return;
    }

    let msg = HvcManageMsg {
        fid: HVC_VMM,
        event: HVC_VMM_REBOOT_VM,
        vm_id,
    };
    if !hvc_send_msg_to_vm(vm_id, &HvcGuestMsg::Manage(msg)) {
        error!("Failed To Notify Vm 0");
    }
}

/* Reset vm os at current core.
 *
 * @param[in] vm : target VM structure to be reboot.
 */
pub fn vmm_reboot() {
    let vm = active_vm().unwrap();
    // If running MVM, reboot the whole system.
    if vm.id() == 0 {
        vmm_shutdown_secondary_vm();
        use crate::board::{PlatOperation, Platform};
        // SAFETY:
        // Here we are ready to reboot the system.
        unsafe {
            Platform::sys_reboot();
        }
    }

    // Reset GVM.
    let vcpu = current_cpu().active_vcpu.clone().unwrap();
    info!("VM [{}] Reset...", vm.id());
    power_arch_vm_shutdown_secondary_cores(&vm);
    info!(
        "Core {} (Vm [{}] vCpu {}) Shutdown Ok",
        current_cpu().id,
        vm.id(),
        active_vcpu_id()
    );

    // Clear memory region.
    for idx in 0..vm.mem_region_num() {
        info!(
            "Core {} (Vm [{}] vCpu {}) Reset Mem Region Start {:x} Size {:x}",
            current_cpu().id,
            vm.id(),
            active_vcpu_id(),
            vm.pa_start(idx),
            vm.pa_length(idx)
        );
        // SAFETY:
        // The 'vm_pa_region' is writable for the Hypervisor in EL2.
        unsafe {
            memset(vm.pa_start(idx) as *mut u8, 0, vm.pa_length(idx));
        }
    }

    // Reset image.
    if !vmm_init_image(&vm) {
        panic!("Init Image Failed");
    }

    // Reset ivc arg.
    vm_if_set_ivc_arg(vm.id(), 0);
    vm_if_set_ivc_arg_ptr(vm.id(), 0);

    IntCtrl::clear();
    vcpu.init(vm.config());
    vcpu.reset_context();

    vmm_load_image_from_mvm(&vm);
}

pub fn vmm_load_image_from_mvm(vm: &Vm) {
    let vm_id = vm.id();
    let msg = HvcManageMsg {
        fid: HVC_CONFIG,
        event: HVC_CONFIG_UPLOAD_KERNEL_IMAGE,
        vm_id,
    };
    // println!("mediated_blk_write send msg to vm0");
    if !hvc_send_msg_to_vm(0, &HvcGuestMsg::Manage(msg)) {
        error!("Failed To Notify Vm 0");
    }
}

/* Get current VM id.
 *
 * @param[in] id_ipa : vm id ipa.
 */
pub fn get_vm_id(id_ipa: usize) -> bool {
    let vm = active_vm().unwrap();
    let id_pa = vm_ipa2pa(&vm, id_ipa);
    if id_pa == 0 {
        error!("Illegal id_ipa {:x}", id_ipa);
        return false;
    }
    // SAFETY: The 'id_pa' is a valid address checked by tinyvm.ko
    unsafe {
        *(id_pa as *mut usize) = vm.id();
    }
    true
}

#[repr(C)]
struct VMInfo {
    pub id: u32,
    pub vm_name: [u8; NAME_MAX_LEN],
    pub vm_type: u32,
    pub vm_state: u32,
}

#[repr(C)]
struct VMInfoList {
    pub vm_num: usize,
    pub info_list: [VMInfo; VM_NUM_MAX],
}

/* List VM info in hypervisor.
 *
 * @param[in] vm_info_ipa : vm info list ipa.
 */
pub fn vmm_list_vm(vm_info_ipa: usize) -> Result<usize, ()> {
    let vm_info_pa = vm_ipa2pa(&active_vm().unwrap(), vm_info_ipa);
    if vm_info_pa == 0 {
        error!("Illegal vm_info_ipa {:x}", vm_info_ipa);
        return Err(());
    }

    // SAFETY:
    // The 'vm_info_pa' is a valid address checked by tinyvm.ko
    // And we have checked the vm_info_pa by vm_ipa2pa to make sure it is in range of vm memory.
    let vm_info = unsafe { &mut *(vm_info_pa as *mut VMInfoList) };

    // Get VM num.
    vm_info.vm_num = vm_num();

    for (idx, vmid) in vm_id_list().iter().enumerate() {
        let vm_cfg = match vm_cfg_entry(*vmid) {
            Some(vm_cfg) => vm_cfg,
            None => {
                error!("Failed To Get Vm Config Entry For Vm[{}]", *vmid);
                continue;
            }
        };
        // Get VM type.
        let vm_type = vm_type(*vmid);
        // Get VM State.
        let vm_state = vm_if_get_state(*vmid);

        vm_info.info_list[idx].id = *vmid as u32;
        vm_info.info_list[idx].vm_type = vm_type as u32;
        vm_info.info_list[idx].vm_state = vm_state as u32;

        let vm_name_u8: Vec<u8> = vm_cfg.vm_name().as_bytes().to_vec();
        // SAFETY:
        // We have both read and write access to the src and dst memory regions.
        // The copied size will not exceed the memory region.
        unsafe {
            memcpy(
                vm_info.info_list[idx].vm_name.as_ptr(),
                vm_name_u8.as_ptr(),
                NAME_MAX_LEN,
            );
        }
        vm_info.info_list[idx].vm_name[vm_name_u8.len()] = 0;
    }
    Ok(0)
}

pub fn vmm_ipi_handler(msg: IpiMessage) {
    info!("Core {} Received IPI", current_cpu().id);
    match msg.ipi_message {
        IpiInnerMsg::VmmMsg(vmm) => match vmm.event {
            VmmEvent::VmmBoot => {
                vmm_boot_vm(vmm.vmid);
            }
            VmmEvent::VmmReboot => {
                vmm_reboot();
            }
            _ => {
                todo!();
            }
        },
        IpiInnerMsg::VmmPercoreMsg(msg) => match msg.event {
            VmmPercoreEvent::VmmAssignCpu => {
                debug!(
                    "Core {} Receive Assigned vCpu Request For Vm[{}]",
                    current_cpu().id,
                    msg.vm.id()
                );
                vmm_assign_vcpu_percore(&msg.vm);
            }
            VmmPercoreEvent::VmmRemoveCpu => {
                debug!(
                    "Core {} Remove vCpu For Vm[{}]",
                    current_cpu().id,
                    msg.vm.id()
                );
                vmm_cpu_remove_vcpu(msg.vm.id());
            }
        },
        _ => {
            error!("Illegal IPI Type");
        }
    }
}
