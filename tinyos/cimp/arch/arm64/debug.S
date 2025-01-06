
#include <asm/offset.h>
#include <asm/entry.h>
#include <asm/asm.h>

// ####################################################

#define UART_LSR          0x14 // Line status register
#define UART_LSR_THRE     0x20 // Indicates THR or TX FIFO is empty
#define UART_THR          0x00

.macro __asm_uart_kick__ xb c
1:
    ldrb  w\c, [\xb, #UART_LSR]
    and   w\c, w\c, #UART_LSR_THRE
    cmp   w\c, #UART_LSR_THRE
    bne   1b
.endm

// data output
.macro __asm_uart_xfer__ xb wt
    strb  \wt, [\xb, #UART_THR]
.endm

// ####################################################

// set up debug uart address base
FUNC(__asm_uart_init__)
    ldr   x23, =__DBG_UART_BA__
    ret
END(__asm_uart_init__)

FUNC(__asm_outs__)
    __asm_uart_kick__ x23, 1
    ldrb  w1, [x0], #1
    cbz   w1, 1f
    __asm_uart_xfer__ x23, w1
    b     __asm_outs__
1:
    ret
END(__asm_outs__)

FUNC(__asm_outc__)
    ldr   x15, =__DBG_UART_BA__
    __asm_uart_kick__ x15, 1
    __asm_uart_xfer__ x15, w0
    ret
END(__asm_outc__)

FUNC(__asm_flush__)
    ldr   x15, =__DBG_UART_BA__
    __asm_uart_kick__ x15, 1
    ret
END(__asm_flush__)

FUNC(__asm_hex__)
    ldr   x1, hex
    mov   x3, #16
1:
    __asm_uart_kick__ x23, 2
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

// store current execution context
FUNC(__store_context__)
    dmb   sy

    str   x29, [x0, #(LR_OFFSET - 8)]

    mov   x1, sp
    str   x1, [x0, #SP_OFFSET]

    mov   x1, lr
    str   x1, [x0, #LR_OFFSET]

#ifdef __NORMAL_OS__
    mrs   x1, ESR_EL1
    str   x1, [x0, #ESR_OFFSET]

    mrs   x1, SPSR_EL1
    str   x1, [x0, #CPSR_OFFSET]
#else
    mrs   x1, ESR_EL2
    str   x1, [x0, #ESR_OFFSET]

    mrs   x1, SPSR_EL2
    str   x1, [x0, #CPSR_OFFSET]
#endif

    ret
END(__store_context__)

// ####################################################
