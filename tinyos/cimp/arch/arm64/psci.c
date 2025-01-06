// @Hustler's Project

#include <asm/smccc.h>

#include <print.h>

/*
 * Make HVC calls
 *
 * a0 function identifier
 * a1-a7 parameters registers
 * res results
 */
extern void arm_smccc_hvc(unsigned long a0, unsigned long a1,
		   unsigned long a2, unsigned long a3,
		   unsigned long a4, unsigned long a5,
		   unsigned long a6, unsigned long a7,
		   struct arm_smccc_res *res);

/*
 * Make SMC calls
 *
 * a0 function identifier
 * a1-a7 parameters registers
 * res results
 */
extern void arm_smccc_smc(unsigned long a0, unsigned long a1,
		   unsigned long a2, unsigned long a3,
		   unsigned long a4, unsigned long a5,
		   unsigned long a6, unsigned long a7,
		   struct arm_smccc_res *res);

static unsigned long __invoke_psci_hvc(
                        unsigned long function_id,
                        unsigned long arg0,
                        unsigned long arg1,
                        unsigned long arg2)
{
	struct arm_smccc_res res;

	arm_smccc_hvc(function_id, arg0, arg1, arg2, 0, 0, 0, 0, &res);

	return res.a0;
}

static unsigned long __invoke_psci_smc(
                        unsigned long function_id,
                        unsigned long arg0,
                        unsigned long arg1,
                        unsigned long arg2)
{
	struct arm_smccc_res res;

	arm_smccc_smc(function_id, arg0, arg1, arg2, 0, 0, 0, 0, &res);

	return res.a0;
}

void __psci_get_version(int method) {
    u32 ver = 0;

    switch (method) {
    case _HVC:
	    ver = __invoke_psci_hvc(PSCI_1_1_FN_PSCI_VERSION, 0, 0, 0);
        break;
    case _SMC:
	    ver = __invoke_psci_smc(PSCI_1_1_FN_PSCI_VERSION, 0, 0, 0);
        break;
    default:
        break;
    }

    __print__("psci %d.%d\n",
        PSCI_VERSION_MAJOR(ver),
        PSCI_VERSION_MINOR(ver));
}

int __psci_cpu_off(int method) {
    int ret = 0;

    switch (method) {
    case _HVC:
	    ret = __invoke_psci_hvc(PSCI_1_1_FN_CPU_OFF, 0, 0, 0);
        break;
    case _SMC:
	    ret = __invoke_psci_smc(PSCI_1_1_FN_CPU_OFF, 0, 0, 0);
        break;
    default:
        break;
    }

    return ret;
}

int __psci_cpu_on(int method, unsigned long cpuid,
                  unsigned long entry) {
    int ret = 0;

    switch (method) {
    case _HVC:
	    ret = __invoke_psci_hvc(PSCI_1_1_FN_CPU_ON, cpuid, entry, 0);
        break;
    case _SMC:
	    ret = __invoke_psci_smc(PSCI_1_1_FN_CPU_ON, cpuid, entry, 0);
        break;
    default:
        break;
    }

    return ret;
}

int __psci_reset(int method, int type) {
    int ret = 0;

    switch (method) {
    case _HVC | _RESET:
	    ret = __invoke_psci_hvc(PSCI_1_1_FN_SYSTEM_RESET, 0, 0, 0);
        break;
    case _SMC | _RESET:
	    ret = __invoke_psci_smc(PSCI_1_1_FN_SYSTEM_RESET, 0, 0, 0);
        break;
    case _HVC | _RESET2:
	    ret = __invoke_psci_hvc(PSCI_1_1_FN_SYSTEM_RESET2, 0, 0, 0);
        break;
    case _SMC | _RESET2:
	    ret = __invoke_psci_smc(PSCI_1_1_FN_SYSTEM_RESET2, 0, 0, 0);
        break;
    default:
        break;
    }

    return ret;
}
