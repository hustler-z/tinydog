// @Hustler's Project

#include <asm/asm.h>
#include <asm/offset.h>

#define MPIDR_AFFn          8
#define EL1                 0x04
#define EL2                 0x08
#define EL3                 0x0C

// SCTLR_EL2
#define MMU                 (1U << 0)
#define DCACHE              (1U << 2)
#define ICACHE              (1U << 12)

// HCR_EL2
#define AMO                 (1U << 5)
#define IMO                 (1U << 4)
#define FMO                 (1U << 3)

// ####################################################

    __HEAD

FUNC(__head__)
    add   x13, x18, #0x16
    b     __entry__
END(__head__)
    .long  0
    .quad  __tiny_end - __head__
    .quad  0
    .quad  0
    .quad  0
    .quad  0
    .ascii "ARM64"
    .long  0

    __ENTRY

__entry__:
    // disable SError, IRQ, FIQ, Sync exception
    msr   DAIFSet, #0x0F

    ldr   x0, =__head__
    adr   x19, __head__
    sub   x20, x19, x0

    // debug uart initialization
    bl    __asm_uart_init__

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

    DBG("[tiny] os not on el2!!\r\n")

2:  wfe
    b     2b

1:
    // disable MMU, dcache, icache
    mrs   x1, SCTLR_EL2
    bic   x1, x1, #MMU
    bic   x1, x1, #ICACHE
    bic   x1, x1, #DCACHE
    msr   SCTLR_EL2, x1

    DBG("[tiny] disable MMU,D-cache,I-cache\r\n")

    // invalidate data cache
    mov   x0, #0
    bl    cache_invalidate

    // set up exception table
    ldr   x1, =exception_vector
    msr   VBAR_EL2, x1

    mrs   x0, HCR_EL2
    orr   x0, x0, #AMO
    orr   x0, x0, #IMO
    orr   x0, x0, #FMO
    msr   HCR_EL2, x0

    // enable SError, IRQ, FIQ, Sync exceptions
    msr   DAIFClr, #0x0F

    // clear bss
    bl    bss_clear

    // set up the stack
    ldr   x1, =boot_cpu
    add   x1, x1, #STACK
    ldr   x1, [x1]
    mov   sp, x1

    mov   x0, x20
    b     bootc

// ####################################################

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
    DBG("[tiny] clear the bss section\r\n")

    adr_l x0, __bss_start
    adr_l x1, __bss_end
1:  str   xzr, [x0], #8
    cmp   x0, x1
    b.lt  1b
    ret
END(bss_clear)

// ####################################################
