// @Hustler

#ifndef _BSP_CPU_H_
#define _BSP_CPU_H_

// ####################################################

#include <asm/cpu.h>
#include <scheduler.h>
#include <list.h>

struct cpu {
    struct context context;
    const u8 id;
    u8 flags;
    struct scheduler *sched;
    // @Hustler
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

// @Hustler
//
// Notifier Chain
struct notifier_block {
    int (*notifier_call)(struct notifier_block *nfb,
                         unsigned long action,
                         void *cpu);
    struct list_head chain;
    int priority;
};

struct notifier_head {
    struct list_head head;
};

/**
 * Notifier flag values: OR into @val passed to notifier_call_chain().
 */
#define NOTIFY_FORWARD   0x0000 /* call chain highest-priority-first */
#define NOTIFY_REVERSE   0x8000 /* call chain lowest-priority-first */

/**
 * Handler completion values
 */
#define NOTIFY_DONE      0x0000
#define NOTIFY_STOP_MASK 0x8000
#define NOTIFY_STOP      (NOTIFY_STOP_MASK|NOTIFY_DONE)
#define NOTIFY_BAD       (NOTIFY_STOP_MASK|EINVAL)

/**
 * Encapsulate (negative) errno value.
 */
static inline int notifier_from_errno(int err)
{
    return err ? (NOTIFY_STOP_MASK | -err) : NOTIFY_DONE;
}

/**
 * Restore (negative) errno value from notify return value.
 */
static inline int notifier_to_errno(int ret)
{
    return (ret == NOTIFY_DONE) ? 0 : -(ret & ~NOTIFY_STOP_MASK);
}

#define NOTIFIER_HEAD(name) \
    struct notifier_head name = { .head = LIST_HEAD_INIT((name).head) }

void notifier_chain_register(
    struct notifier_head *nh, struct notifier_block *nb);
void notifier_chain_unregister(
    struct notifier_head *nh, struct notifier_block *nb);

int notifier_call_chain(
    struct notifier_head *nh, unsigned long val, void *v,
    struct notifier_block **prev_nb);

// ####################################################

#endif /* _BSP_CPU_H_ */
