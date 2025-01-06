// @Hustler's Project

#include <asm/asm.h>

    __TIMER

// u32 __get_cntfrq__(void)
FUNC(__get_cntfrq__)
    mrs   x0, CNTFRQ_EL0
    ret
END(__get_cntfrq__)


// void __set_cntfrq__(u32 freq)
// Sets the value of CNTFRQ_EL0 (only possible at EL3)
// w0 - freq - The value to be written into CNTFRQ_EL0
FUNC(__set_cntfrq__)
    msr   CNTFRQ_EL0, x0
    ret
END(__set_cntfrq__)


// u64 __get_physical_count__(void)
// Returns the current value of physical count (CNTPCT_EL0)
FUNC(__get_physical_count__)
    isb
    mrs   x0, CNTPCT_EL0
    ret
END(__get_physical_count__)


// u64 __get_virtual_count__(void)
// Returns the current value of the virtual count
// register (CNTVCT_EL0)
FUNC(__get_virtual_count__)
    isb
    mrs   x0, CNTVCT_EL0
    ret
END(__get_virtual_count__)


// u32 __get_eL1_ctrl__(void)
// Returns the value of EL1 Timer Control Register (CNTKCTL_EL1)
FUNC(__get_el1_ctrl__)
    mrs   x0, CNTKCTL_EL1
    ret
END(__get_el1_ctrl__)


// void __set_el1_ctrl__(u32 value)
// Sets the value of Counter Non-secure EL1 Control
// Register (CNTKCTL_EL1)
// 0 - value - The value to be written into CNTKCTL_EL1
FUNC(__set_el1_ctrl__)
    msr   CNTKCTL_EL1, x0
    ret
END(__set_el1_ctrl__)


// u32 __get_el2_ctrl__(void)
// Returns the value of the EL2 Timer Control Register (CNTHCTL_EL2)
FUNC(__get_el2_ctrl__)
    mrs   x0, CNTHCTL_EL2
    ret
END(__get_el2_ctrl__)


// void __set_el2_ctrl__(u32 value)
// Sets the value of the EL2 Timer Control Register (CNTHCTL_EL2)
// x0 - value - The value to be written into CNTHCTL_EL2
FUNC(__set_el2_ctrl__)
    msr   CNTHCTL_EL2, x0
    ret
END(__set_el2_ctrl__)


// u64 __get_ns_el1_physical_comp_value__(void)
// Returns the value of Non-secure EL1 Physical Compare Value
// Register (CNTP_CVAL_EL0)
FUNC(__get_ns_el1_physical_comp_value__)
    mrs   x0, CNTP_CVAL_EL0
    ret
END(__get_ns_el1_physical_comp_value__)


// void __set_ns_el1_physical_comp_value__(u64 value)
// Sets the value of the Non-secure EL1 Physical Compare Value
// Register (CNTP_CVAL_EL0)
// x0 - value - The value to be written into CNTP_CVAL_EL0
FUNC(__set_ns_el1_physical_comp_value__)
    msr   CNTP_CVAL_EL0, x0
    ret
END(__set_ns_el1_physical_comp_value__)


// u32 __get_ns_el1_physical_timer_value__(void)
// Returns the value of Non-secure EL1 Physical Timer Value
// Register (CNTP_TVAL_EL0)
FUNC(__get_ns_el1_physical_timer_value__)
    mrs   x0, CNTP_TVAL_EL0
    ret
END(__get_ns_el1_physical_timer_value__)


// void __set_ns_el1_physical_timer_value__(u32 value)
// Sets the value of the Non-secure EL1 Physical Timer Value
// Register (CNTP_TVAL_EL0)
// w0 - value - The value to be written into CNTP_TVAL_EL0
FUNC(__set_ns_el1_physical_timer_value__)
    msr   CNTP_TVAL_EL0, x0
    ret
END(__set_ns_el1_physical_timer_value__)


// u32 __get_ns_el1_physical_timer_ctrl__(void)
// Returns the value of Non-secure EL1 Physical Timer Control
// Register (CNTP_CTL_EL0)
FUNC(__get_ns_el1_physical_timer_ctrl__)
    mrs   x0, CNTP_CTL_EL0
    ret
END(__get_ns_el1_physical_timer_ctrl__)


// void __set_ns_el1_physical_timer_ctrl__(u32 value)
// Sets the value of the Non-secure EL1 Physical Timer Control
// Register (CNTP_CTL_EL0)
// w0 - value - The value to be written into CNTP_CTL_EL0
FUNC(__set_ns_el1_physical_timer_ctrl__)
    msr   CNTP_CTL_EL0, x0
    isb
    ret
END(__set_ns_el1_physical_timer_ctrl__)


// u64 __get_s_el1_physical_comp_value__(void)
// Returns the value of Secure EL1 Physical Compare Value
// Register (CNTPS_CVAL_EL1)
FUNC(__get_s_el1_physical_comp_value__)
    mrs   x0, CNTPS_CVAL_EL1
    ret
END(__get_s_el1_physical_comp_value__)


// void __set_s_el1_physical_comp_value__(u64 value)
// Sets the value of the Secure EL1 Physical Compare Value
// Register (CNTPS_CVAL_EL1)
// x0 - value - The value to be written into CNTPS_CVAL_EL1

FUNC(__set_s_el1_physical_comp_value__)
    msr   CNTPS_CVAL_EL1, x0
    ret
END(__set_s_el1_physical_comp_value__)


// u32 __get_s_el1_physical_timer_value__(void)
// Returns the value of Secure EL1 Physical Timer Value
// Register (CNTPS_TVAL_EL1)
FUNC(__get_s_el1_physical_timer_value__)
    mrs   x0, CNTPS_TVAL_EL1
    ret
END(__get_s_el1_physical_timer_value__)


// void __set_s_el1_physical_timer_value__(u32 value)
// Sets the value of the Secure EL1 Physical Timer Value
// Register (CNTPS_TVAL_EL1)
// w0 - value - The value to be written into CNTPS_TVAL_EL1
FUNC(__set_s_el1_physical_timer_value__)
    msr   CNTPS_TVAL_EL1, x0
    ret
END(__set_s_el1_physical_timer_value__)


// u32 __get_s_el1_physical_timer_ctrl__(void)
// Returns the value of Secure EL1 Physical Timer Control
// Register (CNTPS_CTL_EL1)
FUNC(__get_s_el1_physical_timer_ctrl__)
    mrs   x0, CNTPS_CTL_EL1
    ret
END(__get_s_el1_physical_timer_ctrl__)


// void __set_s_el1_physical_timer_ctrl__(u32 value)
// Sets the value of the Secure EL1 Physical Timer Control
// Register (CNTPS_CTL_EL1)
// w0 - value - The value to be written into CNTPS_CTL_EL1
FUNC(__set_s_el1_physical_timer_ctrl__)
    msr   CNTPS_CTL_EL1, x0
    isb
    ret
END(__set_s_el1_physical_timer_ctrl__)


// void __config_secure_el1_timer_access__(unsigned int config)
// Sets the values of the SCR_EL3.ST bit (bit 11) based on
// the value in x0
// EL3 accessible only!
FUNC(__config_secure_el1_timer_access__)
    mrs   x1, SCR_EL3
    bfi   x1, x0, #11, #1
    msr   SCR_EL3, x1
    ret
END(__config_secure_el1_timer_access__)


// u64 getEL1VirtualCompValue(void)
// Returns the value of EL1 Virtual Compare Value Register
// (CNTV_CVAL_EL0)
FUNC(__get_el1_virtual_comp_value__)
    mrs   x0, CNTV_CVAL_EL0
    ret
END(__get_el1_virtual_comp_value__)


// void __set_el1_virtual_comp_value__(u64 value)
// Sets the value of the EL1 Virtual Compare Value Register
// (CNTV_CVAL_EL0)
// x0 - value - The value to be written into CNTV_CVAL_EL0
FUNC(__set_el1_virtual_comp_value__)
    msr   CNTV_CVAL_EL0, x0
    ret
END(__set_el1_virtual_comp_value__)


// u32 __get_el1_virtual_timer_value__(void)
// Returns the value of EL1 Virtual Timer Value Register
// (CNTV_TVAL_EL0)
FUNC(__get_el1_virtual_timer_value__)
    mrs   x0, CNTV_TVAL_EL0
    ret
END(__get_el1_virtual_timer_value__)


// void __set_el1_virtual_timer_value__(u32 value)
// Sets the value of the EL1 Virtual Timer Value Register
// (CNTV_TVAL_EL0)
// w0 - value - The value to be written into CNTV_TVAL_EL0
FUNC(__set_el1_virtual_timer_value__)
    msr   CNTV_TVAL_EL0, x0
    ret
END(__set_el1_virtual_timer_value__)


// u32 __get_el1_virtual_timer_ctrl__(void)
// Returns the value of EL1 Virtual Timer Control Register
// (CNTV_CTL_EL0)
FUNC(__get_el1_virtual_timer_ctrl__)
    mrs   x0, CNTV_CTL_EL0
    ret
END(__get_el1_virtual_timer_ctrl__)


// void __set_el1_virtual_timer_ctrl__(u32 value)
// Sets the value of the EL1 Virtual Timer Control Register
// (CNTV_CTL_EL0)
// w0 - value - The value to be written into CNTV_CTL_EL0
FUNC(__set_el1_virtual_timer_ctrl__)
    msr   CNTV_CTL_EL0, x0
    isb
    ret
END(__set_el1_virtual_timer_ctrl__)


// u64 __get_virtual_counter_offset__(void)
// Returns the value of the Counter Virtual Offset Register
// (CNTVOFF_EL2)
// EL2 and EL3 only
FUNC(__get_virtual_counter_offset__)
    mrs   x0, CNTVOFF_EL2
    ret
END(__get_virtual_counter_offset__)


// void __set_virtual_counter_offset__(u64 offset)
// Sets the value of the Counter Virtual Offset Register
// (CNTVOFF_EL2)
// x0 - offset - The value to be written into CNTVOFF_EL2
// EL2 and EL3 only
FUNC(__set_virtual_counter_offset__)
    msr   CNTVOFF_EL2, x0
    ret
END(__set_virtual_counter_offset__)


// u64 __get_el2_physical_comp_value__(void)
// Returns the value of EL2 Physical Compare Value Register
// (CNTHP_CVAL_EL2)
FUNC(__get_el2_physical_comp_value__)
    mrs   x0, CNTHP_CVAL_EL2
    ret
END(__get_el2_physical_comp_value__)


// void __set_el2_physical_comp_value__(u64 value)
// Sets the value of the EL2 Physical Compare Value Register
// (CNTHP_CVAL_EL2)
// x0 - value - The value to be written into CNTHP_CVAL_EL2
FUNC(__set_el2_physical_comp_value__)
    msr   CNTHP_CVAL_EL2, x0
    ret
END(__set_el2_physical_comp_value__)


// u32 __get_el2_physical_timer_value__(void)
// Returns the value of EL2 Physical Timer Value Register
// (CNTHP_TVAL_EL2)
FUNC(__get_el2_physical_timer_value__)
    mrs   x0, CNTHP_TVAL_EL2
    ret
END(__get_el2_physical_timer_value__)


// void __set_el2_physical_timer_value__(u32 value)
// Sets the value of the EL2 Physical Timer Value Register
// (CNTHP_TVAL_EL2)
// w0 - value - The value to be written into CNTHP_TVAL_EL2
FUNC(__set_el2_physical_timer_value__)
    msr   CNTHP_TVAL_EL2, x0
    ret
END(__set_el2_physical_timer_value__)


// u32 __get_el2_physical_timer_ctrl__(void)
// Returns the value of EL2 Physical Timer Control Register
// (CNTHP_CTL_EL2)
FUNC(__get_el2_physical_timer_ctrl__)
    mrs   x0, CNTHP_CTL_EL2
    ret
END(__get_el2_physical_timer_ctrl__)


// void __set_el2_physical_timer_ctrl__(u32 value)
// Sets the value of the EL2 Physical Timer Control Register
// (CNTHP_CTL_EL2)
// w0 - value - The value to be written into CNTHP_CTL_EL2
FUNC(__set_el2_physical_timer_ctrl__)
    msr   CNTHP_CTL_EL2, x0
    isb
    ret
END(__set_el2_physical_timer_ctrl__)
