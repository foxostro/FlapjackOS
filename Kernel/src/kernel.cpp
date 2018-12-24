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

static char g_stack_a[PAGE_SIZE] __attribute__ ((aligned(4))) = {0};
static char* g_stack_pointer_a = g_stack_a + sizeof(g_stack_a);

static char g_stack_b[PAGE_SIZE] __attribute__ ((aligned(4))) = {0};
static char* g_stack_pointer_b = g_stack_b + sizeof(g_stack_b);

template<typename T>
static void push(char*& stack_pointer, T value)
{
	stack_pointer -= sizeof(value);
    memcpy(stack_pointer, &value, sizeof(value));
}

static void setup_thread(char*& stack_pointer,
                         uintptr_t instruction_pointer)
{
    push(stack_pointer, /*EIP=*/instruction_pointer);
    char* EBP = stack_pointer - sizeof(EBP);
	push(stack_pointer, /*EBP=*/EBP);
    char* ESP = stack_pointer;
	push(stack_pointer, /*POPA, EAX=*/0xcdcdcdcd);
	push(stack_pointer, /*POPA, ECX=*/0xcdcdcdcd);
	push(stack_pointer, /*POPA, EDX=*/0xcdcdcdcd);
	push(stack_pointer, /*POPA, EBX=*/0xcdcdcdcd);
	push(stack_pointer, /*POPA, ESP=*/ESP);
	push(stack_pointer, /*POPA, EBP=*/EBP);
	push(stack_pointer, /*POPA, ESI=*/0xcdcdcdcd);
	push(stack_pointer, /*POPA, EDI=*/0xcdcdcdcd);
}

extern "C"
void context_switch(char** old_stack_pointer,
                    char* new_stack_pointer); // implemented in context_switch.S

static void thread_a()
{
    while (true) {
        g_terminal->putchar('a');
        context_switch(&g_stack_pointer_a, g_stack_pointer_b);
    }
}

static void thread_b()
{
    while (true) {
        g_terminal->putchar('b');
        context_switch(&g_stack_pointer_b, g_stack_pointer_a);
    }
}

void Kernel::run()
{
    TRACE("Running...");
	setup_thread(g_stack_pointer_a, (uintptr_t)thread_a);
	setup_thread(g_stack_pointer_b, (uintptr_t)thread_b);
    char* old_stack_pointer = nullptr;
    context_switch(&old_stack_pointer, g_stack_pointer_a);
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
