// @Hustler's Project

#ifndef _ASM_TIME_H_
#define _ASM_TIME_H_

// ####################################################

#include <asm/barrier.h>
#include <asm/sysreg.h>
#include <type.h>

#define read_cntpct_enforce_order(val) \
    do {                               \
        u64 tmp, _val = (val);         \
        asm volatile(                  \
            "eor %0, %1, %1\n"         \
            "add %0, sp, %0\n"         \
            "ldr xzr, [%0]"            \
            : "=r" (tmp) : "r" (_val));\
    } while(0)

static inline u64 read_cntpct_stable(void) {
    u64 old, new;

    old = READ_SYSREG64(CNTPCT_EL0);
    new = READ_SYSREG64(CNTPCT_EL0);
    return (((old ^ new) >> 32) & 1) ? old : new;
}

static inline u64 physical_count(void) {
    u64 cnt;

    isb();

    cnt = read_cntpct_stable();

    read_cntpct_enforce_order(cnt);

    return cnt;
}

// ####################################################

#endif /* _ASM_TIME_H_ */
