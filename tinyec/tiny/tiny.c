// @Hustler's Project

#include <compiler.h>
#include <type.h>

typedef int (*bootc_t)(void);

static bootc_t bootflow[] = {
    // @Hustler
    //
    // Here's the booting workflow
    NULL,
};

static int _bootc(const bootc_t *bootflow) {
    // @Hustler
    const bootc_t *step;
    int ret = 0;

    for (step = bootflow; *step; step++) {
        ret = (*step)();

        if (ret)
            goto out;
    }

out:
    return ret;
}

void asmlinkage bootc(unsigned long offset) {
    // @Hustler
    if (_bootc(bootflow))
        return;
}
