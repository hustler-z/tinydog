// @Hustler

#include <asm/barrier.h>
#include <print.h>

// @Halt
static void __halt(void) {
    local_irq_disable();

    tiny_print("cpu halt ...\n");

    dsb(sy);
    isb();

    while (1)
        wfi();
}

void halt(void) {
    __halt();
}
