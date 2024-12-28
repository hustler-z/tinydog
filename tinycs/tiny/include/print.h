// @Hustler's Project

#ifndef _TINY_PRINT_H_
#define _TINY_PRINT_H_

#include <type.h>

int snprint(char *buf, size_t size, const char *fmt, ...);
void __print__(const char *fmt, ...);
#define tiny_print(fmt, ...) __print__("[T] "fmt, ##__VA_ARGS__)

// @Hustler
//
// This is essential before using 'tiny_print'
void set_outs(void (*func)(const char *));
void set_cin(char (*func)(void));
void set_cout(void (*func)(char));

#endif /* _TINY_PRINT_H_ */
