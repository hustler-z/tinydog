// @Hustler

#ifndef _TINY_H_
#define _TINY_H_

// ##############################################################

#ifndef asmlinkage
#define asmlinkage
#endif

#define __aligned(x)          __attribute__((__aligned__(x)))
#define __section(s)          __attribute__((__section__(s)))
#define __weak                __attribute__((__weak__))
#define fallthrough           __attribute__((__fallthrough__))
#define __always_inline       inline __attribute__((__always_inline__))

#define likely(x)             __builtin_expect(!!(x), 1)
#define unlikely(x)           __builtin_expect(!!(x), 0)

// ##############################################################

#define __init                __section(".data.init")


#define NULL                  (void *)0

// ##############################################################
#endif /* _TINY_H_ */
