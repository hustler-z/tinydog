// @Hustler's Project

#include <compiler.h>
#include <console.h>
#include <print.h>
#include <type.h>
#include <time.h>
#include <hal.h>
#include <cpu.h>

typedef int (*bootc_t)(void);

static const bootc_t bootf[] = {
    // @Hustler
    //
    // Here's the booting workflow
    prev_print_init,

    primary_cpu_init,

    secondary_cpu_init,

    time_init,

    hal_init,

    post_print_init,

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

    // @Hustler
    if (__bootc__(bootf))
        return;
}
