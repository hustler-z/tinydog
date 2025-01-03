// @Hustler's Project

#ifndef _TINY_PANIC_H_
#define _TINY_PANIC_H_

#include <asm/smp.h>

DECLARE_PERCPU(unsigned int, __preempt_count);

#define preempt_count()      (this_cpu(__preempt_count))

#define preempt_disable()  \
    do {                   \
        preempt_count()++; \
        barrier();         \
    } while(0)

#define preempt_enable()   \
    do {                   \
        barrier();         \
        preempt_count()--; \
    } while(0)

#endif /* _TINY_PANIC_H_ */
