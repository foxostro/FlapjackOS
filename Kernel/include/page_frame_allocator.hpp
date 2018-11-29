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

    // Allocate a contiguous span of page frames with a size (in bytes) of at
    // least the specified length.
    // Returns the physical address of the first page frame on success.
    // Returns zero if the allocation fails.
    // This method will refuse to allocate page frame zero.
    uintptr_t allocate_span(size_t length)
    {
        for (uintptr_t page_frame = PAGE_SIZE;
             page_frame < PAGE_SIZE*NUMBER_OF_PAGE_FRAMES;
             page_frame += PAGE_SIZE) {

            if (is_span_free(page_frame, length)) {
                uintptr_t result = page_frame;
                for (; length >= PAGE_SIZE; length -= PAGE_SIZE) {
                    allocate(page_frame);
                    page_frame += PAGE_SIZE;
                }
                return result;
            }
        }
        return 0;
    }

    bool is_span_free(uintptr_t begin_page_frame, size_t length)
    {
        assert_is_page_aligned(begin_page_frame);

        if (length == 0) {
            return is_free(begin_page_frame);
        }

        for (uintptr_t page_frame = begin_page_frame,
                       limit = begin_page_frame + length;
             page_frame < limit;
             page_frame += PAGE_SIZE) {
            if (!is_free(page_frame)) {
                return false;
            }
        }

        return true;
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
    // Also returns false if the specified page frame is out of bounds.
    bool is_free(uintptr_t page_frame)
    {
        size_t bit_index = page_frame / PAGE_SIZE;
        if (bit_index < get_capacity()) {
            return !bitmap_.test(bit_index);
        } else {
            return false;
        }
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
