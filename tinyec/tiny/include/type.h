// @Hustler's Project

#ifndef _TINY_TYPE_H_
#define _TINY_TYPE_H_

#define NULL                  (void *)0

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

typedef u64                   addr_t;

#endif /* _TINY_TYPE_H_ */
