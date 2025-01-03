// @Hustler

#ifndef _TINY_COMPILER_H_
#define _TINY_COMPILER_H_

// ####################################################

#ifndef asmlinkage
#define asmlinkage            __attribute__((regparm(0)))
#endif

#define __aligned(x)          __attribute__((__aligned__(x)))
#define __section(s)          __attribute__((__section__(s)))
#define __weak                __attribute__((__weak__))
#define fallthrough           __attribute__((__fallthrough__))
#define __always_inline       inline __attribute__((__always_inline__))
#define __iomem
#define __force

#define likely(x)             __builtin_expect(!!(x), 1)
#define unlikely(x)           __builtin_expect(!!(x), 0)

#define RELOC_HIDE(ptr, off) ({           \
    unsigned long __ptr;                  \
    __asm__("" : "=r"(__ptr) : "0"(ptr)); \
    (typeof(ptr))(__ptr + (off)); })

// ####################################################

#define __init                __section(".text.init")
#define __stack               __section(".stack")

// ####################################################


// @Hustler
//
// compilation flag: -mno-outline-atomics
#define gnu_fetch_and_add(x, v) __sync_fetch_and_add(x, v)

// ####################################################

#endif /* _TINY_COMPILER_H_ */
