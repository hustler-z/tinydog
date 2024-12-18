// @Hustler's Project

#include <asm/asm.h>

// ##############################################################

/**
 * TODO
 */
.macro exception_entry
    dmb   sy

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
    mrs   x0, SPSR_EL2
    mrs   x1, ELR_EL2
    mrs   x2, ESR_EL2
.endm

/**
 * TODO
 */
.macro exception_exit
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
GLOBAL(exception_table)
    // Exception from the current EL while using SP_EL0
    ventry _syn_el0
    ventry _irq_el0
    ventry _fiq_el0
    ventry _err_el0

    // Exception from the current EL while using SP_ELx
    ventry _syn_elx
    ventry _irq_elx
    ventry _fiq_elx
    ventry _err_elx

    // Exception from a lower EL and at least one lower EL is AArch64
    ventry _syn_ell
    ventry _irq_ell
    ventry _fiq_ell
    ventry _err_ell

    // Exception from a lower EL and all lower ELs are AArch32
    ventry _syn_a32
    ventry _irq_a32
    ventry _fiq_a32
    ventry _err_a32
END(exception_table)
