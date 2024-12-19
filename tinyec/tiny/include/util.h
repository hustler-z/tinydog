// @Hustler's Project

#ifndef _TINY_UTILS_H_
#define _TINY_UTILS_H_

#include <type.h>

#define offsetof(type, member) ((size_t)&((type *)0)->member)

#define container_of(ptr, type, member) ({              \
    const typeof(((type *)0)->member) *__mptr = (ptr);  \
    (type *)((char *)__mptr - offsetof(type, member)); })

#define abs(x) ({        \
    typeof(x) x_ = (x);  \
    (x_ < 0) ? -x_ : x_; \
})

#define swap(a, b)       \
    do { typeof(a) t_ = (a); (a) = (b); (b) = t_; } while(0)

#endif /* _TINY_UTILS_H_ */
