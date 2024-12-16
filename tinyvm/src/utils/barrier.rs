// @Hustler
//
// Self-Education Only

use core::sync::atomic::AtomicUsize;
use core::sync::atomic::Ordering;

use super::round_up;
use crate::board::PLAT_DESC;

struct CpuSyncToken {
	n: usize,
	count: AtomicUsize,
}

static CPU_GLB_SYNC: CpuSyncToken = CpuSyncToken {
	n: PLAT_DESC.cpu_desc.num,
	count: AtomicUsize::new(0),
};

#[inline(never)]
/// Wait for all CPUs to reach the barrier.
pub fn barrier() {
	let ori = CPU_GLB_SYNC.count.fetch_add(1, Ordering::Release);
	let next_count = round_up(ori + 1, CPU_GLB_SYNC.n);
	while CPU_GLB_SYNC.count.load(Ordering::Acquire) < next_count {
		core::hint::spin_loop();
	}
}

pub fn reset_barrier() {
	CPU_GLB_SYNC.count.store(0, Ordering::Relaxed);
}
