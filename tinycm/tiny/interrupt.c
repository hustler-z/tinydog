// @Hustler's Project

#include <asm/gic.h>
#include <asm/smp.h>

#include <compiler.h>
#include <rk3566.h>
#include <print.h>

#define GICD_BA            0xFD400000
#define GICR_BA            0xFD460000

void __init primary_gic_init(void) {
    int rd = 0;
    u32 affinity = 0;

    affinity = get_cpu_affinity();

    tiny_print("primary gic init on cpu-%u\n", affinity);

    rd = arch_gic_init((void *)GICD_BA, (void *)GICR_BA, affinity);

    set_irq_priority(DEBUG_IRQ, rd, 0);
    set_irq_group(DEBUG_IRQ, rd, GICV3_GROUP1_NON_SECURE);
    enable_irq(DEBUG_IRQ, rd);
}

// ####################################################

void do_sync(void) {

}

void do_irq(void) {
    u32 id;

    id = __read_iar_grp1__();

    tiny_print("irq: received int-%03u\n", id);

    switch (id) {
    case DEBUG_IRQ:
        tiny_print("irq: debug mode\n");
        break;
    default:
        tiny_error("irq: invalid int-id\n");
        return;
    }

    __write_eoi_grp1__(id);
}

void do_fiq(void) {

}

void do_error(void) {

}

// ####################################################
