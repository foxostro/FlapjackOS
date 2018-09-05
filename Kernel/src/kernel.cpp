#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>

#include <seg.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <ltr.h>
#include <interrupt_asm.h>
#include <isr_install.h>
#include <pic.h>
#include <inout.h>
#include <halt.h>
#include <panic.h>
#include <backtrace.hpp>
#include <multiboot.h>
#include <creg.h>
#include <kernel_image_info.hpp>
#include <logical_addressing.hpp>
#include <kernel_memory_allocators.hpp>

#include <vga_text_display_device.hpp>
#include <pit_timer_device.hpp>
#include <ps2_keyboard_device.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

// We pass the terminal around globally becuase it severely clutters the
// interface of assert() and panic() if we are reqired to pass the terminal
// around literally everywhere.
text_terminal *g_terminal = NULL;

static gdt_entry_t s_gdt[6];
static tss_struct_t s_tss;
static idt_entry_t s_idt[IDT_MAX];
static ps2_keyboard_device *s_keyboard;
static pit_timer_device *s_timer;
static kernel_memory_allocators *s_allocators;

// Called when a page fault occurs.
static void page_fault_handler(unsigned error_code);

// This is marked with "C" linkage because we call it from the assembly code
// ISR stubs in isr_wrapper_asm.S.
extern "C"
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

    switch (interrupt_number) {
        case IDT_KEY:
            s_keyboard->int_handler();
            break;

        case IDT_TIMER:
            s_timer->int_handler();
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
            page_fault_handler(error_code);
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

// The kernel must call global constructors itself as we have no runtime
// support beyond what we implement ourselves.
// We want to call this as early as possible after booting. However, we also
// want the constructors to be able to at least panic() on error. So, call
// this after initializing interrupts and after initializing the console.
static void call_global_constructors()
{
    extern char start_ctors[];
    extern char end_ctors[];

    for (char *ctor = start_ctors;
         ctor < end_ctors;
         ctor += sizeof(void *)) {

        // Read the address from the table, interpret as a function pointer,
        // and then execute the ctor.
        void *value = *((void **)ctor);
        void (*ctor_function_pointer)() = (void (*)())(value);
        ctor_function_pointer();
    }
}

// Returns a string interpretation of a page fault error code.
static const char* get_page_fault_error_string(unsigned error_code)
{
    switch (error_code)
    {
        case 0b000: return "supervisory process tried to read a non-present page entry";
        case 0b001: return "supervisory process tried to read a page and caused a protection fault";
        case 0b010: return "supervisory process tried to write to a non-present page entry";
        case 0b011: return "supervisory process tried to write a page and caused a protection fault";
        case 0b100: return "user process tried to read a non-present page entry";
        case 0b101: return "user process tried to read a page and caused a protection fault";
        case 0b110: return "user process tried to write to a non-present page entry";
        case 0b111: return "user process tried to write a page and caused a protection fault";
        default: return "unknown";
    }
}

static __attribute__((noreturn))
void panic_on_unrecoverable_page_fault(uintptr_t faulting_address,
                                       unsigned error_code)
{
    char message[128]={0};
    snprintf(message, sizeof(message),
             "Page Fault.\nfaulting_address: %p\nerror: %s (%u)\n",
              (void*)faulting_address,
              get_page_fault_error_string(error_code),
              error_code);
    panic(message);
}

static void page_fault_handler(unsigned error_code)
{
    uintptr_t faulting_address = (uintptr_t)get_cr2();
    panic_on_unrecoverable_page_fault(faulting_address, error_code);
}

// This is marked with "C" linkage because we call it from assembly in boot.S.
extern "C"
__attribute__((noreturn))
void kernel_main(multiboot_info_t *mb_info, uint32_t istack)
{
    // Setup the initial Task State Segment. The kernel uses one TSS between all
    // tasks and performs software task switching.
    memset(&s_tss, 0, sizeof(s_tss));
    s_tss.ss0 = SEGSEL_KERNEL_DS;
    s_tss.esp0 = istack;
    s_tss.iomap = sizeof(s_tss);

    // Setup the Global Descriptor Table. The kernel uses a flat memory map.
    memset(s_gdt, 0, sizeof(s_gdt));
    gdt_create_flat_mapping(s_gdt, sizeof(s_gdt), (uintptr_t)&s_tss);
    lgdt(s_gdt, sizeof(s_gdt) - 1);
    load_task_register();

    call_global_constructors();

    // Setup the Interrupt Descriptor Table. This wires various IRQs up to their
    // handler functions.
    isr_install(s_idt);

    // Initialize the PIC chip. It is necessary to remap IRQs before enabling
    // interrupts.
    pic_init();

    // Initialize the VGA text console output driver.
    // The driver lives in the kernel stack since we haven't initialized the
    // kernel heap yet.
    vga_text_display_device vga;
    vga.clear();

    // Initialize the text terminal, built on top of the VGA text display, and
    // make it available to other sub-systems which rely on it globally.
    text_terminal term(vga);
    g_terminal = &term;
    
    // The kernel memory allocators must be initialized after the text terminal
    // (because they print to the screen) and before anything tries to use new
    // or malloc().
    s_allocators = kernel_memory_allocators::create(mb_info, term);

    // Initialize the PS/2 keyboard driver.
    s_keyboard = new ps2_keyboard_device();

    // Configure the PIT timer chip so that it fires an interrupt every 10ms.
    s_timer = new pit_timer_device(pit_timer_device::TIMER_RATE_10ms,
                                   pit_timer_device::TIMER_LEAP_INTERVAL_10ms,
                                   pit_timer_device::TIMER_LEAP_TICKS_10ms);

    // After this point, interrupts will start firing.
    enable_interrupts();

    // Read lines of user input forever, but don't do anything with them.
    // (This operating system doesn't do much yet.)
    {
        term.puts("Entering console loop:\n");
        line_editor ed(term, *s_keyboard);
        while (true) {
            auto user_input = ed.getline();
            term.puts("Got: ");
            term.putv(user_input);
            term.puts("\n");
            ed.add_history(std::move(user_input));
        }
    }

    panic("We should never reach this point.");
    __builtin_unreachable();
}
