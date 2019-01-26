#include <kernel.hpp>

#include <common/logger.hpp>
#include <page_frame_allocator_configuration_operation.hpp>
#include <multiboot_memory_map_page_frame_enumerator.hpp>
#include <multiboot_module_enumerator.hpp>
#include <elf_loader_factory.hpp>
#include <malloc/malloc_zone.hpp>
#include <common/global_allocator.hpp>
#include <common/line_editor.hpp>
#include <common/text_terminal.hpp>
#include <common/mutex.hpp>


const char* KernelErrorDomain = "Kernel";


Kernel::Kernel(multiboot_info_t* mb_info, uintptr_t istack)
 : mb_info_(mb_info),
   istack_(istack),
   device_drivers_(interrupt_controller_),
   terminal_(display_),
   address_space_bootstrapper_(mmu_),
   phys_map_(mmu_, page_frame_allocator_)
{
    initialize_logger();
    TRACE("Flapjack OS (%s)", get_platform());
    TRACE("mb_info=%p ; istack=0x%x", mb_info, static_cast<unsigned>(istack));
    
    Mutex::yield = ::yield;
    hardware_task_configuration_.init(istack_);
    interrupt_controller_.initialize_hardware();
    setup_terminal();
    print_welcome_message();
    prepare_kernel_address_space();
    report_installed_memory();
    initialize_page_frame_allocator();
    initialize_kernel_malloc();
    initialize_physical_memory_map();
    report_free_page_frames();
    initialize_interrupts_and_device_drivers();
}

void Kernel::initialize_logger()
{
    UniquePointer<TextOutputStream> stream{&logger_text_output_stream_};
    stream.set_should_leak();
    g_logger.set_text_output_stream(std::move(stream));
}

const char* Kernel::get_platform() const
{
    PlatformNamer namer;
    return namer.get_platform();
}

class exception
{
public:
    exception() throw() = default;
    exception(const exception&) throw() = default;
    virtual ~exception() = default;

    exception& operator=(const exception&) throw()
    {
        return *this;
    }

    virtual const char* what() const throw()
    {
        return "my exception";
    }
};

void Kernel::run()
{
    TRACE("Running...");

    try {
        TRACE("try...");
        throw exception();
    }
    catch(exception& e) {
        TRACE("caught an exception: %s", e.what());
    }
    TRACE("done with exception test");

    InterruptLock lock;
    {
        LockGuard guard{lock};
        MultibootModuleEnumerator enumerator{mmu_, mb_info_};
        assert(enumerator.has_next());
        Data elf_image = get_module_data(enumerator.get_next());
        create_elf_loader(elf_image)
        .and_then([&](SharedPointer<ElfLoader> elf_loader) -> Expected<Monostate> {
            TRACE("exec");
            unsigned result = elf_loader->exec();
            terminal_.printf("result = 0x%x\n", result);
            if (result == 0xdeadbeef) {
                return Monostate{};
            } else {
                // AFOX_TODO: It would be an improvement to add a std::string equivalent class and use that for the error description field. This way, we could format a description which includes the actual status code.
                return Error{-1, KernelErrorDomain, "Program returned an unexpected status code."};
            }
        })
        .map_error([&](Error error){
            terminal_.printf("Failed to execute program: error_code = %d ; " \
                             "error_domain = %s\n%s\n",
                             error.error_code,
                             error.error_domain,
                             error.error_description);
            return Monostate{};
        });
        
        // AFOX_TODO: There's a bug which makes it impossible to chain a call to Expected::map() after the call to map_error().
        // AFOX_TODO: It would be great to add a Expected::join() method and use it here.
        terminal_.printf("Finished executing user program\n");
    }

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

Expected<SharedPointer<ElfLoader>> Kernel::create_elf_loader(const Data& elf_image)
{
    ElfLoaderFactory factory;
    return factory.create_loader(phys_map_, elf_image);
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

    // Ensure the kernel region of memory is mapped.
    uintptr_t linear_address = mmu_.get_kernel_virtual_start_address();
    phys_map_.map_pages(linear_address,
                        linear_address + KERNEL_MEMORY_REGION_SIZE,
                        WRITABLE | GLOBAL);

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
                     (unsigned)sizeof(ConcretePageFrameAllocator),
                     (unsigned)sizeof(ConcretePageFrameAllocator)/1024);

    using Op = PageFrameAllocatorConfigurationOperation<MultibootMemoryMapPageFrameEnumerator, MultibootModuleEnumerator>;
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

    uintptr_t begin_physical_addr = page_frame_allocator_.allocate(length);
    void* begin = (void*)mmu_.convert_physical_to_logical_address(begin_physical_addr);

    constexpr int MAGIC_NUMBER_UNINITIALIZED_HEAP = 0xCD;
    TRACE("Clearing the malloc zone to 0x%x.", MAGIC_NUMBER_UNINITIALIZED_HEAP);
    memset(begin, MAGIC_NUMBER_UNINITIALIZED_HEAP, length);
    TRACE("Finished clearing the malloc zone.");

    MemoryAllocator *alloc = MallocZone::create((uintptr_t)begin, length);
    set_global_allocator(alloc);
}

void Kernel::initialize_physical_memory_map()
{
    phys_map_.heap_is_ready();
}

void Kernel::initialize_interrupts_and_device_drivers()
{
    TRACE("Initializing device drivers.");
    interrupt_controller_.setup();
    device_drivers_.init();
    interrupt_controller_.become_ready();
}
