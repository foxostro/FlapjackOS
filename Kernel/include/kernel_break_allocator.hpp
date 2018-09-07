#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_BREAK_ALLOCATOR_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_BREAK_ALLOCATOR_HPP

#include <cstddef>
#include <cstdint>
#include <common/memory_allocator.hpp>

// Allocates memory in the kernel bootstrap heap.
// The kernel heap region follows the end of the kernel image. Virtual addresses
// in this region are physically-contiguous kernel logical addresses.
class KernelBreakAllocator : public MemoryAllocator {
public:
    void reset(void *bootstrap_heap_begin);
    void* malloc(size_t size) override;
    void* realloc(void *ptr, size_t new_size) override;
    void free(void *ptr) override;

    // Move the break pointer forward to the next page boundary.
    void align_break_on_next_page_boundary();

    // Gets the address of the end of the kernel's bootstrap heap.
    // The bootstrap heap is the region of the kernel heap which was mapped into
    // virtual memory by the bootstrap assembly start code.
    void* get_bootstrap_heap_end();

    // Gets the current break pointer.
    void* get_kernel_break();

    // Returns true if the page frame is beyond the bootstrap heap.
    bool is_frame_beyond_bootstrap_heap(uintptr_t page_frame);

private:
    void* kernel_heap_begin_;
    void* kernel_break_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_BREAK_ALLOCATOR_HPP
