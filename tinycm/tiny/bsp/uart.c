// @Hustler's Project

#include <asm/io.h>

#include <compiler.h>
#include <rk3566.h>
#include <string.h>
#include <print.h>
#include <uart.h>
#include <util.h>

// ####################################################

#define DEBUG_UART_BASE       UART2_BASE

#define UART_LCRVAL           UART_LCR_8N1
#define UART_MCRVAL           (UART_MCR_DTR | UART_MCR_RTS)

#define serial_out(x, y)      writeb(x, y)
#define serial_in(y)          readb(y)

static inline void serial_out_shift(void *addr, int shift, int value)
{
	writel(value, addr);
}

static inline int serial_in_shift(void *addr, int shift)
{
	return readl(addr);
}

#define serial_dout(reg, value)	\
	serial_out_shift((char *)com_port + \
		((char *)reg - (char *)com_port) * \
			(1 << DEBUG_UART_SHIFT), \
		DEBUG_UART_SHIFT, value)
#define serial_din(reg) \
	serial_in_shift((char *)com_port + \
		((char *)reg - (char *)com_port) * \
			(1 << DEBUG_UART_SHIFT), \
		DEBUG_UART_SHIFT)

int __serial_calc_divisor(struct uart *port, int clock, int baudrate)
{
	const unsigned int mode_x_div = 16;

	return DIV_ROUND_CLOSEST(clock, mode_x_div * baudrate);
}

static void __serial_setbrg(struct uart *com_port, int baud_divisor)
{
	/* to keep serial format, read lcr before writing BKSE
     */
	int lcr_val = serial_in(&com_port->lcr) & ~UART_LCR_BKSE;

	serial_out(UART_LCR_BKSE | lcr_val, &com_port->lcr);
	serial_out(baud_divisor & 0xFF, &com_port->dll);
	serial_out((baud_divisor >> 8) & 0xFF, &com_port->dlm);
	serial_out(lcr_val, &com_port->lcr);
}

static u32 __serial_getfcr(struct uart *port)
{
	return UART_FCR_DEFVAL;
}

void __serial_init(struct uart *com_port, int baud_divisor)
{
	while (!(serial_in(&com_port->lsr) & UART_LSR_TEMT))
		;

	serial_out(0x00, &com_port->ier);
	serial_out(UART_MCRVAL, &com_port->mcr);
	serial_out(__serial_getfcr(com_port), &com_port->fcr);
	/* initialize serial config to 8N1 before writing baudrate
     */
	serial_out(UART_LCRVAL, &com_port->lcr);
	if (baud_divisor != -1)
		__serial_setbrg(com_port, baud_divisor);
}

void __serial_putc(struct uart *com_port, char c)
{
	while (!(serial_in(&com_port->lsr) & UART_LSR_THRE))
		;
	serial_out(c, &com_port->thr);
}

char __serial_getc(struct uart *com_port)
{
	while (!(serial_in(&com_port->lsr) & UART_LSR_DR))
        ;

	return serial_in(&com_port->rbr);
}

int __serial_tstc(struct uart *com_port)
{
	return (serial_in(&com_port->lsr) & UART_LSR_DR) != 0;
}

int __read_baud_divisor(struct uart *com_port)
{
	int ret;

	serial_dout(&com_port->lcr, UART_LCR_BKSE | UART_LCRVAL);
	ret = serial_din(&com_port->dll) & 0xFF;
	ret |= (serial_din(&com_port->dlm) & 0xFF) << 8;
	serial_dout(&com_port->lcr, UART_LCRVAL);

	return ret;
}

// ####################################################

void __init __debug_uart_init(void)
{
	struct uart *com_port = (struct uart *)DEBUG_UART_BASE;
	int baud_divisor;

	/* Wait until tx buffer is empty */
	while (!(serial_din(&com_port->lsr) & UART_LSR_TEMT))
		;

	/*
	 * We copy the code from above because it is already horribly messy.
	 * Trying to refactor to nicely remove the duplication doesn't seem
	 * feasible. The better fix is to move all users of this driver to
	 * driver model.
	 */
	baud_divisor = __serial_calc_divisor(com_port, DEBUG_UART_CLOCK,
                                  DEBUG_UART_BAUDRATE);
	serial_dout(&com_port->ier, 0x00);
	serial_dout(&com_port->mcr, UART_MCRVAL);
	serial_dout(&com_port->fcr, UART_FCR_DEFVAL);

	serial_dout(&com_port->lcr, UART_LCR_BKSE | UART_LCRVAL);
	serial_dout(&com_port->dll, baud_divisor & 0xFF);
	serial_dout(&com_port->dlm, (baud_divisor >> 8) & 0xFF);
	serial_dout(&com_port->lcr, UART_LCRVAL);
}

void __debug_uart_putc(int ch)
{
	struct uart *com_port = (struct uart *)DEBUG_UART_BASE;

	while (!(serial_din(&com_port->lsr) & UART_LSR_THRE))
        ;

	serial_dout(&com_port->thr, ch);
}

char __debug_uart_getc(void) {
	struct uart *com_port = (struct uart *)DEBUG_UART_BASE;

    while (!(serial_din(&com_port->lsr) & UART_LSR_DR))
        ;

    return serial_in(&com_port->rbr);
}

// ####################################################
