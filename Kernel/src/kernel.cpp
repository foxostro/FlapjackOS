#include <kernel.hpp>

#include <inout.h>
#include <logger.hpp>
#include <page_frame_allocator_configuration_operation.hpp>
#include <multiboot_memory_map_page_frame_enumerator.hpp>
#include <malloc/malloc_zone.hpp>
#include <common/global_allocator.hpp>
#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>


// We pass the terminal around globally because it severely clutters the
// interface of assert() and panic() if we are reqired to pass the terminal
// around literally everywhere.
TextTerminal *g_terminal = nullptr;


Kernel::Kernel(multiboot_info_t *mb_info, uintptr_t istack)
 : mb_info_(mb_info),
   istack_(istack),
   device_drivers_(interrupt_controller_),
   address_space_bootstrapper_(mmu_),
   phys_map_(mmu_)
{
    TRACE("Flapjack OS (%s)", get_platform());
    TRACE("mb_info=%p ; istack=0x%x", mb_info, istack);
    
    hardware_task_configuration_.init(istack_);
    interrupt_controller_.initialize_hardware();
    setup_terminal();
    print_welcome_message();
    prepare_kernel_address_space();
    report_installed_memory();
    initialize_page_frame_allocator();
    initialize_kernel_malloc();
    report_free_page_frames();
    initialize_interrupts_and_device_drivers();
}

const char* Kernel::get_platform() const
{
    static const char platform[] = 
#if defined(__x86_64__)
    "x86_64"
#elif defined(__i386__)
    "i386"
#elif defined(__arm__)
    "ARM"
#else
    "unknown"
#endif
    ;
    return platform;
}

extern "C"
void context_switch(char** old_stack_pointer,
                    char* new_stack_pointer); // implemented in context_switch.S

class Thread {
public:
    Thread(void* instruction_pointer)
    {
        memset(stack_, 0, sizeof(stack_));
        stack_pointer_ = stack_ + sizeof(stack_);

        push(/*EIP=*/reinterpret_cast<uintptr_t>(instruction_pointer));
        char* EBP = stack_pointer_ - sizeof(EBP);
        push(/*EBP=*/EBP);
        char* ESP = stack_pointer_;
        push(/*POPA, EAX=*/InitialRegisterValue);
        push(/*POPA, ECX=*/InitialRegisterValue);
        push(/*POPA, EDX=*/InitialRegisterValue);
        push(/*POPA, EBX=*/InitialRegisterValue);
        push(/*POPA, ESP=*/ESP);
        push(/*POPA, EBP=*/EBP);
        push(/*POPA, ESI=*/InitialRegisterValue);
        push(/*POPA, EDI=*/InitialRegisterValue);
    }

    void context_switch()
    {
        char* old_stack_pointer = nullptr;
        ::context_switch(&old_stack_pointer, stack_pointer_);
    }

    void context_switch(Thread& next)
    {
        ::context_switch(&stack_pointer_, next.stack_pointer_);
    }

private:
    static constexpr uint32_t InitialRegisterValue = 0xcdcdcdcd;
    static constexpr size_t StackSize = PAGE_SIZE;
    alignas(4) char stack_[StackSize];
    char* stack_pointer_;

    template<typename T>
    void push(T value)
    {
        stack_pointer_ -= sizeof(value);
        memcpy(stack_pointer_, &value, sizeof(value));
    }
};

static void fn_a();
static void fn_b();

Thread g_thread_a((void*)fn_a);
Thread g_thread_b((void*)fn_b);

static void fn_a()
{
    while (true) {
        g_terminal->puts("a\n");
        g_thread_a.context_switch(g_thread_b);
    }
}

static void fn_b()
{
    while (true) {
        g_terminal->puts("b\n");
        g_thread_b.context_switch(g_thread_a);
    }
}

void Kernel::run()
{
    TRACE("Running...");
    g_thread_a.context_switch();
}

void Kernel::setup_terminal()
{
    display_.clear();
    terminal_.init(&display_);
    g_terminal = &terminal_;
}

void Kernel::print_welcome_message()
{
    terminal_.printf("Flapjack OS (%s)\n", get_platform());
}

void Kernel::prepare_kernel_address_space()
{
    address_space_bootstrapper_.prepare_address_space();
    phys_map_.reload();

    // Ensure the address space is mapped.
    uintptr_t linear_address = mmu_.get_kernel_virtual_start_address();

    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0;
         length -= PAGE_SIZE) {

        phys_map_.map_page(mmu_.convert_logical_to_physical_address(linear_address),
                           linear_address,
                           phys_map_.WRITABLE | phys_map_.GLOBAL);

        linear_address += PAGE_SIZE;
    }

    // Setup correct permissions for the .text and .rodata sections.
    phys_map_.set_readonly((uintptr_t)g_kernel_text_begin,
                           (uintptr_t)g_kernel_text_end);
    phys_map_.set_readonly((uintptr_t)g_kernel_rodata_begin,
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

    using Op = PageFrameAllocatorConfigurationOperation<MultibootMemoryMapPageFrameEnumerator, HardwareMemoryManagementUnit>;
    Op operation((uintptr_t)g_kernel_image_end,
                 MultibootMemoryMapPageFrameEnumerator(mmu_, mb_info_),
                 mmu_);
    operation.configure(page_frame_allocator_);
}

void Kernel::initialize_kernel_malloc()
{
    constexpr size_t EIGHT_MB = 8 * 1024 * 1024;
    size_t length = EIGHT_MB;
    TRACE("Malloc zone size is %u KB.", (unsigned)length/1024);

    uintptr_t begin_physical_addr = page_frame_allocator_.allocate_span(length);
    void* begin = (void*)mmu_.convert_physical_to_logical_address(begin_physical_addr);

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
    interrupt_controller_.setup();
    device_drivers_.init();
    interrupt_controller_.become_ready();
}
