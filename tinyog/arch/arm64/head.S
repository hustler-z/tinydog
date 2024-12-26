// @Hustler's Project

#include <asm/offset.h>
#include <asm/entry.h>
#include <asm/asm.h>

// ####################################################

#define MPIDR_AFF_OFFSET    8
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
    // disable SError, IRQ, FIQ, Sync exception
    msr   DAIFSet, #0x0F

    ldr   x0, =__head__
    adr   x19, __head__
    sub   x20, x19, x0

    // debug uart initialization
    bl    __asm_uart_init__

#if 1
    ldr   x0, =__DBG_UART_BA__
    mov   x1, 'X'
    str   x1, [x0]
    mov   x1, '\r'
    str   x1, [x0]
    mov   x1, '\n'
    str   x1, [x0]
#endif

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

    DBG("[tiny] not on el2\r\n")

    b     __boot_failed__
1:
    // disable MMU, dcache, icache
    mrs   x1, SCTLR_EL2
    bic   x1, x1, #MMU
    bic   x1, x1, #ICACHE
    bic   x1, x1, #DCACHE
    msr   SCTLR_EL2, x1

    DBG("[tiny] mmu,d-cache,i-cache disabled\r\n")

    // invalidate data cache
    mov   x0, #0
    bl    __cache_invalidate__

    // set up exception table
    ldr   x1, =__vector__
    msr   VBAR_EL2, x1

    // set up exception routing
    mrs   x0, HCR_EL2
    orr   x0, x0, #AMO
    orr   x0, x0, #IMO
    orr   x0, x0, #FMO
    msr   HCR_EL2, x0

    // boot on cpu0
    bl    __cpu_affinity__
    cbz   w4, __boot_primary_cpu__

    b     __boot_failed__
END(__head__)

// @Hustler
FUNC(__boot_primary_cpu__)
    // enable SError, IRQ, FIQ, Sync exceptions
    msr   DAIFClr, #0x0F

    // clear bss
    bl    __bss_zero__

    DBG("[tiny] start booting\r\n")

    // set up the stack
    ldr   x1, =boot_cpu
    add   x1, x1, #STACK
    ldr   x1, [x1]
    add   x1, x1, #STACK_SIZE
    mov   sp, x1

    mov   x0, x20
    b     bootc
END(__boot_primary_cpu__)

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
FUNC(__cpu_affinity__)
    // cpu affinity
    mrs   x4, MPIDR_EL1
    ubfx  x5, x4, #32, #MPIDR_AFF_OFFSET
    bfi   w4, w5, #24, #MPIDR_AFF_OFFSET
    ret
END(__cpu_affinity__)


// @Hustler
FUNC(__bss_zero__)
    DBG("[tiny] clear the bss\r\n")

    adr   x0, __bss_start
    adr   x1, __bss_end
1:  str   xzr, [x0], #8
    cmp   x0, x1
    b.lt  1b
    ret
END(__bss_zero__)

// @Hustler
FUNC(__boot_secondary_cpu__)
    nop
    nop
END(__boot_secondary_cpu__)

// @Hustler
FUNC(__boot_failed__)
    DBG("[tiny] boot cpu failed\r\n")
1:  wfi
    b     1b
END(__boot_failed__)

// ####################################################
