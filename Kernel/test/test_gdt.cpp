#include "catch.hpp"

#include <cstdio>
#include <cstddef>
#include <cstring>

#include <seg.h>
#include <gdt.h>
#include <misc.h>

#define VERBOSE 0

// See <http://wiki.osdev.org/GDT_Tutorial> which was consulted to write this test code.

// Each define here is for a specific flag in the descriptor.
// Refer to the intel documentation for a description of what each one does.
#define SEG_DESCTYPE(x)  ((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define SEG_PRES(x)      ((x) << 0x07) // Present
#define SEG_SAVL(x)      ((x) << 0x0C) // Available for system use
#define SEG_LONG(x)      ((x) << 0x0D) // Long mode
#define SEG_SIZE(x)      ((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define SEG_GRAN(x)      ((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define SEG_PRIV(x)     (((x) &  0x03) << 0x05)   // Set privilege level (0 - 3)
 
#define SEG_CODE_EXRD      0x0A // Execute/Read
 
#define GDT_CODE_PL0 SEG_DESCTYPE(1) | SEG_PRES(1) | SEG_SAVL(0) | \
                     SEG_LONG(0)     | SEG_SIZE(1) | SEG_GRAN(1) | \
                     SEG_PRIV(0)     | SEG_CODE_EXRD



#if VERBOSE

static void print_bits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
}

static void print_dwords(size_t count, uint32_t *ptr)
{
    while(count--)
    {
        printf("%08x\n", *ptr++);
    }   
}

static void print_gdt_entry(gdt_entry_t *entry)
{
    uint32_t base = (entry->base_0_15) | (entry->base_16_23 << 16) | (entry->base_24_31 << 24);
    uint32_t limit = (entry->limit_0_15) | (entry->limit_16_19 << 16);

    bool pr = (entry->access & 0b10000000) >> 7;
    unsigned privl = (entry->access & 0b01100000) >> 5;
    bool ex = (entry->access & 0b00001000) >> 3;
    bool dc = (entry->access & 0b00000100) >> 2;
    bool rw = (entry->access & 0b00000010) >> 1;
    bool ac = (entry->access & 0b00000001);

    printf("base\t%08x\n", base);
    printf("limit\t%08x\n", limit);
    printf("access\t%02x\n", entry->access);
    printf("\tpr\t%x\n", pr);
    printf("\tprivl\t%x\n", privl);
    printf("\tex\t%x\n", ex);
    printf("\tdc\t%x\n", dc);
    printf("\trw\t%x\n", rw);
    printf("\tac\t%x\n", ac);
    printf("flags\t%x\n", entry->flags);
}

#endif



static uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag)
{
    uint64_t descriptor;
 
    // Create the high 32 bit segment
    descriptor  =  limit       & 0x000F0000;         // set limit bits 19:16
    descriptor |= (flag <<  8) & 0x00F0FF00;         // set type, p, dpl, s, g, d/b, l and avl fields
    descriptor |= (base >> 16) & 0x000000FF;         // set base bits 23:16
    descriptor |=  base        & 0xFF000000;         // set base bits 31:24
 
    // Shift by 32 to allow for low part of segment
    descriptor <<= 32;
 
    // Create the low 32 bit segment
    descriptor |= base  << 16;                       // set base bits 15:0
    descriptor |= limit  & 0x0000FFFF;               // set limit bits 15:0
 
    return descriptor;
}



TEST_CASE("test_gdt_layout_0", "[gdt]")
{
    uint64_t expected = create_descriptor(0, 0, 0);
    gdt_entry_t entry;
    gdt_create_entry(&entry,
                     0x00000000, // base
                     0x00000000, // limit
                     false,      // gr
                     false,      // sz
                     false,      // pr
                     0,          // privl
                     false,      // ex
                     false,      // dc
                     false,      // rw
                     false);     // ac
    REQUIRE(!memcmp(&expected, &entry, sizeof(expected)));
}

TEST_CASE("test_gdt_layout_1", "[gdt]")
{
    uint64_t expected = create_descriptor(0, 0x000fffff, GDT_CODE_PL0);
    gdt_entry_t entry;
    gdt_create_entry(&entry,
                     0x00000000, // base
                     0x000fffff, // limit
                     true,       // gr
                     true,       // sz
                     true,       // pr
                     0,          // privl
                     true,       // ex
                     false,      // dc
                     true,       // rw
                     false);     // ac
    REQUIRE(!memcmp(&expected, &entry, sizeof(expected)));
}

TEST_CASE("test_gdt_layout_2", "[gdt]")
{
    uint32_t expected[] = { // This hand-crafted GDT establishes a flat memory mapping.
        0x00000000, // 0
        0x00000000,
        0x09300067, // 1
        0x00008910,
        0x0000ffff, // 2
        0x00cf9b00,
        0x0000ffff, // 3
        0x00cf9300,
        0x0000ffff, // 4
        0x00cffb00,
        0x0000ffff, // 5
        0x00cff200,
    };
    gdt_entry_t gdt[6];
    memset(gdt, 0, sizeof(gdt));
    gdt_create_flat_mapping(gdt, sizeof(gdt), 0x00100930);

#if VERBOSE
    printf("expected gdt:\n");
    gdt_entry_t *expected_gdt = (gdt_entry_t *)expected;
    for(int i = 0; i < 6; ++i) {
        print_gdt_entry(&expected_gdt[i]);
        printf("\n");
    }
    printf("\n");

    printf("actual gdt:\n");
    for(int i = 0; i < 6; ++i) {
        print_gdt_entry(&gdt[i]);
        printf("\n");
    }
    printf("\n");
#endif
    
    REQUIRE(!memcmp(gdt, expected, sizeof(expected)));
}
