// @Hustler's Project

#include <compiler.h>
#include <rk3566.h>
#include <print.h>
#include <uart.h>
#include <util.h>

static int set_baudrate(struct uart_dev *dev) {
    u32 div_latch = 0;

    div_latch = DIV_ROUND_CLOSEST(dev->cfg.clock_hz, (16 * dev->cfg.baudrate));

    dev->reg->mcr |= UART_MCR_LOOP;
    dev->reg->lcr |= UART_LCR_DLAB;

    dev->reg->dll = div_latch & 0xFF;
    dev->reg->dlh = (div_latch >> 8) & 0xFF;

    dev->reg->lcr &= ~UART_LCR_DLAB;
    dev->reg->mcr &= ~UART_MCR_LOOP;

    return 0;
}

static int set_lcr(struct uart_dev *dev) {
    u32 lcr = 0;
    int ret = 0;

    switch (dev->cfg.databit) {
    case UART_DATA_5B:
        lcr |= UART_LCR_WLEN5;
        break;
    case UART_DATA_6B:
        lcr |= UART_LCR_WLEN6;
        break;
    case UART_DATA_7B:
        lcr |= UART_LCR_WLEN7;
        break;
    case UART_DATA_8B:
        lcr |= UART_LCR_WLEN8;
        break;
    default:
        ret = -1;
        break;
    }

    switch (dev->cfg.parity) {
    case UART_ODD_PARITY:
    case UART_EVEN_PARITY:
        lcr |= UART_LCR_PARITY;
        lcr |= ((dev->cfg.parity) << 4);
        break;
    case UART_NONE_PARITY:
        lcr &= ~UART_LCR_PARITY;
        break;
    default:
        ret = -1;
        break;
    }

    if (dev->cfg.stopbit == UART_ONE_AND_HALF_OR_TWO_STOPBIT)
        lcr |= UART_LCR_STOP;

    dev->reg->lcr = lcr;

    return ret;
}

static void set_fcr(struct uart_reg *reg) {
    reg->fcr = UART_FCR_ENABLE_FIFO | UART_FCR_R_TRIG_10 | UART_FCR_T_TRIG_10;
}

static void __uart_cout(struct uart_reg *reg, const char c) {
    while (!(reg->usr & UART_USR_TX_FIFO_NOT_FULL))
        ;

    reg->thr = (u32)c;
}

static char __uart_cin(struct uart_reg *reg) {
    while (!(reg->usr & UART_USR_RX_FIFO_NOT_EMPTY))
        break;

    return (char)reg->rbr;
}

static int __uart_outs(struct uart_reg *reg, const char *msg, u32 len) {
    int count = 0;

    while (len--) {
        while (!(reg->usr & UART_USR_TX_FIFO_NOT_FULL))
            ;

        reg->thr = *msg++;
        count++;
    }

    return count;
}

static int uart_init(struct uart_dev *dev) {
    int ret = 0;
    struct uart_reg *reg = dev->reg;

    set_fcr(dev->reg);

    ret = set_lcr(dev);

    set_baudrate(dev);

    return ret;
}

// ####################################################

// @debug uart
static struct uart_dev debug_uart = {
    .reg = (struct uart_reg *)UART2_BASE,
    .cfg = {
        .baudrate = UART_BR_1500K,
        .clock_hz = UART_CLK_HZ,
        .databit = UART_DATA_8B,
        .stopbit = UART_ONE_STOPBIT,
        .parity = UART_NONE_PARITY,
        .irq = UART2_IRQ,
    },
};

static char debug_cin(void) {
    return __uart_cin(debug_uart.reg);
}

static void debug_cout(char c) {
    __uart_cout(debug_uart.reg, c);
}

static void debug_outs(const char *msg) {
    int len = 0, real_len = 0;

    len = strlen(msg);
    __uart_outs(debug_uart.reg, msg, len);
}

int __init debug_uart_init(void) {
    int ret = -1;

    if (!uart_init(&debug_uart)) {
        set_cin(debug_cin);
        set_cout(debug_cout);
        post_print_init(debug_outs);

        ret = 0;
    }

    return ret;
}

// ####################################################
