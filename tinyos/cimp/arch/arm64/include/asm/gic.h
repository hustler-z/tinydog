// @Hustler's Project

#ifndef _ASM_GIC_H_
#define _ASM_GIC_H_

// ####################################################

#include <type.h>
#include <bit.h>

/**
 * GIC Register Interface Base Addresses
 */
#define GIC_DIST_BASE	    0xFD400000 /* XXX: rk3566 */
#define GIC_CPU_BASE	    0xFD400000
#define	IRQ_TYPE_LEVEL		BIT(1)
#define	IRQ_TYPE_EDGE		BIT(2)

/**
 * GIC Distributor Interface
 */

// 0x000  Distributor Control Register
#define	GICD_CTLR		(GIC_DIST_BASE +   0x0)

// 0x004  Interrupt Controller Type Register
#define	GICD_TYPER		(GIC_DIST_BASE +   0x4)

// 0x008  Distributor Implementer Identification Register
#define	GICD_IIDR		(GIC_DIST_BASE +   0x8)

// 0x080  Interrupt Group Registers
#define	GICD_IGROUPRn	(GIC_DIST_BASE +  0x80)

// 0x100  Interrupt Set-Enable Registers
#define	GICD_ISENABLERn	(GIC_DIST_BASE + 0x100)

// 0x180  Interrupt Clear-Enable Registers
#define	GICD_ICENABLERn	(GIC_DIST_BASE + 0x180)

// 0x200  Interrupt Set-Pending Registers
#define	GICD_ISPENDRn	(GIC_DIST_BASE + 0x200)

// 0x280  Interrupt Clear-Pending Registers
#define	GICD_ICPENDRn	(GIC_DIST_BASE + 0x280)

// 0x300  Interrupt Set-Active Registers
#define	GICD_ISACTIVERn	(GIC_DIST_BASE + 0x300)

/* GICD_ICFGR */
#define GICD_ICFGR_MASK			    BIT_MASK(2)
#define GICD_ICFGR_TYPE			    BIT(1)

/* GICD_TYPER.ITLinesNumber 0:4 */
#define GICD_TYPER_ITLINESNUM_MASK	0x1F

/* GICD_TYPER.IDbits */
#define GICD_TYPER_IDBITS(typer)	((((typer) >> 19) & 0x1F) + 1)

/*
 * Common Helper Constants
 */
#define GIC_SGI_INT_BASE		0
#define GIC_PPI_INT_BASE		16

#define GIC_IS_SGI(intid)		(((intid) >= GIC_SGI_INT_BASE) && \
                                ((intid) < GIC_PPI_INT_BASE))

#define GIC_SPI_INT_BASE		32

#define GIC_SPI_MAX_INTID		1019

#define GIC_IS_SPI(intid)		(((intid) >= GIC_SPI_INT_BASE) && \
					((intid) <= GIC_SPI_MAX_INTID))

#define GIC_NUM_INTR_PER_REG	32

#define GIC_NUM_CFG_PER_REG		16

#define GIC_NUM_PRI_PER_REG		4

/* GIC idle priority : value '0xff' will allow all interrupts */
#define GIC_IDLE_PRIO			0xFF

/* Priority levels 0:255 */
#define GIC_PRI_MASK			0xFF

/**
 * '0xa0'is used to initialize each interrupt default
 * priority.
 * This is an arbitrary value in current context.
 * Any value '0x80' to '0xff' will work for both
 * NS and S state.
 * The values of individual interrupt and default
 * has to be chosen carefully if PMR and BPR based
 * nesting and preemption has to be done.
 */
#define GIC_INT_DEF_PRI_X4		0xA0A0A0A0

/* GIC special interrupt id */
#define GIC_INTID_SPURIOUS		1023

/* Fixme: update from platform specific define or dt */
#define GIC_NUM_CPU_IF			MP_MAX_NUM_CPUS

// ####################################################

/* Offsets from GICD base or GICR(n) SGI_base */
#define GIC_DIST_IGROUPR		0x0080
#define GIC_DIST_ISENABLER		0x0100
#define GIC_DIST_ICENABLER		0x0180
#define GIC_DIST_ISPENDR		0x0200
#define GIC_DIST_ICPENDR		0x0280
#define GIC_DIST_ISACTIVER		0x0300
#define GIC_DIST_ICACTIVER		0x0380
#define GIC_DIST_IPRIORITYR		0x0400
#define GIC_DIST_ITARGETSR		0x0800
#define GIC_DIST_ICFGR			0x0C00
#define GIC_DIST_IGROUPMODR		0x0D00
#define GIC_DIST_SGIR			0x0F00

/* GICD GICR common access macros */
#define IGROUPR(base, n)		(base + GIC_DIST_IGROUPR + (n) * 4)
#define ISENABLER(base, n)		(base + GIC_DIST_ISENABLER + (n) * 4)
#define ICENABLER(base, n)		(base + GIC_DIST_ICENABLER + (n) * 4)
#define ISPENDR(base, n)		(base + GIC_DIST_ISPENDR + (n) * 4)
#define ICPENDR(base, n)		(base + GIC_DIST_ICPENDR + (n) * 4)
#define IPRIORITYR(base, n)		(base + GIC_DIST_IPRIORITYR + n)
#define ITARGETSR(base, n)		(base + GIC_DIST_ITARGETSR + (n) * 4)
#define ICFGR(base, n)			(base + GIC_DIST_ICFGR + (n) * 4)
#define IGROUPMODR(base, n)		(base + GIC_DIST_IGROUPMODR + (n) * 4)

// tiny use gicv3 only
#define GET_DIST_BASE(intid)	((intid < GIC_SPI_INT_BASE) ? \
				(gic_get_rdist() + GICR_SGI_BASE_OFF) \
				: GIC_DIST_BASE)

/* Cache and Share ability for ITS & Redistributor LPI state tables */
#define GIC_BASER_CACHE_NGNRNE		0x0UL /* Device-nGnRnE */
#define GIC_BASER_CACHE_INNERLIKE	0x0UL /* Same as Inner Cacheability. */
#define GIC_BASER_CACHE_NCACHEABLE	0x1UL /* Non-cacheable */
#define GIC_BASER_CACHE_RAWT		0x2UL /* Cacheable R-allocate, W-through */
#define GIC_BASER_CACHE_RAWB		0x3UL /* Cacheable R-allocate, W-back */
#define GIC_BASER_CACHE_WAWT		0x4UL /* Cacheable W-allocate, W-through */
#define GIC_BASER_CACHE_WAWB		0x5UL /* Cacheable W-allocate, W-back */
#define GIC_BASER_CACHE_RAWAWT		0x6UL /* Cacheable R-allocate, W-allocate, W-through */
#define GIC_BASER_CACHE_RAWAWB		0x7UL /* Cacheable R-allocate, W-allocate, W-back */
#define GIC_BASER_SHARE_NO		    0x0UL /* Non-shareable */
#define GIC_BASER_SHARE_INNER		0x1UL /* Inner Shareable */
#define GIC_BASER_SHARE_OUTER		0x2UL /* Outer Shareable */

/**
 * GIC Register Interface Base Addresses
 */
#define GIC_RDIST_BASE	        0xFD460000 /* XXX: rk3566 */
#define GIC_RDIST_SIZE	        0x0

/* SGI base is at 64K offset from Redistributor */
#define GICR_SGI_BASE_OFF		0x10000

/* GICR registers offset from RD_base(n) */
#define GICR_CTLR			    0x0000
#define GICR_IIDR			    0x0004
#define GICR_TYPER			    0x0008
#define GICR_STATUSR			0x0010
#define GICR_WAKER			    0x0014
#define GICR_PWRR			    0x0024
#define GICR_PROPBASER			0x0070
#define GICR_PENDBASER			0x0078

/* Register bit definitions */

/* GICD_CTLR Interrupt group definitions */
#define GICD_CTLR_ENABLE_G0		0
#define GICD_CTLR_ENABLE_G1NS	1
#define GICD_CTLR_ENABLE_G1S	2
#define GICD_CTRL_ARE_S			4
#define GICD_CTRL_ARE_NS		5
#define GICD_CTRL_NS			6
#define GICD_CGRL_E1NWF			7

/* GICD_CTLR Register write progress bit */
#define GICD_CTLR_RWP			            31

/* GICR_CTLR */
#define GICR_CTLR_ENABLE_LPIS		        BIT(0)
#define GICR_CTLR_RWP			            3

/* GICR_IIDR */
#define GICR_IIDR_PRODUCT_ID_SHIFT		    24
#define GICR_IIDR_PRODUCT_ID_MASK		    0xFFUL
#define GICR_IIDR_PRODUCT_ID_GET(_val)		\
    MASK_GET(_val, GICR_IIDR_PRODUCT_ID)

/* GICR_TYPER */
#define GICR_TYPER_AFFINITY_VALUE_SHIFT		32
#define GICR_TYPER_AFFINITY_VALUE_MASK		0xFFFFFFFFUL
#define GICR_TYPER_AFFINITY_VALUE_GET(_val)	\
    MASK_GET(_val, GICR_TYPER_AFFINITY_VALUE)
#define GICR_TYPER_LAST_SHIFT			    4
#define GICR_TYPER_LAST_MASK			    0x1UL
#define GICR_TYPER_LAST_GET(_val)		    \
    MASK_GET(_val, GICR_TYPER_LAST)
#define GICR_TYPER_PROCESSOR_NUMBER_SHIFT	8
#define GICR_TYPER_PROCESSOR_NUMBER_MASK	0xFFFFUL
#define GICR_TYPER_PROCESSOR_NUMBER_GET(_val)	\
    MASK_GET(_val, GICR_TYPER_PROCESSOR_NUMBER)

/* GICR_WAKER */
#define GICR_WAKER_PS			            1
#define GICR_WAKER_CA			            2

/* GICR_PWRR */
#define GICR_PWRR_RDPD			            0
#define GICR_PWRR_RDAG			            1
#define GICR_PWRR_RDGPO			            3

/* GICR_PROPBASER */
#define GITR_PROPBASER_ID_BITS_MASK		    0x1FUL
#define GITR_PROPBASER_INNER_CACHE_SHIFT	7
#define GITR_PROPBASER_INNER_CACHE_MASK		0x7UL
#define GITR_PROPBASER_SHAREABILITY_SHIFT	10
#define GITR_PROPBASER_SHAREABILITY_MASK	0x3UL
#define GITR_PROPBASER_ADDR_SHIFT		    12
#define GITR_PROPBASER_ADDR_MASK		    0xFFFFFFFFFFUL
#define GITR_PROPBASER_OUTER_CACHE_SHIFT	56
#define GITR_PROPBASER_OUTER_CACHE_MASK		0x7UL

/* GICR_PENDBASER */
#define GITR_PENDBASER_INNER_CACHE_SHIFT	7
#define GITR_PENDBASER_INNER_CACHE_MASK		0x7UL
#define GITR_PENDBASER_SHAREABILITY_SHIFT	10
#define GITR_PENDBASER_SHAREABILITY_MASK	0x3UL
#define GITR_PENDBASER_ADDR_SHIFT		    16
#define GITR_PENDBASER_ADDR_MASK		    0xFFFFFFFFFUL
#define GITR_PENDBASER_OUTER_CACHE_SHIFT	56
#define GITR_PENDBASER_OUTER_CACHE_MASK		0x7UL
#define GITR_PENDBASER_PTZ			        BIT64(62)

/* GITCD_IROUTER */
#define GIC_DIST_IROUTER		            0x6000
#define IROUTER(base, n)		    \
    (base + GIC_DIST_IROUTER + (n) * 8)

#define MASK(__basename)		    \
    (__basename##_MASK << __basename##_SHIFT)
#define MASK_SET(__val, __basename)	\
    (((__val) & __basename##_MASK) << __basename##_SHIFT)
#define MASK_GET(__reg, __basename)	\
    (((__reg) >> __basename##_SHIFT) & __basename##_MASK)

// ####################################################

int arm_gic_init(void);
void arm_gic_secondary_init(void);

void arm_gic_irq_set_priority(unsigned int intid,
			      unsigned int prio, u32 flags);
void arm_gic_irq_enable(unsigned int intid);
void arm_gic_irq_disable(unsigned int intid);
bool arm_gic_irq_is_enabled(unsigned int intid);
bool arm_gic_irq_is_pending(unsigned int intid);
void arm_gic_irq_set_pending(unsigned int intid);
void arm_gic_irq_clear_pending(unsigned int intid);
unsigned int arm_gic_get_active(void);
void arm_gic_eoi(unsigned int intid);
void gic_raise_sgi(unsigned int sgi_id, u64 target_aff,
		   u16 target_list);

// ####################################################

#endif /* __ASM_GIC_H__ */
