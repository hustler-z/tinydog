// @Hustler's Project

#ifndef _BOARD_RK3566_H_
#define _BOARD_RK3566_H_

// ####################################################

#define NR_CPU              4

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

// @Hustler
#define DEBUG_SERIAL        UART2_BASE

// ####################################################

#endif /* _BOARD_RK3566_H_ */
