// @Hustler's Project

#ifndef _ASM_OFFSET_
#define _ASM_OFFSET_

#define X0_OFFSET 0 /* offsetof(struct context, x0) */
#define X1_OFFSET 8 /* offsetof(struct context, x1) */
#define LR_OFFSET 240 /* offsetof(struct context, lr) */
#define SP_OFFSET 248 /* offsetof(struct context, sp) */
#define PC_OFFSET 256 /* offsetof(struct context, pc) */
#define CPSR_OFFSET 264 /* offsetof(struct context, cpsr) */
#define ESR_OFFSET 272 /* offsetof(struct context, esr) */

#define ELR_OFFSET 280 /* offsetof(struct context, elr) */
#define STACK_OFFSET 296 /* offsetof(struct cpu, stack) */
#define CPU_SIZE 320 /* sizeof(struct cpu) */
#define SMCCC_RES_A0_A1_OFFSET 0 /* offsetof(struct arm_smccc_res, a0) */
#define SMCCC_RES_A2_A3_OFFSET 16 /* offsetof(struct arm_smccc_res, a2) */
#define SMCCC_RES_A4_A5_OFFSET 32 /* offsetof(struct arm_smccc_res, a4) */
#define SMCCC_RES_A6_A7_OFFSET 48 /* offsetof(struct arm_smccc_res, a6) */

#endif /* _ASM_OFFSET_ */
