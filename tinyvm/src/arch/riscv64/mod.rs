pub mod cache;
mod context_frame;
mod cpu;
mod exception;
pub mod interface;
pub mod interrupt;
mod page_fault;
mod page_table;
// test @CHonghao
#[cfg(feature = "aia")]
mod aplic;
#[cfg(feature = "aia")]
mod imsic;
#[cfg(feature = "plic")]
mod plic;
pub mod power;
pub mod regs;
mod sbicall;
mod smmu;
mod start;
pub mod timer;
pub mod tlb;
#[cfg(feature = "aia")]
mod vaplic;
mod vcpu;
mod vm;
#[cfg(feature = "plic")]
mod vplic;

use alloc::sync::Arc;
#[cfg(feature = "aia")]
pub use aplic::*;
pub use cache::*;
pub use context_frame::*;
pub use cpu::*;
#[cfg(feature = "aia")]
pub use imsic::*;
pub use interface::*;
pub use interrupt::*;
pub use page_fault::*;
pub use page_table::*;
#[cfg(feature = "plic")]
pub use plic::*;
pub use power::*;
pub use regs::*;
pub use sbicall::*;
pub use smmu::*;
pub use start::*;
pub use timer::*;
pub use tlb::*;
#[cfg(feature = "aia")]
pub use vaplic::*;
#[cfg(feature = "plic")]
pub use vplic::*;

/// TODO: fake implementations
pub struct GicDesc {
	pub gicd_addr: usize,
	pub gicc_addr: usize,
	pub gich_addr: usize,
	pub gicv_addr: usize,
	pub maintenance_int_id: usize,
}

pub struct SmmuDesc {
	pub base: usize,
	pub interrupt_id: usize,
	pub global_mask: u16,
}

#[repr(C)]
pub struct ArchDesc {
	pub gic_desc: GicDesc,
	pub smmu_desc: SmmuDesc,
}

pub const GIC_SGIS_NUM: usize = 16;

#[derive(Default)]
pub struct GicContext;

#[allow(unused_variables)]
pub fn gicc_clear_current_irq(for_hypervisor: bool) {
	IntCtrl::clear();
}

use crate::{config::VmEmulatedDeviceConfig, device::EmuDev};

use super::InterruptController;

#[allow(unused_variables)]
pub fn emu_smmu_init(
	emu_cfg: &VmEmulatedDeviceConfig,
) -> Result<Arc<dyn EmuDev>, ()> {
	todo!()
}
