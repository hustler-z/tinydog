// @Hustler's Project

#include <asm/offset.h>
#include <asm/entry.h>
#include <asm/reg.h>
#include <asm/asm.h>

// ####################################################

    __HEAD

FUNC(__head__)
    add   x13, x18, #0x16
    b     __entry__
END(__head__)

FUNC(__entry__)
    // disable exceptions
    msr   DAIFSet, #(DAIFSET_FIQ_BIT | DAIFSET_IRQ_BIT | DAIFSET_ABT_BIT)

    ldr   x0, =__head__
    adr   x19, __head__
    sub   x20, x19, x0

    // debug uart initialization
    bl    __asm_uart_init__

    DBG("- booting tinyos\r\n")

    // we shall continue booting on cpu0
    bl    __cpu_affinity__
    cbz   w0, __boot_primary_cpu__

    // @Hustler
    b     __boot_secondary_cpu__
END(__entry__)

// primary cpu booting
FUNC(__boot_primary_cpu__)
    DBG("- booting on cpu-0\r\n")



    switch_el x0, 3f, 2f, 1f
3:  bl    __el3_entry__

2:  bl    __el2_entry__

1:  bl    __el1_entry__
END(__boot_primary_cpu__)

// entry function for exception level 3
FUNC(__el3_entry__)
    DBG("- booting from el3\r\n")

    // set up el3 exception vector table
    ldr   x0, =__el3_vector__
    msr   VBAR_EL3, x0

    ldr   w0, =(SCTLR_EL3_RES1 | SCTLR_I_BIT | SCTLR_SA_BIT)
    msr   SCTLR_EL3, x0

    mov   x0, #0
    bl    __dcache_invalidate__

    mrs   x0, CPTR_EL3
    bic   x0, x0, #CPTR_TFP_BIT
    bic   x0, x0, #CPTR_TTA_BIT
    bic   x0, x0, #CPTR_TCPAC_BIT
    msr   CPTR_EL3, x0

    // use AArch64 in EL2
    mrs   x0, SCR_EL3
    orr   x0, x0, #SCR_RW_BIT
    orr   x0, x0, #SCR_ST_BIT
    orr   x0, x0, #SCR_HCE_BIT
    orr   x0, x0, #SCR_SMD_BIT
    msr   SCR_EL3, x0

    isb

    // switch to EL2
    adr   x0, __el2_entry__
    msr   ELR_EL3, x0

    eret
END(__el3_entry__)

// entry function for exception level 2
FUNC(__el2_entry__)
    DBG("- booting from el2\r\n")

    // set up el2 exception vector table
    ldr   x0, =__el2_vector__
    msr   VBAR_EL2, x0

    // CPTR_EL2 - Architectural Feature Trap Register
    mrs   x0, CPTR_EL2
    bic   x0, x0, #CPTR_TFP_BIT
    bic   x0, x0, #CPTR_TCPAC_BIT
    msr   CPTR_EL2, x0

    isb
#ifdef __NORMAL_OS__
    DBG("- booting normal os\r\n")

    // initialize SCTLR_EL2
    msr   SCTLR_EL2, xzr

    mov   x0, #0
    bl    __dcache_invalidate__

    // use AArch64 in EL1, disable EL2 exception routing
    mov   x0, #HCR_RW_BIT
    msr   HCR_EL2, x0

    isb

    adr   x0, __el1_entry__
    msr   ELR_EL2, x0

    eret
#else
    DBG("- booting hypervisor\r\n")

    // HCR_EL2 - Hypervisor Configuration Register
    mrs   x0, HCR_EL2
    orr   x0, x0, #HCR_RW_BIT
    orr   x0, x0, #HCR_FMO_BIT
    orr   x0, x0, #HCR_IMO_BIT
    orr   x0, x0, #HCR_AMO_BIT
    msr   HCR_EL2, x0

    isb

    b     __bootc__
#endif
END(__el2_entry__)

// entry function for exception level 1
FUNC(__el1_entry__)
#ifdef __NORMAL_OS__
    DBG("- booting from el1\r\n")

    // set up el1 exception vector table
    ldr   x0, =__el1_vector__
    msr   VBAR_EL1, x0

    isb

    b     __bootc__
#else
    nop
#endif
END(__el1_entry__)

// @Hustler
FUNC(__bootc__)
    // enable all exceptions
    msr   DAIFClr, #(DAIFCLR_FIQ_BIT | DAIFCLR_IRQ_BIT | DAIFCLR_ABT_BIT)

    // Stack Pointer Select (SP_ELx)
    msr   SPSel, #1

    // zero out bss for C code when it's primary cpu
    bl    __cpu_affinity__
    cbz   w0, 1f

    // secondary cpu wait
    b     __active_wait__

1:  DBG("- set stack for C\r\n")

    // set up stack pointer
    ldr   x1, =__primary_cpu__ // @tiny/bsp/cpu.c
    add   x1, x1, #STACK_OFFSET
    ldr   x1, [x1]
    add   x1, x1, #STACK_SIZE
    sub   x1, x1, #CPU_SIZE
    mov   sp, x1

    DBG("- booting to C\r\n")

    // boot in C
    mov   x0, x20
    b     bootc
END(__bootc__)

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
    ubfx  x1, x0, #32, #MPIDR_AFF_OFFSET
    bfi   w0, w1, #24, #MPIDR_AFF_OFFSET
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
    DBG("[A] booting secondary cpu\r\n")

    b     __el2_entry__
END(__boot_secondary_cpu__)

// @Hustler
FUNC(__boot_hang__)
    DBG("- boot cpu hung\r\n")

1:  wfi
    b     1b
END(__boot_hang__)

FUNC(__active_wait__)
1:  wfe
    b     1b
END(__active_wait__)

// ####################################################
