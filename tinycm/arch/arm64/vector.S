// @Hustler's Project

#include <asm/asm.h>
#include <asm/offset.h>

// ####################################################

    __VECS

    .equ  __el2, 0x01
    .equ  __el1, 0x00

/**
 * Note
 *
 * save the context of current execution flow.
 *
 * --------------------------- high
 *    |
 *    : [struct context] @asm/cpu.h
 *    |
 *    V
 * --------------------------- low
 */
.macro __exception_entry__, el
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

    .if \el == __el2
    mrs   x21, ELR_EL2
    .else
    mrs   x21, ELR_EL1
    .endif

    str   x21, [sp, #PC]

    add   x21, sp, #CPSR

    .if \el == __el2
    mrs   x22, SPSR_EL2
    mrs   x23, ESR_EL2
    .else
    mrs   x22, SPSR_EL1
    mrs   x23, ESR_EL1
    .endif

    stp   x22, x23, [x21]
.endm

/**
 * TODO
 */
.macro __exception_exit__, el
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

    .if \el == __el2
    msr   ELR_EL2, x21
    msr   SPSR_EL2, x22
    .else
    msr   ELR_EL1, x21
    msr   SPSR_EL1, x22
    .endif

    ldr   lr, [sp], #(ELR - LR)

    dmb   sy

    eret
.endm

// ####################################################

// KISS
.macro ventry, label, el
    .align 7
\label\()_\el:
    __exception_entry__ el=\el
    b     \label\()_handler
    __exception_exit__ el=\el
.endm

/**
 * Note setting up exception vector table
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
GLOBAL(__el2_vector__)
    // Exception from the current EL while using SP_EL0
    ventry syn_sp0, __el2
    ventry irq_sp0, __el2
    ventry fiq_sp0, __el2
    ventry err_sp0, __el2

    // Exception from the current EL while using SP_ELx
    ventry syn_spx, __el2
    ventry irq_spx, __el2
    ventry fiq_spx, __el2
    ventry err_spx, __el2

    // Exception from a lower EL and at least one lower EL is AArch64
    ventry syn_low, __el2
    ventry irq_low, __el2
    ventry fiq_low, __el2
    ventry err_low, __el2

    // Exception from a lower EL and all lower ELs are AArch32
    ventry syn_opt, __el2
    ventry irq_opt, __el2
    ventry fiq_opt, __el2
    ventry err_opt, __el2
END(__el2_vector__)


    .align 11
GLOBAL(__el1_vector__)
    // Exception from the current EL while using SP_EL0
    ventry syn_sp0, __el1
    ventry irq_sp0, __el1
    ventry fiq_sp0, __el1
    ventry err_sp0, __el1

    // Exception from the current EL while using SP_ELx
    ventry syn_spx, __el1
    ventry irq_spx, __el1
    ventry fiq_spx, __el1
    ventry err_spx, __el1

    // Exception from a lower EL and at least one lower EL is AArch64
    ventry syn_low, __el1
    ventry irq_low, __el1
    ventry fiq_low, __el1
    ventry err_low, __el1

    // Exception from a lower EL and all lower ELs are AArch32
    ventry syn_opt, __el1
    ventry irq_opt, __el1
    ventry fiq_opt, __el1
    ventry err_opt, __el1
END(__el1_vector__)

// ####################################################
