#pragma once

#include <idt_defines.h>

#define TASK_GATE       0b0101  // 32-bit task gate
#define INTERRUPT_GATE  0b1110  // 32-bit interrupt gate
#define TRAP_GATE       0b1111  // 32-bit trap gate

#include <stdint.h>

typedef struct {
   uint16_t offset_1;   // offset bits 0..15
   uint16_t selector;   // a code segment selector in GDT or LDT
   uint8_t zero;        // unused, set to 0
   uint8_t gate_type:4; // type of gate
   uint8_t s:1;         // storage segement
   uint8_t dpl:2;       // priviledge level for the gate
   uint8_t p:1;         // present bit, set to zero for unused interrupts
   uint16_t offset_2;   // offset bits 16..31
} idt_entry_t;
_Static_assert(8 == sizeof(idt_entry_t), "each IDT entry is eight bytes");


// Returns a pointer to the beginning of the installed IDT.
idt_entry_t* idt_base();

// Loads a new IDT.
void lidt(void *idt, unsigned limit);

// Build an IDT entry.
// entry -- The entry to write into.
// offset -- Pointer to the handler function.
// gate_type -- The type of gate to use for the entry.
// dpl -- Privilege level for the gate.
void idt_build_entry(idt_entry_t *entry, uint32_t offset, unsigned gate_type, unsigned dpl);
