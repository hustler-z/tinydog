// @Hustler's Project

#include <compiler.h>
#include <print.h>
#include <bsp.h>

// ####################################################

extern u64 __tiny_end;

struct bsp tiny_bsp = {
    .name = "rk3566-cortex-a55",
    .pa_start = 0x0,
    .pa_end = 0x0,
};

int __init bsp_init(void) {
    tiny_print("bsp kicking off\n");

    // [1] memory region
    tiny_bsp.pa_start = __tiny_end;
    tiny_bsp.pa_end = tiny_bsp.pa_start + MEMORY_REGION_SIZE;

    return 0;
}

// ####################################################
