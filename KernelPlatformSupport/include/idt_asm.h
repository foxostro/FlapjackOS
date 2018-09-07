#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_IDT_ASM_H
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_IDT_ASM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IDT_DE          0       // Division Error (Fault)
#define IDT_DB          1       // Debug Exception (Fault/Trap)
#define IDT_NMI         2       // Non-Maskable Interrupt (Interrupt)
#define IDT_BP          3       // Breakpoint (Trap) 
#define IDT_OF          4       // Overflow (Trap)
#define IDT_BR          5       // BOUND Range exceeded (Fault)
#define IDT_UD          6       // Undefined Opcode (Fault)
#define IDT_NM          7       // No Math coprocessor (Fault)
#define IDT_DF          8       // Double Fault (Abort)
#define IDT_CSO         9       // Coprocessor Segment Overrun (Fault)
#define IDT_TS          10      // Invalid Task Segment Selector (Fault)
#define IDT_NP          11      // Segment Not Present (Fault)
#define IDT_SS          12      // Stack Segment Fault (Fault)
#define IDT_GP          13      // General Protection Fault (Fault)
#define IDT_PF          14      // Page Fault (Fault)
                                // IDT entry 15 is reserved.
#define IDT_MF          16      // X87 Math Fault (Fault)
#define IDT_AC          17      // Alignment Check (Fault)
#define IDT_MC          18      // Machine Check (Abort)
#define IDT_XF          19      // SSE Floating Point Exception (Fault)
                                // IDT entries 16 through 31 are reserved.
#define IDT_USER_START  32      // User define IDT entries start here

#define IDT_TIMER       0x20
#define IDT_KEY         0x21

#define IDT_MAX         256     // There are this many IDT entries.

#define TASK_GATE       0b0101  // 32-bit task gate
#define INTERRUPT_GATE  0b1110  // 32-bit interrupt gate
#define TRAP_GATE       0b1111  // 32-bit trap gate

typedef struct idt_entry {
    uint16_t offset_1;   // offset bits 0..15
    uint16_t selector;   // a code segment selector in GDT or LDT
    uint8_t zero;        // unused, set to 0
    uint8_t gate_type:4; // type of gate
    uint8_t s:1;         // storage segement
    uint8_t dpl:2;       // priviledge level for the gate
    uint8_t p:1;         // present bit, set to zero for unused interrupts
    uint16_t offset_2;   // offset bits 16..31
} IDTEntry;

#ifdef __cplusplus
static_assert(8 == sizeof(IDTEntry), "each IDT entry is eight bytes");
#else
_Static_assert(8 == sizeof(IDTEntry), "each IDT entry is eight bytes");
#endif

// Returns a pointer to the beginning of the installed IDT.
IDTEntry* idt_base(void);

// Loads a new IDT.
void lidt(void *idt, unsigned limit);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_IDT_ASM_H
