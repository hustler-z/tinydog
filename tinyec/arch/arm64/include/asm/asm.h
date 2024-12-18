#ifndef _ASM_H_
#define _ASM_H_

// ##############################################################

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

#define DO_CODE_ALIGN(align...)              \
    LASTARG(ARM_FUNC_ALIGN, ## align), CODE_FILL
#else
#define DO_CODE_ALIGN(align...)              \
    LASTARG(ARM_FUNC_ALIGN, ## align)
#endif

#define FUNC(name, align...)                 \
    SYM(name, FUNC, GLOBAL, DO_CODE_ALIGN(align))
#define LABEL(name, align...)                \
    SYM(name, NONE, GLOBAL, DO_CODE_ALIGN(align))
#define DATA(name, align...)                 \
    SYM(name, DATA, GLOBAL, LASTARG(DATA_ALIGN, ## align), DATA_FILL)

#define FUNC_LOCAL(name, align...)           \
    SYM(name, FUNC, LOCAL, DO_CODE_ALIGN(align))
#define LABEL_LOCAL(name, align...)          \
    SYM(name, NONE, LOCAL, DO_CODE_ALIGN(align))
#define DATA_LOCAL(name, align...)           \
    SYM(name, DATA, LOCAL, LASTARG(DATA_ALIGN, ## align), DATA_FILL)

#define END(name)                            \
    .size name, . - name

#define __HEAD           .section .head, "ax", %progbits


#define MMU      (1 << 0)
#define DCACHE   (1 << 2)
#define ICACHE   (1 << 12)

// ##############################################################

#define RODATA(section, label, msg)          \
.pushsection section, "aMS", %progbits, 1;   \
label: .asciz msg;                           \
.popsection

#define PRINT(section, string)               \
    mov   x3, lr;                            \
    adr_l x0, 98f;                           \
    bl    _outs;                             \
    mov   lr, x3;                            \
    RODATA(section, 98, string)

#define DBG(string) PRINT(.rodata.debug, string)

.macro dump_reg xb
    mov   x0, \xb
    mov   x4, lr
    bl    _outx
    mov   lr, x4
.endm

/* :lo12: A special notation is used to
 *        add only the lowest 12 bits of
 *        the label to the adrp address.
 *        4KB page is addressable with
 *        12 bits.
 * adrp - Form PC-relative address to 4KB
 *        page.
 *
 * Load address of <sym> into <reg>, <sym> being in the range
 * +/- 4GB of the PC.
 */
.macro  adr_l, dst, sym
    adrp \dst, \sym
    add  \dst, \dst, :lo12:\sym
.endm

/* Stack pushing/popping register pairs
 */
.macro push, xn, xm
    stp \xn, \xm, [sp, #-16]!
.endm

.macro pop,  xn, xm
    ldp \xn, \xm, [sp], #16
.endm

#endif /* __ASSEMBLY__ */

// ##############################################################

#endif /* _ASM_H_ */
