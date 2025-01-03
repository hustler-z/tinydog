// @Hustler's project

#include <asm/barrier.h>
#include <asm/atomic.h>
#include <spinlock.h>
#include <preempt.h>

static __always_inline
spinlock_tickets_t observe_lock(spinlock_tickets_t *t)
{
    spinlock_tickets_t v;

    smp_rmb();
    v.head_tail = read_atomic(&t->head_tail);
    return v;
}

static __always_inline u16 observe_head(spinlock_tickets_t *t)
{
    smp_rmb();
    return read_atomic(&t->head);
}

static void __always_inline
spin_lock_common(spinlock_tickets_t *t, void (*cb)(void *data),
                 void *data) {
    //
    spinlock_tickets_t tickets = SPINLOCK_TICKET_INC;

    preempt_disable();
    tickets.head_tail = gnu_fetch_and_add(&t->head_tail,
                                          tickets.head_tail);
    while (tickets.tail != observe_head(t)) {
        if (cb)
            cb(data);
        arch_lock_relax();
    }
    arch_lock_acquire_barrier();
}

static void __always_inline
spin_unlock_common(spinlock_tickets_t *t) {
    //
    arch_lock_release_barrier();
    add_sized(&t->head, 1);
    arch_lock_signal();
    preempt_enable();
}

void __spin_lock(spinlock_t *lock) {
    spin_lock_common(&lock->tickets, NULL, NULL);
}

void __spin_unlock(spinlock_t *lock) {
    spin_unlock_common(&lock->tickets);
}

unsigned long _spin_lock_irqsave(spinlock_t *lock)
{
    unsigned long flags;

    local_irq_save(flags);
    __spin_lock(lock);

    return flags;
}

void _spin_unlock_irqrestore(spinlock_t *lock,
                             unsigned long flags)
{
    __spin_unlock(lock);
    local_irq_restore(flags);
}
