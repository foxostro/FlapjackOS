#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_NUMBERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_NUMBERS_HPP

// Refer to Intel manual, volume 3a, section 6.15.

constexpr unsigned IDT_DE  = 0;         // Division Error (Fault)
constexpr unsigned IDT_DB  = 1;         // Debug Exception (Fault/Trap)
constexpr unsigned IDT_NMI = 2;         // Non-Maskable Interrupt (Interrupt)
constexpr unsigned IDT_BP  = 3;         // Breakpoint (Trap) 
constexpr unsigned IDT_OF  = 4;         // Overflow (Trap)
constexpr unsigned IDT_BR  = 5;         // BOUND Range exceeded (Fault)
constexpr unsigned IDT_UD  = 6;         // Undefined Opcode (Fault)
constexpr unsigned IDT_NM  = 7;         // No Math coprocessor (Fault)
constexpr unsigned IDT_DF  = 8;         // Double Fault (Abort)
constexpr unsigned IDT_CSO = 9;         // Coprocessor Segment Overrun (Fault)
constexpr unsigned IDT_TS  = 10;        // Invalid Task Segment Selector (Fault)
constexpr unsigned IDT_NP  = 11;        // Segment Not Present (Fault)
constexpr unsigned IDT_SS  = 12;        // Stack Segment Fault (Fault)
constexpr unsigned IDT_GP  = 13;        // General Protection Fault (Fault)
constexpr unsigned IDT_PF  = 14;        // Page Fault (Fault)
                                        // IDT entry 15 is reserved.
constexpr unsigned IDT_MF  = 16;        // X87 Math Fault (Fault)
constexpr unsigned IDT_AC  = 17;        // Alignment Check (Fault)
constexpr unsigned IDT_MC  = 18;        // Machine Check (Abort)
constexpr unsigned IDT_XF  = 19;        // SSE Floating Point Exception (Fault)
                                        // IDT entries 16 through 31 are reserved.
constexpr unsigned IDT_USER_START = 32; // User define IDT entries start here

constexpr unsigned IDT_TIMER = 0x20;
constexpr unsigned IDT_KEY   = 0x21;

constexpr unsigned IDT_MAX = 256; // There are this many IDT entries.

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_NUMBERS_HPP
