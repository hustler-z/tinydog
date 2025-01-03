// @Hustler's Project

#ifndef _ASM_SYSREG_H_
#define _ASM_SYSREG_H_

// ####################################################

#ifndef __ASSEMBLY__

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

static inline u32 current_el(void) {
    u32 el;

    asm volatile("mov %0, CurrentEL" : "=r"(el));

    return el;
}

#else /* __ASSEMBLY__ */

// Accesses from EL1 and EL0 to Activity Monitor registers are
// trapped to EL2, when EL2 is enabled in the current Security
// state.
#define CPTR_TAM      (1U << 30)
// Traps execution at EL2, EL1, and EL0 of instructions that
// access the Advanced SIMD and floating-point registers from
// both Execution states to EL2, when EL2 is enabled in the
// current Security state. The exception is reported using
// ESR_ELx.EC value 0x07.
#define CPTR_FPEN     (1U << 20)
#define CPTR_CP(x)    (1U << (x))
#define CPTR_CP_MASK  ((1U << 14) - 1)
#define CPTR_EL2_INIT ((CPTR_CP_MASK & ~(CPTR_CP(10) | \
                      CPTR_CP(11))) | \
                      CPTR_FPEN | CPTR_TAM)

// @Hustler
//
// Hypervisor System Trap Register
//
// Controls trapping to EL2 of EL1 or lower AArch32 accesses to
// the System register in the coproc == 0b1111 encoding
// space, by the CRn value used to access the register using MCR
// or MRC instruction. When the register is accessible using an
// MCRR or MRRC instruction, this is the CRm value used to access
// the register.
#define HSTR_MASK(x)  (1U << (x))

#endif

// ####################################################

#endif /* _ASM_SYSREG_H_ */
