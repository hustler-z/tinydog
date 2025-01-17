// @Hustler
//
// Self-Education Only

use crate::arch::ArchTrait;

pub const PAGE_SIZE: usize = 4096;
pub const PAGE_SHIFT: usize = 12;

/// The number of page table entries in a page.
pub const ENTRY_PER_PAGE: usize = PAGE_SIZE / 8;

pub type ContextFrame = super::context_frame::Aarch64ContextFrame;

pub const WORD_SIZE: usize = 8;
pub const PTE_PER_PAGE: usize = PAGE_SIZE / WORD_SIZE;

pub type Arch = Aarch64Arch;

pub struct Aarch64Arch;

impl ArchTrait for Aarch64Arch {
	fn wait_for_interrupt() {
		// SAFETY: Wait for interrupt
		crate::arch::wfi();
	}

	// Restore the MMU context of a VM Stage2 (typically set vmid)
	fn install_vm_page_table(base: usize, vmid: usize) {
		// restore vm's Stage2 MMU context
		let vttbr = (vmid << 48) | base;
		// SAFETY: 'vttbr' is saved in the vcpu struct when last scheduled
		unsafe {
			core::arch::asm!("msr VTTBR_EL2, {0}", in(reg) vttbr);
			crate::arch::isb();
		}
	}
}
