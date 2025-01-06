// @Hustler's Project

#include <asm/interrupt.h>
#include <asm/reg.h>
#include <asm/cpu.h>

// ####################################################

#define EL1            0x04
#define EL2            0x08
#define EL3            0x0C

extern void __asm_outs__(const char *msg);

// ####################################################

/**
 * @Hustler
 *
 * XXX: current el with SP0
 */

void syn_sp0_handler(struct context *context) {
    __asm_outs__("--> [syn sp0]\r\n");

    // synchronous exception
    do_sync();
}

void irq_sp0_handler(struct context *context) {
    __asm_outs__("--> [irq sp0]\r\n");

    // interrupt exception
    do_irq();
}

void fiq_sp0_handler(struct context *context) {
    __asm_outs__("--> [fiq sp0]\r\n");

    do_fiq();
}

void err_sp0_handler(struct context *context) {
    __asm_outs__("--> [err sp0]\r\n");

    // handle system errors
    do_error();
}

// ####################################################

/**
 * @Hustler
 *
 * XXX: current el with SPx
 */

void syn_spx_handler(struct context *context) {
    __asm_outs__("--> [syn spx]\r\n");

    // synchronous exception
    do_sync();
}

void irq_spx_handler(struct context *context) {
    __asm_outs__("--> [irq spx]\r\n");

    // interrupt exception
    do_irq();
}

void fiq_spx_handler(struct context *context) {
    __asm_outs__("--> [fiq spx]\r\n");

    do_fiq();
}

void err_spx_handler(struct context *context) {
    __asm_outs__("--> [err spx]\r\n");

    // handle system errors
    do_error();
}

// ####################################################

void syn_low_handler(struct context *context) {
    __asm_outs__("--> [syn low]\r\n");
}

void irq_low_handler(struct context *context) {
    __asm_outs__("--> [irq low]\r\n");
}

void fiq_low_handler(struct context *context) {
    __asm_outs__("--> [fiq low]\r\n");
}

void err_low_handler(struct context *context) {
    __asm_outs__("--> [err low]\r\n");
}

// ####################################################

void syn_opt_handler(struct context *context) {
    __asm_outs__("--> [syn opt]\r\n");
}

void irq_opt_handler(struct context *context) {
    __asm_outs__("--> [irq opt]\r\n");
}

void fiq_opt_handler(struct context *context) {
    __asm_outs__("--> [fiq opt]\r\n");
}

void err_opt_handler(struct context *context) {
    __asm_outs__("--> [err opt]\r\n");
}

// ####################################################

void dump_reg(struct context *context) {

}

// ####################################################
