// @Hustler's Project

#include <asm/entry.h>
#include <asm/smp.h>

#include <print.h>
#include <cpu.h>

// ####################################################

#define TRACE_INDENT "    "

void dump_trace(const struct context *context) {
    register_t *frame, next, addr, low, high;

    tiny_debug("call backtrace on cpu-%d\n",
            smp_processor_id());

    tiny_debug(TRACE_INDENT"[<%p>] %p (pc)\n",
            __void__(context->pc),
            __void__(context->pc));
    tiny_debug(TRACE_INDENT"[<%p>] %p (lr)\n",
            __void__(context->lr),
            __void__(context->lr));

    low = (register_t)((register_t *)context->sp);
    high = (low & ~(STACK_SIZE - 1)) +
        (STACK_SIZE - sizeof(struct cpu));

    next = context->fp;

    for (;;) {
        if ((next < low) || (next >= high))
            break;

        frame = (register_t *)next;
        next = frame[0];
        addr = frame[1];

        tiny_debug(TRACE_INDENT"[<%p>] %p\n",
                __void__(addr),
                __void__(addr));

        low = (register_t)&frame[1];
    }
}

// @arch/arm64/debug.S
extern void __store_context__(struct context *ctx);

void dump_stack(void) {
    struct context ctx;
    register_t *frame, next, addr, low, high;

    __store_context__(&ctx);

    __print__(" esr: %p\n", __void__(ctx.esr));
    __print__("cpsr: %p\n", __void__(ctx.cpsr));

    __print__("call backtrace on cpu-%d:\n",
            smp_processor_id());

    __print__(TRACE_INDENT"[<%p>] %p\n",
            __void__(ctx.lr),
            __void__(ctx.lr));

    low  = (register_t)((register_t *)ctx.sp);
    high = (low & ~(STACK_SIZE - 1)) +
        (STACK_SIZE - sizeof(struct cpu));

    next = ctx.fp;

    for (;;) {
        if ((next < low) || (next >= high))
            break;

        frame = (register_t *)next;
        next  = frame[0];
        addr  = frame[1];

        __print__(TRACE_INDENT"[<%p>] %p\n",
                __void__(addr),
                __void__(addr));

        low = (register_t)&frame[1];
    }
}

// ####################################################
