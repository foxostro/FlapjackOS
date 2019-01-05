#ifndef FLAPJACKOS_KERNEL_INCLUDE_CONCRETE_PAGE_FRAME_ALLOCATOR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_CONCRETE_PAGE_FRAME_ALLOCATOR_HPP

#include <page_frame_allocator.hpp>
#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION_SIZE
#include "page_size.hpp"
#include "panic.h"

// Allocates physical page frames for use in the kernel.
template<size_t NUMBER_OF_PAGE_FRAMES>
class GenericConcretePageFrameAllocator final : public PageFrameAllocator {
public:
    // Initially, all page frames are marked as "in-use".
    // Page frames must be specifically deallocated before use.
    GenericConcretePageFrameAllocator() : count_(0)
    {
        mark_all_as_used();
    }

    uintptr_t allocate(size_t length) override
    {
        for (uintptr_t page_frame = PAGE_SIZE;
             page_frame < PAGE_SIZE*NUMBER_OF_PAGE_FRAMES;
             page_frame += PAGE_SIZE) {

            if (is_span_free(page_frame, length)) {
                uintptr_t result = page_frame;
                for (; length >= PAGE_SIZE; length -= PAGE_SIZE) {
                    allocate_specific_page_frame(page_frame);
                    page_frame += PAGE_SIZE;
                }
                return result;
            }
        }
        return 0;
    }

    bool is_span_free(uintptr_t begin_page_frame, size_t length) override
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

    bool allocate_specific_page_frame(uintptr_t page_frame) override
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
    
    void deallocate(uintptr_t page_frame) override
    {
        size_t bit_index = page_frame / PAGE_SIZE;
        if (bitmap_.test(bit_index)) {
            count_++;
            bitmap_.clear(bit_index);
        }
    }
    
    bool is_free(uintptr_t page_frame) override
    {
        size_t bit_index = page_frame / PAGE_SIZE;
        if (bit_index < get_capacity()) {
            return !bitmap_.test(bit_index);
        } else {
            return false;
        }
    }

    size_t get_number_of_free_page_frames() const override
    {
        return count_;
    }

    size_t get_capacity() const override
    {
        return NUMBER_OF_PAGE_FRAMES;
    }

    void mark_all_as_used() override
    {
        count_ = 0;
        bitmap_.set_all();
    }

private:
    size_t count_;
    BitArray<NUMBER_OF_PAGE_FRAMES> bitmap_;
};

// The page frame allocator can manage a fixed amount of kernel memory.
using ConcretePageFrameAllocator = GenericConcretePageFrameAllocator<KERNEL_MEMORY_REGION_SIZE/PAGE_SIZE>;

#endif // FLAPJACKOS_KERNEL_INCLUDE_CONCRETE_PAGE_FRAME_ALLOCATOR_HPP
