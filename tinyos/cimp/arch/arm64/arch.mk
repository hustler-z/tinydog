asm-y += vector.o
asm-y += smccc.o
asm-y += cache.o
asm-y += debug.o
asm-y += timer.o
asm-y += head.o
asm-y += mmu.o

arm-y += exception.o
arm-y += interrupt.o
arm-y += memory.o
arm-y += trace.o
arm-y += gicv3.o
arm-y += psci.o
arm-y += cpu.o

lds-y += tiny.lds
