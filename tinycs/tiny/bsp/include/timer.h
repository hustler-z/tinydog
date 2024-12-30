// @Hustler's Project

#ifndef _BSP_TIMER_H_
#define _BSP_TIMER_H_

// ####################################################

#include <type.h>

#ifdef __RK356X__

struct timer_reg {
    __RW u32 load_count[2];         /* Address Offset: 0x0000 */
    __RO u32 current_value[2];      /* Address Offset: 0x0008 */
    __RW u32 controlreg;            /* Address Offset: 0x0010 */
         u32 reserved0014;          /* Address Offset: 0x0014 */
    __RW u32 intstatus;             /* Address Offset: 0x0018 */
};

#endif /* _RK356X__ */

// ####################################################

#endif /* _BSP_TIMER_H_ */
