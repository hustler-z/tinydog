// @Hustler's Project

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

// ####################################################

#include <type.h>

#ifdef __RK356X__

struct gpio_reg {
    __RW u32 swport_dr_l;           /* Address Offset: 0X0000 */
    __RW u32 swport_dr_h;           /* Address Offset: 0X0004 */
    __RW u32 swport_ddr_l;          /* Address Offset: 0X0008 */
    __RW u32 swport_ddr_h;          /* Address Offset: 0X000C */
    __RW u32 int_en_l;              /* Address Offset: 0X0010 */
    __RW u32 int_en_h;              /* Address Offset: 0X0014 */
    __RW u32 int_mask_l;            /* Address Offset: 0X0018 */
    __RW u32 int_mask_h;            /* Address Offset: 0X001C */
    __RW u32 int_type_l;            /* Address Offset: 0X0020 */
    __RW u32 int_type_h;            /* Address Offset: 0X0024 */
    __RW u32 int_polarity_l;        /* Address Offset: 0X0028 */
    __RW u32 int_polarity_h;        /* Address Offset: 0X002C */
    __RW u32 int_bothedge_l;        /* Address Offset: 0X0030 */
    __RW u32 int_bothedge_h;        /* Address Offset: 0X0034 */
    __RW u32 debounce_l;            /* Address Offset: 0X0038 */
    __RW u32 debounce_h;            /* Address Offset: 0X003C */
    __RW u32 dbclk_div_en_l;        /* Address Offset: 0X0040 */
    __RW u32 dbclk_div_en_h;        /* Address Offset: 0X0044 */
    __RW u32 dbclk_div_con;         /* Address Offset: 0X0048 */
         u32 reserved004c;          /* Address Offset: 0X004C */
    __RO u32 int_status;            /* Address Offset: 0X0050 */
         u32 reserved0054;          /* Address Offset: 0X0054 */
    __RO u32 int_rawstatus;         /* Address Offset: 0X0058 */
         u32 reserved005c;          /* Address Offset: 0X005C */
    __RW u32 port_eoi_l;            /* Address Offset: 0X0060 */
    __RW u32 port_eoi_h;            /* Address Offset: 0X0064 */
         u32 reserved0068[2];       /* Address Offset: 0X0068 */
    __RO u32 ext_port;              /* Address Offset: 0X0070 */
         u32 reserved0074;          /* Address Offset: 0X0074 */
    __RO u32 ver_id;                /* Address Offset: 0X0078 */
};

#endif /* _RK356X__ */

// ####################################################

#endif /* _HAL_GPIO_H_ */
