// @Hustler's Project

#include <compiler.h>
#include <console.h>
#include <type.h>
#include <cpu.h>

typedef int (*bootc_t)(void);

static const bootc_t bootf[] = {
    // @Hustler
    //
    // Here's the booting workflow
    primary_cpu_init,

    secondary_cpu_init,

    console_init,

    NULL,
};

static int __init _bootc(const bootc_t *bootf) {
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

void asmlinkage __init bootc(unsigned long offset) {
    // @Hustler
    if (_bootc(bootf))
        return;
}
