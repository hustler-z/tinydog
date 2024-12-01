// @Hustler
//
// Self-Education Only

use core::arch::asm;

pub fn tlb_invalidate_guest_all() {
    // SAFETY:
    // Invalidate all guest tlb entries.
    unsafe {
        asm!("hfence.vvma", "hfence.gvma");
    }
}
