#include <kernel.hpp>

#include <seg.h>
#include <ltr.h>
#include <interrupt_asm.h>
#include <isr_install.h>
#include <pic.h>
#include <inout.h>
#include <panic_interrupt_handler.hpp>
#include <page_fault_interrupt_handler.hpp>
#include <logger.hpp>

#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>


// We pass the terminal around globally because it severely clutters the
// interface of assert() and panic() if we are reqired to pass the terminal
// around literally everywhere.
TextTerminal *g_terminal = nullptr;

// Some things need direct access to the kernel object to manipulate the system.
// There is only one kernel. So, it's exposed as a global.
Kernel g_kernel;


void Kernel::init(multiboot_info_t *mb_info, uintptr_t istack)
{
    TRACE("mb_info=%p ; istack=0x%x", mb_info, istack);

    are_interrupts_ready_ = false;

    initialize_tss_and_gdt(istack);
    isr_install(idt_);
    pic_init();
    call_global_constructors();
    vga_.clear();
    terminal_.init(&vga_);
    g_terminal = &terminal_;
    allocators_ = KernelMemoryAllocators::create(mb_info, terminal_);
    initialize_interrupts_and_device_drivers();

    are_interrupts_ready_ = true;
    enable_interrupts();
}

void Kernel::run()
{
    TRACE("Running...");

    // Read lines of user input forever, but don't do anything with them.
    // (This operating system doesn't do much yet.)
    terminal_.puts("Entering console loop:\n");
    LineEditor ed(terminal_, *keyboard_);
    while (true) {
        auto user_input = ed.getline();
        terminal_.puts("Got: ");
        terminal_.putv(user_input);
        terminal_.puts("\n");
        ed.add_history(std::move(user_input));
    }
}

void Kernel::initialize_tss_and_gdt(uint32_t istack)
{
    memset(&tss_, 0, sizeof(tss_));
    tss_.ss0 = SEGSEL_KERNEL_DS;
    tss_.esp0 = istack;
    tss_.iomap = sizeof(tss_);

    memset(gdt_, 0, sizeof(gdt_));
    gdt_create_flat_mapping(gdt_, sizeof(gdt_), (uintptr_t)&tss_);
    lgdt(gdt_, sizeof(gdt_) - 1);

    load_task_register();
}

void Kernel::call_global_constructors()
{
    for (char *ctor = g_start_ctors;
         ctor < g_end_ctors;
         ctor += sizeof(void *)) {

        // Read the address from the table, interpret as a function pointer,
        // and then execute the ctor.
        void *value = *((void **)ctor);
        void (*ctor_function_pointer)() = (void (*)())(value);
        ctor_function_pointer();
    }
}

void Kernel::initialize_interrupts_and_device_drivers()
{
    TRACE("Initializing device drivers.");

    keyboard_ = new PS2KeyboardDevice();
    
    // TODO: This will leak handlers.
    interrupt_dispatcher_.set_handler(IDT_KEY,   keyboard_);
    interrupt_dispatcher_.set_handler(IDT_TIMER, new PITTimerDevice(PITTimerDevice::TIMER_RATE_10ms,
                                                                    PITTimerDevice::TIMER_LEAP_INTERVAL_10ms,
                                                                    PITTimerDevice::TIMER_LEAP_TICKS_10ms));
    interrupt_dispatcher_.set_handler(IDT_DE,    new PanicInterruptHandler("Division Error", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_DB,    new PanicInterruptHandler("Debug Exception", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_NMI,   new PanicInterruptHandler("Non-Maskable Interrupt", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_BP,    new PanicInterruptHandler("Breakpoint", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_OF,    new PanicInterruptHandler("Overflow", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_BR,    new PanicInterruptHandler("BOUND Range exceeded", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_UD,    new PanicInterruptHandler("Undefined Opcode", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_NM,    new PanicInterruptHandler("No Math coprocessor", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_DF,    new PanicInterruptHandler("Double Fault.", /* error_code_present = */ true));
    interrupt_dispatcher_.set_handler(IDT_CSO,   new PanicInterruptHandler("Coprocessor Segment Overrun", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_TS,    new PanicInterruptHandler("Invalid Task Segment Selector.", /* error_code_present = */ true));
    interrupt_dispatcher_.set_handler(IDT_NP,    new PanicInterruptHandler("Segment Not Present.", /* error_code_present = */ true));
    interrupt_dispatcher_.set_handler(IDT_SS,    new PanicInterruptHandler("Stack Segment Fault.", /* error_code_present = */ true));
    interrupt_dispatcher_.set_handler(IDT_GP,    new PanicInterruptHandler("General Protection Fault.", /* error_code_present = */ true));
    interrupt_dispatcher_.set_handler(IDT_PF,    new PageFaultInterruptHandler());
    interrupt_dispatcher_.set_handler(IDT_MF,    new PanicInterruptHandler("X87 Math Fault", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_AC,    new PanicInterruptHandler("Alignment Check", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_MC,    new PanicInterruptHandler("Machine Check", /* error_code_present = */ false));
    interrupt_dispatcher_.set_handler(IDT_XF,    new PanicInterruptHandler("SSE Floating Point Exception", /* error_code_present = */ false));
}

void Kernel::disable_interrupts()
{
    if (are_interrupts_ready_) {
        ::disable_interrupts();
    }
}

void Kernel::enable_interrupts()
{
    if (are_interrupts_ready_) {
        ::enable_interrupts();
    }
}

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
    const InterruptDispatcher::Params params = {
        edi, esi, ebp, esp, ebx, edx, ecx, eax, error_code, eip
    };
    g_kernel.dispatch_interrupt(interrupt_number, params);
}

// This is marked with "C" linkage because we call it from assembly in boot.S.
extern "C" __attribute__((noreturn))
void kernel_main(multiboot_info_t *mb_info, uintptr_t istack)
{
#ifdef __x86_64__
    panic("Hello, World (x86_64)");
#endif

    g_kernel.init(mb_info, istack);
    g_kernel.run();
    panic("We should never reach this point.");
    __builtin_unreachable();
}
