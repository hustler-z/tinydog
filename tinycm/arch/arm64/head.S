// @Hustler's Project

#include <asm/sysreg.h>
#include <asm/offset.h>
#include <asm/entry.h>
#include <asm/asm.h>

// ####################################################

#define CPU_AFF_OFFSET 8
#define EL1            0x04
#define EL2            0x08
#define EL3            0x0C

// SCTLR_EL2
#define MMU            (1U << 0)
#define DCACHE         (1U << 2)
#define ICACHE         (1U << 12)

// HCR_EL2
#define HCR_AMO        (1U << 5)
#define HCR_IMO        (1U << 4)
#define HCR_FMO        (1U << 3)
#define HCR_RW         (1U << 31) // The execution state for EL1 is AArch64

// SCR_EL3
#define SCR_NS         1U
#define SCR_RW         (1U << 10)

// ####################################################

    __HEAD

FUNC(__head__)
    add   x13, x18, #0x16
    b     __entry__
END(__head__)

FUNC(__entry__)
    // disable serror, irq, fiq, sync exceptions
    msr   DAIFSet, #0x0F

    ldr   x0, =__head__
    adr   x19, __head__
    sub   x20, x19, x0

    // debug uart initialization
    bl    __asm_uart_init__

    DBG("- booting on process\r\n")

    // we shall continue booting on cpu0
    bl    __cpu_affinity__
    cbz   w0, __boot_primary_cpu__

    // hang if not on cpu0
    b     __boot_hang__
END(__entry__)

// primary cpu booting
FUNC(__boot_primary_cpu__)
    mov   x0, #0
    bl    __cache_invalidate__

    mrs   x0, CurrentEL
    cmp   x0, #EL2
    beq   __el2_entry__

    // hang if not on EL2
    b     __boot_hang__
END(__boot_primary_cpu__)

// entry function for exception level 2
FUNC(__el2_entry__)
    DBG("- booting on el2\r\n")

    ldr   x1, __el2_vector__
    msr   VBAR_EL2, x1

    ldr   x0, =HSTR_MASK(15)
    msr   HSTR_EL2, x0

    // Architectural Feature Trap Register (EL2)
    // control trapping to EL2 of a accesses to CPACR
    // CPACR_EL1, trace, activity monitor, SVE, and
    // Advanced SIMD and floating-point functionality
    ldr   x0, =CPTR_EL2_INIT
    msr   CPTR_EL2, x0

    // HCR_EL2 - Hypervisor Configuration Register
    mrs   x0, HCR_EL2
    orr   x0, x0, #HCR_RW
    orr   x0, x0, #HCR_FMO
    orr   x0, x0, #HCR_IMO
    orr   x0, x0, #HCR_AMO
    msr   HCR_EL2, x0

    isb

    // @Hustler
    //
    // Interrupt controller System Register Enable
    // Register
    //
    // controls whether the System register interface
    // or the memory-mapped interface to the GIC CPU
    // interface is used for EL2.
    mov   x0, #1
    msr   ICC_SRE_EL2, x0

    isb

    adr   x0, __el1_entry__
    msr   ELR_EL2, x0

    eret
END(__el2_entry__)

// entry function for exception level 1
FUNC(__el1_entry__)
    DBG("- booting on el1\r\n")

    // set up el1 exception vector table
    ldr   x1, __el1_vector__
    msr   VBAR_EL1, x1

    isb

    // Interrupt controller System Register Enable
    // Register
    mov   x0, #1
    msr   ICC_SRE_EL1, x0

    isb

    // Stack Pointer Select (SP_EL0 and SP_ELx)
    msr   SPSel, #0

    msr   DAIFClr, #0x0F

    bl    __bss_zero__

    // set up stack pointer
    ldr   x1, =__primary_cpu__ // @tiny/bsp/cpu.c
    add   x1, x1, #STACK_OFFSET
    ldr   x1, [x1]
    add   x1, x1, #STACK_SIZE
    mov   sp, x1

    DBG("- booting in C\r\n")

    // boot in C
    mov   x0, x20
    b     bootc
END(__el1_entry__)

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
    mrs   x0, MPIDR_EL1
    ubfx  x1, x0, #32, #CPU_AFF_OFFSET
    bfi   w0, w1, #24, #CPU_AFF_OFFSET
    ret
END(__cpu_affinity__)

// @Hustler
FUNC(__bss_zero__)
    DBG("- zero bss section\r\n")

    adrl  x0, __bss_start
    adrl  x1, __bss_end
1:  str   xzr, [x0], #8
    cmp   x0, x1
    blt   1b
    ret
END(__bss_zero__)

// ####################################################

// @Hustler
FUNC(__boot_secondary_cpu__)
    nop
    nop
END(__boot_secondary_cpu__)

// @Hustler
FUNC(__boot_hang__)
    DBG("- boot cpu hung\r\n")

1:  wfi
    b     1b
END(__boot_hang__)

// ####################################################
