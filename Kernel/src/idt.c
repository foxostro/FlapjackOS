#include <idt.h>
#include <seg.h>
#include <common/misc.h>

#include <stddef.h>

void idt_build_entry(IDTEntry *entry, uint32_t offset, unsigned gate_type, unsigned dpl)
{
    *entry = (IDTEntry) {
        .offset_1 = DWORD_LOWER_WORD(offset),
        .selector = SEGSEL_KERNEL_CS,
        .zero = 0,
        .gate_type = gate_type,
        .s = 0,
        .dpl = dpl,
        .p = 1,
        .offset_2 = DWORD_UPPER_WORD(offset)
    };
}
