#include "page_frame_allocator.hpp"
#include "logical_addressing.hpp"
#include "panic.h"

page_frame_allocator::page_frame_allocator(size_t number_of_page_frames, size_t length)
 : count(0),
   bitmap(number_of_page_frames, length - sizeof(page_frame_allocator))
{}

bool page_frame_allocator::allocate(uintptr_t page_frame)
{
    size_t bit_index = page_frame / PAGE_SIZE;
    if (bitmap.test(bit_index)) {
        return false; // already allocated
    } else {
        assert(count>0);
        count--;
        bitmap.set(bit_index);
        return true;
    }
}

void page_frame_allocator::deallocate(uintptr_t page_frame)
{
    size_t bit_index = page_frame / PAGE_SIZE;
    if (bitmap.test(bit_index)) {
        count++;
        bitmap.clear(bit_index);
    }
}

bool page_frame_allocator::is_free(uintptr_t page_frame)
{
    size_t bit_index = page_frame / PAGE_SIZE;
    return !bitmap.test(bit_index);
}

size_t page_frame_allocator::get_number_of_free_page_frames() const
{
    return count;
}
