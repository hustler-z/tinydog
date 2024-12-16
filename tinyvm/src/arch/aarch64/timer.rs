// @Hustler
//
// Self-Education Only

use core::sync::atomic::{AtomicUsize, Ordering};

use crate::arch::aarch64::regs::WriteableReg;
use crate::arch::{CNTHP_CTL_EL2, CNTHP_TVAL_EL2};
use tock_registers::interfaces::*;

const CTL_IMASK: usize = 1 << 1;

pub static TIMER_FREQ: AtomicUsize = AtomicUsize::new(0);
pub static TIMER_SLICE: AtomicUsize = AtomicUsize::new(0); // ms

/// Set expiration time to `num` ms later
pub fn timer_arch_set(num: usize) {
	let slice = TIMER_SLICE.load(Ordering::Relaxed);
	let val = slice * num;
	// SAFETY: Set timer value
	unsafe {
		CNTHP_TVAL_EL2::write(val);
	}
}

/// Enable timer interrupt
pub fn timer_arch_enable_irq() {
	// SAFETY: Enable[0] timer interrupt
	unsafe {
		CNTHP_CTL_EL2::write(1);
	}
}

/// Disable timer interrupt
pub fn timer_arch_disable_irq() {
	// SAFETY: MASK[1] timer interrupt
	unsafe {
		CNTHP_CTL_EL2::write(2);
	}
}

/// Get current counter value
pub fn timer_arch_get_counter() -> usize {
	cortex_a::registers::CNTPCT_EL0.get() as usize
}

/// Get timer frequency
pub fn timer_arch_get_frequency() -> usize {
	cortex_a::registers::CNTFRQ_EL0.get() as usize
}

/// timer init function for specific architecture
pub fn timer_arch_init() {
	let freq = timer_arch_get_frequency();
	let slice = freq / 1000;
	TIMER_FREQ.store(freq, Ordering::Relaxed);
	TIMER_SLICE.store(freq / 1000, Ordering::Relaxed);

	let ctl = 0x3 & (1 | !CTL_IMASK);
	let tval = slice * 10;
	// SAFETY:
	// Set timer value
	// Init timer with enable but mask
	unsafe {
		CNTHP_CTL_EL2::write(ctl);
		CNTHP_TVAL_EL2::write(tval);
	}
}
