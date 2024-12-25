// @Hustler's Project

#ifndef _TINY_HAL_H_
#define _TINY_HAL_H_

// ####################################################

#include <list.h>

struct device;

struct driver {
    const char *name;
    void *private;
    int (*activate)(struct device *dev);
    int (*deactivate)(struct device *dev);
};

struct device {
    const char *name;
    const int type;
    struct driver *drv;
    struct list_head head;
};

struct platform {
    const char *name;
    // @Hustler - Device Map
    struct device **primary;
};

void platform_register(struct device *dev);

// ####################################################

#endif /* _TINY_HAL_H_ */
