// @Hustler

#include <asm/barrier.h>
#include <print.h>
#include <panic.h>
#include <time.h>

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

void reboot(unsigned int ms) {
    tiny_print("rebooting ...\n");

    mdelay(ms);
}
