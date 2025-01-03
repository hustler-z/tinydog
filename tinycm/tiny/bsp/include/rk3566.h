// @Hustler's Project

#ifndef _BSP_RK3566_H_
#define _BSP_RK3566_H_

#include <type.h>

// ####################################################
#define NR_CPU              4
// ####################################################

#define GRF_BASE            0xFDC20000U /* GRF base address */
#define PMUGRF_BASE         0xFDC60000U
#define PMUCRU_BASE         0xFDD00000U /* PMUCRU base address */
#define TIMER6_BASE         0xFDD1C000U /* TIMER6 base address */
#define TIMER7_BASE         0xFDD1C020U /* TIMER7 base address */
#define CRU_BASE            0xFDD20000U /* CRU base address */
#define I2C0_BASE           0xFDD40000U /* I2C0 base address */
#define UART0_BASE          0xFDD50000U /* UART0 base address */
#define GPIO0_BASE          0xFDD60000U /* GPIO0 base address */
#define PWM0_BASE           0xFDD70000U /* PWM0 base address */
#define PMU_BASE            0xFDD90000U /* PMU base address */
#define SPINLOCK_BASE       0xFDE30000U /* SPINLOCK base address */
#define GMAC1_BASE          0xFE010000U /* GMAC1 base address */
#define GMAC0_BASE          0xFE2A0000U /* GMAC0 base address */
#define FSPI_BASE           0xFE300000U /* FSPI base address */
#define DMA0_BASE           0xFE530000U /* DMA0 base address */
#define DMA1_BASE           0xFE550000U /* DMA1 base address */
#define CAN0_BASE           0xFE570000U /* CAN0 base address */
#define CAN1_BASE           0xFE580000U /* CAN1 base address */
#define CAN2_BASE           0xFE590000U /* CAN2 base address */
#define I2C1_BASE           0xFE5A0000U /* I2C1 base address */
#define I2C2_BASE           0xFE5B0000U /* I2C2 base address */
#define I2C3_BASE           0xFE5C0000U /* I2C3 base address */
#define I2C4_BASE           0xFE5D0000U /* I2C4 base address */
#define I2C5_BASE           0xFE5E0000U /* I2C5 base address */
#define TIMER0_BASE         0xFE5F0000U /* TIMER0 base address */
#define TIMER1_BASE         0xFE5F0020U /* TIMER1 base address */
#define TIMER2_BASE         0xFE5F0040U /* TIMER2 base address */
#define TIMER3_BASE         0xFE5F0060U /* TIMER3 base address */
#define TIMER4_BASE         0xFE5F0080U /* TIMER4 base address */
#define TIMER5_BASE         0xFE5F00A0U /* TIMER5 base address */
#define WDT_BASE            0xFE600000U /* WDT base address */
#define SPI0_BASE           0xFE610000U /* SPI0 base address */
#define SPI1_BASE           0xFE620000U /* SPI1 base address */
#define SPI2_BASE           0xFE630000U /* SPI2 base address */
#define SPI3_BASE           0xFE640000U /* SPI3 base address */
#define UART1_BASE          0xFE650000U /* UART1 base address */
#define UART2_BASE          0xFE660000U /* UART2 base address */
#define UART3_BASE          0xFE670000U /* UART3 base address */
#define UART4_BASE          0xFE680000U /* UART4 base address */
#define UART5_BASE          0xFE690000U /* UART5 base address */
#define UART6_BASE          0xFE6A0000U /* UART6 base address */
#define UART7_BASE          0xFE6B0000U /* UART7 base address */
#define UART8_BASE          0xFE6C0000U /* UART8 base address */
#define UART9_BASE          0xFE6D0000U /* UART9 base address */
#define PWM1_BASE           0xFE6E0000U /* PWM1 base address */
#define PWM2_BASE           0xFE6F0000U /* PWM2 base address */
#define PWM3_BASE           0xFE700000U /* PWM3 base address */
#define TSADC_BASE          0xFE710000U /* TSADC base address */
#define GPIO1_BASE          0xFE740000U /* GPIO1 base address */
#define GPIO2_BASE          0xFE750000U /* GPIO2 base address */
#define GPIO3_BASE          0xFE760000U /* GPIO3 base address */
#define GPIO4_BASE          0xFE770000U /* GPIO4 base address */
#define MBOX0_BASE          0xFE780000U /* MBOX0 base address */

// ####################################################

enum interrupt_id {
    IPI_SGI0              = 0,
    IPI_SGI1              = 1,
    IPI_SGI2              = 2,
    IPI_SGI3              = 3,
    IPI_SGI4              = 4,
    IPI_SGI5              = 5,
    IPI_SGI6              = 6,
    IPI_SGI7              = 7,
    IPI_SGI8              = 8,
    IPI_SGI9              = 9,
    IPI_SGI10             = 10,
    IPI_SGI11             = 11,
    IPI_SGI12             = 12,
    IPI_SGI13             = 13,
    IPI_SGI14             = 14,
    IPI_SGI15             = 15,

    CNTHP_IRQ             = 26,
    CNTV_IRQ              = 27,
    CNTPS_IRQ             = 29,
    CNTPNS_IRQ            = 30,

    DEBUG_IRQ             = 33,

    GPIO0_IRQ             = 65,       /* GPIO0 Interrupt             */
    GPIO1_IRQ             = 66,       /* GPIO1 Interrupt             */
    GPIO2_IRQ             = 67,       /* GPIO2 Interrupt             */
    GPIO3_IRQ             = 68,       /* GPIO3 Interrupt             */
    GPIO4_IRQ             = 69,       /* GPIO4 Interrupt             */
    I2C0_IRQ              = 78,       /* I2C0 Interrupt              */
    I2C1_IRQ              = 79,       /* I2C1 Interrupt              */
    I2C2_IRQ              = 80,       /* I2C2 Interrupt              */
    I2C3_IRQ              = 81,       /* I2C3 Interrupt              */
    I2C4_IRQ              = 82,       /* I2C4 Interrupt              */
    I2C5_IRQ              = 83,       /* I2C5 Interrupt              */
    FSPI0_IRQ             = 133,      /* FSPI Interrupt              */
    SPI0_IRQ              = 135,      /* SPI0 Interrupt              */
    SPI1_IRQ              = 136,      /* SPI1 Interrupt              */
    SPI2_IRQ              = 137,      /* SPI2 Interrupt              */
    SPI3_IRQ              = 138,      /* SPI3 Interrupt              */
    TIMER0_IRQ            = 141,      /* TIMER0 Interrupt            */
    TIMER1_IRQ            = 142,      /* TIMER1 Interrupt            */
    TIMER2_IRQ            = 143,      /* TIMER2 Interrupt            */
    TIMER3_IRQ            = 144,      /* TIMER3 Interrupt            */
    TIMER4_IRQ            = 145,      /* TIMER4 Interrupt            */
    TIMER5_IRQ            = 146,      /* TIMER5 Interrupt            */
    UART0_IRQ             = 148,      /* UART0  Interrupt            */
    UART1_IRQ             = 149,      /* UART1  Interrupt            */
    UART2_IRQ             = 150,      /* UART2  Interrupt            */
    UART3_IRQ             = 151,      /* UART3  Interrupt            */
    UART4_IRQ             = 152,      /* UART4  Interrupt            */
    UART5_IRQ             = 153,      /* UART5  Interrupt            */
    UART6_IRQ             = 154,      /* UART6  Interrupt            */
    UART7_IRQ             = 155,      /* UART7  Interrupt            */
    UART8_IRQ             = 156,      /* UART8  Interrupt            */
    UART9_IRQ             = 157,      /* UART9  Interrupt            */
    WDT0_IRQ              = 181,      /* WDT0  Interrupt             */
    DDR_ECC_CE_IRQ        = 205,      /* DDR ECC correctable fault Interrupt */
    DDR_ECC_UE_IRQ        = 207,      /* DDR ECC uncorrectable fault Interrupt */
    MBOX0_CH0_B2A_IRQ     = 215,      /* MBOX0 CH0 B2A Interrupt     */
    MBOX0_CH1_B2A_IRQ     = 216,      /* MBOX0 CH1 B2A Interrupt     */
    MBOX0_CH2_B2A_IRQ     = 217,      /* MBOX0 CH2 B2A Interrupt     */
    MBOX0_CH3_B2A_IRQ     = 218,      /* MBOX0 CH3 B2A Interrupt     */
    MBOX0_CH0_A2B_IRQ     = 219,      /* MBOX0 CH0 A2B Interrupt     */
    MBOX0_CH1_A2B_IRQ     = 220,      /* MBOX0 CH1 A2B Interrupt     */
    MBOX0_CH2_A2B_IRQ     = 221,      /* MBOX0 CH2 A2B Interrupt     */
    MBOX0_CH3_A2B_IRQ     = 222,      /* MBOX0 CH3 A2B Interrupt     */
    NFAULT0_IRQ           = 272,      /* DSU L3 CACHE ECC FAULT Interrupt */
    NFAULT1_IRQ           = 273,      /* CPU0 L1-L2 CACHE ECC FAULT Interrupt */
    NFAULT2_IRQ           = 274,      /* CPU1 L1-L2 CACHE ECC FAULT Interrupt */
    NFAULT3_IRQ           = 275,      /* CPU2 L1-L2 CACHE ECC FAULT Interrupt */
    NFAULT4_IRQ           = 276,      /* CPU3 L1-L2 CACHE ECC FAULT Interrupt */
    NERR0_IRQ             = 277,      /* DSU L3 CACHE ECC ERROR Interrupt */
    NERR1_IRQ             = 278,      /* CPU0 L1-L2 CACHE ECC ERROR Interrupt */
    NERR2_IRQ             = 279,      /* CPU1 L1-L2 CACHE ECC ERROR Interrupt */
    NERR3_IRQ             = 280,      /* CPU2 L1-L2 CACHE ECC ERROR Interrupt */
    NERR4_IRQ             = 281,      /* CPU3 L1-L2 CACHE ECC ERROR Interrupt */
    RSVD0_IRQ             = 283,      /* RSVD0  Interrupt            */
    RPMSG_01_IRQ          = 285,      /* RPMSG 0->1 Interrupt        */
    RPMSG_02_IRQ          = 286,      /* RPMSG 0->2 Interrupt        */
    RPMSG_03_IRQ          = 287,      /* RPMSG 0->3 Interrupt        */
    RPMSG_10_IRQ          = 288,      /* RPMSG 1->0 Interrupt        */
    RPMSG_12_IRQ          = 289,      /* RPMSG 1->2 Interrupt        */
    RPMSG_13_IRQ          = 290,      /* RPMSG 1->3 Interrupt        */
    RPMSG_20_IRQ          = 291,      /* RPMSG 2->0 Interrupt        */
    RPMSG_21_IRQ          = 292,      /* RPMSG 2->1 Interrupt        */
    RPMSG_23_IRQ          = 293,      /* RPMSG 2->3 Interrupt        */
    RPMSG_30_IRQ          = 294,      /* RPMSG 3->0 Interrupt        */
    RPMSG_31_IRQ          = 295,      /* RPMSG 3->1 Interrupt        */
    RPMSG_32_IRQ          = 296,      /* RPMSG 3->2 Interrupt        */
    NUM_INTERRUPTS        = 352,
};

// GIC600 0xFD400000

#endif /* _BSP_RK3566_H_ */
