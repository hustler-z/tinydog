// @Hustler's Project

#ifndef _ASM_IO_H_
#define _ASM_IO_H_

// ####################################################

#include <asm/barrier.h>
#include <compiler.h>
#include <endian.h>
#include <type.h>

#define ARM64_WORKAROUND_DEVICE_LOAD_ACQUIRE                     (1)

#define ALTINSTR_ENTRY(feature, cb)                               \
    " .word 661b - .\n"             /* label */                   \
    " .if " __stringify(cb) " == 0\n"                             \
    " .word 663f - .\n"             /* new instruction */         \
    " .else\n"                                                    \
    " .word " __stringify(cb) "- .\n"       /* callback */        \
    " .endif\n"                                                   \
    " .hword " __stringify(feature) "\n"        /* feature bit */ \
    " .byte 662b-661b\n"                /* source len */          \
    " .byte 664f-663f\n"                /* replacement len */

#define __ALTERNATIVE_CFG(oldinstr, newinstr, feature, cfg_enabled, cb) \
    ".if "__stringify(cfg_enabled)" == 1\n"                             \
    "661:\n\t"                                                          \
    oldinstr "\n"                                                       \
    "662:\n"                                                            \
    ".pushsection .altinstructions,\"a\"\n"                             \
    ALTINSTR_ENTRY(feature,cb)                                          \
    ".popsection\n"                                                     \
    " .if " __stringify(cb) " == 0\n"                                   \
    ".pushsection .altinstr_replacement, \"ax\"\n"                      \
    "663:\n\t"                                                          \
    newinstr "\n"                                                       \
    "664:\n\t"                                                          \
    ".popsection\n\t"                                                   \
    ".org   . - (664b-663b) + (662b-661b)\n\t"                          \
    ".org   . - (662b-661b) + (664b-663b)\n"                            \
    ".else\n\t"                                                         \
    "663:\n\t"                                                          \
    "664:\n\t"                                                          \
    ".endif\n"                                                          \
    ".endif\n"

#define _ALTERNATIVE_CFG(oldinstr, newinstr, feature, cfg, ...)   \
    __ALTERNATIVE_CFG(oldinstr, newinstr, feature, cfg, 0)

#define ALTERNATIVE(oldinstr, newinstr, ...)                      \
    _ALTERNATIVE_CFG(oldinstr, newinstr, __VA_ARGS__, 1)

// ####################################################

#define __raw_writeb __raw_writeb
static inline void __raw_writeb(u8 val,
                                volatile void __iomem *addr)
{
    asm volatile("strb %w0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_writew __raw_writew
static inline void __raw_writew(u16 val,
                                volatile void __iomem *addr)
{
    asm volatile("strh %w0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_writel __raw_writel
static __always_inline void __raw_writel(u32 val,
                                       volatile void __iomem *addr)
{
    asm volatile("str %w0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_writeq __raw_writeq
static inline void __raw_writeq(u64 val,
                                volatile void __iomem *addr)
{
    asm volatile("str %x0, [%1]" : : "rZ" (val), "r" (addr));
}

#define __raw_readb __raw_readb
static inline u8 __raw_readb(const volatile void __iomem *addr)
{
    u8 val;
    asm volatile(ALTERNATIVE("ldrb %w0, [%1]",
                 "ldarb %w0, [%1]",
                 ARM64_WORKAROUND_DEVICE_LOAD_ACQUIRE)
             : "=r" (val) : "r" (addr));
    return val;
}

#define __raw_readw __raw_readw
static inline u16 __raw_readw(const volatile void __iomem *addr)
{
    u16 val;

    asm volatile(ALTERNATIVE("ldrh %w0, [%1]",
                 "ldarh %w0, [%1]",
                 ARM64_WORKAROUND_DEVICE_LOAD_ACQUIRE)
             : "=r" (val) : "r" (addr));
    return val;
}

#define __raw_readl __raw_readl
static __always_inline u32 __raw_readl(const volatile void __iomem *addr)
{
    u32 val;
    asm volatile(ALTERNATIVE("ldr %w0, [%1]",
                 "ldar %w0, [%1]",
                 ARM64_WORKAROUND_DEVICE_LOAD_ACQUIRE)
             : "=r" (val) : "r" (addr));
    return val;
}

#define __raw_readq __raw_readq
static inline u64 __raw_readq(const volatile void __iomem *addr)
{
    u64 val;
    asm volatile(ALTERNATIVE("ldr %0, [%1]",
                 "ldar %0, [%1]",
                 ARM64_WORKAROUND_DEVICE_LOAD_ACQUIRE)
             : "=r" (val) : "r" (addr));
    return val;
}

/* IO barriers */
#define __iormb(v) ({                                \
    unsigned long tmp;                               \
    dma_rmb();                                       \
    asm volatile("eor  %0, %1, %1\n"                 \
             "cbnz  %0, ."                           \
             : "=r" (tmp) : "r" ((unsigned long)(v)) \
             : "memory");                            \
})

#define __io_par(v)  __iormb(v)
#define __iowmb()    dma_wmb()
#define __iomb()     dma_mb()

/*
 * Relaxed I/O memory access primitives. These follow the
 * Device memory ordering rules but do not guarantee any
 * ordering relative to Normal memory accesses.
 */
#define readb_relaxed(c) ({ u8  __r = __raw_readb(c); __r; })
#define readw_relaxed(c) \
    ({ u16 __r = le16_to_cpu((__force __le16)__raw_readw(c)); __r; })
#define readl_relaxed(c) \
    ({ u32 __r = le32_to_cpu((__force __le32)__raw_readl(c)); __r; })
#define readq_relaxed(c) \
    ({ u64 __r = le64_to_cpu((__force __le64)__raw_readq(c)); __r; })
#define readq_relaxed_non_atomic(c)      readq_relaxed(c)

#define writeb_relaxed(v,c) ((void)__raw_writeb((v),(c)))
#define writew_relaxed(v,c) \
    ((void)__raw_writew((__force u16)cpu_to_le16(v),(c)))
#define writel_relaxed(v,c) \
    ((void)__raw_writel((__force u32)cpu_to_le32(v),(c)))
#define writeq_relaxed(v,c) \
    ((void)__raw_writeq((__force u64)cpu_to_le64(v),(c)))
#define writeq_relaxed_non_atomic(v, c)  writeq_relaxed(v, c)

/*
 * I/O memory access primitives. Reads are ordered relative to
 * any following Normal memory access. Writes are ordered
 * relative to any prior Normal memory access.
 */
#define readb(c) \
    ({ u8  __v = readb_relaxed(c); __iormb(__v); __v; })
#define readw(c) \
    ({ u16 __v = readw_relaxed(c); __iormb(__v); __v; })
#define readl(c) \
    ({ u32 __v = readl_relaxed(c); __iormb(__v); __v; })
#define readq(c) \
    ({ u64 __v = readq_relaxed(c); __iormb(__v); __v; })

#define writeb(v,c) ({ __iowmb(); writeb_relaxed((v),(c)); })
#define writew(v,c) ({ __iowmb(); writew_relaxed((v),(c)); })
#define writel(v,c) ({ __iowmb(); writel_relaxed((v),(c)); })
#define writeq(v,c) ({ __iowmb(); writeq_relaxed((v),(c)); })

// ####################################################

#endif /* _ASM_IO_H_ */
