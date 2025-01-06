// @Hustler's Project

#ifndef _TINY_SCHEDULER_H_
#define _TINY_SCHEDULER_H_

#include <queue.h>
#include <cpu.h>

struct task {
    const char *name;
    struct cpu *core;

    // notifier block
    struct notifier_block nb;
};

CIRCLEQ_HEAD(sched, task);

struct scheduler {
    const char *name;
    struct sched *shead;
};

#endif /* _TINY_SCHEDULER_H_ */
