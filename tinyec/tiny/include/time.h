// @Hustler's Project

#ifndef _TINY_TIME_H_
#define _TINY_TIME_H_

#include <asm/time.h>

time_t nanosecond(void);

struct tm {
    int tm_sec;         /* seconds */
    int tm_min;         /* minutes */
    int tm_hour;        /* hours */
    int tm_mday;        /* day of the month */
    int tm_mon;         /* month */
    int tm_year;        /* year */
    int tm_wday;        /* day of the week */
    int tm_yday;        /* day in the year */
    int tm_isdst;       /* daylight saving time */
};

struct tm timeofday(time_t time);
struct tm wallclock(time_t *ns);

#define SYSTEM_TIME_HZ  1000000000ULL
#define NOW()           ((time_t)nanosecond())
#define SECONDS(_s)     ((time_t)((_s)  * 1000000000ULL))
#define DAYS(_d)        SECONDS((_d) * 86400ULL)
#define MILLISECS(_ms)  ((time_t)((_ms) * 1000000ULL))
#define MICROSECS(_us)  ((time_t)((_us) * 1000ULL))
#define STIME_MAX       ((time_t)((u64)~0ULL >> 1))

/* Chosen so (NOW() + delta) wont overflow without an uptime of
 * 200 years.
 */
#define STIME_DELTA_MAX ((time_t)((u64)~0ULL>>2))


int time_preset(void);

#endif /* _TINY_TIME_H_ */
