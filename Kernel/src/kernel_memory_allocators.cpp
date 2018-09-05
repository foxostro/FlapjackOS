#include "kernel_memory_allocators.hpp"
#include <common/global_allocator.hpp>
#include <malloc/malloc_zone.hpp>

kernel_memory_allocators*
kernel_memory_allocators::create(multiboot_info_t *mb_info,
                                 text_terminal &terminal)
{
    using storage_type = typename std::aligned_storage<sizeof(kernel_memory_allocators), alignof(kernel_memory_allocators)>::type;
    static storage_type storage;
    return new (&storage) kernel_memory_allocators(mb_info, terminal);
}

kernel_memory_allocators::kernel_memory_allocators(multiboot_info_t *mb_info,
                                                   text_terminal &terminal)
 : mb_info_(mb_info),
   terminal_(terminal)
{
    report_installed_memory();
    initialize_kernel_break_allocator();
    prepare_kernel_memory_map();
    initialize_page_frame_allocator();
    initialize_kernel_heap_allocator();
    initialize_kernel_malloc();
    report_free_page_frames();
}

void kernel_memory_allocators::report_installed_memory()
{
    terminal_.printf("%u KB low memory, %u MB high memory\n",
                     mb_info_->mem_lower, mb_info_->mem_upper/1024);
}

void kernel_memory_allocators::report_free_page_frames()
{
    terminal_.printf("Number of free page frames is %u (%uMB)\n",
                     (unsigned)page_frame_allocator_->get_number_of_free_page_frames(),
                     (unsigned)page_frame_allocator_->get_number_of_free_page_frames()*4/1024);
}

void kernel_memory_allocators::initialize_kernel_break_allocator()
{
    kernel_break_allocator_.reset((void*)kernel_image_end);
}

void kernel_memory_allocators::initialize_page_frame_allocator()
{
    page_frame_allocator_factory factory(mb_info_,
                                         kernel_break_allocator_,
                                         terminal_);
    page_frame_allocator_ = factory.create();
}

void kernel_memory_allocators::prepare_kernel_memory_map()
{
    {
        cleanup_kernel_memory_map_operation operation;
        operation.cleanup_kernel_memory_map();
    }

    {
        kernel_page_directory_populate_operation operation(kernel_break_allocator_);
        operation.populate_kernel_page_directory();
    }
}

void kernel_memory_allocators::initialize_kernel_heap_allocator()
{
    void *memory = kernel_break_allocator_.malloc(sizeof(kernel_heap_allocator));
    kernel_heap_allocator_ = new (memory) kernel_heap_allocator(kernel_break_allocator_, *page_frame_allocator_);
}

void kernel_memory_allocators::initialize_kernel_malloc()
{
    constexpr size_t EIGHT_MB = 8 * 1024 * 1024;
    size_t length = EIGHT_MB;
    assert(length > 0);
    terminal_.printf("Malloc zone size is %u KB.", (unsigned)length/1024);
    uintptr_t begin = (uintptr_t)kernel_heap_allocator_->malloc(length);

    terminal_.puts(" Clearing... ");
    constexpr int MAGIC_NUMBER_UNINITIALIZED_HEAP = 0xCD;
    memset((void*)begin, MAGIC_NUMBER_UNINITIALIZED_HEAP, length);
    terminal_.puts("Done.\n");

    memory_allocator *alloc = malloc_zone::create(begin, length);
    set_global_allocator(alloc);
}
