// @Hustler's Project

#ifndef _HAL_WDT_H_
#define _HAL_WDT_H_

// ####################################################

#include <type.h>

#ifdef __RK356X__

struct wdt_reg {
    __RW u32 cr;                    /* Address Offset: 0x0000 */
    __RW u32 torr;                  /* Address Offset: 0x0004 */
    __RO u32 ccvr;                  /* Address Offset: 0x0008 */
    __WO u32 crr;                   /* Address Offset: 0x000C */
    __RO u32 stat;                  /* Address Offset: 0x0010 */
    __RO u32 eoi;                   /* Address Offset: 0x0014 */
};

#endif /* _RK356X__ */

// ####################################################

#endif /* _HAL_WDT_H_ */
