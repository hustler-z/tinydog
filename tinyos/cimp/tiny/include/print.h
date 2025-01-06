// @Hustler's Project

#ifndef _TINY_PRINT_H_
#define _TINY_PRINT_H_

// ####################################################

#include <type.h>

int snprint(char *buf, size_t size, const char *fmt, ...);
void __print__(const char *fmt, ...);
#define tiny_print(fmt, ...) __print__("[T] "fmt, ##__VA_ARGS__)
#define tiny_error(fmt, ...) __print__("[E] "fmt, ##__VA_ARGS__)
#define tiny_debug(fmt, ...) __print__("[D] "fmt, ##__VA_ARGS__)

int prev_print_init(void);
int post_print_init(void);

// ####################################################

#endif /* _TINY_PRINT_H_ */
