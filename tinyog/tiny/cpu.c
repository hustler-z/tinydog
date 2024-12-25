// @Hustler's Project

#include <cpu.h>
#include <compiler.h>

__stack u8 tiny_stack[STACK_SIZE];

struct cpu boot_cpu = {
    .id = 0,
    .flags = CPU_ACTIVE,
    .stack = (void *)tiny_stack,
};

int primary_cpu_init(void) {
    return 0;
}

int secondary_cpu_init(void) {
    return 0;
}
