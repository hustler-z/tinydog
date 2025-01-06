// @Hustler's Project

#include <asm/smccc.h>

#include <util.h>
#include <cpu.h>

#define DEFINE(sym, val)                \
    asm volatile ("\n.ascii \"->" #sym " %0 " #val "\"" : : "i" (val))

#define BLANK() asm volatile ("\n.ascii \"->\"" : : )

#define OFFSET(sym, str, mem)           \
    DEFINE(sym, offsetof(struct str, mem))

void __offset__(void) {
    OFFSET(X0_OFFSET, context, x0);
    OFFSET(X1_OFFSET, context, x1);

    OFFSET(LR_OFFSET, context, lr);
    OFFSET(SP_OFFSET, context, sp);
    OFFSET(PC_OFFSET, context, pc);
    OFFSET(CPSR_OFFSET, context, cpsr);
    OFFSET(ESR_OFFSET, context, esr);
    BLANK();
    OFFSET(ELR_OFFSET, context, elr);

    OFFSET(STACK_OFFSET, cpu, stack);

    DEFINE(CPU_SIZE, sizeof(struct cpu));

    OFFSET(SMCCC_RES_A0_A1_OFFSET, arm_smccc_res, a0);
    OFFSET(SMCCC_RES_A2_A3_OFFSET, arm_smccc_res, a2);
    OFFSET(SMCCC_RES_A4_A5_OFFSET, arm_smccc_res, a4);
    OFFSET(SMCCC_RES_A6_A7_OFFSET, arm_smccc_res, a6);
}
