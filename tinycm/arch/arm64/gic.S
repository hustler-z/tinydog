// @Hustler's Project

#include <asm/asm.h>

    __GIC

// void __set_icc_sre_el1__(unsigned int value)
FUNC(__set_icc_sre_el1__)
    msr   ICC_SRE_EL1, x0
    isb
    ret
END(__set_icc_sre_el1__)

// unsigned int __get_icc_sre_el1__(void)
FUNC(__get_icc_sre_el1__)
    mrs   x0, ICC_SRE_EL1
    ret
END(__get_icc_sre_el1__)

// void __set_icc_sre_el2__(unsigned int value)
FUNC(__set_icc_sre_el2__)
    msr   ICC_SRE_EL2, x0
    isb
    ret
END(__set_icc_sre_el2__)

// unsigned int __get_icc_sre_el2__(void)
FUNC(__get_icc_sre_el2__)
    mrs   x0, ICC_SRE_EL2
    ret
END(__get_icc_sre_el2__)

// void __set_icc_sre_el3__(unsigned int value)
FUNC(__set_icc_sre_el3__)
    msr   ICC_SRE_EL3, x0
    isb
    ret
END(__set_icc_sre_el3__)


// unsigned int __get_icc_sre_el3__(void)
FUNC(__get_icc_sre_el3__)
    mrs   x0, ICC_SRE_EL3
    ret
END(__get_icc_sre_el3__)

// @Hustler
//
// Interrupt Controller Interrupt Group 0 Enable Register
//
// void __enable_group0_irq__(void)
FUNC(__enable_group0_irq__)
    DBG("[A] enable gic group-0\r\n") // TODO

    mrs   x0, ICC_IGRPEN0_EL1
    orr   w0, w0, #1
    msr   ICC_IGRPEN0_EL1, x0
    isb
    ret
END(__enable_group0_irq__)

// void __disable_group0_irq__(void)
FUNC(__disable_group0_irq__)
    mov   w0, #0
    msr   ICC_IGRPEN0_EL1, x0
    isb
    ret
END(__disable_group0_irq__)


// void __enable_group1_irq__(void)
FUNC(__enable_group1_irq__)
    DBG("[A] enable gic group-1\r\n")

    mrs   x0, ICC_IGRPEN1_EL1
    orr   w0, w0, #1
    msr   ICC_IGRPEN1_EL1, x0
    isb
    ret
END(__enable_group1_irq__)


// void __disable_group1_irq__(void)
FUNC(__disable_group1_irq__)
    mov   w1, #1
    mrs   x0, ICC_IGRPEN1_EL1
    bic   x0, x0, x1
    msr   ICC_IGRPEN1_EL1, x0
    isb
    ret
END(__disable_group1_irq__)


// void __enable_ns_group1_irq__(void)
FUNC(__enable_ns_group1_irq__)
    mrs   x0, ICC_IGRPEN1_EL3
    orr   w0, w0, #1
    msr   ICC_IGRPEN1_EL3, x0
    isb
    ret
END(__enable_ns_group1_irq__)


// void __disable_ns_group1_irq__(void)
FUNC(__disable_ns_group1_irq__)
    mov   w1, #0x1
    mrs   x0, ICC_IGRPEN1_EL3
    bic   x0, x0, x1
    msr   ICC_IGRPEN1_EL3, x0
    isb
    ret
END(__disable_ns_group1_irq__)


// unsigned int __get_icc_ctlr_el3__(void)
FUNC(__get_icc_ctlr_el3__)
  MRS   x0, ICC_CTLR_EL3
  RET
END(__get_icc_ctlr_el3__)


// void __set_icc_ctlr_el3__(unsigned int value)
FUNC(__set_icc_ctlr_el3__)
    msr   ICC_CTLR_EL3, x0
    isb
    ret
END(__set_icc_ctlr_el3__)


// unsigned int __get_icc_ctlr_el1__(void)
FUNC(__get_icc_ctlr_el1__)
    mrs   x0, ICC_CTLR_EL1
    ret
END(__get_icc_ctlr_el1__)


// void __set_icc_ctlr_el1__(unsigned int value)
FUNC(__set_icc_ctlr_el1__)
    msr   ICC_CTLR_EL1, x0
    isb
    ret
END(__set_icc_ctlr_el1__)


// unsigned int __read_iar_grp0__(void)
FUNC(__read_iar_grp0__)
    mrs   x0, ICC_IAR0_EL1
    ret
END(__read_iar_grp0__)


// void __write_eoi_grp0__(unsigned int id)
FUNC(__write_eoi_grp0__)
    msr   ICC_EOIR0_EL1, x0
    isb
    ret
END(__write_eoi_grp0__)


// void __write_dir__(unsigned int id)
FUNC(__write_dir__)
    msr   ICC_DIR_EL1, x0
    isb
    ret
END(__write_dir__)


// unsigned int __read_iar_grp1__(void)
FUNC(__read_iar_grp1__)
    mrs   x0, ICC_IAR1_EL1
    ret
END(__read_iar_grp1__)

// void __write_eoi_grp1__(unsigned int id)
FUNC(__write_eoi_grp1__)
    msr   ICC_EOIR1_EL1, x0
    dsb   sy
    ret
END(__write_eoi_grp1__)

// void __set_priority_mask__(unsigned int priority)
//
// Provides an interrupt priority filter. Only
// interrupts with a higher priority than the value
// in this register are signaled to the PE.
FUNC(__set_priority_mask__)
    msr   ICC_PMR_EL1, x0
    ret
END(__set_priority_mask__)

// unsigned int __get_bpr0__(void)
FUNC(__get_bpr0__)
    mrs   x0, ICC_BPR0_EL1
    ret
END(__get_bpr0__)

// void __set_bpr0__(unsigned int priority)
FUNC(__set_bpr0__)
    msr   ICC_BPR0_EL1, x0
    isb
    ret
END(__set_bpr0__)

// unsigned int __get_bpr1__(void)
FUNC(__get_bpr1__)
    mrs   x0, ICC_BPR1_EL1
    ret
END(__get_bpr1__)

// void __set_bpr1__(unsigned int priority)
FUNC(__set_bpr1__)
    msr   ICC_BPR1_EL1, x0
    isb
    ret
END(__set_bpr1__)

// unsigned int __get_running_priority__(void)
FUNC(__get_running_priority__)
    mrs   x0, ICC_RPR_EL1
    ret
END(__get_running_priority__)


// @Hustler
//
// Interrupt Controller Software Generated Interrupt
// Group 0 Register
//
// void __send_group0_sgi__(unsigned int id,
//                          unsigned int mode,
//                          unsigned target_list)
FUNC(__send_group0_sgi__)
    orr   x0, x0, x1
    orr   x0, x0, x2
    msr   ICC_SGI0R_EL1, x0
    ret
END(__send_group0_sgi__)


// void __send_group1_sgi__(unsigned int id,
//                          unsigned int mode,
//                          unsigned target_list)
FUNC(__send_group1_sgi__)
    orr   x0, x0, x1
    orr   x0, x0, x2
    msr   ICC_SGI1R_EL1, x0
    ret
END(__send_group1_sgi__)

// void __send_other_group1_sgi__(unsigned int id,
//                                unsigned int mode,
//                                unsigned target_list)
FUNC(__send_other_group1_sgi__)
    orr   x0, x0, x1
    orr   x0, x0, x2
    msr   ICC_ASGI1R_EL1, x0
    ret
END(__send_other_group1_sgi__)
