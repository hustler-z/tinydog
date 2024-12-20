// @Hustler's Project

#ifndef _TINY_SCHEDULER_H_
#define _TINY_SCHEDULER_H_

#include <queue.h>

struct task;

CIRCLEQ_HEAD(sched, task);

struct scheduler {
    const char *name;
    struct sched *head;
};

#endif /* _TINY_SCHEDULER_H_ */
