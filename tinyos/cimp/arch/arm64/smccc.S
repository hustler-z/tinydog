// @Hustler's Project

#include <asm/offset.h>
#include <asm/asm.h>

.macro SMCCC instr
\instr    #0
	ldr   x4, [sp]
	stp   x0, x1, [x4, SMCCC_RES_A0_A1_OFFSET]
	stp   x2, x3, [x4, SMCCC_RES_A2_A3_OFFSET]
	stp   x4, x5, [x4, SMCCC_RES_A4_A5_OFFSET]
	stp   x6, x7, [x4, SMCCC_RES_A6_A7_OFFSET]
	ret
.endm

FUNC(arm_smccc_smc)
    SMCCC smc
END(arm_smccc_smc)

FUNC(arm_smccc_hvc)
    SMCCC hvc
END(arm_smccc_hvc)
