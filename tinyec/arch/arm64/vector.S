// @Hustler's Project

#include <asm/asm.h>
#include <asm/offset.h>

// ##############################################################

/**
 * TODO
 *
 * save the context of current execution flow.
 *
 * --------------------------- high
 *    |
 *    : [struct context]
 *    |
 *    V
 * --------------------------- low
 */
.macro exception_entry
    dmb   sy

    sub   sp, sp, #(ELR - LR)
    push  x28, x29
    push  x26, x27
    push  x24, x25
    push  x22, x23
    push  x20, x21
    push  x18, x19
    push  x16, x17
    push  x14, x15
    push  x12, x13
    push  x10, x11
    push  x8, x9
    push  x6, x7
    push  x4, x5
    push  x2, x3
    push  x0, x1

    add   x21, sp, #ELR
    stp   lr, x21, [sp, #LR]
    mrs   x21, ELR_EL2
    str   x21, [sp, #PC]

    add   x21, sp, #CPSR
    mrs   x22, SPSR_EL2
    mrs   x23, ESR_EL2
    stp   x22, x23, [x21]
.endm

/**
 * TODO
 */
.macro exception_exit
    msr   DAIFSet, #0x02
    ldr   x21, [sp, #PC]
    ldr   x22, [sp, #CPSR]

    pop   x0, x1
    pop   x2, x3
    pop   x4, x5
    pop   x6, x7
    pop   x8, x9
    pop   x10, x11
    pop   x12, x13
    pop   x14, x15
    pop   x16, x17
    pop   x18, x19
    pop   x20, x21
    pop   x22, x23
    pop   x24, x25
    pop   x26, x27
    pop   x28, x29

    msr   ELR_EL2, x21
    msr   SPSR_EL2, x22

    ldr   lr, [sp], #(ELR - LR)

    dmb   sy

    eret
.endm

// ##############################################################

.macro ventry, label
    .align 7
\label:
    exception_entry
    b     \label\()_handler
    exception_exit
.endm

/**
 * Setting up exception vector table
 *
 * The vector table in AArch64 mode contains 16 entries. Each
 * entry is 128B in size and contains at most 32 instructions.
 * Vector tables must be placed at a 2KB-aligned address. The
 * addresses are specified by initializing VBAR_ELn registers.
 *
 * In Armv8-A, vector tables are an area of normal memory
 * containing instructions. The processor element (PE) holds
 * the base address of the table in a System register, and
 * each exception type has a defined offset from that base.
 *
 * VBAR_ELx
 */

    .align 11
GLOBAL(exception_vector)
    // Exception from the current EL while using SP_EL0
    ventry syn_sp0
    ventry irq_sp0
    ventry fiq_sp0
    ventry err_sp0

    // Exception from the current EL while using SP_ELx
    ventry syn_spx
    ventry irq_spx
    ventry fiq_spx
    ventry err_spx

    // Exception from a lower EL and at least one lower EL is AArch64
    ventry syn_low
    ventry irq_low
    ventry fiq_low
    ventry err_low

    // Exception from a lower EL and all lower ELs are AArch32
    ventry syn_opt
    ventry irq_opt
    ventry fiq_opt
    ventry err_opt
END(exception_vector)
