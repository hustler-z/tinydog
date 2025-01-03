// @Hustler's Project

#include <compiler.h>
#include <print.h>
#include <uart.h>
#include <hal.h>

// ####################################################

extern u64 __tiny_end;

struct hal tiny_hal = {
    .name = "rk3566-cortex-a55",
    .pa_start = 0x0,
    .pa_end = 0x0,
};

int __init hal_init(void) {
    tiny_print("hal kicking off\n");

    // [1] memory region
    tiny_hal.pa_start = __tiny_end;
    tiny_hal.pa_end = tiny_hal.pa_start + MEMORY_REGION_SIZE;

    return 0;
}

// ####################################################
