// @Hustler's Project

#include <asm/debug.h>
#include <compiler.h>
#include <console.h>
#include <type.h>
#include <time.h>
#include <hal.h>
#include <cpu.h>
#include <bsp.h>

typedef int (*bootc_t)(void);

static const bootc_t bootf[] = {
    // @Hustler
    //
    // Here's the booting workflow
    primary_cpu_init,

    secondary_cpu_init,

    bsp_init,

    time_init,

    hal_init,

    console_init,

    NULL,
};

// booting sequence
static int __init __bootc__(const bootc_t *bootf) {
    // @Hustler
    const bootc_t *step;
    int ret = 0;

    for (step = bootf; *step; step++) {
        ret = (*step)();

        if (ret)
            goto out;
    }

out:
    return ret;
}

void __init bootc(unsigned long offset) {
    __asm_outs__("- hello, C world\r\n");

    // @Hustler
    if (__bootc__(bootf))
        return;
}
