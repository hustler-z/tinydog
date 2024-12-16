// @Hustler
//
// Self-Education Only

use alloc::sync::{Arc, Weak};
use core::ops::Range;
use spin::Mutex;

use crate::arch::PAGE_SIZE;
use crate::arch::PTE_S2_NORMAL;
use crate::device::{EmuContext, EmuDev};
use crate::kernel::{
	active_vm, current_cpu, mem_pages_alloc, vm_if_set_ivc_arg,
	vm_if_set_ivc_arg_ptr, vm_ipa2pa, Vm, VM_NUM_MAX,
};
use crate::mm::PageFrame;

// todo: need to rewrite for more vm
pub static SHARED_MEM: Mutex<Option<PageFrame>> = Mutex::new(None);
pub const SHARED_MEM_SIZE_MAX: usize = 0x200000;

/// Inter-VM Call shared memory update
pub fn ivc_update_mq(receive_ipa: usize, cfg_ipa: usize) -> bool {
	let vm = active_vm().unwrap();
	let vm_id = vm.id();
	let receive_pa = vm_ipa2pa(&vm, receive_ipa);
	let cfg_pa = vm_ipa2pa(&vm, cfg_ipa);

	if receive_pa == 0 {
		error!("invalid received pa");
		return false;
	}

	vm_if_set_ivc_arg(vm_id, cfg_pa);
	vm_if_set_ivc_arg_ptr(vm_id, cfg_pa - PAGE_SIZE / VM_NUM_MAX);

	let idx = 0;
	let val = vm_id;
	// TODO: The return value is set to vm_id, but is actually useless
	current_cpu().set_gpr(idx, val);
	// println!("VM {} update message", vm_id);
	true
}

/// init memory region shared by VM
pub fn mem_shared_mem_init() {
	let mut shared_mem = SHARED_MEM.lock();
	if shared_mem.is_none() {
		if let Ok(page_frame) = mem_pages_alloc(SHARED_MEM_SIZE_MAX / PAGE_SIZE)
		{
			*shared_mem = Some(page_frame);
		}
	}
}

pub fn tinyvm_init(
	vm: Weak<Vm>,
	base_ipa: usize,
	len: usize,
) -> Result<Arc<dyn EmuDev>, ()> {
	if base_ipa == 0 || len == 0 {
		return Ok(Arc::new(EmuTinyvm { base_ipa, len }));
	}
	let shared_mem = SHARED_MEM.lock();

	match &*shared_mem {
		Some(page_frame) => {
			vm.upgrade().unwrap().pt_map_range(
				base_ipa,
				len,
				page_frame.pa(),
				PTE_S2_NORMAL,
				true,
			);
			Ok(Arc::new(EmuTinyvm { base_ipa, len }))
		}
		None => {
			error!("shared memory shall not be none");
			Err(())
		}
	}
}

struct EmuTinyvm {
	base_ipa: usize,
	len: usize,
}

impl EmuDev for EmuTinyvm {
	fn emu_type(&self) -> crate::device::EmuDeviceType {
		crate::device::EmuDeviceType::EmuDeviceTTinyvm
	}

	fn address_range(&self) -> Range<usize> {
		0..0
	}

	fn handler(&self, emu_ctx: &EmuContext) -> bool {
		info!("emulate context ipa {:08x}", emu_ctx.address);
		info!("-- [DO NOTHING] --");
		true
	}
}
