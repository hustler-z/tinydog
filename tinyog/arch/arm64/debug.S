#include <asm/asm.h>
#include <serial.h>
#include <rk3566.h>

// ####################################################

    __BUGS

.macro __asm_uart_wake__ xb c
1:
    ldrb  w\c, [\xb, #UART_LSR]
    and   w\c, w\c, #UART_LSR_THRE
    cmp   w\c, #UART_LSR_THRE
    bne   1b
.endm

.macro __asm_uart_xfer__ xb wt
    strb  \wt, [\xb, #UART_THR]
.endm

FUNC(__asm_uart_init__)
    ldr   x23, =DEBUG_SERIAL

    DBG("[tiny] uart enabled\r\n")

    ret
END(__asm_uart_init__)

// ####################################################

FUNC(__asm_outs__)
    __asm_uart_wake__ x23, 1
    ldrb  w1, [x0], #1
    cbz   w1, 1f
    __asm_uart_xfer__ x23, w1
    b     __asm_outs__
1:
    ret
END(__asm_outs__)

FUNC(__asm_outc__)
    ldr   x15, =DEBUG_SERIAL
    __asm_uart_wake__ x15, 1
    __asm_uart_xfer__ x15, w0
    ret
END(__asm_outc__)

FUNC(__asm_flush__)
    ldr   x15, =DEBUG_SERIAL
    __asm_uart_wake__ x15, 1
    ret
END(__asm_flush__)

FUNC(__asm_hex__)
    adr_l x1, hex
    mov   x3, #16
1:
    __asm_uart_wake__ x23, 2
    and   x2, x0, #(0x0F << 60)
    lsr   x2, x2, #60
    ldrb  w2, [x1, x2]
    __asm_uart_xfer__ x23, w2
    lsl   x0, x0, #4
    subs  x3, x3, #1
    bne   1b
    ret
END(__asm_hex__)

RODATA(.rodata.debug, hex, "0123456789abcdef")

// ####################################################
