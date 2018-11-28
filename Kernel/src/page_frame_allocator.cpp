#include "page_frame_allocator.hpp"
#include "page_size.hpp"
#include "logical_addressing.hpp"
#include "panic.h"

PageFrameAllocator::PageFrameAllocator(size_t, size_t, char*)
 : count_(0)
{}

bool PageFrameAllocator::allocate(uintptr_t page_frame)
{
    size_t bit_index = page_frame / PAGE_SIZE;
    if (bitmap_.test(bit_index)) {
        return false; // already allocated
    } else {
        assert(count_>0);
        count_--;
        bitmap_.set(bit_index);
        return true;
    }
}

void PageFrameAllocator::deallocate(uintptr_t page_frame)
{
    size_t bit_index = page_frame / PAGE_SIZE;
    if (bitmap_.test(bit_index)) {
        count_++;
        bitmap_.clear(bit_index);
    }
}

bool PageFrameAllocator::is_free(uintptr_t page_frame)
{
    size_t bit_index = page_frame / PAGE_SIZE;
    return !bitmap_.test(bit_index);
}

size_t PageFrameAllocator::get_number_of_free_page_frames() const
{
    return count_;
}
