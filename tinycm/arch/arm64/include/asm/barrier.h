// @Hustler's Project

#ifndef _ASM_BARRIER_H_
#define _ASM_BARRIER_H_

// ####################################################

/* Instruction Synchronization Barrier flushes the
 * pipeline in the PE and is a context synchronization
 * event.
 *
 * Used to guarantee that any subsequent instructions
 * are fetched, again, so that privilege and access are
 * checked with the current MMU configuration.
 */
#define isb()         asm volatile("isb" : : : "memory")

/* Data Synchronization Barrier is a memory barrier
 * that ensures the completion of memory accesses.
 *
 * Enforces the same ordering as the Data Memory Barrier,
 * but has the additional effect of blocking execution
 * of any further instructions, not just loads or stores,
 * or both, until synchronization is complete.
 */
#define dsb(scope)    asm volatile("dsb " #scope : : : "memory")

/* Data Memory Barrier is a memory barrier that ensures
 * the ordering of observations of memory accesses.
 */
#define dmb(scope)    asm volatile("dmb " #scope : : : "memory")

/* Send Event is a hint instruction. It causes an event
 * to be signaled to all PEs in the multiprocessor system.
 */
#define sev()         asm volatile("sev" : : : "memory")

/* Wait For Event is a hint instruction that indicates
 * that the PE can enter a low-power state and remain there
 * until a wakeup event occurs. Wakeup events include the
 * event signaled as a result of executing the SEV instruction
 * on any PE in the multiprocessor system.
 */
#define wfe()         asm volatile("wfe" : : : "memory")

/* Wait For Interrupt is a hint instruction that indicates
 * that the PE can enter a low-power state and remain
 * there until a wakeup event occurs.
 */
#define wfi()         asm volatile("wfi" : : : "memory")

#define __nops(n)     ".rept   " #n "\nnop\n.endr\n"
#define nops(n)       asm volatile(__nops(n))

#define psb_csync()   asm volatile("hint #17" : : : "memory")
#define csdb()        asm volatile("hint #20" : : : "memory")

#define barrier()     asm volatile("" : : : "memory")

#define cpu_relax()   barrier()

#define mb()          dsb(sy)
#define rmb()         dsb(ld)
#define wmb()         dsb(st)

#define smp_mb()      dmb(ish)
#define smp_rmb()     dmb(ishld)
#define smp_wmb()     dmb(ishst)

#define dma_mb()      dmb(osh)
#define dma_rmb()     dmb(oshld)
#define dma_wmb()     dmb(oshst)

#define smp_mb__before_atomic()  smp_mb()
#define smp_mb__after_atomic()   smp_mb()

// ####################################################

#define local_fiq_disable()   \
    asm volatile ("msr DAIFSet, #1\n" ::: "memory")
#define local_irq_disable()   \
    asm volatile ("msr DAIFSet, #2\n" ::: "memory")
#define local_abort_disable() \
    asm volatile ("msr DAIFSet, #4\n" ::: "memory")

#define local_daif_disable() \
    asm volatile ("msr DAIFSet, #7\n" ::: "memory")

#define local_fiq_enable()    \
    asm volatile ("msr DAIFClr, #1\n" ::: "memory")
#define local_irq_enable()    \
    asm volatile ("msr DAIFClr, #2\n" ::: "memory")
#define local_abort_enable()  \
    asm volatile ("msr DAIFCLr, #4\n" ::: "memory")

#define local_irq_restore(x) ({ \
    asm volatile (              \
        "msr DAIF, %0"          \
        :                       \
        : "r" (x)               \
        : "memory");            \
})

#define local_save_flags(x) ({  \
    asm volatile (              \
        "mrs %0, DAIF"          \
        : "=r" (x)              \
        :                       \
        : "memory");            \
})

#define local_irq_save(x) ({    \
    local_save_flags(x);        \
    local_irq_disable();        \
})

// ####################################################

#define PSR_THUMB         (1U << 5)
#define PSR_FIQ_MASK      (1U << 6)
#define PSR_IRQ_MASK      (1U << 7)
#define PSR_ABT_MASK      (1U << 8)
#define PSR_BIG_ENDIAN    (1U << 9)
#define PSR_DBG_MASK      (1U << 9)

#define tiny_daif_set(x)        \
    do {                        \
        asm volatile (          \
        "msr DAIF, %0"          \
        : : "r" (x)             \
        : "memory");            \
    } while (0)

static inline unsigned long tiny_daif_get(void)
{
    unsigned long daif;

    asm volatile ("mrs %0, DAIF" : "=r" (daif) : : "memory");

    return daif;
}

static inline int local_irq_is_enabled(void)
{
    unsigned long flags;
    local_save_flags(flags);
    return !(flags & PSR_IRQ_MASK);
}

static inline int local_fiq_is_enabled(void)
{
    unsigned long flags;
    local_save_flags(flags);
    return !(flags & PSR_FIQ_MASK);
}

static inline int local_abort_is_enabled(void)
{
    unsigned long flags;
    local_save_flags(flags);
    return !(flags & PSR_ABT_MASK);
}

#define arch_lock_acquire_barrier() smp_mb()
#define arch_lock_release_barrier() smp_mb()

#define arch_lock_relax()  wfe()
#define arch_lock_signal() do { \
    dsb(ishst);                 \
    sev();                      \
} while(0)

#define arch_lock_signal_wmb()  arch_lock_signal()


#define active_wait()           wfi()

// ####################################################

#endif /* _ASM_BARRIER_H_ */
