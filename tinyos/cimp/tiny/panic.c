// @Hustler

#include <asm/barrier.h>
#include <asm/smccc.h>

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
    int method = _HVC;

    __psci_get_version(method);

    __print__("rebooting ...\n");

    if (__psci_reset(method, _RESET))
        __print__("reboot failed\n");

    mdelay(ms);
}

void cpu_off(void) {
    int method = _HVC;

    if (__psci_cpu_off(method))
        __print__("cpu off failed\n");
}
