#include "catch.hpp"

#include <cstdio>
#include <cstddef>

#include <seg.h>
#include <idt.h>
#include <misc.h>

// Useful for installing Trap Gates
#define DPL_FIELD(DPL)    (unsigned)( ((DPL) & 0x03) << 13 )
#define TRAP_GATE_WORD_1  0x8F00
#define SIZEOF_IDT_ENTRY  8 // IDT entry is 8 bytes long (64 bits)

// This is a sanity check where we construct a trap gate the hard way using
// direct bit manipulation and check to make sure it's the same as the one get
// out of idt_build_entry(). This gives assurance that the memory layout of the
// bitfields in idt_entry_t is correct.
TEST_CASE("test_trap_gate", "[idt]")
{
    unsigned DPL = 0;
    unsigned offset = 0xdeadbeef;

    idt_entry_t entry_expected;
    memset(&entry_expected, 0, sizeof(entry_expected));

    // Construct the IDT entry according to the Intel documentation for a
    // "Trap Gate" entry. (See page 151 of intel-sys.pdf for details.)
    unsigned short *entry_words = (unsigned short *)(&entry_expected);

    entry_words[0] = DWORD_LOWER_WORD(offset);
    entry_words[1] = SEGSEL_KERNEL_CS;
    entry_words[2] = TRAP_GATE_WORD_1 | DPL_FIELD(DPL);
    entry_words[3] = DWORD_UPPER_WORD(offset);

    idt_entry_t entry;
    idt_build_entry(&entry, offset, TRAP_GATE, DPL);

    REQUIRE(!memcmp(&entry, &entry_expected, sizeof(entry_expected)));
}
