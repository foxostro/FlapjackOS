#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP

#include <common/bit_array.hpp>

// Interface for an allocator of physical page frames.
class PageFrameAllocator {
public:
    // Allocate a contiguous span of page frames with a size (in bytes) of at
    // least the specified length.
    // Returns the physical address of the first page frame on success.
    // Returns zero if the allocation fails.
    // This method will refuse to allocate page frame zero.
    virtual uintptr_t allocate(size_t length) = 0;

    // Returns true if all page frames in the specified span are free.
    virtual bool is_span_free(uintptr_t begin_page_frame, size_t length) = 0;

    // Allocates a specific page frame.
    // Returns true if the allocation was successful, false otherwise.
    virtual bool allocate_specific_page_frame(uintptr_t page_frame) = 0;

    // Deallocates a specific page frame.
    // If the page frame was already deallocated then this does nothing.
    virtual void deallocate(uintptr_t page_frame) = 0;

    // Returns true if the specified page frame is free.
    // Also returns false if the specified page frame is out of bounds.
    virtual bool is_free(uintptr_t page_frame) = 0;

    // Return the number of free page frames remaining.
    virtual size_t get_number_of_free_page_frames() const = 0;

    // Return the capacity of the page frame allocator.
    // This is the largest number of pages which can be managed.
    virtual size_t get_capacity() const = 0;

    // Mark all page frames as being used.
    virtual void mark_all_as_used() = 0;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP
