// @Hustler's Project

#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

// ####################################################

#include <type.h>

#ifdef __RK356X__

struct pwm_channel {
    __RO u32 cnt;
    __RW u32 period_hpr;
    __RW u32 duty_lpr;
    __RW u32 ctrl;
};

struct pwm_reg {
    struct pwm_channel channels[4]; /* Address Offset: 0x0000 */
    __RW u32 intsts;                /* Address Offset: 0x0040 */
    __RW u32 int_en;                /* Address Offset: 0x0044 */
         u32 reserved0048[2];       /* Address Offset: 0x0048 */
    __RW u32 fifo_ctrl;             /* Address Offset: 0x0050 */
    __RW u32 fifo_intsts;           /* Address Offset: 0x0054 */
    __RW u32 fifo_toutthr;          /* Address Offset: 0x0058 */
    __RW u32 version_id;            /* Address Offset: 0x005C */
    __RO u32 fifo;                  /* Address Offset: 0x0060 */
         u32 reserved0064[7];       /* Address Offset: 0x0064 */
    __RW u32 pwrmatch_ctrl;         /* Address Offset: 0x0080 */
    __RW u32 pwrmatch_lpre;         /* Address Offset: 0x0084 */
    __RW u32 pwrmatch_hpre;         /* Address Offset: 0x0088 */
    __RW u32 pwrmatch_ld;           /* Address Offset: 0x008C */
    __RW u32 pwrmatch_hd_zero;      /* Address Offset: 0x0090 */
    __RW u32 pwrmatch_hd_one;       /* Address Offset: 0x0094 */
    __RW u32 pwrmatch_value[10];    /* Address Offset: 0x0098 */
         u32 reserved00c0[3];       /* Address Offset: 0x00C0 */
    __RO u32 pwm3_pwrcapture_value; /* Address Offset: 0x00CC */
    __RW u32 filter_ctrl;           /* Address Offset: 0x00D0 */
};

#endif /* _RK356X__ */

// ####################################################

#endif /* _HAL_I2C_H_ */
