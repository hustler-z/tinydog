// @Hustler's Project

#include <asm/time.h>
#include <asm/cpu.h>
#include <compiler.h>
#include <spinlock.h>
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

int atoi(const char* str) {
    int res = 0, i;

    for (i = 0; str[i] != '\0'; ++i) {
        res = res * 10 + str[i] - '0';
    }

    return res;
}

u32 atoh(const char* str) {
    u32 hex = 0, value = 0, i;

    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            value = str[i] - '0';
        } else if (str[i] >= 'a' && str[i] <= 'f') {
            value = str[i] - 'a' + 10;
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            value = str[i] - 'A' + 10;
        } else {
            continue;
        }

        // make space for the new nibble on the right
        hex = hex << 4;
        hex |= value;
    }

    return hex;
}

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

/* Nonzero if YEAR is a leap year (every 4 years,
 * except every 100th isn't, and every 400th is).
 */
#define __isleap(year) \
  ((year) % 4 == 0 && ((year) % 100 != 0 || (year) % 400 == 0))

/* How many days are in each month.  */
static const u16 __mon_lengths[2][12] = {
    /* Normal years.  */
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    /* Leap years.  */
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

#define SECS_PER_HOUR (60 * 60)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24)

static u64 wc_sec; /* UTC time at last 'time update'. */
static unsigned int wc_nsec;

struct tm date(unsigned long t)
{
    struct tm tbuf;
    long days, rem;
    int y;
    const u16 *ip;

    y = 1970;

#if BITS_PER_LONG >= 64
    /* Allow the concept of time before 1970.  64-bit only;
     * for 32-bit time after 2038 seems more important than
     * time before 1970.
     */
    while (t & (1UL << 39)) {
        y -= 400;
        t += ((u64)(365 * 303 + 366 * 97)) * SECS_PER_DAY;
    }

    t &= (1UL << 40) - 1;
#endif

    days = t / SECS_PER_DAY;
    rem  = t % SECS_PER_DAY;

    tbuf.tm_hour = rem / SECS_PER_HOUR;
    rem %= SECS_PER_HOUR;
    tbuf.tm_min = rem / 60;
    tbuf.tm_sec = rem % 60;
    /* January 1, 1970 was a Thursday.  */
    tbuf.tm_wday = (4 + days) % 7;
    if (tbuf.tm_wday < 0)
        tbuf.tm_wday += 7;

    while (days >= (rem = __isleap(y) ? 366 : 365)) {
        ++y;
        days -= rem;
    }

    while (days < 0) {
        --y;
        days += __isleap(y) ? 366 : 365;
    }

    tbuf.tm_year = y - 1900;
    tbuf.tm_yday = days;
    ip = (const u16 *)__mon_lengths[__isleap(y)];

    for (y = 0; days >= ip[y]; ++y)
        days -= ip[y];
    tbuf.tm_mon = y;
    tbuf.tm_mday = days + 1;
    tbuf.tm_isdst = -1;

    return tbuf;
}

u64 get_sec(void)
{
    return wc_sec + (wc_nsec + NOW()) / 1000000000UL;
}

/* Generic Timer
 *
 * XXX: Time Unit Conversion
 *
 * s ◀-- ms ◀-- us ◀-- ns
 *    |      |      |
 *  1000     |      |
 *         1000     |
 *                1000
 */
u64 get_usec(void)
{
    return wc_sec * 1000000UL + (wc_nsec + NOW()) / 1000UL;
}

u64 get_msec(void)
{
    return wc_sec * 1000UL + (wc_nsec + NOW()) / 1000000UL;
}

u64 get_msec_bias(u64 base)
{
    return (nanosecond() / 1000000UL) - base;
}

struct tm timeofday(void) {
    u64 seconds = 0;

    if (!wc_sec)
        return (struct tm) { 0 };

    seconds = get_sec();

    return date(seconds);
}

// ####################################################
