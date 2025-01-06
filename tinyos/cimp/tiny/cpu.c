// @Hustler's Project

#include <asm/interrupt.h>
#include <asm/entry.h>
#include <asm/smp.h>

#include <hal/rk3566.h>

#include <compiler.h>
#include <print.h>
#include <cpu.h>

// ####################################################

u64 __percpu_offset[NR_CPU];

struct xstack {
    u8 stack[STACK_SIZE];
};

static __stack struct xstack tiny_stack[NR_CPU]
       __aligned(STACK_SIZE);

// boot on cpu 0
struct cpu __primary_cpu__ = {
    .id = 0,
    .flags = CPU_ACTIVE,
    .stack = (void *)tiny_stack[0].stack,
};

struct platform {
    struct cpu cpu[NR_CPU];
};

// @Hustler
// static __stack struct notifier_block tiny_nb[NR_CPU];

// ####################################################

int __init primary_cpu_init(void) {
    int ret = 0;

    tiny_print("booting primary cpu\n");

    set_processor_id(0);

    ret = arch_primary_cpu_init();

    return ret;
}

int __init secondary_cpu_init(void) {
    int ret = 0;

    tiny_print("booting secondary cpu\n");

    ret = arch_secondary_cpu_init();

    return ret;
}

// ####################################################

void __init notifier_chain_register(
    struct notifier_head *nh, struct notifier_block *nb) {
    struct list_head *chain = &nh->head;
    struct notifier_block *__nb;

    while (chain->next != &nh->head) {
        __nb = list_entry(chain->next, struct notifier_block, chain);
        if (nb->priority > __nb->priority)
            break;

        chain = chain->next;
    }

    list_add(&nb->chain, chain);
}

void __init notifier_chain_unregister(
        struct notifier_head *nh, struct notifier_block *nb) {
    list_del(&nb->chain);
}

int __init notifier_call_chain(
    struct notifier_head *nh, unsigned long val, void *v,
    struct notifier_block **prev_nb) {

    // @Hustler
    int ret = NOTIFY_DONE;
    struct list_head *this_nb;
    struct notifier_block *nb = NULL;
    bool reverse = val & NOTIFY_REVERSE;

    this_nb = prev_nb && *prev_nb ? &(*prev_nb)->chain : &nh->head;

    do {
        this_nb = reverse ? this_nb->prev : this_nb->next;
        if (this_nb == &nh->head)
            break;

        nb = list_entry(this_nb, struct notifier_block, chain);
        ret = nb->notifier_call(nb, val, v);
    } while (!(ret & NOTIFY_STOP_MASK));

    if (prev_nb)
        *prev_nb = nb;

    return ret;
}

// ####################################################
