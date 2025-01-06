// @Hustler's Project

#include <asm/reg.h>

#include <hal/hal.h>

#include <compiler.h>
#include <console.h>
#include <print.h>
#include <type.h>
#include <time.h>
#include <cpu.h>

typedef int (*bootc_t)(void);

static const bootc_t bootf[] = {
    // @Hustler
    //
    // Here's the booting workflow
    primary_cpu_init,

    secondary_cpu_init,

    time_init,

    hal_init,

    console_init,

    NULL,
};

// booting sequence
static int __init __bootc(const bootc_t *bootf) {
    // @Hustler
    const bootc_t *step;
    int ret = 0;
    u32 el = (current_el() >> 2) & 0x03;

    tiny_print("booting sequence (EL%u)\n", el);

    for (step = bootf; *step; step++) {
        ret = (*step)();

        if (ret)
            goto out;
    }

out:
    return ret;
}

void __init bootc(unsigned long offset) {
    if (prev_print_init())
        return;

    // @Hustler
    if (__bootc(bootf))
        return;
}
