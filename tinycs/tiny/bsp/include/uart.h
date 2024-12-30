// @Hustler

#ifndef _BSP_UART_H_
#define _BSP_UART_H_

// @NS16550 base on rk3568 TRM
#include <type.h>

enum {
    UART_DATA_5B = 5,
    UART_DATA_6B,
    UART_DATA_7B,
    UART_DATA_8B,
};

enum {
    UART_ONE_STOPBIT,
    UART_ONE_AND_HALF_OR_TWO_STOPBIT,
};

enum {
    UART_ODD_PARITY,
    UART_EVEN_PARITY,
    UART_NONE_PARITY,
};

#ifdef __RK356X__

// ####################################################

#define UART_BR_1500K           _AC(1500000, U)
#define UART_CLK_HZ             _AC(24 * 1000 * 1000, U)

/* Out: Interrupt Enable Register */
#define UART_IER_PTIME          0x80 /* Enable Programmable THRE Interrupt Mode */
#define UART_IER_MSI            0x08 /* Enable Modem status interrupt */
#define UART_IER_RLSI           0x04 /* Enable receiver line status interrupt */
#define UART_IER_THRI           0x02 /* Enable Transmitter holding register int. */
#define UART_IER_RDI            0x01 /* Enable receiver data interrupt */

/* In:  Interrupt ID Register */
#define UART_IIR_MASK           0x0f /* Interrupt ID Mask */
#define UART_IIR_NO_INT         0x01 /* No interrupts pending */
#define UART_IIR_ID             0x0e /* Mask for the interrupt ID */
#define UART_IIR_MSI            0x00 /* Modem status interrupt */
#define UART_IIR_THRI           0x02 /* Transmitter holding register empty */
#define UART_IIR_RDI            0x04 /* Receiver data interrupt */
#define UART_IIR_RLSI           0x06 /* Receiver line status interrupt */
#define UART_IIR_BUSY           0x07 /* DesignWare APB Busy Detect */
#define UART_IIR_RX_TIMEOUT     0x0c /* RX Timeout interrupt */

/* Out: FIFO Control Register */
#define UART_FCR_ENABLE_FIFO    0x01 /* Enable the FIFO */
#define UART_FCR_CLEAR_RCVR     0x02 /* Clear the RCVR FIFO */
#define UART_FCR_CLEAR_XMIT     0x04 /* Clear the XMIT FIFO */
#define UART_FCR_DMA_SELECT     0x08 /* For DMA applications */

#define UART_FCR_R_TRIG_00      0x00
#define UART_FCR_R_TRIG_01      0x40
#define UART_FCR_R_TRIG_10      0x80
#define UART_FCR_R_TRIG_11      0xc0
#define UART_FCR_T_TRIG_00      0x00
#define UART_FCR_T_TRIG_01      0x10
#define UART_FCR_T_TRIG_10      0x20
#define UART_FCR_T_TRIG_11      0x30

#define UART_FCR_TRIGGER_MASK   0xC0 /* Mask for the FIFO trigger range */
#define UART_FCR_TRIGGER_1      0x00 /* Mask for trigger set at 1 */
#define UART_FCR_TRIGGER_4      0x40 /* Mask for trigger set at 4 */
#define UART_FCR_TRIGGER_8      0x80 /* Mask for trigger set at 8 */
#define UART_FCR_TRIGGER_14     0xC0 /* Mask for trigger set at 14 */
/* 16650 definitions */
#define UART_FCR6_R_TRIGGER_8   0x00 /* Mask for receive trigger set at 1 */
#define UART_FCR6_R_TRIGGER_16  0x40 /* Mask for receive trigger set at 4 */
#define UART_FCR6_R_TRIGGER_24  0x80 /* Mask for receive trigger set at 8 */
#define UART_FCR6_R_TRIGGER_28  0xC0 /* Mask for receive trigger set at 14 */
#define UART_FCR6_T_TRIGGER_16  0x00 /* Mask for transmit trigger set at 16 */
#define UART_FCR6_T_TRIGGER_8   0x10 /* Mask for transmit trigger set at 8 */
#define UART_FCR6_T_TRIGGER_24  0x20 /* Mask for transmit trigger set at 24 */
#define UART_FCR6_T_TRIGGER_30  0x30 /* Mask for transmit trigger set at 30 */
#define UART_FCR7_64BYTE        0x20
/* Go into 64 byte mode (TI16C750 and some Freescale UARTs) */

#define UART_FCR_R_TRIG_SHIFT     6
#define UART_FCR_R_TRIG_BITS(x)   (((x)&UART_FCR_TRIGGER_MASK) >> UART_FCR_R_TRIG_SHIFT)
#define UART_FCR_R_TRIG_MAX_STATE 4

/* Out: Line Control Register */
/*
 * Note: if the word length is 5 bits (UART_LCR_WLEN5), then setting
 * UART_LCR_STOP will select 1.5 stop bits, not 2 stop bits.
 */
#define UART_LCR_DLAB           0x80 /* Divisor latch access bit */
#define UART_LCR_SBC            0x40 /* Set break control */
#define UART_LCR_SPAR           0x20 /* Stick parity (?) */
#define UART_LCR_EPAR           0x10 /* Even parity select */
#define UART_LCR_PARITY         0x08 /* Parity Enable */
#define UART_LCR_STOP           0x04 /* Stop bits: 0=1 bit, 1=2 bits */
#define UART_LCR_WLEN5          0x00 /* Wordlength: 5 bits */
#define UART_LCR_WLEN6          0x01 /* Wordlength: 6 bits */
#define UART_LCR_WLEN7          0x02 /* Wordlength: 7 bits */
#define UART_LCR_WLEN8          0x03 /* Wordlength: 8 bits */

/* Out: Modem Control Register */
#define UART_MCR_CLKSEL         0x80 /* Divide clock by 4 (TI16C752, EFR[4]=1) */
#define UART_MCR_TCRTLR         0x40 /* Access TCR/TLR (TI16C752, EFR[4]=1) */
#define UART_MCR_XONANY         0x20 /* Enable Xon Any (TI16C752, EFR[4]=1) */
#define UART_MCR_AFE            0x20 /* Enable auto-RTS/CTS (TI16C550C/TI16C750) */
#define UART_MCR_LOOP           0x10 /* Enable loopback test mode */
#define UART_MCR_OUT2           0x08 /* Out2 complement */
#define UART_MCR_OUT1           0x04 /* Out1 complement */
#define UART_MCR_RTS            0x02 /* RTS complement */
#define UART_MCR_DTR            0x01 /* DTR complement */

/* In:  Line Status Register */
#define UART_LSR_FIFOE          0x80 /* Fifo error */
#define UART_LSR_TEMT           0x40 /* Transmitter empty */
#define UART_LSR_THRE           0x20 /* Transmit-hold-register empty */
#define UART_LSR_BI             0x10 /* Break interrupt indicator */
#define UART_LSR_FE             0x08 /* Frame error indicator */
#define UART_LSR_PE             0x04 /* Parity error indicator */
#define UART_LSR_OE             0x02 /* Overrun error indicator */
#define UART_LSR_DR             0x01 /* Receiver data ready */
#define UART_LSR_BRK_ERROR_BITS 0x1E /* BI, FE, PE, OE bits */

/* In:  Modem Status Register */
#define UART_MSR_DCD            0x80 /* Data Carrier Detect */
#define UART_MSR_RI             0x40 /* Ring Indicator */
#define UART_MSR_DSR            0x20 /* Data Set Ready */
#define UART_MSR_CTS            0x10 /* Clear to Send */
#define UART_MSR_DDCD           0x08 /* Delta DCD */
#define UART_MSR_TERI           0x04 /* Trailing edge ring indicator */
#define UART_MSR_DDSR           0x02 /* Delta DSR */
#define UART_MSR_DCTS           0x01 /* Delta CTS */
#define UART_MSR_ANY_DELTA      0x0F /* Any of the delta bits! */

#define UART_USR_RX_FIFO_FULL      0x10 /* Receive FIFO full */
#define UART_USR_RX_FIFO_NOT_EMPTY 0x08 /* Receive FIFO not empty */
#define UART_USR_TX_FIFO_EMPTY     0x04 /* Transmit FIFO empty */
#define UART_USR_TX_FIFO_NOT_FULL  0x02 /* Transmit FIFO not full */
#define UART_USR_BUSY              0x01 /* UART busy indicator */

#define UART_SRR_UR                0x1 /* UART Reset */
#define UART_SRR_RFR               0X2 /* RCVR FIFO Reset */
#define UART_SRR_XFR               0x4 /* XMIT FIFO Reset */

#define MODE_X_DIV                 16 /* baud = f / 16 / div */

// ####################################################

struct uart_reg {
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

#endif /* _RK356X__ */

struct uart_cfg {
    int baudrate;
    u64 clock_hz;
    int databit;
    int stopbit;
    int parity;

    // interrupt number
    int irq;
};

struct uart_dev {
    // @Hustler
    //
    // rockchip uart memory map
    struct uart_reg *reg;
    struct uart_cfg cfg;
};

int debug_uart_init(void);

// ####################################################

#endif /* _BSP_UART_H_ */
