#ifndef FLAPJACKOS_KERNEL_INCLUDE_IDT_H
#define FLAPJACKOS_KERNEL_INCLUDE_IDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <idt_asm.h>

// Build an IDT entry.
// entry -- The entry to write into.
// offset -- Pointer to the handler function.
// gate_type -- The type of gate to use for the entry.
// dpl -- Privilege level for the gate.
void idt_build_entry(IDTEntry *entry, uint32_t offset, unsigned gate_type, unsigned dpl);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_IDT_H