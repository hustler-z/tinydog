// @Hustler
//
// Self-Education Only

// rCore buddy system allocator
use buddy_system_allocator::{Heap, LockedHeapWithRescue};
use core::sync::atomic::{AtomicBool, Ordering};

use crate::{arch::PAGE_SIZE, board::PLAT_DESC, utils::round_up};

extern "C" {
	fn _image_end();
}
const HEAP_SIZE: usize = 1024 * PAGE_SIZE; // 4MB
static ALLOCED: AtomicBool = AtomicBool::new(false);

#[repr(align(4096))]
struct HeapRegion([u8; HEAP_SIZE]);

static mut HEAP_REGION: HeapRegion = HeapRegion([0; HEAP_SIZE]);

#[global_allocator]
/// Global heap allocator
static HEAP_ALLOCATOR: LockedHeapWithRescue<32> =
	LockedHeapWithRescue::new(heap_rescue);

/// Initialize heap allocator
pub fn heap_init() {
	// SAFEFY:
	// HEAP_REGION is aligned and HEAP_SIZE is a multiple of PAGE_SIZE
	unsafe {
		info!(
			"heap memory region @ 0x{:08x} with {} mb",
			HEAP_REGION.0.as_mut_ptr() as usize,
			HEAP_SIZE / (1024 * 1024)
		);
		HEAP_ALLOCATOR
			.lock()
			.init(HEAP_REGION.0.as_mut_ptr() as usize, HEAP_SIZE);
	}
}

fn heap_rescue(heap: &mut Heap<32>, layout: &core::alloc::Layout) {
	if !ALLOCED.fetch_or(true, Ordering::SeqCst) {
		// SAFETY:
		// The '_image_end' is the end of kernel image and we will use 'round_up'
		// to make it page aligned.
		// The new heap range is valid and not overlap with the old one.
		unsafe {
			heap.add_to_heap(
				round_up(_image_end as usize, PAGE_SIZE),
				PLAT_DESC.mem_desc.regions[0].size
					+ PLAT_DESC.mem_desc.regions[0].base,
			);
		}
	} else {
		panic!(
            "rescure failed! memoryRegion_0 has been exhausted\nout of memory: heap allocation error, layout = {:x?}",
            layout
        )
	}
}

#[alloc_error_handler]
fn alloc_error_handler(layout: core::alloc::Layout) -> ! {
	panic!(
		"out of memory: heap allocation error, layout = {:x?}",
		layout
	);
}
