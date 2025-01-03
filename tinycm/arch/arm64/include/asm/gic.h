// @Hustler's Project

#ifndef _ASM_GIC_H_
#define _ASM_GIC_H_

#include <type.h>

struct gicv3_dist_if
{
    __RW u32 gicd_ctlr;              // +0x0000 - RW - Distributor Control Register
    __RO u32 gicd_typer;             // +0x0004 - RO - Interrupt Controller Type Register
    __RO u32 gicd_iidr;              // +0x0008 - RO - Distributor Implementer Identification Register

    __RO u32 padding0;               // +0x000C - RESERVED

    __RW u32 gicd_statusr;           // +0x0010 - RW - Status register

    __RO u32 padding1[3];            // +0x0014 - RESERVED

    __RW u32 imp_def[8];             // +0x0020 - RW - Implementation defined registers

    __WO u32 gicd_setspi_nsr;        // +0x0040 - WO - Non-secure Set SPI Pending (Used when SPI is signalled using MSI)
    __RO u32 padding2;               // +0x0044 - RESERVED
    __WO u32 gicd_clrspi_nsr;        // +0x0048 - WO - Non-secure Clear SPI Pending (Used when SPI is signalled using MSI)
    __RO u32 padding3;               // +0x004C - RESERVED
    __WO u32 gicd_setspi_sr;         // +0x0050 - WO - Secure Set SPI Pending (Used when SPI is signalled using MSI)
    __RO u32 padding4;               // +0x0054 - RESERVED
    __WO u32 gicd_clrspi_sr;         // +0x0058 - WO - Secure Clear SPI Pending (Used when SPI is signalled using MSI)

    __RO u32 padding5[3];            // +0x005C - RESERVED

    __WO u32 gicd_seir;              // +0x0068 - WO - System Error Interrupt Register (Note: This was recently removed from the spec)

    __RO u32 padding6[5];            // +0x006C - RESERVED

    __RW u32 gicd_igroupr[32];       // +0x0080 - RW - Interrupt Group Registers (Note: In GICv3, need to look at IGROUPR and IGRPMODR)

    __RW u32 gicd_isenabler[32];     // +0x0100 - RW - Interrupt Set-Enable Registers
    __RW u32 gicd_icenabler[32];     // +0x0180 - RW - Interrupt Clear-Enable Registers
    __RW u32 gicd_ispendr[32];       // +0x0200 - RW - Interrupt Set-Pending Registers
    __RW u32 gicd_icpendr[32];       // +0x0280 - RW - Interrupt Clear-Pending Registers
    __RW u32 gicd_isactiver[32];     // +0x0300 - RW - Interrupt Set-Active Register
    __RW u32 gicd_icactiver[32];     // +0x0380 - RW - Interrupt Clear-Active Register

    __RW u8  gicd_ipriorityr[1024];  // +0x0400 - RW - Interrupt Priority Registers
    __RW u32 gicd_itargetsr[256];    // +0x0800 - RW - Interrupt Processor Targets Registers
    __RW u32 gicd_icfgr[64];         // +0x0C00 - RW - Interrupt Configuration Registers
    __RW u32 gicd_grpmodr[32];       // +0x0D00 - RW - Interrupt Group Modifier (Note: In GICv3, need to look at IGROUPR and IGRPMODR)
    __RO u32 padding7[32];           // +0x0D80 - RESERVED

    __RW u32 gicd_nsacr[64];         // +0x0E00 - RW - Non-secure Access Control Register
    __RW u32 gicd_sgir;              // +0x0F00 - WO - Software Generated Interrupt Register

    __RO u32 padding8[3];            // +0x0F04 - RESERVED

    __RW u32 gicd_cpendsgir[4];      // +0x0F10 - RW - Clear pending for SGIs
    __RW u32 gicd_spendsgir[4];      // +0x0F20 - RW - Set pending for SGIs

    __RO u32 padding9[52];           // +0x0F30 - RESERVED

    // GICv3.1 extended SPI range
    __RW u32 gicd_igroupre[128];     // +0x1000 - RW - Interrupt Group Registers (GICv3.1)
    __RW u32 gicd_isenablere[128];   // +0x1200 - RW - Interrupt Set-Enable Registers (GICv3.1)
    __RW u32 gicd_icenablere[128];   // +0x1400 - RW - Interrupt Clear-Enable Registers (GICv3.1)
    __RW u32 gicd_ispendre[128];     // +0x1600 - RW - Interrupt Set-Pending Registers (GICv3.1)
    __RW u32 gicd_icpendre[128];     // +0x1800 - RW - Interrupt Clear-Pending Registers (GICv3.1)
    __RW u32 gicd_isactivere[128];   // +0x1A00 - RW - Interrupt Set-Active Register (GICv3.1)
    __RW u32 gicd_icactivere[128];   // +0x1C00 - RW - Interrupt Clear-Active Register (GICv3.1)

    __RO u32 padding10[128];         // +0x1E00 - RESERVED

    __RW u8  gicd_ipriorityre[4096]; // +0x2000 - RW - Interrupt Priority Registers (GICv3.1)

    __RW u32 gicd_icfgre[256];       // +0x3000 - RW - Interrupt Configuration Registers (GICv3.1)
    __RW u32 gicd_igrpmodre[128];    // +0x3400 - RW - Interrupt Group Modifier (GICv3.1)
    __RW u32 gicd_nsacre[256];       // +0x3600 - RW - Non-secure Access Control Register (GICv3.1)

    __RO u32 padding11[2432];        // +0x3A00 - RESERVED

    // GICv3.0
    __RW u64 gicd_router[1024];      // +0x6000 - RW - Controls SPI routing when ARE=1

    // GICv3.1
    __RW u64 gicd_routere[1024];     // +0x8000 - RW - Controls SPI routing when ARE=1 (GICv3.1)
};

struct gicv3_rdist_lpis_if
{
    __RW u32 gicr_ctlr;             // +0x0000 - RW - Redistributor Control Register
    __RO u32 gicr_iidr;             // +0x0004 - RO - Redistributor Implementer Identification Register
    __RO u32 gicr_typer[2];         // +0x0008 - RO - Redistributor Type Register
    __RW u32 gicr_statusr;          // +0x0010 - RW - Redistributor Status register
    __RW u32 gicr_waker;            // +0x0014 - RW - Wake Request Registers
    __RO u32 gicr_mpamidr;          // +0x0018 - RO - Reports maximum PARTID and PMG (GICv3.1)
    __RW u32 gicr_partid;           // +0x001C - RW - Set PARTID and PMG used for Redistributor memory accesses (GICv3.1)
    __RO u32 padding1[8];           // +0x0020 - RESERVED
    __WO u64 gicr_setlpir;          // +0x0040 - WO - Set LPI pending (Note: IMP DEF if ITS present)
    __WO u64 gicr_clrlpir;          // +0x0048 - WO - Set LPI pending (Note: IMP DEF if ITS present)
    __RO u32 padding2[6];           // +0x0058 - RESERVED
    __WO u32 gicr_seir;             // +0x0068 - WO - (Note: This was removed from the spec)
    __RO u32 padding3;              // +0x006C - RESERVED
    __RW u64 gicr_propbaser;        // +0x0070 - RW - Sets location of the LPI configuration table
    __RW u64 gicr_pendbaser;        // +0x0078 - RW - Sets location of the LPI pending table
    __RO u32 padding4[8];           // +0x0080 - RESERVED
    __WO u64 gicr_invlpir;          // +0x00A0 - WO - Invalidates cached LPI config (Note: In GICv3.x: IMP DEF if ITS present)
    __RO u32 padding5[2];           // +0x00A8 - RESERVED
    __WO u64 gicr_invallr;          // +0x00B0 - WO - Invalidates cached LPI config (Note: In GICv3.x: IMP DEF if ITS present)
    __RO u32 padding6[2];           // +0x00B8 - RESERVED
    __WO u64 gicr_syncr;            // +0x00C0 - WO - Redistributor Sync
    __RO u32 padding7[2];           // +0x00C8 - RESERVED
    __RO u32 padding8[12];          // +0x00D0 - RESERVED
    __WO u64 gicr_movlpir;          // +0x0100 - WO - IMP DEF
    __RO u32 padding9[2];           // +0x0108 - RESERVED
    __WO u64 gicr_movallr;          // +0x0110 - WO - IMP DEF
    __RO u32 padding10[2];          // +0x0118 - RESERVED
};

struct gicv3_rdist_sgis_if
{
    __RO u32 padding1[32];          // +0x0000 - RESERVED
    __RW u32 gicr_igroupr[3];       // +0x0080 - RW - Interrupt Group Registers (Security Registers in GICv1)
    __RO u32 padding2[29];          // +0x008C - RESERVED
    __RW u32 gicr_isenabler[3];     // +0x0100 - RW - Interrupt Set-Enable Registers
    __RO u32 padding3[29];          // +0x010C - RESERVED
    __RW u32 gicr_icenabler[3];     // +0x0180 - RW - Interrupt Clear-Enable Registers
    __RO u32 padding4[29];          // +0x018C - RESERVED
    __RW u32 gicr_ispendr[3];       // +0x0200 - RW - Interrupt Set-Pending Registers
    __RO u32 padding5[29];          // +0x020C - RESERVED
    __RW u32 gicr_icpendr[3];       // +0x0280 - RW - Interrupt Clear-Pending Registers
    __RO u32 padding6[29];          // +0x028C - RESERVED
    __RW u32 gicr_isactiver[3];     // +0x0300 - RW - Interrupt Set-Active Register
    __RO u32 padding7[29];          // +0x030C - RESERVED
    __RW u32 gicr_icactiver[3];     // +0x0380 - RW - Interrupt Clear-Active Register
    __RO u32 padding8[29];          // +0x018C - RESERVED
    __RW u8  gicr_ipriorityr[96];   // +0x0400 - RW - Interrupt Priority Registers
    __RO u32 padding9[488];         // +0x0460 - RESERVED
    __RW u32 gicr_icfgr[6];         // +0x0C00 - RW - Interrupt Configuration Registers
    __RO u32 padding10[58];	        // +0x0C18 - RESERVED
    __RW u32 gicr_igrpmodr[3];      // +0x0D00 - RW - Interrupt Group Modifier Register
    __RO u32 padding11[61];	        // +0x0D0C - RESERVED
    __RW u32 gicr_nsacr;            // +0x0E00 - RW - Non-secure Access Control Register
};

struct gicv3_rdist_vlpis_if
{
    __RO u32 padding1[28];          // +0x0000 - RESERVED
    __RW u64 gicr_vpropbaser;       // +0x0070 - RW - Sets location of the LPI vPE Configuration table
    __RW u64 gicr_vpendbaser;       // +0x0078 - RW - Sets location of the LPI Pending table
};

struct gicv3_rdist_res_if
{
    __RO u32 padding1[32];          // +0x0000 - RESERVED
};

struct gicv3_rdist_if
{
    struct gicv3_rdist_lpis_if  lpis  __attribute__((aligned (0x10000)));
    struct gicv3_rdist_sgis_if  sgis  __attribute__((aligned (0x10000)));

#ifdef __GICv4__
    struct gicv3_rdist_vlpis_if vlpis __attribute__((aligned (0x10000)));
    struct gicv3_rdist_res_if   res   __attribute__((aligned (0x10000)));
#endif
};

// +0 from ITS_BASE
struct gicv3_its_ctlr_if
{
    __RW u32 gits_ctlr;             // +0x0000 - RW - ITS Control Register
    __RO u32 gits_iidr;             // +0x0004 - RO - Implementer Identification Register
    __RO u64 gits_typer;            // +0x0008 - RO - ITS Type register
    __RO u32 gits_mpamidr;          // +0x0010 - RO - Reports maxmimum PARTID and PMG (GICv3.1)
    __RW u32 gits_partidr;          // +0x0004 - RW - Sets the PARTID and PMG used for ITS memory accesses (GICv3.1)
    __RO u32 gits_mpidr;            // +0x0018 - RO - ITS affinity, used for shared vPE table
    __RO u32 padding5;              // +0x001C - RESERVED
    __RW u32 gits_impdef[8];        // +0x0020 - RW - IMP DEF registers
    __RO u32 padding2[16];          // +0x0040 - RESERVED
    __RW u64 gits_cbaser;           // +0x0080 - RW - Sets base address of ITS command queue
    __RW u64 gits_cwriter;          // +0x0088 - RW - Points to next enrty to add command
    __RW u64 gits_creadr;           // +0x0090 - RW - Points to command being processed
    __RO u32 padding3[2];           // +0x0098 - RESERVED
    __RO u32 padding4[24];          // +0x00A0 - RESERVED
    __RW u64 gits_baser[8];         // +0x0100 - RW - Sets base address of Device and Collection tables
};

// +0x010000 from ITS_BASE
struct gicv3_its_int_if
{
    __RO u32 padding1[16];          // +0x0000 - RESERVED
    __RW u32 gits_translater;       // +0x0040 - RW - Written by peripherals to generate LPI
};

// +0x020000 from ITS_BASE
struct gicv3_its_sgi_if
{
    __RO u32 padding1[8];           // +0x0000 - RESERVED
    __RW u64 gits_sgir;             // +0x0020 - RW - Written by peripherals to generate vSGI (GICv4.1)
};

// Sets the address of the Distributor and Redistributors
// dist   = virtual address of the Distributor
// rdist  = virtual address of the first RD_base register page
void set_gic_addr(void* dist, void* rdist);

// Returns the number of PPIs in the GICv3.1 extended range
u32 get_ext_ppi(u32 rd);

// Returns the number of SPIs in the GICv3.0 range
u32 get_spi(void);

// Returns the number of SPIs in the GICv3.1 extended SPI range
u32 get_ext_spi(void);

// Sets the group enable bits in the Distributor, with GICv3 mode selected
u32 enable_gic(void);

// Get Redistributer number for a given affinity
// affinity
u32 get_redist_id(u32 affinity);


// Wakes the currently select redistributor
// rd    = Redistributor number
u32 wake_up_redist(u32 rd);

// Enables the id
// id = id of interrupt
// rd    = Redistributor number (ignored if id is a SPI)
u32 enable_irq(u32 id, u32 rd);


// Disables the id
// id = id of interrupt
// rd    = Redistributor number (ignored if id is a SPI)
u32 disable_irq(u32 id, u32 rd);


// Sets the priority of the specified INITD
// id    = id of interrupt
// rd    = Redistributor number (ignored if id is a SPI)
// priority = priority (8-bit unsigned value)
u32 set_irq_priority(u32 id, u32 rd, u8 priority);


// Sets the target CPUs of the specified id
// id    = id of interrupt (must be in the range 32 to 1019)
// mode     = Routing mode
// aff<n>   = Affinity co-ordinate of target
#define GICV3_ROUTE_MODE_ANY             (0x80000000)
#define GICV3_ROUTE_MODE_COORDINATE      (0)
u32 set_irq_route(u32 id, u32 mode, u32 affinity);


// Configures the id as edge or level sensitive
// id = id of interrupt
// rd    = Redistributor number (ignored if id is a SPI)
// conf  = Whether the id should edge or level (use define above)
#define GICV3_CONFIG_LEVEL               (0)
#define GICV3_CONFIG_EDGE                (2)
u32 set_irq_type(u32 id, u32 rd, u32 type);


// Set security/group of the specified id
// id = id of interrupt (ID must be less than 32)
// rd    = Redistributor number (ignored if id is a SPI)
// group = Security/group setting
#define GICV3_GROUP0                     (0)
#define GICV3_GROUP1_SECURE              (1)
#define GICV3_GROUP1_NON_SECURE          (2)

u32 set_irq_group(u32 id, u32 rd, u32 security);


// Sets the pending bit of the specified INIT
// id   = id of interrupt (ID must be less than 1020)
u32 set_irq_pending(u32 id, u32 rd);

// Clears the pending bit of the specified id
// id   = id of interrupt (ID must be less than 1020)
u32 clear_irq_pending(u32 id, u32 rd);

// Enables group 0 interrupts
// The lowest EL that access the ICC_IGRPEN0_EL1 is determined
// by the routine of the FIQ exception.
extern void __enable_group0_irq__(void);


// Disables group 0 interrupts
// The lowest EL that access the ICC_IGRPEN0_EL1 is determined
// by the routine of the FIQ exception.
extern void __disable_group0_irq__(void);


// Enables group 1 interrupts for current security state
// The lowest EL that access the ICC_IGRPEN1_EL1 is determined
// by the routine of the IRQ exception.
extern void __enable_group1_irq__(void);


// Disables group 1 interrupts for current security state
// The lowest EL that access the ICC_IGRPEN1_EL1 is determined
// by the routine of the IRQ exception.
extern void __disable_group1_irq__(void);


// Enables group 1 interrupts for non-secure state
//
// Used by EL3 to enable non-secure group 1 interrupts
extern void __enable_ns_group1_irq__(void);


// Disables group 1 interrupts for non-secure state
//
// Used by EL3 to disable non-secure group 1 interrupts
extern void __disable_ns_group1_irq__(void);

// Returns the value of the ICC_IAR0_EL1 (Group 0 Interrupt Acknowledge)
extern u32 __read_iar_grp0__(void);


// Writes id to the End Of Interrupt register
extern void __write_eoi_grp0__(u32 intid);


// Writes id to the De-active Interrupt register
extern void __write_dir__(u32 intid);


// Returns the value of the ICC_IAR1_EL1 (Group 1 Interrupt Acknowledge)
extern u32 __read_iar_grp1__(void);


// Writes id to the Aliased End Of Interrupt register
extern void __write_eoi_grp1__(u32 intid);


// Sets the Priority mask register for the core run on
// priority - mask value (8-bit unsigned value).
//
// NOTE: An interrupt must be high priority (lower numeric value) than the mask to be sent
extern void __set_priority_mask__(u32 priority);


// Sets ICC_BPR0_EL1 for the core run on
// priority - Binary piont value
extern void __set_bpr0__(u32 priority);


// Sets ICC_BPR1_EL1 for the core run on
// priority - Binary piont value
extern void __set_bpr1__(u32 priority);


// Returns the priority of the current active interrupt
extern u32 __get_running_priority__(void);

#define GICV3_SGI_AFF3_SHIFT          (48)
#define GICV3_SGI_AFF2_SHIFT          (32)
#define GICV3_SGI_AFF1_SHIFT          (16)

#define GICV3_SGI_ROUTING_ALL         ((u64)1 << 40)
#define GICV3_SGI_ROUTING_LIST        (0)

#define GICV3_SGI_TARGET_CPU0         (0x0001)
#define GICV3_SGI_TARGET_CPU1         (0x0002)
#define GICV3_SGI_TARGET_CPU2         (0x0004)
#define GICV3_SGI_TARGET_CPU3         (0x0008)
#define GICV3_SGI_TARGET_CPU4         (0x0010)
#define GICV3_SGI_TARGET_CPU5         (0x0020)
#define GICV3_SGI_TARGET_CPU6         (0x0040)
#define GICV3_SGI_TARGET_CPU7         (0x0080)
#define GICV3_SGI_TARGET_CPU8         (0x0100)
#define GICV3_SGI_TARGET_CPU9         (0x0200)
#define GICV3_SGI_TARGET_CPU10        (0x0400)
#define GICV3_SGI_TARGET_CPU11        (0x0800)
#define GICV3_SGI_TARGET_CPU12        (0x1000)
#define GICV3_SGI_TARGET_CPU13        (0x2000)
#define GICV3_SGI_TARGET_CPU14        (0x4000)
#define GICV3_SGI_TARGET_CPU15        (0x8000)

#define GICV3_SGI_ID0                 (0x0 << 24)
#define GICV3_SGI_ID1                 (0x1 << 24)
#define GICV3_SGI_ID2                 (0x2 << 24)
#define GICV3_SGI_ID3                 (0x3 << 24)
#define GICV3_SGI_ID4                 (0x4 << 24)
#define GICV3_SGI_ID5                 (0x5 << 24)
#define GICV3_SGI_ID6                 (0x6 << 24)
#define GICV3_SGI_ID7                 (0x7 << 24)
#define GICV3_SGI_ID8                 (0x8 << 24)
#define GICV3_SGI_ID9                 (0x9 << 24)
#define GICV3_SGI_ID10                (0xA << 24)
#define GICV3_SGI_ID11                (0xB << 24)
#define GICV3_SGI_ID12                (0xC << 24)
#define GICV3_SGI_ID13                (0xD << 24)
#define GICV3_SGI_ID14                (0xE << 24)
#define GICV3_SGI_ID15                (0xF << 24)

// Send a Group 0 SGI
extern void __send_group0_sgi__(u32 id, u64 mode, u32 target_list);


// Send a Group 1 SGI, current security state
extern void __send_group1_sgi__(u32 id, u64 mode, u32 target_list);


// Send a Group 1 SGI, other security state
extern void __send_other_group1_sgi__(u32 id, u64 mode, u32 target_list);


// Selects an ITS
// ITS_base = virtual address of ITS_base register page
void set_its_base_addr(void* its_base);

// Used for setLPIConfigTableAddr() and setLPIPendingTableAddr()
#define GICV3_LPI_INNER_SHARED  (0x1 << 10)
#define GICV3_LPI_OUTER_SHARED  (0x2 << 10)
#define GICV3_LPI_NON_SHARED    (0)

#define GICV3_LPI_NORMAL_WBWA   (((u64)0x5 << 56) | (u64)(0x5 << 7))
#define GICV3_LPI_DEVICE_nGnRnE (0)


// Set address of LPI config table for the selected Redistributor
// rd         = Redistributor
// addr       = physical address of allocated memory
// attributes = cacheability/shareabilty settings
// IDbits     = number of ID bits
//
// NOTE: If IDbits > GICD_TYPER.IDbits, GICD_CTLR.IDbits will be used
// NOTE: The amount of memory allocated must be enough for the number of IDbits!
// NOTE: This function will use memset() to zero the allocated memory.
u32 set_lpi_config_table_addr(u32 rd, u64 addr, u64 attributes, u32 idbits);


// Set address of the LPI pending table for the selected Redistributor
// rd         = Redistributor
// addr       = physical address of allocated memory
// attributes = cacheability/shareabilty settings
// IDbits     = number of ID bits (needed in order to work how much memory to zero)
//
// NOTE: The amount of memory allocated must be enough for the number of IDbits!
// NOTE: This function will use memset() to zero the allocated memory.
u32 set_lpi_pending_table_addr(u32 rd, u64 addr, u64 attributes, u32 idbits);


// Enables LPIs for the currently selected Redistributor
// NOTE: DO NOT CALL THIS BEFORE ALLOCATING TABLES!
void enable_lpis(u32 rd);


// Returns the RD's Processor_Number, used in ITS commands when GITS_TYPER.PTA==0
u32 get_rd_proc_nr(u32 rd);


// Returns the maximum LPI id supported by the GIC
u32 get_max_lpi(u32 rd);

#define GICV3_LPI_ENABLE                 (1)
#define GICV3_LPI_DISABLE                (0)

// Configures specified LPI
// rd       = Redistributor
// id    = id of LPI to be configured (must be 8192 or greater,
// no upper limit check)
// enable   = whether ID is enabled (1=enabled, 0=disabled)
// priority = priority for ID
u32 configure_lpi(u32 rd, u32 id, u32 enable, u32 priority);

#define GICV3_ITS_CQUEUE_VALID          ((u64)1 << 63)
#define GICV3_ITS_CQUEUE_INVALID        (0)

// Initializes the currently selected ITS's command queue
// addr       = base address of allocated memory, must be 64K aligned
// attributes = cacheabilit, shareabilty and valid bit
// num_pages  = number of 4K pages (must be 1 or greater)
//
// NOTE: The GITS_CBASER encodes the number of pages as (number - 1).
// This functions expects an unmodified value (i.e. pass 1, if 1 page allocated).
// NOTE: This function will use memset() to zero the allocated memory.
u32 init_its_cmd_queue(u64 addr, u64 attributes, u32 num_pages);

// Returns the type and entry size of GITS_BASER[index] of the]
// currently selected ITS
// index = which GITS_BASERn register to access, must be in the
// range 0 to 7
// type  = pointer to a u32, function write the vale of the
// GITS_BASERn.Type to this pointer
// entry_size  = pointer to a u32, function write the vale of
// the GITS_BASERn.Entry_Size to this pointer

#define GICV3_ITS_TABLE_TYPE_UNIMPLEMENTED (0x0)
#define GICV3_ITS_TABLE_TYPE_DEVICE        (0x1)
#define GICV3_ITS_TABLE_TYPE_VIRTUAL       (0x2)
#define GICV3_ITS_TABLE_TYPE_COLLECTION    (0x4)

#define GICV3_ITS_TABLE_PAGE_SIZE_4K       (0x0)
#define GICV3_ITS_TABLE_PAGE_SIZE_16K      (0x1)
#define GICV3_ITS_TABLE_PAGE_SIZE_64K      (0x2)

u64 get_its_table_type(u32 index, u32* type, u32* entry_size);


// Sets GITS_BASER[entry]
// index = which GITS_BASERn register to access, must be in the range 0 to 7
// addr  = phsyical address of allocated memory.  Must be at least 4K aligned.
// attributes = Cacheability, shareabilty, validity and indirect settings
// page_size  = Size of allocated pages (4K=0x0, 16K=0x100, 64K=0x200)
// num_pages  = The number of allocated pages.  Must be greater than 0.
//
// NOTE: The registers encodes as (number - 1), this function expecst then
// actual number

#define GICV3_ITS_TABLE_PAGE_VALID            ((u64)1 << 63)
#define GICV3_ITS_TABLE_PAGE_INVALID          (0)

#define GICV3_ITS_TABLE_PAGE_DIRECT           (0)
#define GICV3_ITS_TABLE_PAGE_INDIRECT         (1 << 62)

#define GICV3_ITS_TABLE_PAGE_DEVICE_nGnRnE    (0)
#define GICV3_ITS_TABLE_PAGE_NORM_WBWA        (((u64)0x5 << 59) | ((u64)0x5 << 53))

#define GICV3_ITS_TABLE_INNER_SHAREABLE       (0x1 << 10)
#define GICV3_ITS_TABLE_OUTER_SHAREABLE       (0x2 << 10)
#define GICV3_ITS_TABLE_NON_SHAREABLE         (0)

u32 set_its_table_addr(u32 index, u64 addr, u64 attributes,
                       u32 page_size, u32 num_pages);

// Returns the value of GITS_TYPER.PTA bit (shifted down to LSB)

#define GICV3_ITS_PTA_ADDRESS              (1)
#define GICV3_ITS_PTA_ID                   (0)

u32 get_its_pta(void);


// Sets the GITS_CTLR.Enabled bit
void enable_its(void);


// Clears the GITS_CTLR.Enabled bit
void disable_its(void);

// Issues a MAPD command to the currently selected ITS
// device = The device id
// table  = Physical address of the ITT table to be used
// size   = The number of bits of ID used by the device
//
// NOTE: The underlying command records the size as (number - 1).
// This functions expects an unmodified value (i.e. pass 2, if 2 bits).
void its_mapd(u32 device_id, u64 table, u32 size);


// Issues a MAPC command to the currently selected ITS
// target     = the target Redistributor.  Either the PA
// or the ID, depending on GITS_TYPER
// collection = the collection id
void its_mapc(u32 target, u32 collection);


// Issues a MAPTI command to the currently selected ITS
// device_id= the DeviceID
// event_id = the EventID (the "ID" the peripheral will write)
// intid    = the GIC id (the "ID" that will be reported when readin IAR)
// cid      = the collection id
void its_mapti(u32 device_id, u32 event_id, u32 intid, u32 cid);


// Issues an INVALL command to the currently selected ITS
// cid      = the collection id
void its_invall(u32 cid);


// Issues an INV command to the currently selected ITS
// device_id = the DeviceID
// event_id  = the EventID
void its_inv(u32 device_id, u32 event_id);


// Issues a SYNC command to the currently selected ITS
// target = The target Redistributor.  Either the PA or the ID, depending on GITS_TYPER
void its_sync(u64 target);


// Issues a INT command to the currently selected ITS
// device = device id
// id     = EventID  (the value the peripheral would have written)
void its_irq(u32 device_id, u32 event_id);

// ####################################################

int arch_gic_init(void *dist_ba, void *rd_ba, u32 aff);

// ####################################################

#endif /* _ASM_GIC_H_ */
