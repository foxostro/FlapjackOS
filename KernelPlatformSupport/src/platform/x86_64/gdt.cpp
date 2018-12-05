#include <platform/x86_64/gdt.hpp>
#include <platform/x86_64/lgdt.hpp>
#include <seg.h>
#include <cstring> // for memset()

// Declare a Global Descriptor Table object.
// Parameters for this macro are named so as to match the Intel documentation
// in vol. 3a, section 3.4.5. Refer to the documentation for more details.
// BASE -- Base address of the segment.
// LIMIT -- Specifies the size of the segment.
// G -- Granularity flag.
// D/B -- This flag should always be set to 1 for 32-bit code and data segments.
// L -- 64-bit code segment flag. The D bit must be cleared if the L bit is set.
// AVL -- Available and reserved bits.
// P -- Segment present flag.
// DPL -- Descriptor privilege level field.
// S -- Descriptor type flag.
// TYPE -- Type field.
#define DECLARE_GDT_ENTRY(BASE, LIMIT, G, DB, L, AVL, P, DPL, S, TYPE)         \
(                                                                              \
((((GlobalDescriptorTableEntry)(BASE) >> 24) & 0xFF) << 56) |                  \
(((GlobalDescriptorTableEntry)(G) & 0x1) << 55) |                              \
(((GlobalDescriptorTableEntry)(DB) & 0x1) << 54) |                             \
(((GlobalDescriptorTableEntry)(L) & 0x1) << 53) |                              \
(((GlobalDescriptorTableEntry)(AVL) & 0x1) << 52) |                            \
((((GlobalDescriptorTableEntry)(LIMIT) >> 16) & 0xF) << 48) |                  \
(((GlobalDescriptorTableEntry)(P) & 0x1) << 47) |                              \
(((GlobalDescriptorTableEntry)(DPL) & 0x3) << 45) |                            \
(((GlobalDescriptorTableEntry)(S) & 0x1) << 44) |                              \
(((GlobalDescriptorTableEntry)(TYPE) & 0xf) << 40) |                           \
(((GlobalDescriptorTableEntry)(BASE) & 0xFFF) << 16) |                         \
((GlobalDescriptorTableEntry)(LIMIT) & 0xFFFF)                                 \
)

// Declare the lower eight bytes of Global Descriptor Table entry for the TSS.
// Parameters for this macro are named so as to match the Intel documentation
// in vol. 3a, section 7.2.3. Refer to the documentation for more details.
// BASE -- The 64-bit base address of the TSS structure.
// LIMIT -- The size of the TSS structure.
// AVL -- Available and reserved bits.
// DPL -- Descriptor privilege level field.
// G -- Granularity flag.
// P -- Segment present flag.
// TYPE -- Type field.
#define DECLARE_GDT_TSS_ENTRY_LO(BASE, LIMIT, AVL, DPL, G, P, TYPE)            \
(                                                                              \
((((GlobalDescriptorTableEntry)(BASE) >> 24) & 0xFF) << 56) |                  \
(((GlobalDescriptorTableEntry)(G) & 0x1) << 55) |                              \
(((GlobalDescriptorTableEntry)(AVL) & 0x1) << 52) |                            \
((((GlobalDescriptorTableEntry)(LIMIT) >> 16) & 0xF) << 48) |                  \
(((GlobalDescriptorTableEntry)(P) & 0x1) << 47) |                              \
(((GlobalDescriptorTableEntry)(DPL) & 0x3) << 45) |                            \
(((GlobalDescriptorTableEntry)(TYPE) & 0xf) << 40) |                           \
(((GlobalDescriptorTableEntry)(BASE) & 0xFFF) << 16) |                         \
((GlobalDescriptorTableEntry)(LIMIT) & 0xFFFF)                                 \
)

// Declare the upper eight bytes of Global Descriptor Table entry for the TSS.
// Parameters for this macro are named so as to match the Intel documentation
// in vol. 3a, section 7.2.3. Refer to the documentation for more details.
// BASE -- The 64-bit base address of the TSS structure.
#define DECLARE_GDT_TSS_ENTRY_HI(BASE)                                         \
(                                                                              \
(((GlobalDescriptorTableEntry)(BASE) >> 32) & 0xFFFFFFFF)                      \
)

namespace x86_64 {

constexpr unsigned GDT_D_BIT = 0;
constexpr unsigned GDT_L_BIT = 1;

// Type field values are described in the Intel manual vol. 3a, section 3.5.4.1.
//constexpr unsigned GDT_TYPE_DATA_READ_ONLY                        = 0b0000;
//constexpr unsigned GDT_TYPE_DATA_READ_ONLY_ACCESSED               = 0b0001;
constexpr unsigned GDT_TYPE_DATA_READ_WRITE                         = 0b0010;
//constexpr unsigned GDT_TYPE_DATA_READ_WRITE_ACCESSED              = 0b0011;
//constexpr unsigned GDT_TYPE_DATA_READ_ONLY_EXPAND_DOWN            = 0b0100;
//constexpr unsigned GDT_TYPE_DATA_READ_ONLY_EXPAND_DOWN_ACCESSED   = 0b0101;
//constexpr unsigned GDT_TYPE_DATA_READ_WRITE_EXPAND_DOWN           = 0b0110;
//constexpr unsigned GDT_TYPE_DATA_READ_WRITE_EXPAND_DOWN_ACCESSED  = 0b0111;
//constexpr unsigned GDT_TYPE_CODE_EXECUTE_ONLY                     = 0b1000;
//constexpr unsigned GDT_TYPE_CODE_EXECUTE_ONLY_ACCESSED            = 0b1001;
constexpr unsigned GDT_TYPE_CODE_EXECUTE_READ                       = 0b1010;
//constexpr unsigned GDT_TYPE_CODE_EXECUTE_READ_ACCESSED            = 0b1011;
//constexpr unsigned GDT_TYPE_CODE_EXECUTE_ONLY_CONFORMING          = 0b1100;
//constexpr unsigned GDT_TYPE_CODE_EXECUTE_ONLY_CONFORMING_ACCESSED = 0b1101;
//constexpr unsigned GDT_TYPE_CODE_EXECUTE_READ_CONFORMING          = 0b1110;
//constexpr unsigned GDT_TYPE_CODE_EXECUTE_READ_CONFORMING_ACCESSED = 0b1111;

// The "type" value for a TSS descriptor is specified in the Intel manual
// volume 3a, section 3.5, table 3-2.
constexpr unsigned GDT_TYPE_TSS_DESCRIPTOR = 0b1001;

constexpr size_t GlobalDescriptorTable::COUNT;

void GlobalDescriptorTable::init(TaskStateSegment *tss)
{
    memset(entries_, 0, sizeof(entries_));

    static_assert(sizeof(TaskStateSegment)-1 == 0x67, "");

    entries_[SEGSEL_KERNEL_TSS_IDX_LO] = DECLARE_GDT_TSS_ENTRY_LO(
        /*BASE=*/(uintptr_t)tss,
        /*LIMIT=*/sizeof(TaskStateSegment)-1,
        /*AVL=*/0,
        /*DPL=*/0,
        /*G=*/0,
        /*P=*/1,
        /*TYPE=*/GDT_TYPE_TSS_DESCRIPTOR
    );

    entries_[SEGSEL_KERNEL_TSS_IDX_HI] = DECLARE_GDT_TSS_ENTRY_HI(
        /*BASE=*/(uintptr_t)tss
    );

    entries_[SEGSEL_KERNEL_CS_IDX] = DECLARE_GDT_ENTRY(
        /*BASE=*/0x00000000,
        /*LIMIT=*/0xFFFFF,
        /*G=*/1,
        /*DB=*/GDT_D_BIT,
        /*L=*/GDT_L_BIT,
        /*AVL=*/0,
        /*P=*/1,
        /*DPL=*/0,
        /*S=*/1,
        /*TYPE=*/GDT_TYPE_CODE_EXECUTE_READ
    );

    entries_[SEGSEL_KERNEL_DS_IDX] = DECLARE_GDT_ENTRY(
        /*BASE=*/0x00000000,
        /*LIMIT=*/0xFFFFF,
        /*G=*/1,
        /*DB=*/GDT_D_BIT,
        /*L=*/GDT_L_BIT,
        /*AVL=*/0,
        /*P=*/1,
        /*DPL=*/0,
        /*S=*/1,
        /*TYPE=*/GDT_TYPE_DATA_READ_WRITE
    );

    entries_[SEGSEL_USER_CS_IDX] = DECLARE_GDT_ENTRY(
        /*BASE=*/0x00000000,
        /*LIMIT=*/0xFFFFF,
        /*G=*/1,
        /*DB=*/GDT_D_BIT,
        /*L=*/GDT_L_BIT,
        /*AVL=*/0,
        /*P=*/1,
        /*DPL=*/3,
        /*S=*/1,
        /*TYPE=*/GDT_TYPE_CODE_EXECUTE_READ
    );

    entries_[SEGSEL_USER_DS_IDX] = DECLARE_GDT_ENTRY(
        /*BASE=*/0x00000000,
        /*LIMIT=*/0xFFFFF,
        /*G=*/1,
        /*DB=*/GDT_D_BIT,
        /*L=*/GDT_L_BIT,
        /*AVL=*/0,
        /*P=*/1,
        /*DPL=*/3,
        /*S=*/1,
        /*TYPE=*/GDT_TYPE_DATA_READ_WRITE
    );
}

void GlobalDescriptorTable::load()
{
    lgdt(entries_, sizeof(entries_) - 1);
}

} // namespace x86_64
