// @Hustler's Project

#ifndef _ASM_SPINLOCK_H_
#define _ASM_SPINLOCK_H_

typedef unsigned long   __spinlock_t;

extern void __spin_lock__(__spinlock_t *l);
extern int __spin_trylock__(__spinlock_t *l);
extern void __spin_unlock__(__spinlock_t *l);

#endif /* _ASM_SPINLOCK_H_ */
