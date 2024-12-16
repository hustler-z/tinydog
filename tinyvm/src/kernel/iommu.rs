// @Hustler
//
// Self-Education Only
#![allow(unused_variables)]

use alloc::sync::Arc;

// use crate::arch::{smmu_add_device, smmu_vm_init};
use crate::config::VmEmulatedDeviceConfig;
use crate::device::EmuDev;
use crate::kernel::Vm;

/// init iommu
pub fn iommu_init() {
	warn!("platform not support iommu");
}

/// init iommu for vm
pub fn iommmu_vm_init(vm: &Vm) -> bool {
	warn!("platform not support iommu");

	false
}

/// add device to iommu
pub fn iommu_add_device(vm: &Vm, stream_id: usize) -> bool {
	warn!("platform not support iommu");

	false
}

/// init emu_iommu for vm
pub fn emu_iommu_init(
	emu_cfg: &VmEmulatedDeviceConfig,
) -> Result<Arc<dyn EmuDev>, ()> {
	Err(())
}
