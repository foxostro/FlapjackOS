#pragma once

#define IDT_DE          0       // Devision Error (Fault)
#define IDT_DB          1       // Debug Exception (Fault/Trap)
#define IDT_NMI         2       // Non-Maskable Interrupt (Interrupt)
#define IDT_BP          3       // Breakpoint (Trap) 
#define IDT_OF          4       // Overflow (Trap)
#define IDT_BR          5       // BOUND Range exceeded (Fault)
#define IDT_UD          6       // UnDefined Opcode (Fault)
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

#define IDT_ENTS        256     /* There are this many IDT entries */

// Returns a pointer to the beginning of the installed IDT.
void* idt_base();

// Installs an interrupt handler in the IDT.
// TODO: not all interrupt handlers should use Trap Gates
// index -- Index of the appropriate IDT entry
// pOffset -- Due to the segment setup that the 410 support code provides,
// this is merely the address of the interrupt handler assembly
// wrapper.
// DPL -- Caller must be running at this Privilege Level, or lower
void idt_install_trap_gate(unsigned int index, void *pOffset, unsigned DPL);

// Resets all entries in the IDT so they do nothing.
void idt_reset();
