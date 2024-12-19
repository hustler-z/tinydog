// @Hustler's Project

#include <asm/cpu.h>

extern void _outs(char *msg);

// ####################################################

void syn_sp0_handler(struct context *context) {
    _outs("-----> [syn sp0]\r\n");
}

void irq_sp0_handler(struct context *context) {

    _outs("-----> [irq sp0]\r\n");
}

void fiq_sp0_handler(struct context *context) {

    _outs("-----> [fiq sp0]\r\n");
}

void err_sp0_handler(struct context *context) {

    _outs("-----> [err sp0]\r\n");
}

// ####################################################

void syn_spx_handler(struct context *context) {

    _outs("-----> [syn spx]\r\n");
}

void irq_spx_handler(struct context *context) {

    _outs("-----> [irq spx]\r\n");
}

void fiq_spx_handler(struct context *context) {

    _outs("-----> [fiq spx]\r\n");
}

void err_spx_handler(struct context *context) {

    _outs("-----> [err spx]\r\n");
}

// ####################################################

void syn_low_handler(struct context *context) {

    _outs("-----> [syn low]\r\n");
}

void irq_low_handler(struct context *context) {

    _outs("-----> [irq low]\r\n");
}

void fiq_low_handler(struct context *context) {

    _outs("-----> [fiq low]\r\n");
}

void err_low_handler(struct context *context) {

    _outs("-----> [err low]\r\n");
}

// ####################################################

void syn_opt_handler(struct context *context) {

    _outs("-----> [syn opt]\r\n");
}

void irq_opt_handler(struct context *context) {

    _outs("-----> [irq opt]\r\n");
}

void fiq_opt_handler(struct context *context) {

    _outs("-----> [fiq opt]\r\n");
}

void err_opt_handler(struct context *context) {
    _outs("-----> [err opt]\r\n");
}

// ####################################################
