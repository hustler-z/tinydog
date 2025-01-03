// @Hustler's Project

#ifndef _BSP_HAL_H_
#define _BSP_HAL_H_

// ####################################################

#include <rk3566.h>
#include <type.h>

// @Hustler                     512MB
#define MEMORY_REGION_SIZE      _AC(1024 * 1024 * 512, UL)

struct hal {
    const char *name;
    addr_t pa_start;
    addr_t pa_end;
};

int hal_init(void);

// ####################################################

#endif /* _BSP_HAL_H_ */
