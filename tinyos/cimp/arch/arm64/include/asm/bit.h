// @Hustler's Project

#ifndef _ASM_BIT_
#define _ASM_BIT_

#define BIT(n)                (1UL << (n))
#define BIT_MASK(n)           (BIT(n) - 1UL)
#define WRITE_BIT(var, bit, set) \
    ((var) = (set) ? ((var) | BIT(bit)) : ((var) & ~BIT(bit))

#endif /* _ASM_BIT_ */
