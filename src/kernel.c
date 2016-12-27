#include <stddef.h>
#include <stdint.h>
#include <strings.h>
#include <seg.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <ltr.h>
#include <interrupt_asm.h>
#include <isr_install.h>
#include <pic.h>
#include <console.h>
#include <kprintf.h>
#include <timer.h>
#include <inout.h>
#include <halt.h>
#include <panic.h>
#include <backtrace.h>
#include <keyboard.h>
#include <readline.h>

// This global is really only for use in panic() because it severely clutters
// the interface of assert() and panic() if we are reqired to pass down the
// console interface.
console_interface_t g_console;

static gdt_entry_t s_gdt[6];
static tss_struct_t s_tss;
static idt_entry_t s_idt[IDT_MAX];

void interrupt_dispatch(unsigned interrupt_number,
                        unsigned edi,
                        unsigned esi,
                        unsigned ebp,
                        unsigned esp,
                        unsigned ebx,
                        unsigned edx,
                        unsigned ecx,
                        unsigned eax,
                        unsigned error_code,
                        unsigned eip)
{
    bool spurious = pic_clear(interrupt_number);

    if (spurious) {
        return;
    }

    switch(interrupt_number) {
        case IDT_KEY:
            keyboard_int_handler();
            break;

        case IDT_TIMER:
            timer_int_handler();
            break;

        case IDT_DE:
            panic2("Division Error", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_DB:
            panic2("Debug Exception", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_NMI:
            panic2("Non-Maskable Interrupt", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_BP:
            panic2("Breakpoint", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_OF:
            panic2("Overflow", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_BR:
            panic2("BOUND Range exceeded", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_UD:
            panic2("Undefined Opcode", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_NM:
            panic2("No Math coprocessor", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_DF:
            panic2("Double Fault.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_CSO:
            panic2("Coprocessor Segment Overrun", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_TS:
            panic2("Invalid Task Segment Selector.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_NP:
            panic2("Segment Not Present.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_SS:
            panic2("Stack Segment Fault.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_GP:
            panic2("General Protection Fault.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_PF:
            panic2("Page Fault.", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, true, error_code, eip);
            break;

        case IDT_MF:
            panic2("X87 Math Fault", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_AC:
            panic2("Alignment Check", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_MC:
            panic2("Machine Check", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        case IDT_XF:
            panic2("SSE Floating Point Exception", 
                   edi, esi, ebp, esp, ebx, edx, ecx, eax, false, error_code, eip);
            break;

        default:
            panic("Unknown interrupt: 0x%x", interrupt_number);
    }
}

__attribute__((noreturn))
void kernel_main(void *mb_info, void *istack)
{
    console_interface_t *console = &g_console;

    // Setup the initial Task State Segment. The kernel uses one TSS between all
    // tasks and performs software task switching.
    bzero(&s_tss, sizeof(s_tss));
    s_tss.ss0 = SEGSEL_KERNEL_DS;
    s_tss.esp0 = (uint32_t)istack;
    s_tss.iomap = sizeof(s_tss);

    // Setup the Global Descriptor Table. The kernel uses a flat memory map.
    bzero(s_gdt, sizeof(s_gdt));
    gdt_create_flat_mapping(s_gdt, sizeof(s_gdt), (uint32_t)&s_tss);
    lgdt(s_gdt, sizeof(s_gdt) - 1);
    load_task_register();

    // Setup the Interrupt Descriptor Table. This wires various IRQs up to their
    // handler functions.
    isr_install(s_idt);

    // Initialize the PIC chip. It is necessary to remap IRQs before enabling
    // interrupts.
    pic_init();

    // Initialize the console output driver.
    get_console_interface(console);
    console->init((vgachar_t *)0xB8000);
    kprintf(console, "mb_info = %p\n", mb_info);
    kprintf(console, "istack = %p\n", istack);

    // Initialize the keyboard driver.
    keyboard_init();

    // Configure the PIC timer chip so that it fires an interrupt every 10ms.
    timer_init(TIMER_RATE_10ms, TIMER_LEAP_INTERVAL_10ms, TIMER_LEAP_TICKS_10ms);

    // After this point, interrupts will start firing.
    enable_interrupts();

    // Read lines of input from the user forever, but don't do anything with them.
    // (This operating system doesn't do much yet.)
    while (true) {
        char buffer[512];
        readline(console, ">", sizeof(buffer), buffer);
        kprintf(console, "Got: %s\n", buffer);
    }

    panic("We should never reach this point.");
    __builtin_unreachable();
}
