// @Hustler's Project

#ifndef _TINY_UTIL_H_
#define _TINY_UTIL_H_

#include <type.h>

// ####################################################

#define offsetof(type, member) ((size_t)&((type *)0)->member)

#define container_of(ptr, type, member) ({              \
    const typeof(((type *)0)->member) *__mptr = (ptr);  \
    (type *)((char *)__mptr - offsetof(type, member)); })

#define swap(a, b)       \
    do { typeof(a) t_ = (a); (a) = (b); (b) = t_; } while(0)

#define _U  0x01    /* upper */
#define _L  0x02    /* lower */
#define _D  0x04    /* digit */
#define _C  0x08    /* cntrl */
#define _P  0x10    /* punct */
#define _S  0x20    /* white space (space/lf/tab) */
#define _X  0x40    /* hex digit */
#define _SP 0x80    /* hard space (0x20) */

extern unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])

#define isalnum(c)  ((__ismask(c)&(_U|_L|_D)) != 0)
#define isalpha(c)  ((__ismask(c)&(_U|_L)) != 0)
#define iscntrl(c)  ((__ismask(c)&(_C)) != 0)
#define isdigit(c)  ((__ismask(c)&(_D)) != 0)
#define isgraph(c)  ((__ismask(c)&(_P|_U|_L|_D)) != 0)
#define islower(c)  ((__ismask(c)&(_L)) != 0)
#define isprint(c)  ((__ismask(c)&(_P|_U|_L|_D|_SP)) != 0)
#define ispunct(c)  ((__ismask(c)&(_P)) != 0)
#define isspace(c)  ((__ismask(c)&(_S)) != 0)
#define isupper(c)  ((__ismask(c)&(_U)) != 0)
#define isxdigit(c) ((__ismask(c)&(_D|_X)) != 0)

#define isblank(c)  (c == ' ' || c == '\t')

#define isascii(c) (((unsigned char)(c)) <= 0x7F)
#define toascii(c) (((unsigned char)(c)) & 0x7F)

static inline unsigned char __tolower(unsigned char c)
{
    if (isupper(c))
        c -= 'A'-'a';
    return c;
}

static inline unsigned char __toupper(unsigned char c)
{
    if (islower(c))
        c -= 'a'-'A';
    return c;
}

#define tolower(c) __tolower(c)
#define toupper(c) __toupper(c)

#define ASSERT(exp)
#define BUG_ON(exp)

// ####################################################

#define do_div(n,base) ({            \
    u32 __base = (base);             \
    u32 __rem;                       \
    __rem = ((u64)(n)) % __base;     \
    (n) = ((u64)(n)) / __base;       \
    __rem;                           \
})

#define max(x, y) ({\
    typeof(x) _max1 = (x);           \
    typeof(y) _max2 = (y);           \
    (void) (&_max1 == &_max2);       \
    _max1 > _max2 ? _max1 : _max2; })

#define min(x, y) ({                 \
    typeof(x) _min1 = (x);           \
    typeof(y) _min2 = (y);           \
    (void) (&_min1 == &_min2);       \
    _min1 < _min2 ? _min1 : _min2; })

#define ROUNDUP(x, y) ({             \
    const typeof(y) __y = y;         \
    (((x) + (__y - 1)) / __y) * __y; \
})

#define ROUNDDOWN(x, y) ({\
	typeof(x) __x = (x);\
	__x - (__x % (y));\
})

#define min_t(type,x,y) \
        ({ type __x = (x); type __y = (y); __x < __y ? __x: __y; })
#define max_t(type,x,y) \
        ({ type __x = (x); type __y = (y); __x > __y ? __x: __y; })

#define REPEAT_BYTE(x)	     ((~0ul / 0xff) * (x))

#define ALIGN(x,a)           __ALIGN_MASK((x),(typeof(x))(a)-1)
#define ALIGN_DOWN(x, a)     ALIGN((x) - ((a) - 1), (a))
#define __ALIGN_MASK(x,mask) (((x)+(mask))&~(mask))
#define PTR_ALIGN(p, a)      ((typeof(p))ALIGN((unsigned long)(p), (a)))
#define IS_ALIGNED(x, a)     (!((x) & ((typeof(x))(a) - 1)))

#define ARRAY_SIZE(x)        (sizeof(x) / sizeof((x)[0]))

#define __round_mask(x, y)   ((__typeof__(x))((y)-1))
#define round_up(x, y)       ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y)     ((x) & ~__round_mask(x, y))

#define FIELD_SIZEOF(t, f)   (sizeof(((t*)0)->f))
#define DIV_ROUND_UP(n,d)    (((n) + (d) - 1) / (d))

#define DIV_ROUND_DOWN_ULL(ll, d) \
	({ unsigned long long _tmp = (ll); do_div(_tmp, d); _tmp; })

#define DIV_ROUND_UP_ULL(ll, d) \
    DIV_ROUND_DOWN_ULL((ll) + (d) - 1, (d))

#define ROUND(a, b)  (((a) + (b) - 1) & ~((b) - 1))

#define DIV_ROUND_CLOSEST(x, divisor) ({              \
    typeof(x) __x = x;                                \
    typeof(divisor) __d = divisor;                    \
    (((typeof(x))-1) > 0 ||                           \
        ((typeof(divisor))-1) > 0 || (__x) > 0) ?     \
        (((__x) + ((__d) / 2)) / (__d)) :             \
        (((__x) - ((__d) / 2)) / (__d));              \
})

#define DIV_ROUND_CLOSEST_ULL(x, divisor) ({          \
    typeof(divisor) __d = divisor;                    \
    unsigned long long _tmp = (x) + (__d) / 2;        \
    do_div(_tmp, __d);                                \
    _tmp;                                             \
})

#define mult_frac(x, numer, denom) ({                 \
    typeof(x) quot = (x) / (denom);                   \
    typeof(x) rem  = (x) % (denom);                   \
    (quot * (numer)) + ((rem * (numer)) / (denom));   \
})

#define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((u32)(n))

#define abs(x) ({                                     \
    long ret;                                         \
    if (sizeof(x) == sizeof(long)) {                  \
        long __x = (x);                               \
        ret = (__x < 0) ? -__x : __x;                 \
    } else {                                          \
        int __x = (x);                                \
        ret = (__x < 0) ? -__x : __x;                 \
    }                                                 \
    ret;                                              \
})

#define abs64(x) ({                                   \
    s64 __x = (x);                                    \
    (__x < 0) ? -__x : __x;                           \
})

#define __find_closest(x, a, as, op) ({                  \
    typeof(as) __fc_i, __fc_as = (as) - 1;               \
    typeof(x) __fc_x = (x);                              \
    typeof(*a) const *__fc_a = (a);                      \
    for (__fc_i = 0; __fc_i < __fc_as; __fc_i++) {       \
        if (__fc_x op DIV_ROUND_CLOSEST(__fc_a[__fc_i] + \
                        __fc_a[__fc_i + 1], 2))          \
            break;                                       \
    }                                                    \
    (__fc_i);                                            \
})

#define find_closest(x, a, as) \
    __find_closest(x, a, as, <=)

// ####################################################

#endif /* _TINY_UTIL_H_ */
