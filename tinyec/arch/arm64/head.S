// @Hustler's Project

#include <asm/asm.h>

#define MPIDR_EL1_AFF0          0         // [07:00]
#define MPIDR_EL1_AFF1          8         // [15:08]
#define MPIDR_EL1_AFF2          16        // [23:16]
#define MPIDR_EL1_AFF3          32        // [39:32]

    __HEAD

FUNC(_head)
    add  x13, x18, #0x16
    b    _entry
END(_head)
    .long  0
    .quad  __tiny_end - _head
    .quad  0
    .quad  0
    .quad  0
    .quad  0
    .ascii "ARM64"
    .long  0

_entry:
    ldr  x0, =_head
    adr  x19, _head
    sub  x20, x19, x0

    // disable all exception
    msr  DAIFSet, #0x0F

    // debug uart initialization
    bl   _uart_init

    // @Hustler
    //
    // check current exception level
    //
    // CurrentEL [3:2]
    // ob0100    0x04      El1
    // 0b1000    0x08      El2
    // 0b1100    0x0c      El3
    mrs  x1, CurrentEL
    cmp  x1, #0x08
    b.eq 1f

    DBG("[tiny] not on el2\r\n")

2:  wfe
    b    2b

1:
    // disable MMU, dcache, icache
    mrs  x1, SCTLR_EL2
    bic  x1, x1, #MMU
    bic  x1, x1, #ICACHE
    bic  x1, x1, #DCACHE
    msr  SCTLR_EL2, x1





/**
 * ubfx
 */
FUNC(cpu_affinity)
    // cpu affinity
    mrs  x4, MPIDR_EL1
    ubfx x5, x4, #32, #MPIDR_EL1_AFF1
    bfi  w4, w5, #24, #MPIDR_EL1_AFF1
    ret
END(cpu_affinity)
