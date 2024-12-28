// @Hustler's Project

#include <asm/time.h>
#include <asm/cpu.h>
#include <compiler.h>
#include <print.h>
#include <time.h>

// ####################################################

#define _U  0x01    /* upper */
#define _L  0x02    /* lower */
#define _D  0x04    /* digit */
#define _C  0x08    /* cntrl */
#define _P  0x10    /* punct */
#define _S  0x20    /* white space (space/lf/tab) */
#define _X  0x40    /* hex digit */
#define _SP 0x80    /* hard space (0x20) */

const unsigned char _ctype[] = {
    _C,_C,_C,_C,_C,_C,_C,_C,                             /* 0-7     */
    _C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,              /* 8-15    */
    _C,_C,_C,_C,_C,_C,_C,_C,                             /* 16-23   */
    _C,_C,_C,_C,_C,_C,_C,_C,                             /* 24-31   */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,                         /* 32-39   */
    _P,_P,_P,_P,_P,_P,_P,_P,                             /* 40-47   */
    _D,_D,_D,_D,_D,_D,_D,_D,                             /* 48-55   */
    _D,_D,_P,_P,_P,_P,_P,_P,                             /* 56-63   */
    _P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,           /* 64-71   */
    _U,_U,_U,_U,_U,_U,_U,_U,                             /* 72-79   */
    _U,_U,_U,_U,_U,_U,_U,_U,                             /* 80-87   */
    _U,_U,_U,_P,_P,_P,_P,_P,                             /* 88-95   */
    _P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,           /* 96-103  */
    _L,_L,_L,_L,_L,_L,_L,_L,                             /* 104-111 */
    _L,_L,_L,_L,_L,_L,_L,_L,                             /* 112-119 */
    _L,_L,_L,_P,_P,_P,_P,_C,                             /* 120-127 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                     /* 128-143 */
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                     /* 144-159 */
    _S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P, /* 160-175 */
    _P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,     /* 176-191 */
    _U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,     /* 192-207 */
    _U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,     /* 208-223 */
    _L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,     /* 224-239 */
    _L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L      /* 240-255 */
};

// ####################################################

// @Hustler - Time
static u64 boot_count;
static u64 cpu_khz;

/* Compute with 96 bit intermediate result: (a*b)/c
 */
u64 muldiv64(u64 a, u32 b, u32 c)
{
    union {
        u64 ll;
        struct {
            u32 low, high;
        } l;
    } u, res;
    u64 rl, rh;

    u.ll = a;
    rl = (u64)u.l.low * (u64)b;
    rh = (u64)u.l.high * (u64)b;
    rh += (rl >> 32);
    res.l.high = rh / c;
    res.l.low = (((rh % c) << 32) + (u32)rl) / c;

    return res.ll;
}

time_t ticks_to_ns(u64 ticks) {
    return muldiv64(ticks, SECONDS(1), 1000 * cpu_khz);
}

u64 ns_to_ticks(time_t ns) {
    return muldiv64(ns, 1000 * cpu_khz, SECONDS(1));
}

time_t nanosecond(void) {
    u64 ticks = physical_count() - boot_count;
    return ticks_to_ns(ticks);
}

void udelay(time_t usecs) {
    time_t deadline = nanosecond() + 1000 * (time_t)usecs;
    while((nanosecond() - deadline) < 0)
        ;

    dsb(sy);
    isb();
}

// can only call once
int __init time_init(void) {
    tiny_print("time kicking off\n");

    boot_count = physical_count();
    cpu_khz = cpufreq_khz();

    return 0;
}

// ####################################################
