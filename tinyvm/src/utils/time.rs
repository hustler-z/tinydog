// @Hustler
//
// Self-Education Only

use crate::arch::{timer_arch_get_counter, timer_arch_get_frequency};

/// Get current time in microseconds.
pub fn time_current_us() -> usize {
	let count = timer_arch_get_counter();
	let freq = timer_arch_get_frequency();
	count * 1000000 / freq
}

/// Get current time in milliseconds.
pub fn time_current_ms() -> usize {
	let count = timer_arch_get_counter();
	let freq = timer_arch_get_frequency();
	count * 1000 / freq
}

/// Sleep for `us` microseconds.
pub fn sleep(us: usize) {
	let end = time_current_us() + us;
	while time_current_us() < end {
		core::hint::spin_loop();
	}
}
