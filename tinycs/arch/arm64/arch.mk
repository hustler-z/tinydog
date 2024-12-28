asm-y += vector.o
asm-y += cache.o
asm-y += debug.o
asm-y += head.o
asm-y += mmu.o

arm-y += exception.o
arm-y += memory.o
arm-y += gicv3.o
arm-y += cpu.o
arm-y += smp.o

lds-y += tiny.lds
