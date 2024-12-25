// @Hustler's Project

#include <cpu.h>
#include <util.h>

#define DEFINE(sym, val)                \
    asm volatile ("\n.ascii \"->" #sym " %0 " #val "\"" : : "i" (val))

#define BLANK() asm volatile ("\n.ascii \"->\"" : : )

#define OFFSET(sym, str, mem)           \
    DEFINE(sym, offsetof(struct str, mem))

void __offset__(void) {
    OFFSET(X0, context, x0);
    OFFSET(X1, context, x1);

    OFFSET(LR, context, lr);
    OFFSET(SP, context, sp);
    OFFSET(PC, context, pc);
    OFFSET(CPSR, context, cpsr);
    OFFSET(ESR, context, esr);
    BLANK();
    OFFSET(ELR, context, elr);

    OFFSET(STACK, cpu, stack);
}
