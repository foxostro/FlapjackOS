#include "catch.hpp"

#include <cstdio>
#include <cstddef>
#include <cstring>

#include <seg.h>
#include <platform/i386/gdt.hpp>
#include <common/misc.h>

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
    i386::GlobalDescriptorTableEntry entry;
    entry.init(0x00000000, // base
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
    i386::GlobalDescriptorTableEntry entry;
    entry.init(0x00000000, // base
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
        0x00000000, // 2
        0x00000000,
        0x0000ffff, // 3
        0x00cf9b00,
        0x0000ffff, // 4
        0x00cf9300,
        0x0000ffff, // 5
        0x00cffb00,
        0x0000ffff, // 6
        0x00cff200,
    };
    i386::GlobalDescriptorTable gdt;
    gdt.establish_flat_mapping(0x00100930);
    
    REQUIRE(!memcmp(gdt.entries, expected, sizeof(expected)));
}
