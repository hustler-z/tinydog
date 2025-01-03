// @Hustler's Project

#ifndef _ASM_CPU_H_
#define _ASM_CPU_H_

// ####################################################

#include <type.h>

struct context {
    register_t x0;
    register_t x1;
    register_t x2;
    register_t x3;
    register_t x4;
    register_t x5;
    register_t x6;
    register_t x7;
    register_t x8;
    register_t x9;
    register_t x10;
    register_t x11;
    register_t x12;
    register_t x13;
    register_t x14;
    register_t x15;
    register_t x16;
    register_t x17;
    register_t x18;
    register_t x19;
    register_t x20;
    register_t x21;
    register_t x22;
    register_t x23;
    register_t x24;
    register_t x25;
    register_t x26;
    register_t x27;
    register_t x28;

    register_t fp; /* x29 */
    register_t lr; /* x30 */

    register_t sp;
    register_t pc;

    register_t cpsr;
    register_t esr;

    register_t elr;
};

// @Hustler
//
// @arch/arm64/arm.c
u64 cpufreq_khz(void);

// ####################################################

#endif /* _ASM_CPU_H_ */
