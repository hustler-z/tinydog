// @Hustler's Project

#ifndef _ASM_MEMORY_H_
#define _ASM_MEMORY_H_

#include <type.h>

struct mmu_blk {
    addr_t base_pa;
    addr_t base_va;
    size_t size;
    const u8 tag;
    u32 attrs;
};

struct mmu_cfg {
    u32 nr_blk;
    u64 ttbr0;
    const struct mmu_blk *mmu_blks;
};

#endif /* _ASM_MEMORY_H_ */
