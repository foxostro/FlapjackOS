#include "kernel_memory_allocators.hpp"
#include <logger.hpp>
#include <common/global_allocator.hpp>
#include <malloc/malloc_zone.hpp>
#include <page_frame_allocator_configuration_operation.hpp>
#include <multiboot_memory_map_page_frame_enumerator.hpp>

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
    initialize_page_frame_allocator();
    initialize_kernel_malloc();
    report_free_page_frames();
}

void KernelMemoryAllocators::report_installed_memory()
{
    TRACE("%u KB low memory, %u MB high memory",
          mb_info_->mem_lower, mb_info_->mem_upper/1024);
    terminal_.printf("%u KB low memory, %u MB high memory\n",
                     mb_info_->mem_lower, mb_info_->mem_upper/1024);
}

void KernelMemoryAllocators::report_free_page_frames()
{
    TRACE("Number of free page frames is %u (%uMB)",
          (unsigned)page_frame_allocator_.get_number_of_free_page_frames(),
          (unsigned)page_frame_allocator_.get_number_of_free_page_frames()*4/1024);
    terminal_.printf("Number of free page frames is %u (%uMB)\n",
                     (unsigned)page_frame_allocator_.get_number_of_free_page_frames(),
                     (unsigned)page_frame_allocator_.get_number_of_free_page_frames()*4/1024);
}

void KernelMemoryAllocators::initialize_kernel_break_allocator()
{
    kernel_break_allocator_.reset((void*)g_kernel_image_end);
}

void KernelMemoryAllocators::initialize_page_frame_allocator()
{
    terminal_.printf("Page frame allocator will use %u bytes (%u KB)\n",
                     (unsigned)sizeof(PageFrameAllocator),
                     (unsigned)sizeof(PageFrameAllocator)/1024);

    using Op = PageFrameAllocatorConfigurationOperation<MultibootMemoryMapPageFrameEnumerator>;
    Op operation((uintptr_t)kernel_break_allocator_.get_kernel_break(),
                 MultibootMemoryMapPageFrameEnumerator(mb_info_));
    operation.configure(page_frame_allocator_);
}

void KernelMemoryAllocators::initialize_kernel_malloc()
{
    constexpr size_t EIGHT_MB = 8 * 1024 * 1024;
    size_t length = EIGHT_MB;
    assert(length > 0);
    TRACE("Malloc zone size is %u KB.", (unsigned)length/1024);
    uintptr_t begin = (uintptr_t)kernel_break_allocator_.malloc(length);

    constexpr int MAGIC_NUMBER_UNINITIALIZED_HEAP = 0xCD;
    TRACE("Clearing the malloc zone to 0x%x.", MAGIC_NUMBER_UNINITIALIZED_HEAP);
    memset((void*)begin, MAGIC_NUMBER_UNINITIALIZED_HEAP, length);
    TRACE("Finished clearing the malloc zone.");

    MemoryAllocator *alloc = MallocZone::create(begin, length);
    set_global_allocator(alloc);
}
