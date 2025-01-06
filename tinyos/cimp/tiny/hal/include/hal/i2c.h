// @Hustler's Project

#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

// ####################################################

#include <type.h>

#ifdef __RK356X__

struct i2c_reg {
    __RW u32 con;                   /* Address Offset: 0x0000 */
    __RW u32 clkdiv;                /* Address Offset: 0x0004 */
    __RW u32 mrxaddr;               /* Address Offset: 0x0008 */
    __RW u32 mrxraddr;              /* Address Offset: 0x000C */
    __RW u32 mtxcnt;                /* Address Offset: 0x0010 */
    __RW u32 mrxcnt;                /* Address Offset: 0x0014 */
    __RW u32 ien;                   /* Address Offset: 0x0018 */
    __RW u32 ipd;                   /* Address Offset: 0x001C */
    __RO u32 fcnt;                  /* Address Offset: 0x0020 */
    __RW u32 scl_oe_db;             /* Address Offset: 0x0024 */
         u32 reserved0028[54];      /* Address Offset: 0x0028 */
    __RW u32 txdata[8];             /* Address Offset: 0x0100 */
         u32 reserved0120[56];      /* Address Offset: 0x0120 */
    __RO u32 rxdata[8];             /* Address Offset: 0x0200 */
    __RO u32 st;                    /* Address Offset: 0x0220 */
    __RW u32 dbgctrl;               /* Address Offset: 0x0224 */
};

#endif /* _RK356X__ */

// ####################################################

#endif /* _HAL_I2C_H_ */
