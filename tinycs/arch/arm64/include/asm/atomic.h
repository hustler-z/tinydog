// @Hustler's Project

#ifndef _ASM_ATOMIC_H_
#define _ASM_ATOMIC_H_

// ####################################################

#include <asm/barrier.h>

#define declare_atomic_read(name, size, width, type) \
static inline type name(const volatile type *addr)   \
{                                                    \
    type ret;                                        \
    asm volatile("ldr" size " %" width(0) ",%1"      \
    : "=r" (ret)                                     \
    : "m" (*addr));                                  \
    return ret;                                      \
}

#define declare_atomic_write(name, size, width, type)  \
static inline void name(volatile type *addr, type val) \
{                                                      \
    asm volatile("str" size " %" width(1) ",%0"        \
    : "=m" (*addr)                                     \
    : "r" (val));                                      \
}

#define build_add_sized(name, size, width, type)                    \
static inline void name(volatile type *addr, type val)              \
{                                                                   \
    type t;                                                         \
    asm volatile("ldr" size " %" width(1) ",%0\n"                   \
                 "add %" width(1) ",%" width(1) ",%" width(2) "\n"  \
                 "str" size " %" width(1) ",%0"                     \
                 : "+m" (*addr), "=&r" (t)                          \
                 : "ri" (val));                                     \
}

#define BYTE(n)  "w" #n
#define WORD(n)  "w" #n
#define DWORD(n) "" #n
#define PAIR     ""

declare_atomic_read(read_u8_atomic, "b", BYTE, u8)
declare_atomic_read(read_u16_atomic, "h", WORD, u16)
declare_atomic_read(read_u32_atomic, "", WORD, u32)
declare_atomic_read(read_u64_atomic, "", DWORD, u64)

declare_atomic_write(write_u8_atomic, "b", BYTE, u8)
declare_atomic_write(write_u16_atomic, "h", WORD, u16)
declare_atomic_write(write_u32_atomic, "", WORD, u32)
declare_atomic_write(write_u64_atomic, "", DWORD, u64)

build_add_sized(add_u8_sized, "b", BYTE, u8)
build_add_sized(add_u16_sized, "h", WORD, u16)
build_add_sized(add_u32_sized, "", WORD, u32)

void bad_atomic_size(void);
void bad_atomic_read(const volatile void *p, void *res);

static __always_inline void read_atomic_size(volatile void *p,
        void *res, unsigned int size)
{
    switch (size) {
    case 1:
        *(u8 *)res = read_u8_atomic(p);
        break;
    case 2:
        *(u16 *)res = read_u16_atomic(p);
        break;
    case 4:
        *(u32 *)res = read_u32_atomic(p);
        break;
    case 8:
        *(u64 *)res = read_u64_atomic(p);
        break;
    default:
        bad_atomic_read(p, res);
        break;
    }
}

static __always_inline void write_atomic_size(volatile void *p,
        void *val, unsigned int size)
{
    switch (size) {
    case 1:
        write_u8_atomic(p, *(u8 *)val);
        break;
    case 2:
        write_u16_atomic(p, *(u16 *)val);
        break;
    case 4:
        write_u32_atomic(p, *(u32 *)val);
        break;
    case 8:
        write_u64_atomic(p, *(u64 *)val);
        break;
    default:
        bad_atomic_size();
        break;
    }
}

#define read_atomic(p) ({                                   \
    union { typeof(*(p)) val; char c[0]; } x_;              \
    read_atomic_size(p, x_.c, sizeof(*(p)));                \
    x_.val;                                                 \
})

#define write_atomic(p, x)                                  \
    do {                                                    \
        typeof(*(p)) x_ = (x);                              \
        write_atomic_size(p, &x_, sizeof(*(p)));            \
    } while (false)

#define add_sized(p, x) ({                                  \
    typeof(*(p)) __x = (x);                                 \
    switch (sizeof(*(p))) {                                 \
    case 1: add_u8_sized((u8 *)(p), __x); break;            \
    case 2: add_u16_sized((u16 *)(p), __x); break;          \
    case 4: add_u32_sized((u32 *)(p), __x); break;          \
    default: bad_atomic_size(); break;                      \
    }                                                       \
})

#define ATOMIC_INIT(i)  { (i) }

typedef struct { int counter; } atomic_t;

static inline int atomic_read(const atomic_t *v)
{
    return *(const volatile int *)&v->counter;
}

static inline int _atomic_read(atomic_t v)
{
    return v.counter;
}

static inline void atomic_set(atomic_t *v, int i)
{
    v->counter = i;
}

static inline void _atomic_set(atomic_t *v, int i)
{
    v->counter = i;
}

static inline void atomic_add(int i, atomic_t *v)
{
    unsigned long tmp;
    int result;

    asm volatile("// atomic_add\n"
"1: ldxr  %w0, %2\n"
"   add   %w0, %w0, %w3\n"
"   stxr  %w1, %w0, %2\n"
"   cbnz  %w1, 1b"
    : "=&r" (result), "=&r" (tmp), "+Q" (v->counter)
    : "Ir" (i));
}

static inline int atomic_add_return(int i, atomic_t *v)
{
    unsigned long tmp;
    int result;

    asm volatile("// atomic_add_return\n"
"1: ldxr  %w0, %2\n"
"   add   %w0, %w0, %w3\n"
"   stlxr %w1, %w0, %2\n"
"   cbnz  %w1, 1b"
    : "=&r" (result), "=&r" (tmp), "+Q" (v->counter)
    : "Ir" (i)
    : "memory");

    smp_mb();
    return result;
}

static inline void atomic_sub(int i, atomic_t *v)
{
    unsigned long tmp;
    int result;

    asm volatile("// atomic_sub\n"
"1: ldxr  %w0, %2\n"
"   sub   %w0, %w0, %w3\n"
"   stxr  %w1, %w0, %2\n"
"   cbnz  %w1, 1b"
    : "=&r" (result), "=&r" (tmp), "+Q" (v->counter)
    : "Ir" (i));
}

static inline int atomic_sub_return(int i, atomic_t *v)
{
    unsigned long tmp;
    int result;

    asm volatile("// atomic_sub_return\n"
"1: ldxr  %w0, %2\n"
"   sub   %w0, %w0, %w3\n"
"   stlxr %w1, %w0, %2\n"
"   cbnz  %w1, 1b"
    : "=&r" (result), "=&r" (tmp), "+Q" (v->counter)
    : "Ir" (i)
    : "memory");

    smp_mb();
    return result;
}

static inline void atomic_and(int m, atomic_t *v)
{
    unsigned long tmp;
    int result;

    asm volatile("// atomic_and\n"
"1: ldxr  %w0, %2\n"
"   and   %w0, %w0, %w3\n"
"   stxr  %w1, %w0, %2\n"
"   cbnz  %w1, 1b"
    : "=&r" (result), "=&r" (tmp), "+Q" (v->counter)
    : "Ir" (m));
}

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
    unsigned long tmp;
    int oldval;

    smp_mb();

    asm volatile("// atomic_cmpxchg\n"
"1: ldxr  %w1, %2\n"
"   cmp   %w1, %w3\n"
"   b.ne  2f\n"
"   stxr  %w0, %w4, %2\n"
"   cbnz  %w0, 1b\n"
"2:"
    : "=&r" (tmp), "=&r" (oldval), "+Q" (v->counter)
    : "Ir" (old), "r" (new)
    : "cc");

    smp_mb();
    return oldval;
}

static inline int __atomic_add_unless(atomic_t *v,
                                      int a, int u)
{
    int c, old;

    c = atomic_read(v);
    while (c != u && (old =
                      atomic_cmpxchg((v), c, c + a)) != c)
        c = old;
    return c;
}

static inline int atomic_sub_and_test(int i, atomic_t *v)
{
    return atomic_sub_return(i, v) == 0;
}

static inline void atomic_inc(atomic_t *v)
{
    atomic_add(1, v);
}

static inline int atomic_inc_return(atomic_t *v)
{
    return atomic_add_return(1, v);
}

static inline int atomic_inc_and_test(atomic_t *v)
{
    return atomic_add_return(1, v) == 0;
}

static inline void atomic_dec(atomic_t *v)
{
    atomic_sub(1, v);
}

static inline int atomic_dec_return(atomic_t *v)
{
    return atomic_sub_return(1, v);
}

static inline int atomic_dec_and_test(atomic_t *v)
{
    return atomic_sub_return(1, v) == 0;
}

static inline int atomic_add_negative(int i, atomic_t *v)
{
    return atomic_add_return(i, v) < 0;
}

static inline int atomic_add_unless(atomic_t *v, int a, int u)
{
    return __atomic_add_unless(v, a, u);
}

// ####################################################

#endif /* _ASM_ATOMIC_H_ */
