#include "page_frame_allocator_factory.hpp"
#include <common/minmax.hpp>

page_frame_allocator_factory::page_frame_allocator_factory(multiboot_info_t *mb_info,
                                                           kernel_break_allocator &break_allocator,
                                                           text_terminal &terminal)
 : mb_info_(mb_info),
   break_allocator_(break_allocator),
   terminal_(terminal)
{}

size_t page_frame_allocator_factory::count_page_frames()
{
    size_t max = 0;   
    enumerate_page_frames([&](uintptr_t page_frame){
        size_t page_frame_number = page_frame/PAGE_SIZE;
        max = MAX(max, page_frame_number);
    });
    return max+1;
}

page_frame_allocator* page_frame_allocator_factory::create()
{
    size_t number_of_page_frames = count_page_frames();
    size_t number_of_storage_bytes = bit_array::calc_number_of_storage_bytes(number_of_page_frames);
    size_t size = sizeof(page_frame_allocator) + number_of_storage_bytes;
    terminal_.printf("Page frame allocator will use %u bytes (%u KB)\n",
                     (unsigned)size, (unsigned)size/1024);
    void *page_frame_allocator_address = break_allocator_.malloc(size);
    page_frame_allocator *page_frames = new (page_frame_allocator_address) page_frame_allocator(number_of_page_frames, size);
    
    page_frames->mark_all_as_used();
    enumerate_page_frames([&](uintptr_t page_frame){
        if (break_allocator_.is_frame_beyond_bootstrap_heap(page_frame)) {
            page_frames->deallocate(page_frame);
        }
    });

    return page_frames;
}
