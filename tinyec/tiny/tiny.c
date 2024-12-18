// @Hustler's Project
#include <compiler.h>

typedef int (*bootcall_t)(void);

static bootcall_t bootflow[] = {
    // @Hustler
    //
    // Here's the booting workflow
    NULL,
};

static int bootcalls(const bootcall_t *bootflow) {
    // @Hustler
    const bootcall_t *step;
    int ret = 0;

    for (step = bootflow; *step; step++) {
        ret = (*step)();

        if (ret)
            goto out;
    }

out:
    return ret;
}

void asmlinkage __bootc(unsigned long offset) {
    // @Hustler
    if (bootcalls(bootflow))
        return;
}
