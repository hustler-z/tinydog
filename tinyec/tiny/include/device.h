// @Hustler's Project

#ifndef _TINY_DEVICE_H_
#define _TINY_DEVICE_H_

#include <list.h>

struct tiny_driver;
struct driver_ops;

struct tiny_device {
    const char *name;
    const struct tiny_driver *primary_driver;
    struct list_head head;
};

struct tiny_driver {
    const char *name;
    struct driver_ops *ops;
};

struct driver_ops {
    int (*probe)(struct tiny_device *dev);
    int (*bind)(struct tiny_device *dev);
    int (*remove)(struct tiny_device *dev);

    // specific ops related to the driver
    const void *tiny_ops;
};


#endif /* _TINY_DEVICE_H_ */
