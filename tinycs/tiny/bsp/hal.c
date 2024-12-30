// @Hustler's Project

#include <compiler.h>
#include <print.h>
#include <uart.h>
#include <hal.h>

// ####################################################

int __init hal_init(void) {
    // @Hustler
    int ret = 0;

    ret |= debug_uart_init();

    tiny_print("switch debug uart\n");

    return ret;
}

// ####################################################
