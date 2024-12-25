#include <asm/asm.h>

// ####################################################

#define UART_LSR         0x05 /* Line Status */
#define UART_LSR_THRE    0x20 /* Transmit Hold Reg Empty */
#define UART_THR         0x00 /* Transmit Holding */
#define DEBUG_SERIAL     0xFE660000

.macro _uart_wake xb c
1:
    ldrb  w\c, [\xb, #UART_LSR]
    and   w\c, w\c, #UART_LSR_THRE
    cmp   w\c, #UART_LSR_THRE
    bne   1b
.endm

.macro _uart_xfer xb wt
    strb  \wt, [\xb, #UART_THR]
.endm

FUNC(_uart_init)
    ldr   x23, =DEBUG_SERIAL

    DBG("[tiny] uart enabled\r\n")

    ret
END(_uart_init)

// ####################################################

FUNC(_outs)
    _uart_wake x23, 1
    ldrb  w1, [x0], #1
    cbz   w1, 1f
    _uart_xfer x23, w1
    b     _outs
1:
    ret
END(_outs)

FUNC(_outc)
    ldr   x15, =DEBUG_SERIAL
    _uart_wake x15, 1
    _uart_xfer x15, w0
    ret
END(_outc)

FUNC(_flush)
    ldr   x15, =DEBUG_SERIAL
    _uart_wake x15, 1
    ret
END(_flush)

FUNC(_outx)
    adr_l x1, hex
    mov   x3, #16
1:
    _uart_wake x23, 2
    and   x2, x0, #(0xF<<60)
    lsr   x2, x2, #60
    ldrb  w2, [x1, x2]
    _uart_xfer x23, w2
    lsl   x0, x0, #4
    subs  x3, x3, #1
    bne   1b
    ret
END(_outx)

RODATA(.rodata.debug, hex, "0123456789abcdef")

// ####################################################
