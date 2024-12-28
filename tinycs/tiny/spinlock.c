// @Hustler's project

#include <spinlock.h>

static void __always_inline
spin_lock_common(spinlock_tickets_t *t, void (*cb)(void *data),
                 void *data) {
    //
}

static void __always_inline
spin_unlock_common(spinlock_tickets_t *t) {
    //
}

void __spin_lock(spinlock_t *lock) {
    spin_lock_common(&lock->tickets, NULL, NULL);
}

void __spin_unlock(spinlock_t *lock) {
    spin_unlock_common(&lock->tickets);
}
