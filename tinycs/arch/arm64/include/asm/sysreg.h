// @Hustler's Project

#ifndef _ASM_SYSREG_H_
#define _ASM_SYSREG_H_

// ####################################################

#include <type.h>

#define WRITE_SYSREG64(v, name) do {                          \
    u64 _r = (v);                                             \
    asm volatile("msr "__stringify(name)", %0" : : "r" (_r)); \
} while (0)

#define READ_SYSREG64(name) ({                                \
    u64 _r;                                                   \
    asm volatile("mrs  %0, "__stringify(name) : "=r" (_r));   \
    _r; })

#define READ_REG(Xn) ({                                       \
    u64 reg;                                                  \
    asm volatile("mov %0, "__stringify(Xn) : "=r"(reg));      \
    reg; })

#define READ_SYSREG(name)          READ_SYSREG64(name)
#define WRITE_SYSREG(v, name)      WRITE_SYSREG64(v, name)

// ####################################################

#endif /* _ASM_SYSREG_H_ */
