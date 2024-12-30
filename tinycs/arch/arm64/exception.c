// @Hustler's Project

#include <asm/cpu.h>

extern void __asm_outs__(const char *msg);

// @Hustler [ISR]
void do_sync(struct context *context);
void do_irq(struct context *context);
void do_fiq(struct context *context);
void do_error(struct context *context);

// ####################################################

void syn_sp0_handler(struct context *context) {
    __asm_outs__("--> [syn sp0]\r\n");

    // synchronous exception
    do_sync(context);
}

void irq_sp0_handler(struct context *context) {
    __asm_outs__("--> [irq sp0]\r\n");

    // interrupt exception
    do_irq(context);
}

void fiq_sp0_handler(struct context *context) {
    __asm_outs__("--> [fiq sp0]\r\n");

    do_fiq(context);
}

void err_sp0_handler(struct context *context) {
    __asm_outs__("--> [err sp0]\r\n");

    // handle system errors
    do_error(context);
}

// ####################################################

void syn_spx_handler(struct context *context) {
    __asm_outs__("--> [syn spx]\r\n");
}

void irq_spx_handler(struct context *context) {
    __asm_outs__("--> [irq spx]\r\n");
}

void fiq_spx_handler(struct context *context) {
    __asm_outs__("--> [fiq spx]\r\n");
}

void err_spx_handler(struct context *context) {
    __asm_outs__("--> [err spx]\r\n");
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

// @Hustler
void do_sync(struct context *context) {

}

// @Hustler
void do_irq(struct context *context) {

}

// @Hustler
void do_fiq(struct context *context) {

}

// @Hustler
void do_error(struct context *context) {

}

// ####################################################
