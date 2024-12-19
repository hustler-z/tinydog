// @Hustler's Project

#ifndef _ASM_OFFSET_
#define _ASM_OFFSET_

#define X0 0 /* offsetof(struct context, x0) */
#define X1 8 /* offsetof(struct context, x1) */
#define LR 240 /* offsetof(struct context, lr) */
#define SP 248 /* offsetof(struct context, sp) */
#define PC 256 /* offsetof(struct context, pc) */
#define CPSR 264 /* offsetof(struct context, cpsr) */
#define ESR 272 /* offsetof(struct context, esr) */

#define ELR 280 /* offsetof(struct context, elr) */
#define STACK 296 /* offsetof(struct cpu, stack) */

#endif /* _ASM_OFFSET_ */
