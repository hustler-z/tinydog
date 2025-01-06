// @Hustler's Project

#ifndef _ASM_H_
#define _ASM_H_

// ####################################################

#ifdef __ASSEMBLY__

#ifndef CODE_FILL
#define CODE_FILL               ~0
#endif

#ifndef DATA_ALIGN
#define DATA_ALIGN              0
#endif

#ifndef DATA_FILL
#define DATA_FILL               ~0
#endif

#define SYM_ALIGN(align...)     .balign align

#define SYM_L_GLOBAL(name)      .globl name; .hidden name
#define SYM_L_WEAK(name)        .weak name
#define SYM_L_LOCAL(name)       /* nothing */

#define SYM_T_FUNC              STT_FUNC
#define SYM_T_DATA              STT_OBJECT
#define SYM_T_NONE              STT_NOTYPE

#define SYM(name, typ, linkage, align...)     \
    .type name, SYM_T_ ## typ;                \
    SYM_L_ ## linkage(name);                  \
    SYM_ALIGN(align);                         \
    name:

#define count_args_(dot, a1, a2, a3, a4, a5, a6, a7, a8, x, ...) x
#define count_args(args...) \
    count_args_(., ## args, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define ARG1_(x, y...)          (x)
#define ARG2_(x, y...)          ARG1_(y)
#define ARG3_(x, y...)          ARG2_(y)
#define ARG4_(x, y...)          ARG3_(y)

#define ARG__(nr)               ARG ## nr ## _
#define ARG_(nr)                ARG__(nr)
#define LASTARG(x, y...)        ARG_(count_args(x, ## y))(x, ## y)

#define ARM_FUNC_ALIGN          (4)
#define count_args_exp(args...) count_args(args)
#if count_args_exp(CODE_FILL)

#define CODE_ALIGN(align...)                 \
    LASTARG(ARM_FUNC_ALIGN, ## align), CODE_FILL
#else
#define CODE_ALIGN(align...)                 \
    LASTARG(ARM_FUNC_ALIGN, ## align)
#endif

#define FUNC(name, align...)                 \
    SYM(name, FUNC, GLOBAL, CODE_ALIGN(align))
#define LABEL(name, align...)                \
    SYM(name, NONE, GLOBAL, CODE_ALIGN(align))
#define DATA(name, align...)                 \
    SYM(name, DATA, GLOBAL, LASTARG(DATA_ALIGN, ## align), DATA_FILL)

#define FUNC_LOCAL(name, align...)           \
    SYM(name, FUNC, LOCAL, CODE_ALIGN(align))
#define LABEL_LOCAL(name, align...)          \
    SYM(name, NONE, LOCAL, CODE_ALIGN(align))
#define DATA_LOCAL(name, align...)           \
    SYM(name, DATA, LOCAL, LASTARG(DATA_ALIGN, ## align), DATA_FILL)

#define GLOBAL(name)                         \
    .globl name;                             \
    name:

#define END(name)                            \
    .size name, . - name

// ####################################################

#define __HEAD   .section .text.head, "ax", %progbits
#define __VECS   .section .text.vecs, "ax", %progbits
#define __GIC    .section .text.gic, "ax", %progbits
#define __TIMER  .section .text.timer, "ax", %progbits

// ####################################################

/* stack pushing/popping register pairs
 */
.macro push, xn, xm
    stp \xn, \xm, [sp, #-16]!
.endm

.macro pop,  xn, xm
    ldp \xn, \xm, [sp], #16
.endm

.macro adrl, dst, sym
    adrp \dst, \sym
    add  \dst, \dst, :lo12:\sym
.endm

// exception level switch cases
.macro	switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, 0xc
	beq	\el3_label
	cmp	\xreg, 0x8
	beq	\el2_label
	cmp	\xreg, 0x4
	beq	\el1_label
.endm

// ####################################################

#define RODATA(section, label, msg)          \
.pushsection section, "aMS", %progbits, 1;   \
label: .asciz msg;                           \
.popsection

#define PRINT(section, string)               \
    mov   x3, lr;                            \
    adrl  x0, 502f;                          \
    bl    __asm_outs__;                      \
    mov   lr, x3;                            \
    RODATA(section, 502, string)

#define DBG(string) PRINT(.rodata.debug, string)

.macro dump_reg xb
    mov   x0, \xb
    mov   x4, lr
    bl    __asm_hex__
    mov   lr, x4
.endm

#endif /* __ASSEMBLY__ */

// ####################################################

#endif /* _ASM_H_ */
