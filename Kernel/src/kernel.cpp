#include <kernel.hpp>

#include <pit_timer_device.hpp>
#include <inout.h>
#include <panic_interrupt_handler.hpp>
#include <page_fault_interrupt_handler.hpp>
#include <logger.hpp>
#include <page_frame_allocator_configuration_operation.hpp>
#include <multiboot_memory_map_page_frame_enumerator.hpp>

#include <drivers/pc/ps2_keyboard_device.hpp>

#include <malloc/malloc_zone.hpp>

#include <common/global_allocator.hpp>
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

    call_global_constructors(); // NOTE: Kernel::Kernel() is invoked here!

    mb_info_ = mb_info;
    are_interrupts_ready_ = false;

    hardware_task_configuration_.init(istack);
    hardware_interrupt_controller_.init();
    setup_terminal();
    prepare_kernel_address_space();
    report_installed_memory();
    initialize_page_frame_allocator();
    initialize_kernel_malloc();
    report_free_page_frames();
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

void Kernel::setup_terminal()
{
    display_.clear();
    terminal_.init(&display_);
    g_terminal = &terminal_;
}

void Kernel::prepare_kernel_address_space()
{
    address_space_bootstrapper_.prepare_address_space();

    auto& phys_map = address_space_bootstrapper_.get_physical_memory_map();

    // Ensure the address space is mapped.
    uintptr_t linear_address = (uintptr_t)KERNEL_VIRTUAL_START_ADDR;
    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0; length -= PAGE_SIZE) {
        phys_map.map_page(convert_logical_to_physical_address(linear_address),
                          linear_address,
                          phys_map.WRITABLE);
        linear_address += PAGE_SIZE;
    }

    // Setup correct permissions for the .text and .rodata sections.
    phys_map.set_readonly((uintptr_t)g_kernel_text_begin,
                          (uintptr_t)g_kernel_text_end);
    phys_map.set_readonly((uintptr_t)g_kernel_rodata_begin,
                          (uintptr_t)g_kernel_rodata_end);
}

void Kernel::report_installed_memory()
{
    TRACE("%u KB low memory, %u MB high memory",
          mb_info_->mem_lower, mb_info_->mem_upper/1024);
    terminal_.printf("%u KB low memory, %u MB high memory\n",
                     mb_info_->mem_lower, mb_info_->mem_upper/1024);
}

void Kernel::report_free_page_frames()
{
    TRACE("Number of free page frames is %u (%uMB)",
          (unsigned)page_frame_allocator_.get_number_of_free_page_frames(),
          (unsigned)page_frame_allocator_.get_number_of_free_page_frames()*4/1024);
    terminal_.printf("Number of free page frames is %u (%uMB)\n",
                     (unsigned)page_frame_allocator_.get_number_of_free_page_frames(),
                     (unsigned)page_frame_allocator_.get_number_of_free_page_frames()*4/1024);
}

void Kernel::initialize_page_frame_allocator()
{
    terminal_.printf("Page frame allocator will use %u bytes (%u KB)\n",
                     (unsigned)sizeof(PageFrameAllocator),
                     (unsigned)sizeof(PageFrameAllocator)/1024);

    using Op = PageFrameAllocatorConfigurationOperation<MultibootMemoryMapPageFrameEnumerator>;
    Op operation((uintptr_t)g_kernel_image_end,
                 MultibootMemoryMapPageFrameEnumerator(mb_info_));
    operation.configure(page_frame_allocator_);
}

void Kernel::initialize_kernel_malloc()
{
    constexpr size_t EIGHT_MB = 8 * 1024 * 1024;
    size_t length = EIGHT_MB;
    TRACE("Malloc zone size is %u KB.", (unsigned)length/1024);

    uintptr_t begin_physical_addr = page_frame_allocator_.allocate_span(length);
    void* begin = (void*)convert_physical_to_logical_address(begin_physical_addr);

    constexpr int MAGIC_NUMBER_UNINITIALIZED_HEAP = 0xCD;
    TRACE("Clearing the malloc zone to 0x%x.", MAGIC_NUMBER_UNINITIALIZED_HEAP);
    memset(begin, MAGIC_NUMBER_UNINITIALIZED_HEAP, length);
    TRACE("Finished clearing the malloc zone.");

    MemoryAllocator *alloc = MallocZone::create((uintptr_t)begin, length);
    set_global_allocator(alloc);
}

void Kernel::initialize_interrupts_and_device_drivers()
{
    TRACE("Initializing device drivers.");

    PS2KeyboardDevice *keyboard_driver = new PS2KeyboardDevice();
    keyboard_ = keyboard_driver;
    
    // TODO: These interrupt numbers are platform-specific. I'd like to abstract these for an eventual port to the Raspberry Pi. How can I do that?
    // TODO: This will leak handlers.
    interrupt_dispatcher_.set_handler(IDT_KEY,   keyboard_driver);
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

    interrupt_dispatcher_.set_should_panic_on_null_handler(false);
}

void Kernel::disable_interrupts()
{
    if (are_interrupts_ready_) {
        hardware_interrupt_controller_.disable_interrupts();
    }
}

void Kernel::enable_interrupts()
{
    if (are_interrupts_ready_) {
        hardware_interrupt_controller_.enable_interrupts();
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
