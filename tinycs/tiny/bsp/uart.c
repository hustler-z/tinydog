// @Hustler's Project

#include <asm/io.h>
#include <rk3566.h>
#include <print.h>
#include <uart.h>
#include <util.h>

struct uart debug_uart = {
    .baudrate = DEBUG_UART_BAUDRATE,
    .clock_hz = DEBUG_UART_CLOCK,
    .access = (struct rk_uart *)UART2_BASE,
};

/**
 * baud rate = (serial clock frequency) / (16 * divisor)
 */
int __calc_divisor(int clock, int baudrate) {
    return DIV_ROUND_CLOSEST(clock, 16 * baudrate);
}

// @Hustler
void debug_uart_init(void) {

}
