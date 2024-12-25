// @Hustler's Project

#include <hal.h>
#include <list.h>
#include <print.h>

// ####################################################

enum device_type {
    UART_HAL = 0,
    I2C_HAL,
    SPI_HAL,
    MAX_HAL,
};

static struct device *device_map[MAX_HAL] = { NULL };

static struct platform tinyhal = {
    .name = "tinyhal",
    .primary = device_map,
};

void platform_register(struct device *dev) {
    struct device *primary = tinyhal.primary[dev->type];

    if (dev->type >= 0 && dev->type < MAX_HAL) {
        if (!primary)
            primary = dev;
        else
            list_add(&dev->head, &primary->head);
    }
}

// ####################################################
