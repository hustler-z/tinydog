// @Hustler

#ifndef _TINY_CPU_H_
#define _TINY_CPU_H_

// ####################################################
#include <asm/cpu.h>
#include <scheduler.h>

struct cpu {
    struct context context;
    const u8 id;
    u8 flags;

    struct scheduler *sched;

    const void *stack;
};

enum cpu_state {
    CPU_ACTIVE = 0,
    CPU_IDLE,
    CPU_PENDING,
    CPU_DEAD,
};

int primary_cpu_init(void);
int secondary_cpu_init(void);

// ####################################################

#endif /* _TINY_CPU_H_ */
