// @Hustler's Project

#include <asm/gic.h>
#include <asm/smp.h>

#include <hal/rk3566.h>

#include <compiler.h>
#include <print.h>

// enable interrupts
void __init arch_gic_init(void) {

    // @Hustler
    arm_gic_init();

    arm_gic_irq_enable(DEBUG_IRQ);
}

// ####################################################

void do_sync(void) {

}

void do_irq(void) {

}

void do_fiq(void) {

}

void do_error(void) {

}

// ####################################################
