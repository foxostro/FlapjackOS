#include "kernel_memory_allocators.hpp"
#include <common/global_allocator.hpp>
#include <malloc/malloc_zone.hpp>

KernelMemoryAllocators*
KernelMemoryAllocators::create(multiboot_info_t *mb_info,
                               TextTerminal &terminal)
{
    using storage_type = typename std::aligned_storage<sizeof(KernelMemoryAllocators), alignof(KernelMemoryAllocators)>::type;
    static storage_type storage;
    return new (&storage) KernelMemoryAllocators(mb_info, terminal);
}

KernelMemoryAllocators::KernelMemoryAllocators(multiboot_info_t *mb_info,
                                               TextTerminal &terminal)
 : mb_info_(mb_info),
   terminal_(terminal)
{
    report_installed_memory();
    initialize_kernel_break_allocator();
    prepare_kernel_memory_map();
    initialize_page_frame_allocator();
    initialize_contiguous_memory_allocator();
    initialize_kernel_malloc();
    report_free_page_frames();
}

void KernelMemoryAllocators::report_installed_memory()
{
    terminal_.printf("%u KB low memory, %u MB high memory\n",
                     mb_info_->mem_lower, mb_info_->mem_upper/1024);
}

void KernelMemoryAllocators::report_free_page_frames()
{
    terminal_.printf("Number of free page frames is %u (%uMB)\n",
                     (unsigned)page_frame_allocator_->get_number_of_free_page_frames(),
                     (unsigned)page_frame_allocator_->get_number_of_free_page_frames()*4/1024);
}

void KernelMemoryAllocators::initialize_kernel_break_allocator()
{
    kernel_break_allocator_.reset((void*)g_kernel_image_end);
}

void KernelMemoryAllocators::initialize_page_frame_allocator()
{
    PageFrameAllocatorFactory factory(mb_info_,
                                      kernel_break_allocator_,
                                      terminal_);
    page_frame_allocator_ = factory.create();
}

void KernelMemoryAllocators::prepare_kernel_memory_map()
{
    {
        CleanupKernelMemoryMapOperation operation;
        operation.cleanup_kernel_memory_map();
    }

    {
        KernelPageDirectoryPopulateOperation operation(kernel_break_allocator_);
        operation.populate_kernel_page_directory();
    }
}

void KernelMemoryAllocators::initialize_contiguous_memory_allocator()
{
    void *memory = kernel_break_allocator_.malloc(sizeof(KernelContiguousMemoryAllocator));
    contiguous_memory_allocator_ = new (memory) KernelContiguousMemoryAllocator(kernel_break_allocator_, *page_frame_allocator_);
}

void KernelMemoryAllocators::initialize_kernel_malloc()
{
    constexpr size_t EIGHT_MB = 8 * 1024 * 1024;
    size_t length = EIGHT_MB;
    assert(length > 0);
    terminal_.printf("Malloc zone size is %u KB.", (unsigned)length/1024);
    uintptr_t begin = (uintptr_t)contiguous_memory_allocator_->malloc(length);

    terminal_.puts(" Clearing... ");
    constexpr int MAGIC_NUMBER_UNINITIALIZED_HEAP = 0xCD;
    memset((void*)begin, MAGIC_NUMBER_UNINITIALIZED_HEAP, length);
    terminal_.puts("Done.\n");

    MemoryAllocator *alloc = MallocZone::create(begin, length);
    set_global_allocator(alloc);
}
