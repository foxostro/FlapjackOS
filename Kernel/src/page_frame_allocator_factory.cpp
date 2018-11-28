#include "page_frame_allocator_factory.hpp"
#include "logger.hpp"
#include <new>
#include <common/minmax.hpp>

PageFrameAllocatorFactory::PageFrameAllocatorFactory(multiboot_info_t *mb_info,
                                                     KernelBreakAllocator &break_allocator,
                                                     TextTerminal &terminal)
 : mb_info_(mb_info),
   break_allocator_(break_allocator),
   terminal_(terminal)
{}

size_t PageFrameAllocatorFactory::count_page_frames()
{
    size_t max = 0;   
    enumerate_page_frames([&](uintptr_t page_frame){
        size_t page_frame_number = page_frame/PAGE_SIZE;
        max = MAX(max, page_frame_number);
    });
    return max+1;
}

PageFrameAllocator* PageFrameAllocatorFactory::create()
{
    PageFrameAllocator *page_frames = create_page_frame_allocator();
    configure_page_frame_allocator(page_frames);
    return page_frames;
}

PageFrameAllocator* PageFrameAllocatorFactory::create_page_frame_allocator()
{
    size_t size = sizeof(PageFrameAllocator);
    terminal_.printf("Page frame allocator will use %u bytes (%u KB)\n",
                     (unsigned)size, (unsigned)size/1024);
    void* page_frame_allocator_address = break_allocator_.malloc(size);
    PageFrameAllocator *page_frames = new (page_frame_allocator_address) PageFrameAllocator;
    return page_frames;
}

void PageFrameAllocatorFactory::configure_page_frame_allocator(PageFrameAllocator* page_frames)
{
    // Enumerate the useable page frames of the system. Those which map to a VMA
    // beyond the kernel break pointer are available. We assume all other page
    // frames are in use, e.g., by the kernel image, or for use in paging
    // structures.
    uintptr_t break_address = (uintptr_t)break_allocator_.get_kernel_break();
    page_frames->mark_all_as_used();
    enumerate_page_frames([&](uintptr_t page_frame){
        uintptr_t corresponding_logical_address = convert_physical_to_logical_address(page_frame);
        if (corresponding_logical_address >= break_address) {
            page_frames->deallocate(page_frame);
        }
    });
}