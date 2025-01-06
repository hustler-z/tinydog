// @Hustler's Project

#include <asm/interrupt.h>
#include <asm/spinlock.h>
#include <asm/smccc.h>
#include <asm/reg.h>
#include <asm/cpu.h>
#include <asm/smp.h>

#include <hal/rk3566.h>

#include <compiler.h>
#include <print.h>

// ####################################################

DEFINE_PERCPU(u32, cpu_id);

#define CNTFRQ_MASK      GENMASK(31, 0)

u64 cpufreq_khz(void) {
    return (READ_SYSREG(CNTFRQ_EL0) & CNTFRQ_MASK) / 1000;
}

int __init arch_primary_cpu_init(void) {

    arch_gic_init();

    return 0;
}

extern void __boot_secondary_cpu__(void);
extern void dump_stack(void);

void smc_boot_cpu(int cpu_id) {
    if (__psci_cpu_on(_SMC, cpu_id,
                (unsigned long)(void *)&__boot_secondary_cpu__)) {
        tiny_error("boot secondary cpu %d failed\n", cpu_id);

        dump_stack();
    }
}

void hvc_boot_cpu(int cpu_id) {
    if (__psci_cpu_on(_HVC, cpu_id,
                (unsigned long)(void *)&__boot_secondary_cpu__)) {
        tiny_error("boot secondary cpu %d failed\n", cpu_id);

        dump_stack();
    }
}

int __init arch_secondary_cpu_init(void) {

#ifdef __SMP__ // @Hustler
    __spinlock_t lock = 0;
    int i, method = _HVC;

    // @Hustler
    //
    // boot other cpus ???
    for (i = 1; i < NR_CPU; i++) {
        __spin_lock__(&lock);

        switch (method) {
        case _HVC:
            hvc_boot_cpu(i);
            break;
        case _SMC:
            smc_boot_cpu(i);
            break;
        default:
            break;
        }

        __spin_unlock(&lock);
    }
#endif

    return 0;
}

void cpu_dump(void) {

}

// ####################################################
