#ifndef FLAPJACKOS_BOOT_INCLUDE_DECLARE_GDT_H
#define FLAPJACKOS_BOOT_INCLUDE_DECLARE_GDT_H

#ifndef __ASSEMBLER__
#error "This header is intended for use only in assembly code."
#endif

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
((((BASE) >> 24) & 0xFF) << 56) |                                              \
(((G) & 0x1) << 55) |                                                          \
(((DB) & 0x1) << 54) |                                                         \
(((L) & 0x1) << 53) |                                                          \
(((AVL) & 0x1) << 52) |                                                        \
((((LIMIT) >> 16) & 0xF) << 48) |                                              \
(((P) & 0x1) << 47) |                                                          \
(((DPL) & 0x3) << 45) |                                                        \
(((S) & 0x1) << 44) |                                                          \
(((TYPE) & 0xf) << 40) |                                                       \
(((BASE) & 0xFFF) << 16) |                                                     \
((LIMIT) & 0xFFFF)                                                             \
)

// Intel Manual 3a, section 3.5.1 specifies that a GDT should be aligned on an
// eight byte boundary.
.set GDT_TABLE_ALIGNMENT, 8

.set GDT_TABLE_SIZE, 0x800

.set GDT_ENTRY_SIZE, 8

#ifdef __x86_64__
#define GDT_D_BIT 0
#define GDT_L_BIT 1
#else
#define GDT_D_BIT 1
#define GDT_L_BIT 0
#endif

// Type field values are described in the Intel manual vol. 3a, section 3.5.4.1.
#define GDT_TYPE_DATA_READ_ONLY                        0b0000
#define GDT_TYPE_DATA_READ_ONLY_ACCESSED               0b0001
#define GDT_TYPE_DATA_READ_WRITE                       0b0010
#define GDT_TYPE_DATA_READ_WRITE_ACCESSED              0b0011
#define GDT_TYPE_DATA_READ_ONLY_EXPAND_DOWN            0b0100
#define GDT_TYPE_DATA_READ_ONLY_EXPAND_DOWN_ACCESSED   0b0101
#define GDT_TYPE_DATA_READ_WRITE_EXPAND_DOWN           0b0110
#define GDT_TYPE_DATA_READ_WRITE_EXPAND_DOWN_ACCESSED  0b0111
#define GDT_TYPE_CODE_EXECUTE_ONLY                     0b1000
#define GDT_TYPE_CODE_EXECUTE_ONLY_ACCESSED            0b1001
#define GDT_TYPE_CODE_EXECUTE_READ                     0b1010
#define GDT_TYPE_CODE_EXECUTE_READ_ACCESSED            0b1011
#define GDT_TYPE_CODE_EXECUTE_ONLY_CONFORMING          0b1100
#define GDT_TYPE_CODE_EXECUTE_ONLY_CONFORMING_ACCESSED 0b1101
#define GDT_TYPE_CODE_EXECUTE_READ_CONFORMING          0b1110
#define GDT_TYPE_CODE_EXECUTE_READ_CONFORMING_ACCESSED 0b1111

// Declare a Global Descriptor Table.
.macro DECLARE_GDT GDT_TABLE_NAME, GDT_TABLE_END_NAME, GDT_PTR_NAME
.align GDT_TABLE_ALIGNMENT
\GDT_TABLE_NAME:
// The first entry is zero.
.8byte 0
// SEGSEL_KERNEL_TSS_IDX_LO. We'll need to specify the TSS later.
.8byte 0
// SEGSEL_KERNEL_TSS_IDX_HI. We'll need to specify the TSS later.
.8byte 0
// SEGSEL_KERNEL_CS_IDX
.8byte DECLARE_GDT_ENTRY(/*BASE=*/0x00000000,
                         /*LIMIT=*/0xFFFFF,
                         /*G=*/1,
                         /*DB=*/GDT_D_BIT,
                         /*L=*/GDT_L_BIT,
                         /*AVL=*/0,
                         /*P=*/1,
                         /*DPL=*/0,
                         /*S=*/1,
                         /*TYPE=*/GDT_TYPE_CODE_EXECUTE_READ)
// SEGSEL_KERNEL_DS_IDX
.8byte DECLARE_GDT_ENTRY(/*BASE=*/0x00000000,
                         /*LIMIT=*/0xFFFFF,
                         /*G=*/1,
                         /*DB=*/GDT_D_BIT,
                         /*L=*/GDT_L_BIT,
                         /*AVL=*/0,
                         /*P=*/1,
                         /*DPL=*/0,
                         /*S=*/1,
                         /*TYPE=*/GDT_TYPE_DATA_READ_WRITE)
// SEGSEL_USER_CS_IDX
.8byte DECLARE_GDT_ENTRY(/*BASE=*/0x00000000,
                         /*LIMIT=*/0xFFFFF,
                         /*G=*/1,
                         /*DB=*/GDT_D_BIT,
                         /*L=*/GDT_L_BIT,
                         /*AVL=*/0,
                         /*P=*/1,
                         /*DPL=*/3,
                         /*S=*/1,
                         /*TYPE=*/GDT_TYPE_CODE_EXECUTE_READ)
// SEGSEL_USER_DS_IDX
.8byte DECLARE_GDT_ENTRY(/*BASE=*/0x00000000,
                         /*LIMIT=*/0xFFFFF,
                         /*G=*/1,
                         /*DB=*/GDT_D_BIT,
                         /*L=*/GDT_L_BIT,
                         /*AVL=*/0,
                         /*P=*/1,
                         /*DPL=*/3,
                         /*S=*/1,
                         /*TYPE=*/GDT_TYPE_DATA_READ_WRITE)
\GDT_TABLE_END_NAME:
.skip (GDT_TABLE_SIZE - (\GDT_TABLE_END_NAME - \GDT_TABLE_NAME))
\GDT_PTR_NAME:
.short GDT_TABLE_SIZE - 1
.long \GDT_TABLE_NAME
.endm

#endif // FLAPJACKOS_BOOT_INCLUDE_DECLARE_GDT_H
