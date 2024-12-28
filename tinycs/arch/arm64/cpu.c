// @Hustler's Project

#include <asm/sysreg.h>
#include <util.h>
#include <cpu.h>

// ####################################################
#define CNTFRQ_MASK      GENMASK(31, 0)

u64 cpufreq_khz(void) {
    return (READ_SYSREG(CNTFRQ_EL0) & CNTFRQ_MASK) / 1000;
}

// ####################################################
