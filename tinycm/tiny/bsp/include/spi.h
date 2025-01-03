// @Hustler's Project

#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

// ####################################################

#include <type.h>

#ifdef __RK356X__

struct spi_reg {
    __RW u32 ctrlr[2];              /* Address Offset: 0x0000 */
    __RW u32 enr;                   /* Address Offset: 0x0008 */
    __RW u32 ser;                   /* Address Offset: 0x000C */
    __RW u32 baudr;                 /* Address Offset: 0x0010 */
    __RW u32 txftlr;                /* Address Offset: 0x0014 */
    __RW u32 rxftlr;                /* Address Offset: 0x0018 */
    __RO u32 txflr;                 /* Address Offset: 0x001C */
    __RO u32 rxflr;                 /* Address Offset: 0x0020 */
    __RO u32 sr;                    /* Address Offset: 0x0024 */
    __RW u32 ipr;                   /* Address Offset: 0x0028 */
    __RW u32 imr;                   /* Address Offset: 0x002C */
    __RW u32 isr;                   /* Address Offset: 0x0030 */
    __RW u32 risr;                  /* Address Offset: 0x0034 */
    __WO u32 icr;                   /* Address Offset: 0x0038 */
    __RW u32 dmacr;                 /* Address Offset: 0x003C */
    __RW u32 dmatdlr;               /* Address Offset: 0x0040 */
    __RW u32 dmardlr;               /* Address Offset: 0x0044 */
         u32 reserved0048;          /* Address Offset: 0x0048 */
    __RW u32 timeout;               /* Address Offset: 0x004C */
    __RW u32 bypass;                /* Address Offset: 0x0050 */
         u32 reserved0054[235];     /* Address Offset: 0x0054 */
    __WO u32 txdr;                  /* Address Offset: 0x0400 */
         u32 reserved0404[255];     /* Address Offset: 0x0404 */
    __RO u32 rxdr;                  /* Address Offset: 0x0800 */
};

#endif /* _RK356X__ */

// ####################################################

#endif /* _BSP_SPI_H_ */
