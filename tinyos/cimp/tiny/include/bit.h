// @Hustler's Project

#ifndef _TINY_BIT_H_
#define _TINY_BIT_H_

// ####################################################

#include <asm/bit.h>

#include <compiler.h>
#include <type.h>

static __always_inline void __set_bit(xmem_t addr, unsigned int bit)
{
	u32 temp = *(volatile u32 *)addr;

	*(volatile u32 *)addr = temp | (1 << bit);
}

static __always_inline void __clear_bit(xmem_t addr, unsigned int bit)
{
	u32 temp = *(volatile u32 *)addr;

	*(volatile u32 *)addr = temp & ~(1 << bit);
}

static __always_inline int __test_bit(xmem_t addr, unsigned int bit)
{
	u32 temp = *(volatile u32 *)addr;

	return temp & (1 << bit);
}

static __always_inline void __set_bits(xmem_t addr, unsigned int mask)
{
	u32 temp = *(volatile u32 *)addr;

	*(volatile u32 *)addr = temp | mask;
}

static __always_inline void __clear_bits(xmem_t addr, unsigned int mask)
{
	u32 temp = *(volatile u32 *)addr;

	*(volatile u32 *)addr = temp & ~mask;
}

static __always_inline
	void __bitfield_set_bit(xmem_t addr, unsigned int bit)
{
	/* Doing memory offsets in terms of 32-bit values to prevent
	 * alignment issues
	 */
	__set_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

static __always_inline
	void __bitfield_clear_bit(xmem_t addr, unsigned int bit)
{
	__clear_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

static __always_inline
	int __bitfield_test_bit(xmem_t addr, unsigned int bit)
{
	return __test_bit(addr + ((bit >> 5) << 2), bit & 0x1F);
}

static __always_inline
	int __test_and_set_bit(xmem_t addr, unsigned int bit)
{
	int ret;

	ret = __test_bit(addr, bit);
	__set_bit(addr, bit);

	return ret;
}

static __always_inline
	int __test_and_clear_bit(xmem_t addr, unsigned int bit)
{
	int ret;

	ret = __test_bit(addr, bit);
	__clear_bit(addr, bit);

	return ret;
}

static __always_inline
	int __bitfield_test_and_set_bit(xmem_t addr, unsigned int bit)
{
	int ret;

	ret = __bitfield_test_bit(addr, bit);
	__bitfield_set_bit(addr, bit);

	return ret;
}

static __always_inline
	int __bitfield_test_and_clear_bit(xmem_t addr, unsigned int bit)
{
	int ret;

	ret = __bitfield_test_bit(addr, bit);
	__bitfield_clear_bit(addr, bit);

	return ret;
}

// ####################################################

#endif /* _TINY_BIT_H_ */
