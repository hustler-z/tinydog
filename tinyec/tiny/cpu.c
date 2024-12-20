// @Hustler's Project

#include <cpu.h>

u8 boot_stack[STACK_SIZE];

struct cpu boot_cpu = {
    .id = 0,
    .flags = CPU_ACTIVE,
    .stack = (void *)boot_stack,
};

int primary_cpu_init(void) {
    return 0;
}

int secondary_cpu_init(void) {
    return 0;
}
