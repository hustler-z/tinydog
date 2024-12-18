// @Hustler's Project

#include <asm/asm.h>

#define MPIDR_AFFn          8
#define EL1                 0x04
#define EL2                 0x08
#define EL3                 0x0C

// SCTLR_EL2
#define MMU                 (1 << 0)
#define DCACHE              (1 << 2)
#define ICACHE              (1 << 12)

    __HEAD

FUNC(_head)
    add   x13, x18, #0x16
    b     _entry
END(_head)
    .long  0
    .quad  __tiny_end - _head
    .quad  0
    .quad  0
    .quad  0
    .quad  0
    .ascii "ARM64"
    .long  0

_entry:
    ldr   x0, =_head
    adr   x19, _head
    sub   x20, x19, x0

    // disable all exception
    msr   DAIFSet, #0x0F

    // debug uart initialization
    bl    _uart_init

    // @Hustler
    //
    // check current exception level
    //
    // CurrentEL [3:2]
    // ob0100    0x04      El1
    // 0b1000    0x08      El2
    // 0b1100    0x0c      El3
    mrs   x1, CurrentEL
    cmp   x1, #EL2
    b.eq  1f

    DBG("[tiny] not on el2!!\r\n")

2:  wfe
    b     2b

1:
    // disable MMU, dcache, icache
    mrs   x1, SCTLR_EL2
    bic   x1, x1, #MMU
    bic   x1, x1, #ICACHE
    bic   x1, x1, #DCACHE
    msr   SCTLR_EL2, x1





/**
 * @Hustler - bit manipulation
 *
 * ubfx  - unsigned bitfield extract
 *         ubfx <Wd>, <Wn>, #<lsb>, #<width>
 *
 *         lsb                     0
 * ----x----x----------------------+ Wn
 *      \    \
 *       \    +-----------------+
 *        +-----------------+    \
 *                           \    \
 * ---------------------------x----x Wd
 *                            |<-->| width
 *
 * bfi   - bitfield insert
 *         bfi  <Wd>, <Wn>, #<lsb>, #<width>
 *
 * ---------------------------x----x Wn
 *                           /    /
 *               +----------+    /
 *              /    +----------+
 *             /    /              0
 * -----------x----x---------------+ Wd
 *                lsb
 *
 * MPIDR_EL1 - Multiprocessor Affinity Register
 */
FUNC(cpu_affinity)
    // cpu affinity
    mrs   x4, MPIDR_EL1
    ubfx  x5, x4, #32, #MPIDR_AFFn
    bfi   w4, w5, #24, #MPIDR_AFFn
    ret
END(cpu_affinity)

FUNC(bss_clear)
    DBG("[tiny] clear the bss\r\n")

    adr_l x0, __bss_start
    adr_l x1, __bss_end
1:  str   xzr, [x0], #8
    cmp   x0, x1
    b.lt  1b
    ret
END(bss_clear)
