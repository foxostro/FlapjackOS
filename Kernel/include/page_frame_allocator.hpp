#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP

#include <common/bit_array.hpp>

// Allocates physical page frames for use in the kernel.
class PageFrameAllocator {
public:
    // Constructor.
    // The bitmap is stored in the provided buffer, but the PageFrameAllocator
    // does not gain ownership of this memory and is not responsible for freeing
    // it later.
    PageFrameAllocator(size_t number_of_page_frames,
                       size_t length,
                       char* storage);

    // Allocates a specific page frame.
    // Returns true if the allocation was successful, false otherwise.
    bool allocate(uintptr_t page_frame);

    // Deallocates a specific page frame.
    // If the page frame was already deallocated then this does nothing.
    void deallocate(uintptr_t page_frame);

    // Returns true if the specified page frame is free.
    bool is_free(uintptr_t page_frame);

    // Return the number of free page frames remaining.
    size_t get_number_of_free_page_frames() const;

    // Mark all page frames as being used.
    void mark_all_as_used()
    {
        bitmap_.set_all();
    }

private:
    size_t count_;
    BitArray bitmap_; // must be last
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_HPP
