// @Hustler's Project

#ifndef _ASM_REG_H_
#define _ASM_REG_H_

// ####################################################

#ifndef __ASSEMBLY__

#include <compiler.h>
#include <type.h>

#define WRITE_SYSREG64(v, name) do {                          \
    u64 _r = (v);                                             \
    asm volatile("msr "__stringify(name)", %0" : : "r" (_r)); \
} while (0)

#define READ_SYSREG64(name) ({                                \
    u64 _r;                                                   \
    asm volatile("mrs  %0, "__stringify(name) : "=r" (_r));   \
    _r; })

#define READ_REG(Xn) ({                                       \
    u64 reg;                                                  \
    asm volatile("mov %0, "__stringify(Xn) : "=r"(reg));      \
    reg; })

#define READ_SYSREG(name)          READ_SYSREG64(name)
#define WRITE_SYSREG(v, name)      WRITE_SYSREG64(v, name)

/* System register interface to GICv3 */
#define ICC_IGRPEN1_EL1		S3_0_C12_C12_7
#define ICC_SGI1R		    S3_0_C12_C11_5
#define ICC_SRE_EL1		    S3_0_C12_C12_5
#define ICC_SRE_EL2		    S3_4_C12_C9_5
#define ICC_SRE_EL3		    S3_6_C12_C12_5
#define ICC_CTLR_EL1		S3_0_C12_C12_4
#define ICC_CTLR_EL3		S3_6_C12_C12_4
#define ICC_PMR_EL1		    S3_0_C4_C6_0
#define ICC_RPR_EL1		    S3_0_C12_C11_3
#define ICC_IGRPEN1_EL3		S3_6_C12_C12_7
#define ICC_IGRPEN0_EL1		S3_0_C12_C12_6
#define ICC_HPPIR0_EL1		S3_0_C12_C8_2
#define ICC_HPPIR1_EL1		S3_0_C12_C12_2
#define ICC_IAR0_EL1		S3_0_C12_C8_0
#define ICC_IAR1_EL1		S3_0_C12_C12_0
#define ICC_EOIR0_EL1		S3_0_C12_C8_1
#define ICC_EOIR1_EL1		S3_0_C12_C12_1
#define ICC_SGI0R_EL1		S3_0_C12_C11_7

/* register constants */
#define ICC_SRE_ELx_SRE_BIT	BIT(0)
#define ICC_SRE_ELx_DFB_BIT	BIT(1)
#define ICC_SRE_ELx_DIB_BIT	BIT(2)
#define ICC_SRE_EL3_EN_BIT	BIT(3)

/* ICC SGI macros */
#define SGIR_TGT_MASK		(0xffff)
#define SGIR_AFF1_SHIFT		(16)
#define SGIR_AFF2_SHIFT		(32)
#define SGIR_AFF3_SHIFT		(48)
#define SGIR_AFF_MASK		(0xff)
#define SGIR_INTID_SHIFT	(24)
#define SGIR_INTID_MASK		(0xf)
#define SGIR_IRM_SHIFT		(40)
#define SGIR_IRM_MASK		(0x1)
#define SGIR_IRM_TO_AFF		(0)

#define GICV3_SGIR_VALUE(_aff3, _aff2, _aff1, _intid, _irm, _tgt) \
	((((u64) (_aff3) & SGIR_AFF_MASK) << SGIR_AFF3_SHIFT) |	      \
	 (((u64) (_irm) & SGIR_IRM_MASK) << SGIR_IRM_SHIFT) |	      \
	 (((u64) (_aff2) & SGIR_AFF_MASK) << SGIR_AFF2_SHIFT) |	      \
	 (((_intid) & SGIR_INTID_MASK) << SGIR_INTID_SHIFT) |		  \
	 (((_aff1) & SGIR_AFF_MASK) << SGIR_AFF1_SHIFT) |		      \
	 ((_tgt) & SGIR_TGT_MASK))

#endif /* !__ASSEMBLY__ */

#include <asm/bit.h>

#define CPTR_EZ_BIT		    BIT(8)
#define CPTR_TFP_BIT		BIT(10)
#define CPTR_TSM_BIT        BIT(12)
#define CPTR_TTA_BIT		BIT(20) // access trace register
#define CPTR_TCPAC_BIT		BIT(31)
#define CPTR_EL2_RES1		(BIT(13) | BIT(9) | (0xFFUL))

// @Hustler
//
// Hypervisor System Trap Register
//
// Controls trapping to EL2 of EL1 or lower AArch32 accesses to
// the System register in the coproc == 0b1111 encoding
// space, by the CRn value used to access the register using MCR
// or MRC instruction. When the register is accessible using an
// MCRR or MRRC instruction, this is the CRm value used to access
// the register.
#define HSTR_MASK(x)        BIT(x)
/* MPIDR */
#define MPIDR_AFFLVL_MASK	(0xFFULL)

#define MPIDR_AFF0_SHIFT	(0)
#define MPIDR_AFF1_SHIFT	(8)
#define MPIDR_AFF2_SHIFT	(16)
#define MPIDR_AFF3_SHIFT	(32)

#define MPIDR_AFF_MASK		(GENMASK(23, 0) | GENMASK(39, 32))

#define MPIDR_AFFLVL(mpidr, aff_level) \
		(((mpidr) >> MPIDR_AFF##aff_level##_SHIFT) & MPIDR_AFFLVL_MASK)

// @Hustler
#define SCTLR_EL3_RES1		(BIT(29) | BIT(28) | BIT(23) | \
                            BIT(22) | BIT(18) | BIT(16) | \
                            BIT(11) | BIT(5)  | BIT(4))

#define SCTLR_EL2_RES1		(BIT(29) | BIT(28) | BIT(23) | \
                            BIT(22) | BIT(18) | BIT(16) | \
                            BIT(11) | BIT(5)  | BIT(4))

#define SCTLR_EL1_RES1		(BIT(29) | BIT(28) | BIT(23) | \
                            BIT(22) | BIT(20) | BIT(11))

#define SCTLR_M_BIT		    BIT(0)
#define SCTLR_A_BIT		    BIT(1)
#define SCTLR_C_BIT		    BIT(2)
#define SCTLR_SA_BIT		BIT(3)
#define SCTLR_I_BIT		    BIT(12)
#define SCTLR_BR_BIT		BIT(17)

#define DAIFSET_FIQ_BIT		BIT(0)
#define DAIFSET_IRQ_BIT		BIT(1)
#define DAIFSET_ABT_BIT		BIT(2)
#define DAIFSET_DBG_BIT		BIT(3)

#define DAIFCLR_FIQ_BIT		BIT(0)
#define DAIFCLR_IRQ_BIT		BIT(1)
#define DAIFCLR_ABT_BIT		BIT(2)
#define DAIFCLR_DBG_BIT		BIT(3)

#define SCR_NS_BIT		    BIT(0)
#define SCR_IRQ_BIT		    BIT(1)
#define SCR_FIQ_BIT		    BIT(2)
#define SCR_EA_BIT		    BIT(3)
#define SCR_SMD_BIT		    BIT(7)
#define SCR_HCE_BIT		    BIT(8)
#define SCR_RW_BIT		    BIT(10)
#define SCR_ST_BIT		    BIT(11)
#define SCR_EEL2_BIT		BIT(18)

#define ESR_EC_SHIFT		(26)
#define ESR_EC_MASK		    BIT_MASK(6)
#define ESR_ISS_SHIFT		(0)
#define ESR_ISS_MASK		BIT_MASK(25)
#define ESR_IL_SHIFT		(25)
#define ESR_IL_MASK		    BIT_MASK(1)

#define GET_ESR_EC(esr)		(((esr) >> ESR_EC_SHIFT) & ESR_EC_MASK)
#define GET_ESR_IL(esr)		(((esr) >> ESR_IL_SHIFT) & ESR_IL_MASK)
#define GET_ESR_ISS(esr)	(((esr) >> ESR_ISS_SHIFT) & ESR_ISS_MASK)

#define HCR_FMO_BIT		    BIT(3)
#define HCR_IMO_BIT		    BIT(4)
#define HCR_AMO_BIT		    BIT(5)
#define HCR_TGE_BIT		    BIT(27)
#define HCR_RW_BIT		    BIT(31)

#define MPIDR_AFF_OFFSET    (8)

// ####################################################

#endif /* _ASM_REG_H_ */
