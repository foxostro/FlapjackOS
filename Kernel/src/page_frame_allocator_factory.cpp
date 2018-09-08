#include "page_frame_allocator_factory.hpp"
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
    size_t number_of_page_frames = count_page_frames();
    size_t number_of_storage_bytes = BitArray::calc_number_of_storage_bytes(number_of_page_frames);
    size_t size = sizeof(PageFrameAllocator) + number_of_storage_bytes;
    terminal_.printf("Page frame allocator will use %u bytes (%u KB)\n",
                     (unsigned)size, (unsigned)size/1024);
    void* page_frame_allocator_address = break_allocator_.malloc(size);
    char* storage = (char*)page_frame_allocator_address + sizeof(PageFrameAllocator);
    PageFrameAllocator *page_frames = new (page_frame_allocator_address) PageFrameAllocator(number_of_page_frames, number_of_storage_bytes, storage);
    
    page_frames->mark_all_as_used();
    enumerate_page_frames([&](uintptr_t page_frame){
        if (break_allocator_.is_frame_beyond_bootstrap_heap(page_frame)) {
            page_frames->deallocate(page_frame);
        }
    });

    return page_frames;
}
