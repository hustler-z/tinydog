// @Hustler's Project

#include <hal/uart.h>
#include <hal/hal.h>

#include <compiler.h>
#include <print.h>

// ####################################################

int __init hal_init(void) {
    int ret = 0;

    // switch to hal uart
    ret = post_print_init();

    tiny_print("switch console io\n");

    return ret;
}

// ####################################################
