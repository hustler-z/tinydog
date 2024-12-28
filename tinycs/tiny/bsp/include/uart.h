// @Hustler

#ifndef _BSP_UART_H_
#define _BSP_UART_H_

// @NS16550 base on rk3568 TRM
#include <type.h>

// ####################################################

#define DEBUG_UART_BAUDRATE         _AC(1500000, UL)
#define DEBUG_UART_CLOCK            _AC(1843200, UL)

struct rk_uart {
    union {
        __RO  u32 rbr;              /* Address Offset: 0x0000 */
        __RW  u32 dll;              /* Address Offset: 0x0000 */
        __WO  u32 thr;              /* Address Offset: 0x0000 */
    };
    union {
        __RW  u32 dlh;              /* Address Offset: 0x0004 */
        __RW  u32 ier;              /* Address Offset: 0x0004 */
    };
    union {
        __WO  u32 fcr;              /* Address Offset: 0x0008 */
        __RO  u32 iir;              /* Address Offset: 0x0008 */
    };
    __RW  u32 lcr;                  /* Address Offset: 0x000C */
    __RW  u32 mcr;                  /* Address Offset: 0x0010 */
    __RO  u32 lsr;                  /* Address Offset: 0x0014 */
    __RO  u32 msr;                  /* Address Offset: 0x0018 */
    __RW  u32 scr;                  /* Address Offset: 0x001C */
          u32 reserved0020[4];      /* Address Offset: 0x0020 */
    union {
        __RO  u32 srbr;             /* Address Offset: 0x0030 */
        __WO  u32 sthr;             /* Address Offset: 0x0030 */
    };
          u32 reserved0034[15];     /* Address Offset: 0x0034 */
    __RW  u32 far;                  /* Address Offset: 0x0070 */
    __RO  u32 tfr;                  /* Address Offset: 0x0074 */
    __WO  u32 rfw;                  /* Address Offset: 0x0078 */
    __RO  u32 usr;                  /* Address Offset: 0x007C */
    __RO  u32 tfl;                  /* Address Offset: 0x0080 */
    __RO  u32 rfl;                  /* Address Offset: 0x0084 */
    __WO  u32 srr;                  /* Address Offset: 0x0088 */
    __RW  u32 srts;                 /* Address Offset: 0x008C */
    __RW  u32 sbcr;                 /* Address Offset: 0x0090 */
    __RW  u32 sdmam;                /* Address Offset: 0x0094 */
    __RW  u32 sfe;                  /* Address Offset: 0x0098 */
    __RW  u32 srt;                  /* Address Offset: 0x009C */
    __RW  u32 stet;                 /* Address Offset: 0x00A0 */
    __RW  u32 htx;                  /* Address Offset: 0x00A4 */
    __WO  u32 dmasa;                /* Address Offset: 0x00A8 */
          u32 reserved00ac[18];     /* Address Offset: 0x00AC */
    __RO  u32 cpr;                  /* Address Offset: 0x00F4 */
    __RO  u32 ucv;                  /* Address Offset: 0x00F8 */
    __RO  u32 ctr;                  /* Address Offset: 0x00FC */
};

struct uart {
    const int baudrate;
    const int clock_hz;
    int data_bits;
    int parity;
    int stop_bits;
    int fifo_size;
    int irq;

    // @Hustler
    //
    // rockchip uart memory map
    struct rk_uart *access;
};

// ####################################################

#endif /* _BSP_UART_H_ */
