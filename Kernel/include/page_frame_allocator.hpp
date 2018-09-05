#pragma once

#include <common/bit_array.hpp>

// Allocates physical page frames for use in the kernel.
class page_frame_allocator {
public:
    // Constructor.
    // Use placement-new to allocate the page_frame_allocator in a suitably-
    // sized block of memory for the number of page frames in the system.
    page_frame_allocator(size_t number_of_page_frames, size_t length);

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
        bitmap.set_all();
    }

private:
    size_t count;
    bit_array bitmap; // must be last
};
