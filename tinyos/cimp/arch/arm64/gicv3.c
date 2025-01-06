// @Hustler's Project

#include <asm/reg.h>
#include <asm/gic.h>
#include <asm/smp.h>
#include <asm/io.h>

#include <string.h>

#ifdef __RK356X__
#include <hal/rk3566.h>
#endif

#include <print.h>
#include <bit.h>

// ####################################################

#define MP_MAX_NUM_CPUS     NR_CPU

/**
 * XXX: Redistributor base addresses for each core
 *
 * There is one Redistributor per connected core.
 * The Redistributors provide a programming interface
 * to:
 * [0] Enable and disable SGIs and PPIs
 * [1] Set the priority level of SGIs and PPIs
 * [2] Set each PPI to be level-sensitive or edge-triggered
 * [3] Assign each SGI and PPI to an interrupt group
 * [4] Control the state of SGIs and PPIs
 * [5] Control the base address for the data structures in
 *     memory that support the associated interrupt
 *     properties and pending state for LPIs
 * [6] Provide power management support for the connected PE
 */
xmem_t gic_rdists[MP_MAX_NUM_CPUS];

#define ARMV8_A_NS        0

#if ARMV8_A_NS
#define IGROUPR_VAL	      0xFFFFFFFFU
#else
#define IGROUPR_VAL	      0x0U
#endif

static inline xmem_t gic_get_rdist(void)
{
	return gic_rdists[smp_processor_id()];
}

/**
 * XXX:
 *
 * The Distributor registers are memory-mapped and used to
 * configure SPIs. The Distributor provides a programming
 * interface for:
 *
 * [0] Enable and disable SPIs
 * [1] Set the priority level of each SPI
 * [2] Route information for each SPI
 * [3] Set each SPI to be level-sensitive or edge-triggered
 * [4] Generate message-signaled SPIs
 * [5] Control the active and pending state of SPIs
 * [6] Determine the programmer’s model that is used in each
 *     Security state: affinity routing or legacy.
 *
 * Wait for register write pending
 * TODO: add timed wait
 */
static int gic_wait_rwp(u32 intid)
{
	u32 rwp_mask;
	xmem_t base;

	if (intid < GIC_SPI_INT_BASE) {
		base = (gic_get_rdist() + GICR_CTLR);
		rwp_mask = BIT(GICR_CTLR_RWP);
	} else {
		base = GICD_CTLR;
		rwp_mask = BIT(GICD_CTLR_RWP);
	}

	while (readl(base) & rwp_mask)
		;

	return 0;
}

#if ARMV8_A_NS
static inline void arm_gic_write_irouter(u64 val, unsigned int intid)
{
	xmem_t addr = IROUTER(GET_DIST_BASE(intid), intid);

	writeq(val, addr);
}
#endif

void arm_gic_irq_set_priority(unsigned int intid,
			      unsigned int prio, u32 flags)
{
	u32 mask = BIT(intid & (GIC_NUM_INTR_PER_REG - 1));
	u32 idx = intid / GIC_NUM_INTR_PER_REG;
	u32 shift;
	u32 val;
	xmem_t base = GET_DIST_BASE(intid);

	/* Disable the interrupt */
	writel(mask, ICENABLER(base, idx));
	gic_wait_rwp(intid);

	/* PRIORITYR registers provide byte access */
	writeb(prio & GIC_PRI_MASK, IPRIORITYR(base, intid));

	/* Interrupt type config */
	if (!GIC_IS_SGI(intid)) {
		idx = intid / GIC_NUM_CFG_PER_REG;
		shift = (intid & (GIC_NUM_CFG_PER_REG - 1)) * 2;

		val = readl(ICFGR(base, idx));
		val &= ~(GICD_ICFGR_MASK << shift);
		if (flags & IRQ_TYPE_EDGE) {
			val |= (GICD_ICFGR_TYPE << shift);
		}
		writel(val, ICFGR(base, idx));
	}
}

void arm_gic_irq_enable(unsigned int intid)
{
	u32 mask = BIT(intid & (GIC_NUM_INTR_PER_REG - 1));
	u32 idx = intid / GIC_NUM_INTR_PER_REG;

#if ARMV8_A_NS
	/*
	 * Affinity routing is enabled for Armv8-A Non-secure
     * state (GICD_CTLR.ARE_NS
	 * is set to '1') and for GIC single security state
     * (GICD_CTRL.ARE is set to '1'),
	 * so need to set SPI's affinity, now set it to be the
     * PE on which it is enabled.
	 */
	if (GIC_IS_SPI(intid)) {
		arm_gic_write_irouter(MPIDR_TO_CORE(GET_MPIDR()), intid);
	}
#endif

	writel(mask, ISENABLER(GET_DIST_BASE(intid), idx));
}

void arm_gic_irq_disable(unsigned int intid)
{
	u32 mask = BIT(intid & (GIC_NUM_INTR_PER_REG - 1));
	u32 idx = intid / GIC_NUM_INTR_PER_REG;

	writel(mask, ICENABLER(GET_DIST_BASE(intid), idx));
	/* poll to ensure write is complete */
	gic_wait_rwp(intid);
}

bool arm_gic_irq_is_enabled(unsigned int intid)
{
	u32 mask = BIT(intid & (GIC_NUM_INTR_PER_REG - 1));
	u32 idx = intid / GIC_NUM_INTR_PER_REG;
	u32 val;

	val = readl(ISENABLER(GET_DIST_BASE(intid), idx));

	return (val & mask) != 0;
}

bool arm_gic_irq_is_pending(unsigned int intid)
{
	u32 mask = BIT(intid & (GIC_NUM_INTR_PER_REG - 1));
	u32 idx = intid / GIC_NUM_INTR_PER_REG;
	u32 val;

	val = readl(ISPENDR(GET_DIST_BASE(intid), idx));

	return (val & mask) != 0;
}

void arm_gic_irq_set_pending(unsigned int intid)
{
	u32 mask = BIT(intid & (GIC_NUM_INTR_PER_REG - 1));
	u32 idx = intid / GIC_NUM_INTR_PER_REG;

	writel(mask, ISPENDR(GET_DIST_BASE(intid), idx));
}

void arm_gic_irq_clear_pending(unsigned int intid)
{
	u32 mask = BIT(intid & (GIC_NUM_INTR_PER_REG - 1));
	u32 idx = intid / GIC_NUM_INTR_PER_REG;

	writel(mask, ICPENDR(GET_DIST_BASE(intid), idx));
}

/**
 * XXX:
 *
 * Each core contains a CPU interface, which are
 * system registers that are used during interrupt
 * handling. The CPU interfaces provide a
 * programming interface to:
 *
 * [0] Provide general control and configuration to
 *     enable interrupt handling
 * [1] Acknowledge an interrupt
 * [2] Perform a priority drop and deactivation of
 *     interrupts
 * [3] Set an interrupt priority mask for the PE
 * [4] Define the preemption policy for the PE
 * [5] Determine the highest priority pending
 *     interrupt for the PE
 */
unsigned int arm_gic_get_active(void)
{
	int intid;

	/* (Pending -> Active / AP) or (AP -> AP) */
	intid = READ_SYSREG(ICC_IAR1_EL1);

	return intid;
}

void arm_gic_eoi(unsigned int intid)
{
	/**
     * XXX:
     *
	 * Interrupt request deassertion from peripheral to GIC happens
	 * by clearing interrupt condition by a write to the peripheral
	 * register. It is desired that the write transfer is complete
	 * before the core tries to change GIC state from 'AP/Active' to
	 * a new state on seeing 'EOI write'.
	 * Since ICC interface writes are not ordered against Device
	 * memory writes, a barrier is required to ensure the ordering.
	 * The dsb will also ensure *completion* of previous writes with
	 * DEVICE nGnRnE attribute.
	 */
	dsb(sy);

	/* (AP -> Pending) Or (Active -> Inactive) or (AP to AP)
     * nested case
     */
	WRITE_SYSREG(intid, ICC_EOIR1_EL1);
}

void gic_raise_sgi(unsigned int sgi_id, u64 target_aff,
		   u16 target_list)
{
	u32 aff3, aff2, aff1;
	u64 sgi_val;

	/* __ASSERT_NO_MSG(GIC_IS_SGI(sgi_id)); */

	/* Extract affinity fields from target */
	aff1 = MPIDR_AFFLVL(target_aff, 1);
	aff2 = MPIDR_AFFLVL(target_aff, 2);
	aff3 = MPIDR_AFFLVL(target_aff, 3);

	sgi_val = GICV3_SGIR_VALUE(aff3, aff2, aff1, sgi_id,
				   SGIR_IRM_TO_AFF, target_list);

	dsb(sy);
	WRITE_SYSREG(sgi_val, ICC_SGI1R);
	isb();
}

/**
 * XXX:
 *
 * Wake up GIC redistributor.
 * clear ProcessorSleep and wait till ChildAsleep is cleared.
 * ProcessSleep to be cleared only when ChildAsleep is set
 * Check if redistributor is not powered already.
 */
static void gicv3_rdist_enable(xmem_t rdist)
{
	if (!(readl(rdist + GICR_WAKER) & BIT(GICR_WAKER_CA))) {
		return;
	}

	if (GICR_IIDR_PRODUCT_ID_GET(readl(rdist + GICR_IIDR)) >= 0x2) {
		if (readl(rdist + GICR_PWRR) & BIT(GICR_PWRR_RDPD)) {
			__set_bit(rdist + GICR_PWRR, GICR_PWRR_RDAG);
			__clear_bit(rdist + GICR_PWRR, GICR_PWRR_RDPD);
			while (readl(rdist + GICR_PWRR) & BIT(GICR_PWRR_RDPD)) {
				;
			}
		}
	}

	__clear_bit(rdist + GICR_WAKER, GICR_WAKER_PS);

	while (readl(rdist + GICR_WAKER) & BIT(GICR_WAKER_CA))
		;
}

/*
 * Initialize the cpu interface. This should be called by each core.
 */
static void gicv3_cpuif_init(void)
{
	u32 icc_sre;
	u32 intid;

	xmem_t base = gic_get_rdist() + GICR_SGI_BASE_OFF;

	/* Disable all sgi ppi */
	writel(BIT_MASK(GIC_NUM_INTR_PER_REG), ICENABLER(base, 0));
	/* Any sgi/ppi intid ie. 0-31 will select GICR_CTRL */
	gic_wait_rwp(0);

	/* Clear pending */
	writel(BIT_MASK(GIC_NUM_INTR_PER_REG), ICPENDR(base, 0));

	/* Configure all SGIs/PPIs as G1S or G1NS depending on Zephyr
	 * is run in EL1S or EL1NS respectively.
	 * All interrupts will be delivered as irq
	 */
	writel(IGROUPR_VAL, IGROUPR(base, 0));
	writel(BIT_MASK(GIC_NUM_INTR_PER_REG), IGROUPMODR(base, 0));

	/*
	 * Configure default priorities for SGI 0:15 and PPI 0:15.
	 */
	for (intid = 0; intid < GIC_SPI_INT_BASE;
	     intid += GIC_NUM_PRI_PER_REG) {
		writel(GIC_INT_DEF_PRI_X4, IPRIORITYR(base, intid));
	}

	/* Configure PPIs as level triggered */
	writel(0, ICFGR(base, 1));

	/*
	 * Check if system interface can be enabled.
	 * 'icc_sre_el3' needs to be configured at 'EL3'
	 * to allow access to 'icc_sre_el1' at 'EL1'
	 * eg: z_arch_el3_plat_init can be used by platform.
	 */
	icc_sre = READ_SYSREG(ICC_SRE_EL1);

	if (!(icc_sre & ICC_SRE_ELx_SRE_BIT)) {
		icc_sre = (icc_sre | ICC_SRE_ELx_SRE_BIT |
			   ICC_SRE_ELx_DIB_BIT | ICC_SRE_ELx_DFB_BIT);
		WRITE_SYSREG(icc_sre, ICC_SRE_EL1);
		icc_sre = READ_SYSREG(ICC_SRE_EL1);

		/* __ASSERT_NO_MSG(icc_sre & ICC_SRE_ELx_SRE_BIT); */
	}

	WRITE_SYSREG(GIC_IDLE_PRIO, ICC_PMR_EL1);

	/* Allow group1 interrupts */
	WRITE_SYSREG(1, ICC_IGRPEN1_EL1);
}

static void gicv3_dist_init(void)
{
	unsigned int num_ints;
	unsigned int intid;
	unsigned int idx;
	xmem_t base = GIC_DIST_BASE;

	/*
	 * Currently multiple OSes can run one the different CPU
     * Cores which share single GIC,
	 * but GIC distributor should avoid to be re-configured
     * in order to avoid crash the
	 * OSes has already been started.
	 */
	if (readl(GICD_CTLR) & (BIT(GICD_CTLR_ENABLE_G0)
                | BIT(GICD_CTLR_ENABLE_G1NS))) {
		return;
	}

	num_ints = readl(GICD_TYPER);
	num_ints &= GICD_TYPER_ITLINESNUM_MASK;
	num_ints = (num_ints + 1) << 5;

	/* Disable the distributor */
	writel(0, GICD_CTLR);
	gic_wait_rwp(GIC_SPI_INT_BASE);

	/*
	 * Default configuration of all SPIs
	 */
	for (intid = GIC_SPI_INT_BASE; intid < num_ints;
	     intid += GIC_NUM_INTR_PER_REG) {
		idx = intid / GIC_NUM_INTR_PER_REG;
		/* Disable interrupt */
		writel(BIT_MASK(GIC_NUM_INTR_PER_REG),
			    ICENABLER(base, idx));
		/* Clear pending */
		writel(BIT_MASK(GIC_NUM_INTR_PER_REG),
			    ICPENDR(base, idx));
		writel(IGROUPR_VAL, IGROUPR(base, idx));
		writel(BIT_MASK(GIC_NUM_INTR_PER_REG),
			    IGROUPMODR(base, idx));

	}

	/* wait for rwp on GICD */
	gic_wait_rwp(GIC_SPI_INT_BASE);

	/* Configure default priorities for all SPIs. */
	for (intid = GIC_SPI_INT_BASE; intid < num_ints;
	     intid += GIC_NUM_PRI_PER_REG) {
		writel(GIC_INT_DEF_PRI_X4, IPRIORITYR(base, intid));
	}

	/* Configure all SPIs as active low, level triggered by default */
	for (intid = GIC_SPI_INT_BASE; intid < num_ints;
	     intid += GIC_NUM_CFG_PER_REG) {
		idx = intid / GIC_NUM_CFG_PER_REG;
		writel(0, ICFGR(base, idx));
	}

    tiny_print("configure gic distributor\n");

#if ARMV8_A_NS
	/* Enable distributor with ARE */
	writel(BIT(GICD_CTRL_ARE_NS) | BIT(GICD_CTLR_ENABLE_G1NS),
		    GICD_CTLR);

	/* enable Group 1 secure interrupts */
	__set_bit(GICD_CTLR, GICD_CTLR_ENABLE_G1S);
#endif
}

static u64 arm_gic_mpidr_to_affinity(u64 mpidr)
{
	u64 aff3, aff2, aff1, aff0;

	aff3 = MPIDR_AFFLVL(mpidr, 3);
	aff2 = MPIDR_AFFLVL(mpidr, 2);
	aff1 = MPIDR_AFFLVL(mpidr, 1);
	aff0 = MPIDR_AFFLVL(mpidr, 0);

	return (aff3 << 24 | aff2 << 16 | aff1 << 8 | aff0);
}

static bool arm_gic_aff_matching(u64 gicr_aff, u64 aff)
{
#if 0
	u64 mask = BIT_MASK(8);

	return (gicr_aff & mask) == (aff & mask);
#else
	return gicr_aff == aff;
#endif
}

static inline u64 arm_gic_get_typer(xmem_t addr)
{
	u64 val;

	val = readq(addr);

	return val;
}

static xmem_t arm_gic_iterate_rdists(void)
{
	u64 aff = arm_gic_mpidr_to_affinity(READ_SYSREG(mpidr_el1));

	for (xmem_t rdist_addr = GIC_RDIST_BASE;
		rdist_addr < GIC_RDIST_BASE + GIC_RDIST_SIZE;
		rdist_addr += 0x20000) {
		u64 val = arm_gic_get_typer(rdist_addr + GICR_TYPER);
		u64 gicr_aff = GICR_TYPER_AFFINITY_VALUE_GET(val);

		if (arm_gic_aff_matching(gicr_aff, aff))
			return rdist_addr;

		if (GICR_TYPER_LAST_GET(val) == 1)
			return (xmem_t)NULL;
	}

	return (xmem_t)NULL;
}

static void __arm_gic_init(void)
{
	u8 cpu;
	xmem_t gic_rd_base;

	cpu = smp_processor_id();

	gic_rd_base = arm_gic_iterate_rdists();
	/* __ASSERT(gic_rd_base != (xmem_t)NULL, ""); */

	gic_rdists[cpu] = gic_rd_base;

	gicv3_rdist_enable(gic_get_rdist());

    tiny_print("enble gic redistributor\n");

	gicv3_cpuif_init();
}

int __init arm_gic_init(void)
{
	gicv3_dist_init();

	__arm_gic_init();

    tiny_print("gic init done\n");

	return 0;
}

void __init arm_gic_secondary_init(void)
{
	__arm_gic_init();
}

// ####################################################
