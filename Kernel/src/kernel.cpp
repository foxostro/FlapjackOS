#include <kernel.hpp>

#include <inout.h>
#include <logger.hpp>
#include <page_frame_allocator_configuration_operation.hpp>
#include <multiboot_memory_map_page_frame_enumerator.hpp>
#include <multiboot_module_enumerator.hpp>
#include <malloc/malloc_zone.hpp>
#include <common/global_allocator.hpp>
#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>
#include <common/mutex.hpp>


Kernel::Kernel(multiboot_info_t* mb_info, uintptr_t istack)
 : mb_info_(mb_info),
   istack_(istack),
   device_drivers_(interrupt_controller_),
   terminal_(display_),
   address_space_bootstrapper_(mmu_),
   phys_map_(mmu_)
{
    TRACE("Flapjack OS (%s)", get_platform());
    TRACE("mb_info=%p ; istack=0x%x", mb_info, istack);
    
    Mutex::yield = ::yield;
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
    PlatformNamer namer;
    return namer.get_platform();
}

void Kernel::run()
{
    TRACE("Running...");

    MultibootModuleEnumerator enumerator{mmu_, mb_info_};

    Data mod_data = get_module_data(enumerator.get_next());

    elf32::Parser32 parser{mod_data.length, mod_data.bytes};
    assert(parser.is_ia32());
    assert(parser.is_executable());
    assert(parser.get_number_of_program_headers() == 1);

    const elf32::Elf32_Phdr& header = parser.get_program_header(0);
    assert(elf32::SegmentType::PT_LOAD == header.p_type);

    Data segment_data;
    segment_data.bytes = mod_data.bytes + header.p_offset;
    segment_data.length = header.p_memsz;

    print_segment_data(segment_data);

    uintptr_t start_offset = parser.get_start_address() - header.p_vaddr;
    using Procedure = unsigned(*)();
    Procedure procedure = reinterpret_cast<Procedure>(segment_data.bytes + start_offset);

    unsigned result = procedure();
    terminal_.printf("result = 0x%x\n", result);
    assert(result == 0xdeadbeef);

    scheduler_.begin(new ThreadExternalStack);
    do_console_loop();
}

Data Kernel::get_module_data(multiboot_module_t& module)
{
    uintptr_t mod_start = mmu_.convert_physical_to_logical_address(module.mod_start);
    uintptr_t mod_end = mmu_.convert_physical_to_logical_address(module.mod_end);
    Data mod_data;
    mod_data.length = mod_end - mod_start + 1;
    mod_data.bytes = reinterpret_cast<unsigned char*>(mod_start);
    return mod_data;
}

Data Kernel::get_segment_data(const elf32::Elf32_Phdr& header,
                              const Data& mod_data)
{
    assert(elf32::SegmentType::PT_LOAD == header.p_type);
    Data segment_data;
    segment_data.bytes = mod_data.bytes + header.p_offset;
    segment_data.length = header.p_memsz;
    return segment_data;
}

void Kernel::print_segment_data(const Data& data)
{
    terminal_.printf("Segment Data: ");
    for (size_t i = 0; i < data.length; ++i) {
        unsigned byte = (unsigned)data.bytes[i];
        terminal_.printf(" %x", byte);
    }
    terminal_.printf("\n");
}

void Kernel::do_console_loop()
{
    // Read lines of user input forever, but don't do anything with them.
    // (This operating system doesn't do much yet.)
    terminal_.puts("\n\nEntering console loop:\n");
    KeyboardDevice& keyboard = device_drivers_.get_keyboard();
    LineEditor ed(terminal_, keyboard);
    while (true) {
        auto user_input = ed.getline();
        terminal_.puts("Got: ");
        terminal_.putv(user_input);
        terminal_.puts("\n");
        ed.add_history(std::move(user_input));
    }
}

void Kernel::setup_terminal()
{
    display_.clear();
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

    using Op = PageFrameAllocatorConfigurationOperation<MultibootMemoryMapPageFrameEnumerator, MultibootModuleEnumerator, HardwareMemoryManagementUnit>;
    Op operation((uintptr_t)g_kernel_image_end,
                 MultibootMemoryMapPageFrameEnumerator(mmu_, mb_info_),
                 MultibootModuleEnumerator(mmu_, mb_info_),
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
