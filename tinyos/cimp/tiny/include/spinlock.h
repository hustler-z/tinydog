// @Hustler's Project

#ifndef _TINY_SPINLOCK_H_
#define _TINY_SPINLOCK_H_

#include <compiler.h>
#include <type.h>

typedef union {
    u32 head_tail;
    struct {
        u16 head;
        u16 tail;
    };
} spinlock_tickets_t;

#define SPINLOCK_TICKET_INC { .head_tail = 0x10000, }

typedef struct spinlock {
    spinlock_tickets_t tickets;
} spinlock_t;

#define SPINLOCK_UNLOCKED {  }

#define DEFINE_SPINLOCK(l) \
    spinlock_t l = SPINLOCK_UNLOCKED

#define spinlock_init(l) \
    (*(l) = (spinlock_t)SPINLOCK_UNLOCKED)

void __spin_lock(spinlock_t *lock);
void __spin_unlock(spinlock_t *lock);

static __always_inline void spin_lock(spinlock_t *lock) {
    __spin_lock(lock);
}

#define spin_unlock(l)   __spin_unlock(l)

#endif /* _TINY_SPINLOCK_H_ */
