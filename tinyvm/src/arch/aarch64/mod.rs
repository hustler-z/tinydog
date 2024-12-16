// @Hustler
//
// Self-Education Only

pub use self::cache::*;
pub use self::context_frame::*;
pub use self::cpu::*;
pub use self::exception::*;
#[cfg(not(feature = "gicv3"))]
pub use self::gic::*;
#[cfg(feature = "gicv3")]
pub use self::gicv3::*;
pub use self::interface::*;
pub use self::interrupt::*;
pub use self::page_table::*;
pub use self::psci::*;
pub use self::regs::*;
pub use self::smc::*;
pub use self::smmu::*;
pub use self::start::*;
pub use self::sync::*;
pub use self::timer::*;
pub use self::tlb::*;
#[cfg(not(feature = "gicv3"))]
pub use self::vgic::*;
#[cfg(feature = "gicv3")]
pub use self::vgicv3::*;

#[macro_use]
mod regs;
pub mod cache;
mod context_frame;
mod cpu;
mod exception;
#[cfg(not(feature = "gicv3"))]
mod gic;
#[cfg(feature = "gicv3")]
mod gicv3;
mod interface;
mod interrupt;
mod mmu;
mod page_table;
mod psci;
mod smc;
mod smmu;
mod start;
mod sync;
mod timer;
pub mod tlb;
mod vcpu;
#[cfg(not(feature = "gicv3"))]
mod vgic;
#[cfg(feature = "gicv3")]
mod vgicv3;
mod vm;

#[repr(C)]
pub struct ArchDesc {
	pub gic_desc: GicDesc,
	pub smmu_desc: SmmuDesc,
}
