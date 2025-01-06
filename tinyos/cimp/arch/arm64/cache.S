// @Hustler's Project

#include <asm/asm.h>

/**
 * Note __cache_invalidate__
 *
 * Baremetal Boot Code for ARMv8-A Processors
 *
 * The content in cache RAM is invalid after reset.
 * ARMv8-A processors implement hardware that automatically
 * invalidates all cache RAMs after reset, so software
 * invalidation is unnecessary after reset. However, cleaning
 * and invalidating data cache is still necessary in some
 * situations, such as the core powerdown process.
 *
 * x0 - cache level
 */
FUNC(__dcache_invalidate__)
    msr   csselr_el1, x0
    mrs   x4, ccsidr_el1      // read cache size id.
    and   x1, x4, #0x7
    add   x1, x1, #0x4        // x1 = cache line size.
    ldr   x3, =0x7FFF
    and   x2, x3, x4, lsr #13 // x2 = cache set number - 1.
    ldr   x3, =0x3FF
    and   x3, x3, x4, lsr #3  // x3 = cache associativity number - 1.
    clz   w4, w3              // x4 = way position in the cisw instruction.
    mov   x5, #0              // x5 = way counter way_loop.
1:
    mov   x6, #0              // x6 = set counter set_loop.
2:
    lsl   x7, x5, x4
    orr   x7, x0, x7          // set way.
    lsl   x8, x6, x1
    orr   x7, x7, x8          // set set.
    dc    cisw, x7            // clean and invalidate cache line.
    add   x6, x6, #1          // increment set counter.
    cmp   x6, x2              // last set reached yet?
    ble   2b                  // if not, iterate set_loop,
    add   x5, x5, #1          // else, next way.
    cmp   x5, x3              // last way reached yet?
    ble   1b                  // if not, iterate way_loop

    ret
END(__dcache_invalidate__)

/**
 * Note dcache_line_size
 *
 * Get the minimum D-cache line size from the CTR register.
 */
.macro  dcache_line_size, reg, tmp
    mrs  \tmp, CTR_EL0        // read CTR
    ubfm \tmp, \tmp, #16, #19 // cache line size encoding
    mov  \reg, #4             // bytes per word
    lsl  \reg, \reg, \tmp     // actual cache line size
.endm

/**
 * Note __flush_dcache_area__(kaddr, size)
 *
 * Ensure that the data held in the page kaddr is written back to the
 * page in question.
 *
 * - kaddr   - kernel address
 * - size    - size in question
 */
FUNC(__flush_dcache_area__)
    dcache_line_size x2, x3
    add   x1, x0, x1
    sub   x3, x2, #1
    bic   x0, x0, x3
1:  dc    civac, x0           // clean & invalidate D line / unified line
    add   x0, x0, x2
    cmp   x0, x1
    blo   1b
    dsb   sy
    ret
END(__flush_dcache_area__)
