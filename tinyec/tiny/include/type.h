// @Hustler's Project

#ifndef _TINY_TYPE_H_
#define _TINY_TYPE_H_

#define NULL                  ((void *)0)

typedef char                  i8;
typedef short                 i16;
typedef int                   i32;
typedef long                  i64;

typedef unsigned char         u8;
typedef unsigned short        u16;
typedef unsigned int          u32;
typedef unsigned long         u64;

#define _ro                   volatile const
#define _rw                   volatile
#define _wo                   volatile

typedef u64                   register_t;

typedef u64                   size_t;

// address pointer
typedef u64 __attribute__((__mode__(__pointer__))) addr_t;

typedef enum {
    true = 0,
    false,
} bool;

#define __void__(x)           ((void *)(unsigned long)(x))

#define __stringify_1(x...)   #x
#define __stringify(x...)     __stringify_1(x)

#endif /* _TINY_TYPE_H_ */
