// @Hustler's Project

#ifndef _TINY_TYPE_H_
#define _TINY_TYPE_H_

// ####################################################

#ifndef __ASSEMBLY__
#define NULL                  ((void *)0)

typedef char                  i8;
typedef short                 i16;
typedef int                   i32;
typedef long                  i64;

typedef unsigned char         u8;
typedef unsigned short        u16;
typedef unsigned int          u32;
typedef unsigned long         u64;

#define __RO                  volatile const
#define __RW                  volatile
#define __WO                  volatile

typedef u16                   __le16;
typedef u32                   __le32;
typedef u64                   __le64;

typedef u64                   register_t;
typedef u64                   size_t;
typedef u64                   time_t;
typedef u64                   xmem_t;

// address pointer
typedef u64 __attribute__((__mode__(__pointer__))) addr_t;

typedef enum {
    false = 0,
    true,
} bool;

#define __void__(x)           ((void *)(unsigned long)(x))
#endif /* !__ASSEMBLY__ */

// ####################################################

#define __stringify_1(x...)   #x
#define __stringify(x...)     __stringify_1(x)

#ifdef __ASSEMBLY__
#define _AC(X, Y)             X
#define _AT(T, X)             X
#else
#define __AC(X, Y)            (X##Y)
#define _AC(X, Y)             __AC(X, Y)
#define _AT(T, X)             ((T)(X))
#endif

#define KB(_kb)               (_AC(_kb, UL) << 10)
#define MB(_mb)               (_AC(_mb, UL) << 20)
#define GB(_gb)               (_AC(_gb, UL) << 30)

#define BYTES_PER_LONG        8
#define BITS_PER_LONG         (BYTES_PER_LONG << 3)

#define GENMASK(h, l) \
    (((~0UL) << (l)) & (~0UL >> (BITS_PER_LONG - 1 - (h))))

// ####################################################

#endif /* _TINY_TYPE_H_ */
