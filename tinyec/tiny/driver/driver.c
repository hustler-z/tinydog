// @Hustler's Project

#include <device.h>

struct tiny_device_plat {
    struct list_head primary_device;
};

// TODO
static struct tiny_device_plat tiny_plat = {
    .primary_device = { NULL, NULL },
};
