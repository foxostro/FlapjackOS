#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP

#include <common/bit_array.hpp>
#include "page_size.hpp"
#include "panic.h"
#include "logical_addressing.hpp" // for KERNEL_MEMORY_REGION_SIZE

// Allocates physical page frames for use in the kernel.
template<size_t NUMBER_OF_PAGE_FRAMES>
class PageFrameAllocator_ {
public:
    // Initially, all page frames are marked as "in-use".
    // Page frames must be specifically deallocated before use.
    PageFrameAllocator_() : count_(0)
    {
        mark_all_as_used();
    }

    // Allocates a specific page frame.
    // Returns true if the allocation was successful, false otherwise.
    bool allocate(uintptr_t page_frame)
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

    // Deallocates a specific page frame.
    // If the page frame was already deallocated then this does nothing.
    void deallocate(uintptr_t page_frame)
    {
        size_t bit_index = page_frame / PAGE_SIZE;
        if (bitmap_.test(bit_index)) {
            count_++;
            bitmap_.clear(bit_index);
        }
    }

    // Returns true if the specified page frame is free.
    bool is_free(uintptr_t page_frame)
    {
        size_t bit_index = page_frame / PAGE_SIZE;
        return !bitmap_.test(bit_index);
    }

    // Return the number of free page frames remaining.
    size_t get_number_of_free_page_frames() const
    {
        return count_;
    }

    // Return the capacity of the page frame allocator.
    // This is the largest number of pages which can be managed.
    constexpr size_t get_capacity()
    {
        return NUMBER_OF_PAGE_FRAMES;
    }

    // Mark all page frames as being used.
    void mark_all_as_used()
    {
        count_ = 0;
        bitmap_.set_all();
    }

private:
    size_t count_;
    BitArray<NUMBER_OF_PAGE_FRAMES> bitmap_;
};

// The page frame allocator can manage 1GB of memory. No more.
using PageFrameAllocator = PageFrameAllocator_<KERNEL_MEMORY_REGION_SIZE/PAGE_SIZE>;

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP
