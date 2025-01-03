// @Hustler's Project

#include <asm/barrier.h>
#include <asm/gic.h>

#include <compiler.h>
#include <string.h>
#include <print.h>

// ####################################################

struct gicv3_dist_if  *gic_dist;
struct gicv3_rdist_if *gic_rdist;

static u32 gic_addr_valid = 0;
static u32 gic_max_rd = 0;

// Sets the address of the Distributor and Redistributors
// dist   = virtual address of the Distributor
// rdist  = virtual address of the first RD_base register page
void set_gic_addr(void* dist, void* rdist)
{
    u32 index = 0;

    tiny_print("gicd: 0x%08X gicr: 0x%08X\n",
               (u64)dist, (u64)rdist);

    gic_dist = (struct gicv3_dist_if*)dist;
    gic_rdist = (struct gicv3_rdist_if*)rdist;
    gic_addr_valid = 1;

    // Now find the maximum RD id that I can use
    // This is used for range checking in later functions
    // Keep incrementing until GICR_TYPER.Last reports no
    // more RDs in block.
    while(!(gic_rdist[index].lpis.gicr_typer[0] & (1 << 4)))
        index++;

    gic_max_rd = index;

    return;
}

u32 get_ext_ppi(u32 rd)
{
    return (((gic_rdist[rd].lpis.gicr_typer[0] >> 27) & 0x1F) + 1) * 32;
}

// Returns the number of SPIs in the GICv3.0 range
u32 get_spi(void)
{
    return ((gic_dist->gicd_typer & 0x1F) + 1) * 32;
}

// Returns the number of SPIs in the Extended SPI range
u32 get_ext_spi(void)
{
    if (((gic_dist->gicd_typer >> 8) & 0x1) == 1)
        return (((gic_dist->gicd_typer >> 27) & 0x1F) + 1) * 32;
    else
        return 0;
}

// Returns 1 if the specified INTid is within the implemented Extended
// PPI range
static u32 is_valid_ext_ppi(u32 rd, u32 id)
{
    u32 max_ppi;

    max_ppi = ((gic_rdist[rd].lpis.gicr_typer[0] >> 27) & 0x1F);
    if (!max_ppi) {
#ifdef __DEBUG__
        tiny_debug("%s: extended ppi range not implemented,"
                   " intid %d is invalid.\n", __func__, id);
#endif
      return 0;
    } else if ((max_ppi == 1) && (id > 1087)) {
#ifdef __DEBUG__
        tiny_debug("%s: intid %d outside of implemented range.\n",
                __func__, id);
#endif
        // Extended PPI range implemented, but supplied intid
        // beyond range
        return 0;
    }

    return 1;
}

// Returns 1 if the specified INTid is within the implemented
// Extended SPI range
static u32 is_valid_ext_spi(u32 id)
{
    u32 max_spi;

    // Check Ext SPI implemented
    if (!((gic_dist->gicd_typer >> 8) & 0x1)) {
#ifdef __DEBUG__
        tiny_debug("%s: extended spi range not implemented,"
                " intid %d is invalid.\n", __func__, id);
#endif
        return 0;  // GICD_TYPER.ESPI==0: Extended SPI range not present
    } else {
        // Get field which reports the number ESPIs in blocks of 32, minus 1
        max_spi = ((gic_dist->gicd_typer >> 27) & 0x1F);
        // Convert into number of ESPIs
        max_spi = (max_spi + 1) * 32;
        // Range starts at 4096
        max_spi = max_spi + 4096;

        if (!(id < max_spi)) {
#ifdef __DEBUG__
            tiny_debug("%s: intid %d is out of implemented range.\n",
                    __func__, id);
#endif
            return 0;
        }
    }

    return 1;
}

// Enables and configures of the Distributor Interface
u32 enable_gic(void)
{
    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 1;

    // First set the ARE bits
    gic_dist->gicd_ctlr = (1 << 5) | (1 << 4);

    // The split here is because the register layout is
    // different once ARE==1

    // Now set the rest of the options
    gic_dist->gicd_ctlr = 7 | (1 << 5) | (1 << 4);

    tiny_print("gic enabled\n");

    return 0;
}

u32 get_redist_id(u32 affinity)
{
    u32 index = 0;

    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 0xFFFFFFFF;

    do {
        if (gic_rdist[index].lpis.gicr_typer[1] == affinity)
            return index;

        index++;
    } while(index <= gic_max_rd);

    return 0xFFFFFFFF; // return -1 to signal not RD found
}

u32 wake_up_redist(u32 rd)
{
    u32 tmp;

    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 1;

    // Tell the Redistributor to wake-up by clearing ProcessorSleep bit
    tmp = gic_rdist[rd].lpis.gicr_waker;
    tmp = tmp & ~0x2;
    gic_rdist[rd].lpis.gicr_waker = tmp;

    // Poll ChildrenAsleep bit until Redistributor wakes
    do {
        tmp = gic_rdist[rd].lpis.gicr_waker;
    } while((tmp & 0x4));

    tiny_print("gic rd-%u woke up\n", rd);

    return 0;
}

u32 enable_irq(u32 id, u32 rd)
{
    u32 bank;

#ifdef __DEBUG__
    tiny_debug("%s: enabling intid %d on rd-%d\n", __func__, id, rd);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid) {
#ifdef __DEBUG__
        tiny_error("%s: gic pointers not intialized\n", __func__);
#endif
        return 1;
    }

    if (id < 31) {
        // Check rd in range
        if (rd > gic_max_rd) {
#ifdef __DEBUG__
            tiny_error("%s: invalid rd index.\n", __func__);
#endif
            return 1;
        }

        // SGI or PPI
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_isenabler[0] = id;
    } else if (id < 1020) {
        // SPI
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access.
        id   = id & 0x1F;    // ... and which bit within the register

        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_isenabler[bank] = id;
    } else if ((id > 1055) && (id < 1120)) {
        // Extended PPI

        // Check rd in range
        if (rd > gic_max_rd) {
#ifdef __DEBUG__
            tiny_error("%s: invalid rd index.\n", __func__);
#endif
            return 1;
        }

        // Check Ext PPI implemented
        if (!is_valid_ext_ppi(rd, id))
            return 1;

        id   = id - 1024;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access.
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_isenabler[bank] = id;
    } else if ((id > 4095) && (id < 5120)) {
        // Extended SPI

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
            return 1;

        id   = id - 4096;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access.
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_isenablere[bank] = id;
    } else {
#ifdef __DEBUG__
        tiny_error("%s: Invalid interrupt.\n", __func__);
#endif
        return 1;
    }

    return 0;
}

u32 disable_irq(u32 id, u32 rd)
{
    u32 bank;

#ifdef __DEBUG__
    tiny_debug("%s: disabling intid %d on rd-%d\n", __func__, id, rd);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid) {
#ifdef __DEBUG__
        tiny_error("%s: gic pointers not intialized\n", __func__);
#endif
        return 1;
    }

    if (id < 31) {
        // Check rd in range
        if (rd > gic_max_rd)
           return 1;

        // SGI or PPI
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_icenabler[0] = id;
    } else if (id < 1020) {
        // SPI
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1f;    // ... and which bit within the register

        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_icenabler[bank] = id;
    } else if ((id > 1055) && (id < 1120)) {
        // Extended PPI

        // Check rd in range
        if (rd > gic_max_rd) {
#ifdef __DEBUG__
            tiny_error("%s: extended ppi range not implemented.\n", __func__);
#endif
            return 1;
        }

        // Check Ext PPI implemented
        if (!is_valid_ext_ppi(rd, id))
           return 1;

        id   = id - 1024;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_icenabler[bank] = id;
    } else if ((id > 4095) && (id < 5120)) {
        // Extended SPI

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
           return 1;


        id   = id - 4096;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_icenablere[bank] = id;
    } else {
#ifdef __DEBUG__
        tiny_error("%s: invalid interrupt.\n", __func__);
#endif
        return 1;
    }

    return 0;
}

u32 set_irq_priority(u32 id, u32 rd, u8 priority)
{

#ifdef __DEBUG__
    tiny_debug("%s: setting priority of intid %d on rd-%d to 0x%x\n",
            __func__, id, rd, priority);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid) {
#ifdef __DEBUG__
        tiny_error("%s: gic pointers not intialized\n", __func__);
#endif
        return 1;
    }

    if (id < 31) {
        // Check rd in range
        if (rd > gic_max_rd)
            return 1;

        // SGI or PPI
        gic_rdist[rd].sgis.gicr_ipriorityr[id] = priority;
    } else if (id < 1020) {
        // SPI
        gic_dist->gicd_ipriorityr[id] = priority;
    } else if ((id > 1055) && (id < 1120)) {
        // Extended PPI

        // Check rd in range
        if (rd > gic_max_rd)
            return 1;

        // Check Ext PPI implemented
        if (!is_valid_ext_ppi(rd, id))
            return 1;

        id = id - 1024;
        gic_rdist[rd].sgis.gicr_ipriorityr[id] = priority;
    } else if ((id > 4095) && (id < 5120)) {
        // Extended SPI

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
            return 1;

        gic_dist->gicd_ipriorityre[(id - 4096)] = priority;
    } else {
#ifdef __DEBUG__
        tiny_debug("%s: invalid interrupt.\n", __func__);
#endif
        return 1;
    }

    return 0;
}

u32 set_irq_type(u32 id, u32 rd, u32 type)
{
    u32 bank, tmp, conf = 0;

#ifdef __DEBUG__
    tiny_debug("%s: setting intid %d on rd-%d as type 0x%x\n",
            __func__, id, rd, type);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 1;

    if (id < 31) {
        // SGI or PPI
        // Config of SGIs is fixed
        // It is IMP DEF whether ICFG for PPIs is write-able,
        // on Arm implementations it is fixed
        return 1;
    } else if (id < 1020) {
        // SPI
        type = type & 0x3;            // Mask out unused bits

        bank = id / 16;               // There are 16 ids per register, need to work
                                      // out which register to access
        id   = id & 0xF;              // ... and which field within the register
        id   = id * 2;                // Convert from which field to a bit offset
                                      // (2-bits per field)

        conf = conf << id;            // Move configuration value into correct
                                      // bit position

        tmp = gic_dist->gicd_icfgr[bank];     // Read current value
        tmp = tmp & ~(0x3 << id);             // Clear the bits for the specified field
        tmp = tmp | conf;                     // OR in new configuration
        gic_dist->gicd_icfgr[bank] = tmp;     // Write updated value back
    } else if ((id > 4095) && (id < 5120)) {
        // Extended SPI

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
            return 1;

        type = type & 0x3;            // Mask out unused bits

        id   = id - 4096;
        bank = id / 16;               // There are 16 ids per register, need to work
                                      // out which register to access
        id   = id & 0xF;              // ... and which field within the register
        id   = id * 2;                // Convert from which field to a bit offset
                                      // (2-bits per field)

        conf = conf << id;            // Move configuration value into correct bit
                                      // position

        tmp = gic_dist->gicd_icfgre[bank];    // Read current value
        tmp = tmp & ~(0x3 << id);             // Clear the bits for the specified field
        tmp = tmp | conf;                     // OR in new configuration
        gic_dist->gicd_icfgre[bank] = tmp;    // Write updated value back
    } else
        return 1;

    return 0;
}

u32 set_irq_group(u32 id, u32 rd, u32 security)
{
    u32 bank, group, mod;

#ifdef __DEBUG__
    tiny_debug("%s: setting intid %d on rd-%d as groups 0x%x\n",
            __func__, id, rd, security);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 1;

    if (id < 31) {
        // Check rd in range
        if (rd > gic_max_rd)
            return 1;

        // SGI or PPI
        id   = id & 0x1f;    // Find which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        // Read current values
        group = gic_rdist[rd].sgis.gicr_igroupr[0];
        mod   = gic_rdist[rd].sgis.gicr_igrpmodr[0];

        // Update required bits
        switch (security) {
        case GICV3_GROUP0:
            group = (group & ~id);
            mod   = (mod & ~id);
            break;
        case GICV3_GROUP1_SECURE:
            group = (group & ~id);
            mod   = (mod | id);
            break;
        case GICV3_GROUP1_NON_SECURE:
            group = (group | id);
            mod   = (mod & ~id);
            break;
        default:
            return 1;
        }

        // Write modified version back
        gic_rdist[rd].sgis.gicr_igroupr[0] = group;
        gic_rdist[rd].sgis.gicr_igrpmodr[0] = mod;

    } else if (id < 1020) {
        // SPI
        bank = id / 32;      // There are 32 ids per register, need to work
                             // out which register to access
        id   = id & 0x1f;    // ... and which bit within the register

        id   = 1 << id;      // Move a '1' into the correct bit position

        group = gic_dist->gicd_igroupr[bank];
        mod   = gic_dist->gicd_grpmodr[bank];

        switch (security) {
        case GICV3_GROUP0:
            group = (group & ~id);
            mod   = (mod & ~id);
            break;
        case GICV3_GROUP1_SECURE:
            group = (group & ~id);
            mod   = (mod | id);
            break;
        case GICV3_GROUP1_NON_SECURE:
            group = (group | id);
            mod   = (mod & ~id);
            break;
        default:
            return 1;
        }

        gic_dist->gicd_igroupr[bank] = group;
        gic_dist->gicd_grpmodr[bank] = mod;
    } else if ((id > 1055) && (id < 1120)) {
        // Extended PPI

        // Check rd in range
        if (rd > gic_max_rd)
            return 1;

        // Check Ext PPI implemented
        if (!is_valid_ext_ppi(rd, id))
            return 1;

        id   = id - 1024;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        // Read current values
        group = gic_rdist[rd].sgis.gicr_igroupr[bank];
        mod   = gic_rdist[rd].sgis.gicr_igrpmodr[bank];

        // Update required bits
        switch (security) {
        case GICV3_GROUP0:
            group = (group & ~id);
            mod   = (mod & ~id);
            break;
        case GICV3_GROUP1_SECURE:
            group = (group & ~id);
            mod   = (mod | id);
            break;
        case GICV3_GROUP1_NON_SECURE:
            group = (group | id);
            mod   = (mod & ~id);
            break;
        default:
            return 1;
        }

        // Write modified version back
        gic_rdist[rd].sgis.gicr_igroupr[bank] = group;
        gic_rdist[rd].sgis.gicr_igrpmodr[bank] = mod;
    } else if ((id > 4095) && (id < 5120)) {
        // Extended SPI

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
            return 1;

        id   = id - 4096;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        // Read current values
        group = gic_dist->gicd_igroupre[bank];
        mod   = gic_dist->gicd_igrpmodre[bank];

        // Update required bits
        switch (security) {
        case GICV3_GROUP0:
            group = (group & ~id);
            mod   = (mod & ~id);
            break;
        case GICV3_GROUP1_SECURE:
            group = (group & ~id);
            mod   = (mod | id);
            break;
        case GICV3_GROUP1_NON_SECURE:
            group = (group | id);
            mod   = (mod & ~id);
            break;
        default:
            return 1;
        }

        // Write modified version back
        gic_dist->gicd_igroupre[bank] = group;
        gic_dist->gicd_igrpmodre[bank] = mod;
    } else
        return 1;

    return 0;
}

#define GICV3_ROUTE_AFF3_SHIFT           (8)

// Sets the target CPUs of the specified id
// For 'target' use one of the above defines
// id       = INTid of interrupt (id must be less than 1020)
// mode     = Routing mode
// aff<n>   = Affinity co-ordinate of target
u32 set_irq_route(u32 id, u32 mode, u32 affinity)
{
    u64 tmp;

#ifdef __DEBUG__
    tiny_debug("%s: routing intid %d to mode=0x%x and affinity=0x%08x\n",
            __func__, id, mode, affinity);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 0xFFFFFFFF;

    // Check for SPI ranges
    if (!((id > 31) && (id < 1020))) {
        // Not a GICv3.0 SPI

        if (!((id > 4095) && (id < 5120))) {
            // Not a GICv3.1 SPI either
#ifdef __DEBUG__
            tiny_debug("%s: cannot set routing information for non-spi\n",
                    __func__);
#endif
            return 1;
        }

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
            return 1;
    }

    // Combine routing in
    tmp = (u64)(affinity & 0x00FFFFFF) |
    (((u64)affinity & 0xFF000000) << GICV3_ROUTE_AFF3_SHIFT) |
      (u64)mode;

    if ((id > 31) && (id < 1020))
        gic_dist->gicd_router[id] = tmp;
    else
        gic_dist->gicd_routere[(id - 4096)] = tmp;

    return 0;
}

u32 set_irq_pending(u32 id, u32 rd)
{
    u32 bank;

#ifdef __DEBUG__
    tiny_debug("%s: setting intid %d on rd-%d as pending\n",
            __func__, id, rd);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 0xFFFFFFFF;

    if (id < 31) {
        // Check rd in range
        if (rd > gic_max_rd)
        return 1;

        id   = id & 0x1F;    // Find which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_ispendr[0] = id;
    } else if (id < 1020) {
        // SPI
        bank = id / 32;      // There are 32 ids per register, need to work
                             // out which register to access
        id   = id & 0x1F;    // ... and which bit within the register

        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_ispendr[bank] = id;
    } else if ((id > 1055) && (id < 1120)) {
        // Extended PPI

        // Check rd in range
        if (rd > gic_max_rd)
            return 1;

        // Check Ext PPI implemented
        if (!is_valid_ext_ppi(rd, id))
            return 1;

        id   = id - 1024;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_ispendr[bank] = id;
    } else if ((id > 4095) && (id < 5120)) {
        // Extended SPI

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
            return 1;

        id   = id - 4096;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_ispendre[bank] = id;
    } else
        return 1;

    return 0;
}

u32 clear_irq_pending(u32 id, u32 rd)
{
    u32 bank;

#ifdef __DEBUG__
    tiny_debug("%s: clearing pending state of intid %d on rd-%d\n",
            __func__, id, rd);
#endif

    // Check that GIC pointers are valid
    if (!gic_addr_valid)
        return 0xFFFFFFFF;

    if (id < 31) {
        // Check rd in range
        if (rd > gic_max_rd)
            return 1;

        id   = id & 0x1f;    // Find which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_icpendr[0] = id;
    } else if (id < 1020) {
        // SPI
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register

        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_icpendr[bank] = id;
    } else if ((id > 1055) && (id < 1120)) {
        // Extended PPI

        // Check rd in range
        if (rd > gic_max_rd)
            return 1;

        // Check Ext PPI implemented
        if (!is_valid_ext_ppi(rd, id))
            return 1;

        id   = id - 1024;
        bank = id / 32;      // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_rdist[rd].sgis.gicr_icpendr[bank] = id;
    } else if ((id > 4095) && (id < 5120)) {
        // Extended SPI

        // Check Ext SPI implemented
        if (!is_valid_ext_spi(id))
            return 1;

        id   = id - 4096;
        bank = id/32;        // There are 32 ids per register, need to
                             // work out which register to access
        id   = id & 0x1F;    // ... and which bit within the register
        id   = 1 << id;      // Move a '1' into the correct bit position

        gic_dist->gicd_icpendre[bank] = id;
    } else
        return 1;

    return 0;
}

// ####################################################

struct gicv3_its_ctlr_if *gic_its;
struct gicv3_its_int_if  *gic_its_ints;

// selects an ITS
// its_base = virtual address of ITS_base register page
void set_its_base_addr(void* its_base)
{
    gic_its      = (struct gicv3_its_ctlr_if *)its_base;
    gic_its_ints = (struct gicv3_its_int_if *)(its_base + 0x010000);

    return;
}

u32 set_lpi_config_table_addr(u32 rd, u64 addr, u64 attributes, u32 idbits)
{
    u64 tmp = 0;

#ifdef __DEBUG__
    tiny_debug("%s: installing lpi configuration table on rd-%d\n",
            __func__, rd);
    tiny_debug("%s: tabble base 0x%lx, with %d id bits\n",
            __func__, addr, idbits);
#endif

    // The function takes the number of id bits, the register expects (id bits - 1)
    // If the number of idbits is less than 14, then there are no LPIs
    // Also the maximum size of the Eventid is 32 bit
    if ((idbits < 14) || (idbits > 24)) {
#ifdef __DEBUG__
        tiny_debug("%s: invalid number of id bits\n", __func__);
#endif
        return 1;
    }

    tmp = (1 << idbits) - 8192;
    idbits = idbits - 1;

    // Zero table
    // This is ensure that all interrupts have a known (safe) initial config
    memset((void*)addr, 0, tmp);
    dsb(sy);

    // Write GICR_PROPBASER
    addr =         addr       & 0x0000FFFFFFFFF000;    // PA is bits 47:12
    tmp  = addr | (attributes & 0x0700000000000F80);   // Attributes are in bits 58:56 and 11:7
    tmp  = tmp  | (idbits     & 0x000000000000001F);   // idBits is bits 4:0
    gic_rdist[rd].lpis.gicr_propbaser = tmp;

    return 0;
}

u32 set_lpi_pending_table_addr(u32 rd, u64 addr, u64 attributes, u32 idbits)
{
    u64 tmp = 0;

#ifdef __DEBUG__
    tiny_debug("%s: installing lpi pending table on rd-%d\n",
            __func__, rd);
    tiny_debug("%s: tabble base 0x%lx, with %d id bits\n",
            __func__, addr, idbits);
#endif

    // The function takes the number of id bits, the register expects (id bits - 1)
    // If the number of idbits is less than 14, then there are no LPIs
    if ((idbits < 14) || (idbits > 24)) {
#ifdef __DEBUG__
        tiny_debug("%s: invalid number of id bits\n", __func__);
#endif
        return 1;
    }

    // Zero table
    // This is ensure that all interrupts have a known (inactive) initial state
    // TBD: Fix memset to only clear required bytes!
    tmp = 1 << idbits;
    memset((void*)addr, 0, tmp);
    dsb(sy);

    // Write GICR_PENDBASER
    addr =         addr       & 0x0000FFFFFFFF0000;    // PA is bits 47:16
    tmp  = addr | (attributes & 0x0700000000000F80);   // Attributes are in bits
                                                       // 58:56 and 11:7
    gic_rdist[rd].lpis.gicr_pendbaser = tmp;

    return 0;
}

// Enables LPIs for the currently selected Redistributor
void enable_lpis(u32 rd)
{
#ifdef __DEBUG__
    tiny_debug("%s: enabling physical lpis on rd-%d\n",
            __func__, rd);
#endif

    gic_rdist[rd].lpis.gicr_ctlr = (gic_rdist[rd].lpis.gicr_ctlr | 0x1);
    dsb(sy);

    return;
}

u32 get_rd_proc_nr(u32 rd)
{
    return ((gic_rdist[rd].lpis.gicr_typer[0] >> 8) & 0xFFFF);
}

u32 get_max_lpi(u32 rd)
{
    u32 max_lpi;

    // First check whether specified RD supports LPIs
    if (!(gic_rdist[rd].lpis.gicr_typer[0] & 0x1))
        return 0;

    // Now find the maximum LPI
    max_lpi = ((gic_dist->gicd_typer >> 19) & 0x1F) + 1; // the number of bits of INTid
    max_lpi = (1 << max_lpi) - 1;                        // maximum LPI

    return max_lpi;
}

// Configures specified LPI
// id       = id of LPI to be configured (must be 8192 or greater, no upper limit check)
// enable   = whether id is enabled (1=enabled, 0=disabled)
// priority = priority for id
u32 configure_lpi(u32 rd, u32 id, u32 enable, u32 priority)
{
    u8* config;
    u32 max_lpi;

#ifdef __DEBUG__
    tiny_debug("%s: configuring intid %d, with priority 0x%x and enable 0x%x, on rd-%d\n",
            __func__, id, priority, enable, rd);
#endif

    // Check static limits
    max_lpi = ((gic_dist->gicd_typer >> 19) & 0x1F) + 1; // the number of bits of INTid
    max_lpi = (1 << max_lpi) - 1;                        // maximum LPI

    if ((id < 8192) || (id > max_lpi)) {
        // INITD not within static LPI range
#ifdef __DEBUG__
        tiny_error("%s: intid %d is beyond supported maximum intid %d\n",
                __func__, id, max_lpi);
#endif
        return 1;
    }

    // Check LPI fits within allocated tables
    max_lpi = (gic_rdist[rd].lpis.gicr_propbaser & 0x1F) + 1; // the number of bits of intid for
                                                              // which there if memory allocated
    max_lpi = (1 << max_lpi) - 1;                             // maximum LPI

    if (id > max_lpi) {
    // LPI not within range supported by allocated memory
#ifdef __DEBUG__
        tiny_debug("%s: intid %d is beyond configured maximum intid %d\n",
                __func__, id, max_lpi);
#endif
        return 1;
    }

    // Set up a pointer to the configuration table
    config = (u8*)(gic_rdist[rd].lpis.gicr_propbaser & 0x0000FFFFFFFFF000);

    // Mask off unused bits of the priority and enable
    enable = enable & 0x1;
    priority = priority & 0x7C;

    // Combine priority and enable, write result into table
    // Note: bit 1 is RES1
    config[(id - 8192)] = (0x2 | enable | priority);
    dsb(sy);

    // Perform invalidation
    // How invalidation is performed depends on whether an ITS is present.
    // Where there is no ITS it is performed using registers in the
    // Redistributors.  Where an ITS is present, an INV cmd is needed.
    // Can't include this here, as it requires the Eventid and Deviceid.
#ifdef DIRECT
    gic_rdist[rd].lpis.gicr_invlpir = id;

    while (gic_rdist[rd].lpis.gicr_syncr)
        ;
#endif

    return 0;
}

// Initializes the currently selected ITS's cmd queue
// addr       = base address of allocated memory, must be 64K aligned
// attributes = cacheabilit, shareabilty and valid bit
// num_pages  = number of 4K pages (must be 1 or greater)
//
// NOTE: The GITS_CBASER encodes the number of pages as (number - 1).
// This functions expects an unmodified value (i.e. pass 1, if 1 page
// allocated).
u32 init_its_cmd_queue(u64 addr, u64 attributes, u32 num_pages)
{
    u64 tmp;

    addr       = addr       & (u64)0x0000FFFFFFFFF000;
    attributes = attributes & (u64)0xF800000000000C00;
    num_pages  = num_pages  & 0x00000000000000FF;

#ifdef __DEBUG__
    tiny_debug("%s: setting up command queue at 0x%lx, with %d pages\n",
            __func__, addr, num_pages);
#endif

    // Check number of pages is not 0
    if (!num_pages) {
#ifdef __DEBUG__
        tiny_error("%s: page count for command queue cannot be 0\n",
                __func__);
#endif
        return 1;
    }

    // check number of pages within permitted max
    if (num_pages > 256) {
#ifdef __DEBUG__
        tiny_debug("%s: page count for command queue cannot be 256\n",
                __func__);
#endif
        return 1;
    }

    // work out queue size in bytes, then zero memory
    // This code assumes that VA=PA for allocated memory
    tmp = num_pages * 4096;
    memset((void*)addr, 0, tmp);

    // Combine address, attributes and size
    tmp = addr | attributes | (u64)(num_pages - 1);

    gic_its->gits_cbaser  = tmp;
    // This register contains the offset from the start, hence
    // setting to 0
    gic_its->gits_cwriter = 0;

    dsb(sy);

    return 0;
}

// Returns the type and entry size of GITS_BASER[index] of the
// currently selected ITS
// index = which GITS_BASERn register to access, must be in the
// range 0 to 7
// type  = pointer to a u32, function write the vale of the
// GITS_BASERn.Type to this pointer
// entry_size  = pointer to a u32, function write the vale of
// the GITS_BASERn.Entry_Size to this pointer
u64 get_its_table_type(u32 index, u32* type, u32* entry_size)
{
    // Range check (there are only 8 (0-7) registers)
    if (index > 7)
        return 0;  // error

    *type       = (u32)(0x7  & (gic_its->gits_baser[index] >> 56));
    *entry_size = (u32)(0x1F & (gic_its->gits_baser[index] >> 48));

    return 1;
}

// Sets GITS_BASER[entry]
// index = which GITS_BASERn register to access, must be in the range 0 to 7
// addr  = phsyical address of allocated memory.  Must be at least 4K aligned.
// attributes = Cacheability, shareabilty, validity and indirect settings
// page_size  = Size of allocated pages (4K=0x0, 16K=0x100, 64K=0x200)
// num_pages  = The number of allocated pages.  Must be greater than 0.
//
// NOTE: The registers encodes as (number - 1), this function expecst then
// actual number
u32 set_its_table_addr(u32 index, u64 addr, u64 attributes,
                       u32 page_size, u32 num_pages)
{
    u64 tmp;

#ifdef __DEBUG__
    tiny_debug("%s: setting up its table %d at 0x%lx, with %d pages\n",
            __func__, index, addr, num_pages);
#endif

    // Range check:
    // - there are only 8 (0-7) registers
    // - code doesn't allow for 0 pages (minimum that can be
    //   encoded into the register is 1)
    if (index > 7) {
#ifdef __DEBUG__
        tiny_error("%s: invalid index vales\n", __func__);
#endif
        return 1;
    }

    if (!num_pages) {
#ifdef __DEBUG__
        tiny_error("%s: invalid number of pages\n", __func__);
#endif
        return 1;
    }

    // Combine fields to form entery
    tmp = (num_pages - 1) & 0xFF;
    tmp = tmp | (page_size  & 0x300);
    tmp = tmp | (addr       & (u64)0x0000FFFFFFFFF000);
    tmp = tmp | (attributes & (u64)0xF800000000000C00);

    gic_its->gits_baser[index] = tmp;


    // Zero memory
    // work out queue size in bytes, then zero memory
    // This code assumes that VA=PA for allocated memory
    if (page_size == GICV3_ITS_TABLE_PAGE_SIZE_4K)
        tmp = 0x1000 * num_pages;
    else if (page_size == GICV3_ITS_TABLE_PAGE_SIZE_16K)
        tmp = 0x4000 * num_pages;
    else if (page_size == GICV3_ITS_TABLE_PAGE_SIZE_64K)
        tmp = 0x10000 * num_pages;

    memset((void*)addr, 0, tmp);

    return 0;
}

// Returns the value of GITS_TYPER.PTA bit (shifted down to LSB)
u32 get_its_pta(void)
{
    return ((gic_its->gits_typer >> 19) & 1);
}

// Sets the GITS_CTLR.Enabled bit
void enable_its(void)
{
    gic_its->gits_ctlr = 1;

    return;
}

// Clears the GITS_CTLR.Enabled bit
void disable_its(void)
{
    gic_its->gits_ctlr = 0;

    // Poll for the disabling to take affect
    while(!(gic_its->gits_ctlr & (1 << 31)))
        ;

    return;
}

#define COMMAND_SIZE  (32)

// Adds a cmd to the currently selected ITS's queue.
// cmd = pointer to 32 bytes of memory containing a ITS cmd
//
// NOTE: This function is intended for internal use only.
void its_add_cmd(u8* cmd)
{
    u32 i, queue_size;
    u64 new_cwriter, queue_base, queue_offset, queue_read;
    u8* entry;

    // GITS_CBASER.Size returns the number of 4K pages, minus -1
    queue_size = ((gic_its->gits_cbaser & 0xFF) + 1) * 0x1000;
    queue_base = (gic_its->gits_cbaser & (u64)0x0000FFFFFFFFF000);
    // GITS_CWRITER contains the offset
    queue_offset = gic_its->gits_cwriter;
    queue_read   = gic_its->gits_creadr;

    // Check that the queue is not full
    // The queue is full when GITS_CWRITER points at
    // the entry before GITS_CREADR. To make the check
    // simpler I've moved the GITS_CREADR down one entry.
    if (!queue_read)
        queue_read = queue_size - COMMAND_SIZE;
    else
        queue_read = queue_read - COMMAND_SIZE;

    // Wait until queue not full
    // In practice it should very rarely be full
    while (queue_offset == queue_read)
        ;


    // Get the address of the next (base + write offset)
    entry = (u8*)(queue_base + queue_offset);

    // Copy cmd into queue
    for(i = 0; i < 32; i++)
        entry[i] = cmd[i];

#ifdef __DEBUG__
    tiny_debug("%s: Wrote cmd with: \n", __func__);
    tiny_debug("DW0: %02x %02x %02x %02x %02x %02x %02x %02x\n",
            cmd[7],  cmd[6],  cmd[5],  cmd[4],
            cmd[3],  cmd[2],  cmd[1],  cmd[0]);
    tiny_debug("DW1: %02x %02x %02x %02x %02x %02x %02x %02x\n",
            cmd[15], cmd[14], cmd[13], cmd[12],
            cmd[11], cmd[10], cmd[9],  cmd[8]);
    tiny_debug("DW2: %02x %02x %02x %02x %02x %02x %02x %02x\n",
            cmd[23], cmd[22], cmd[21], cmd[20],
            cmd[19], cmd[18], cmd[17], cmd[16]);
    tiny_debug("DW0: %02x %02x %02x %02x %02x %02x %02x %02x\n",
            cmd[31], cmd[30], cmd[29], cmd[28],
            cmd[27], cmd[26], cmd[25], cmd[24]);
#endif

    dsb(sy);

    // Move on queue
    new_cwriter = queue_offset + COMMAND_SIZE;

    // Check for roll-over
    if (new_cwriter == queue_size)
        new_cwriter = 0;

    // Update GITS_CWRITER, which also tells the ITS there is a new cmd
    gic_its->gits_cwriter = new_cwriter;

    // Poll for read pointer to move on (consuming cmd)
    //
    // This driver is cautious, and waits for cmds to be consumed
    // before returning.  It could have been less cautious, and
    // required software to issue an INT to detect completion.
    while(gic_its->gits_cwriter != gic_its->gits_creadr)
        ;

    return;
}

// Issues a MAPD cmd to the currently selected ITS
// device = The device id
// table  = Physical address (must be in Non-secure PA space)
// of the ITT table to be used
// size   = The number of bits of id used by the device
//
// NOTE: The underlying cmd records the size as (number - 1).
// This functions expects an unmodified value (i.e. pass 2, if 2 bits).
void its_mapd(u32 device_id, u64 table, u32 size)
{
    u8 cmd[32];

    // The cmd takes "actual_size - 1", the function takes
    // actual_size.  Therefore need to subtract one.
    if (size > 0)
        size--;
    else
        return; // error

    // Fill cmd with 0s
    memset((void *)cmd, 0, 32);

    table = table >> 8;

    // Construct cmd
    cmd[0]  = 0x8;
    cmd[1]  = 0x0;

    cmd[4]  = (u8)(0xFF & device_id);
    cmd[5]  = (u8)(0xFF & (device_id >> 8));
    cmd[6]  = (u8)(0xFF & (device_id >> 16));
    cmd[7]  = (u8)(0xFF & (device_id >> 24));

    cmd[8]  = (u8)(0x3F & size);

    cmd[17] = (u8)(0xFF & table);
    cmd[18] = (u8)(0xFF & (table >> 8));
    cmd[19] = (u8)(0xFF & (table >> 16));
    cmd[20] = (u8)(0xFF & (table >> 24));
    cmd[21] = (u8)(0xFF & (table >> 32));

    cmd[23] = 0x80;  // valid bit

    // Add cmd to queue
    its_add_cmd(cmd);

    return;
}

void its_mapc(u32 target, u32 collection)
{
    u8 cmd[32];

    // Fill cmd with 0s
    memset((void *)cmd, 0, 32);

    // Check whether "target" is PA or Processor Number
    if ((gic_its->gits_typer & (1 << 19)))
        target = target >> 16;

    // Construct cmd
    cmd[0]  = 0x9;
    cmd[1]  = 0x0;

    cmd[16] = (u8)(0xFF & collection);
    cmd[17] = (u8)(0xFF & (collection >> 8));

    cmd[18] = (u8)(0xFF & target);
    cmd[19] = (u8)(0xFF & (target >> 8));
    cmd[20] = (u8)(0xFF & (target >> 16));
    cmd[21] = (u8)(0xFF & (target >> 24));

    cmd[23] = 0x80;  // valid bit

    // Add cmd to queue
    its_add_cmd(cmd);

    return;
}

// Issues a MAPTI cmd to the currently selected ITS
// device   = the device id
// event_id = the event id (the "id" the peripheral will write)
// intid    = the GIC INTid (the "id" that will be reported when readin IAR)
// cid      = the collection id
void its_mapti(u32 device_id, u32 event_id, u32 intid, u32 cid)
{
    u8 cmd[32];

    // Fill cmd with 0s
    memset((void *)cmd, 0, 32);

    // Construct cmd
    cmd[0]   = 0xA;
    cmd[1]   = 0x0;

    cmd[4]   = (u8)(0xFF & device_id);
    cmd[5]   = (u8)(0xFF & (device_id >> 8));
    cmd[6]   = (u8)(0xFF & (device_id >> 16));
    cmd[7]   = (u8)(0xFF & (device_id >> 24));

    cmd[8]   = (u8)(0xFF & event_id);
    cmd[9]   = (u8)(0xFF & (event_id >> 8));
    cmd[10]  = (u8)(0xFF & (event_id >> 16));
    cmd[11]  = (u8)(0xFF & (event_id >> 24));

    cmd[12]  = (u8)(0xFF & intid);
    cmd[13]  = (u8)(0xFF & (intid >> 8));
    cmd[14]  = (u8)(0xFF & (intid >> 16));
    cmd[15]  = (u8)(0xFF & (intid >> 24));

    cmd[16]  = (u8)(0xFF & cid);
    cmd[17]  = (u8)(0xFF & (cid >> 8));

    // Add cmd to queue
    its_add_cmd(cmd);

    return;
}

// Issues an INVALL cmd to the currently selected ITS
// cid      = the collection id
void its_invall(u32 cid)
{
    u8 cmd[32];

    // Fill cmd with 0s
    memset((void *)cmd, 0, 32);

    // Construct cmd
    cmd[0]   = 0xD;
    cmd[1]   = 0x0;

    cmd[16]  = (u8)(0xFF & cid);
    cmd[17]  = (u8)(0xFF & (cid >> 8));

    // Add cmd to queue
    its_add_cmd(cmd);

    return;
}

void its_inv(u32 device_id, u32 event_id)
{
    u8 cmd[32];

    // Fill cmd with 0s
    memset((void *)cmd, 0, 32);

    // Construct cmd
    cmd[0]   = 0xC;
    cmd[1]   = 0x0;

    cmd[4]   = (u8)(0xFF & device_id);
    cmd[5]   = (u8)(0xFF & (device_id >> 8));
    cmd[6]   = (u8)(0xFF & (device_id >> 16));
    cmd[7]   = (u8)(0xFF & (device_id >> 24));

    cmd[8]   = (u8)(0xFF & event_id);
    cmd[9]   = (u8)(0xFF & (event_id >> 8));
    cmd[10]  = (u8)(0xFF & (event_id >> 16));
    cmd[11]  = (u8)(0xFF & (event_id >> 24));

    // Add cmd to queue
    its_add_cmd(cmd);

    return;
}

void its_sync(u64 target)
{
    u8 cmd[32];

    // Fill cmd with 0s
    memset((void *)cmd, 0, 32);

    target = target >> 16;

    // Construct cmd
    cmd[0]   = 0x5;
    cmd[1]   = 0x0;

    cmd[18] = (u8)(0xFF & target);
    cmd[19] = (u8)(0xFF & (target >> 8));
    cmd[20] = (u8)(0xFF & (target >> 16));
    cmd[21] = (u8)(0xFF & (target >> 24));

    // Add cmd to queue
    its_add_cmd(cmd);

    return;
}

// Issues a INT cmd to the currently selected ITS
// device = device id
// id     = event id  (the value the peripheral would have written)
void its_irq(u32 device_id, u32 event_id)
{
    u8 cmd[32];

    // Fill cmd with 0s
    memset((void *)cmd, 0, 32);

    // Construct cmd
    cmd[0]   = 0x3;
    cmd[1]   = 0x0;

    cmd[4]   = (u8)(0xFF & device_id);
    cmd[5]   = (u8)(0xFF & (device_id >> 8));
    cmd[6]   = (u8)(0xFF & (device_id >> 16));
    cmd[7]   = (u8)(0xFF & (device_id >> 24));

    cmd[8]   = (u8)(0xFF & event_id);
    cmd[9]   = (u8)(0xFF & (event_id >> 8));
    cmd[10]  = (u8)(0xFF & (event_id >> 16));
    cmd[11]  = (u8)(0xFF & (event_id >> 24));

    // Add cmd to queue
    its_add_cmd(cmd);

    return;
}

// ####################################################

// @Hustler
int __init arch_gic_init(void *dist_ba, void *rd_ba, u32 aff) {
    //
    u32 rd;

    set_gic_addr(dist_ba, rd_ba);

    enable_gic();

    rd = get_redist_id(aff);

    wake_up_redist(rd);

    __set_priority_mask__(0xFF);

    // TODO
    // __enable_group0_irq__();
    __enable_group1_irq__();

    // @Hustler
    tiny_print("gic init done\n");

    return rd;
}

// ####################################################

/* XXX: GICv3 Overview
 *
 * ####################################################
 *
 * GICv3 defines the following types of interrupt:
 *
 * (a) SPI (Shared Peripheral Interrupt)
 *     This is a global peripheral interrupt that can be routed
 *     to a specified PE, or to one of a group of PEs.
 *
 * (b) PPI (Private Peripheral Interrupt)
 *     This is peripheral interrupt that targets a single,
 *     specific PE. An example of a PPI is an interrupt from the
 *     Generic Timer of a PE.
 *
 * (c) SGI (Software Generated Interrupt)
 *     SGIs are typically used for inter-processor communication,
 *     and are generated by a write to an SGI register in the GIC.
 *
 * (d) LPI (Locality-specific Peripheral Interrupt)
 *     LPIs are edge-triggered message-based interrupts that
 *     can use an Interrupt Translation Service (ITS), if it
 *     is implemented, to route an interrupt to a specific
 *     redistributor and connected PE.
 *
 * ####################################################
 *
 * XXX: INTIDs
 *
 * Each interrupt source is identified by an ID number,
 * referred to as an INTID. The available INTIDs are grouped
 * into ranges, and each range is assigned to a particular
 * type of interrupt.
 *
 * ####################################################
 * INTID        Interrupt Type       Notes
 * ####################################################
 * 0    - 15    SGIs                 Banked per PE
 * 16   - 31    PPIs                 Banked per PE
 * 32   - 1019  SPIs
 * 1020 - 1023  Speical IRQ-num
 * 1034 - 8191  Reserved
 * >= 8192      LPIs
 * ####################################################
 *
 * +------------+            +----------------+  IRQ   +------+
 * |            | Interrupt  |                |------->|      |
 * | Peripheral |----------->| IRQ Controller |        |  PE  |
 * |            |   Signal   |                |------->|      |
 * +------------+            +----------------+  FIQ   +------+
 *        |                           |                    |
 * +----------------------------------------------------------+
 * |                      Interconnect                        |
 * +----------------------------------------------------------+
 *
 * A message-based interrupt is an interrupt that is set and
 * cleared by a write to a register in the interrupt controller.
 *
 * Using a message to forward the interrupt from a peripheral
 * to the interrupt controller removes the requirement for a
 * dedicated signal per interrupt source.
 *
 * XXX: A message-based interrupt is an interrupt that is
 *      asserted because of a memory write access to an
 *      assigned address. Physical interrupts can be converted
 *      to message-based interrupts. Message-based interrupts
 *      can support either level-sensitive or edge-triggered
 *      behavior, although LPIs are always edge-triggered.
 *
 * ####################################################
 * Interrupt Type        Registers
 * ####################################################
 * SPI                   GICD_SETSPI_NSR asserts an interrupt
 *                       GICD_CLRSPI_NSR deasserts an interrupt
 * LPI                   GITS_TRANSLATER
 * ####################################################
 *
 * XXX: IRQ State Machine (中断状态机)
 *
 * (Inactive)<------->(Pending)<------->(Active and Pending)
 *    A                   A                      A
 *    |                   |                      |
 *    |                   +----->(Active)<-------+
 *    |                              |
 *    +------------------------------+
 *
 * The GICv3 architecture supports the ARM TrustZone technology.
 * Each INTID must be assigned a group and security setting.
 *
 * XXX: GIC Programmer's Model
 *
 * Distributor (GICD_*)
 * The Distributor registers are memory-mapped, and contain
 * global settings that affect all PEs connected to the interrupt
 * controller. The Distributor provides a programming interface
 * for:
 * (1) Interrupt prioritization and distribution of SPIs.
 * (2) Enabling and disabling SPIs.
 * (3) Setting the priority level of each SPI.
 * (4) Routing information for each SPI.
 * (5) Setting each SPI to be level-sensitive or edge-triggered.
 * (6) Generating message-based SPIs.
 * (7) Controlling the active and pending state of SPIs.
 * (8) Controls to determine the programmers’ model that is used
 *     in each Security state (affinity routing or legacy).
 *
 * Redistributors (GICR_*)
 *
 * The part of the IRI that is connected to the CPU interface
 * of the PE.
 *
 * For each connected PE there is a Redistributor. The
 * Redistributors provides a programming interface for:
 * (1) Enabling and disabling SGIs and PPIs.
 * (2) Setting the priority level of SGIs and PPIs.
 * (3) Setting each PPI to be level-sensitive or edge-triggered.
 * (4) Assigning each SGI and PPI to an interrupt group.
 * (5) Controlling the state of SGIs and PPIs.
 * (6) Base address control for the data structures in memory
 * that support the associated interrupt properties and pending
 * state for LPIs.
 * (7) Power management support for the connected PE.
 *
 * CPU interfaces (ICC_*_ELn)
 * Each Redistributor is connected to a CPU interface. The CPU
 * interface provides a programming interface for:
 * (1) General control and configuration to enable interrupt
 *     handling.
 * (2) Acknowledging an interrupt.
 * (3) Performing a priority drop and deactivation of interrupts.
 * (4) Setting an interrupt priority mask for the PE.
 * (5) Defining the preemption policy for the PE.
 * (6) Determining the highest priority pending interrupt for
 *     the PE.
 *
 * In GICv3 the CPU Interface registers are accessed as System
 * registers (ICC_*_ELn).
 *
 * Software must enable the System register interface before
 * using these registers. This is controlled by the SRE bit in
 * the ICC_SRE_ELn registers, where “n” specifies the Exception
 * level (EL1-EL3).
 *
 * ####################################################
 *
 * XXX: Interrupt Routing Infrastructure (IRI)
 *
 *        +-----------------------+  +-----+
 * SPIs ->|      Distributor      |  | ITS |
 *        +-----------------------+  +-----+
 *           |                 |        | LPIs
 *        +-----+           +-----+     |
 * PPIs ->| RD0 |           | RDn |<----+
 *        +-----+           +-----+
 *           |                 | SGIs
 *          <I>               <I>
 *        +-----+           +-----+
 *        | PE0 |           | PEn |  <I> CPU Interface
 *        +-----+           +-----+
 *
 * XXX: Interrupt Translation Service (ITS)
 *
 * The CPU interface handles physical interrupts at all
 * implemented exception levels:
 *
 * (a) Interrupts that are translated into LPIs are optionally
 *     routed through the ITS to the Redistributor and the CPU
 *     interface.
 * (b) PPIs are routed directly from the source to the local
 *     Redistributor.
 * (c) SPIs are routed from the source through the Distributor
 *     to the target Redistributor and the associated CPU
 *     interface.
 * (d) SGIs are generated by software through the CPU interface
 *     and Redistributor. They are then routed through the
 *     Distributor to one or more target Redistributors and
 *     the associated CPU interfaces.
 *
 * ####################################################
 */
