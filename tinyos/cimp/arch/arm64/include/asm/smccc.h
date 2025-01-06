// @Hustler's Project

#ifndef _ASM_SMCCC_H_
#define _ASM_SMCCC_H_

// ####################################################

// @Hustler

/*
 * Result from SMC/HVC call
 * @a0-a7 result values from registers 0 to 7
 */
struct arm_smccc_res {
	unsigned long a0;
	unsigned long a1;
	unsigned long a2;
	unsigned long a3;
	unsigned long a4;
	unsigned long a5;
	unsigned long a6;
	unsigned long a7;
};

enum smmccc_method {
    _HVC = 0x0, // 0b0000
    _SMC = 0x1, // 0b0001
};

// ####################################################

enum psci_reset {
    _RESET  = 0x2, // 0b0010
    _RESET2 = 0x4, // 0b0100
};

// @Hustler
// - Power State Coordination Interface

/* PSCI v1.1 interface. */
#define PSCI_1_1_FN_BASE                  (0x84000000U)
#define PSCI_1_1_64BIT                    (0x40000000U)
#define PSCI_1_1_FN64_BASE                (PSCI_1_1_FN_BASE + PSCI_1_1_64BIT)
#define PSCI_1_1_FN(n)                    (PSCI_1_1_FN_BASE + (n))
#define PSCI_1_1_FN64(n)                  (PSCI_1_1_FN64_BASE + (n))
#define PSCI_1_1_FN_PSCI_VERSION          PSCI_1_1_FN(0)
#define PSCI_1_1_FN_CPU_SUSPEND           PSCI_1_1_FN(1)
#define PSCI_1_1_FN_CPU_OFF               PSCI_1_1_FN(2)
#define PSCI_1_1_FN_CPU_ON                PSCI_1_1_FN(3)
#define PSCI_1_1_FN_AFFINITY_INFO         PSCI_1_1_FN(4)
#define PSCI_1_1_FN_MIGRATE               PSCI_1_1_FN(5)
#define PSCI_1_1_FN_MIGRATE_INFO_TYPE     PSCI_1_1_FN(6)
#define PSCI_1_1_FN_MIGRATE_INFO_UP_CPU   PSCI_1_1_FN(7)
#define PSCI_1_1_FN_SYSTEM_OFF            PSCI_1_1_FN(8)
#define PSCI_1_1_FN_SYSTEM_RESET          PSCI_1_1_FN(9)
#define PSCI_1_1_FN_PSCI_FEATURES         PSCI_1_1_FN(10)
#define PSCI_1_1_FN_SYSTEM_RESET2         PSCI_1_1_FN(18)
#define PSCI_1_1_FN64_CPU_SUSPEND         PSCI_1_1_FN64(1)
#define PSCI_1_1_FN64_CPU_ON              PSCI_1_1_FN64(3)
#define PSCI_1_1_FN64_AFFINITY_INFO       PSCI_1_1_FN64(4)
#define PSCI_1_1_FN64_MIGRATE             PSCI_1_1_FN64(5)
#define PSCI_1_1_FN64_MIGRATE_INFO_UP_CPU PSCI_1_1_FN64(7)
/* PSCI function ID is same for both 32 and 64 bit.*/
#define PSCI_1_1_FN64_SYSTEM_RESET        PSCI_1_1_FN(9)
#define PSCI_1_1_FN64_PSCI_FEATURES       PSCI_1_1_FN(10)
#define PSCI_1_1_FN64_SYSTEM_RESET2       PSCI_1_1_FN64(18)


/* PSCI version decoding (independent of PSCI version) */
#define PSCI_VERSION_MAJOR_SHIFT		16
#define PSCI_VERSION_MINOR_MASK			\
		((1U << PSCI_VERSION_MAJOR_SHIFT) - 1)
#define PSCI_VERSION_MAJOR_MASK			~PSCI_VERSION_MINOR_MASK

#define PSCI_VERSION_MAJOR(ver)			\
		(((ver) & PSCI_VERSION_MAJOR_MASK) >> PSCI_VERSION_MAJOR_SHIFT)
#define PSCI_VERSION_MINOR(ver)			\
		((ver) & PSCI_VERSION_MINOR_MASK)

// ####################################################

void __psci_get_version(int method);
int __psci_cpu_off(int method);
int __psci_cpu_on(int method, unsigned long cpuid,
                  unsigned long entry);
int __psci_reset(int method, int type);

// ####################################################

#endif /* _ASM_SMCCC_H_ */
