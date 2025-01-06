// @Hustler's Project

#ifndef _ASM_IO_H_
#define _ASM_IO_H_

// ####################################################

#include <asm/barrier.h>
#include <compiler.h>
#include <type.h>

static __always_inline u8 readb(xmem_t addr)
{
	u8 val;

	asm volatile("ldrb %w0, [%1]" : "=r" (val) : "r" (addr));

	dmb(sy);
	return val;
}

static __always_inline void writeb(u8 data, xmem_t addr)
{
	dmb(sy);
	asm volatile("strb %w0, [%1]" : : "r" (data), "r" (addr));
}

static __always_inline u16 readw(xmem_t addr)
{
	u16 val;

	asm volatile("ldrh %w0, [%1]" : "=r" (val) : "r" (addr));

	dmb(sy);
	return val;
}

static __always_inline void writew(u16 data, xmem_t addr)
{
	dmb(sy);
	asm volatile("strh %w0, [%1]" : : "r" (data), "r" (addr));
}

static __always_inline u32 readl(xmem_t addr)
{
	u32 val;

	asm volatile("ldr %w0, [%1]" : "=r" (val) : "r" (addr));

	dmb(sy);
	return val;
}

static __always_inline void writel(u32 data, xmem_t addr)
{
	dmb(sy);
	asm volatile("str %w0, [%1]" : : "r" (data), "r" (addr));
}

static __always_inline u64 readq(xmem_t addr)
{
	u64 val;

	asm volatile("ldr %x0, [%1]" : "=r" (val) : "r" (addr));

	dmb(sy);
	return val;
}

static __always_inline void writeq(u64 data, xmem_t addr)
{
	dmb(sy);
	asm volatile("str %x0, [%1]" : : "r" (data), "r" (addr));
}

// ####################################################

#endif /* _ASM_IO_H_ */
