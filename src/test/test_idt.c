#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <check.h>

#include <seg.h>
#include <idt.h>
#include <misc.h>

idt_entry_t g_idt[IDT_MAX];

idt_entry_t* idt_base()
{
    return g_idt;
}

// Useful for installing Trap Gates
#define DPL_FIELD(DPL)    (unsigned)( ((DPL) & 0x03) << 13 )
#define TRAP_GATE_WORD_1  0x8F00
#define SIZEOF_IDT_ENTRY  8 // IDT entry is 8 bytes long (64 bits)

// This is a sanity check where we construct a trap gate the hard way using
// direct bit manipulation and check to make sure it's the same as the one get
// out of idt_build_entry(). This gives assurance that the memory layout of the
// bitfields in idt_entry_t is correct.
START_TEST(test_trap_gate)
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

    ck_assert(!memcmp(&entry, &entry_expected, sizeof(entry_expected)));
}
END_TEST

static const struct { char *name; void *fn; } tests[] = {
    { "test_trap_gate", test_trap_gate },
};
static const size_t num_tests = sizeof(tests) / sizeof(tests[0]);

Suite* test_suite_idt(void)
{
    Suite *suite = suite_create("test_suite_idt");

    for (size_t i = 0; i < num_tests; ++i) {
        TCase *testCase = tcase_create(tests[i].name);
        tcase_add_test(testCase, tests[i].fn);
        suite_add_tcase(suite, testCase);        
    }

    return suite;
}
