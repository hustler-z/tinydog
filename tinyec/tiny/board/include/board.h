// @Hustler's Project

#ifndef _BOARD_H_
#define _BOARD_H_

#include <rk3566.h>
#include <type.h>

struct board {
    const char *name;
    const addr_t pa_start;
    const addr_t pa_end;
};

#endif /* _BOARD_H_ */
