// @Hustler's Project

#ifndef _BSP_H_
#define _BSP_H_

// ####################################################

#include <rk3566.h>
#include <type.h>

// @Hustler                     512MB
#define MEMORY_REGION_SIZE      _AC(1024 * 1024 * 512, UL)

struct bsp {
    const char *name;
    addr_t pa_start;
    addr_t pa_end;
};

int bsp_init(void);

// ####################################################

#endif /* _BSP_H_ */
