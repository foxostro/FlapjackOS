#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <kernel_break_allocator.hpp>
#include <page_frame_allocator.hpp>
#include <mmu.hpp>

// Allocates memory in the kernel heap.
// The kernel heap region follows the end of the kernel image. Virtual addresses
// in this region are physically-contiguous kernel logical addresses.
class kernel_heap_allocator : public memory_allocator {
public:
    // The kernel heap region follows the end of the kernel image.
    // The bootstrap assembly procedure will have ensured that we have enough
    // physical memory mapped into the address space to setup the page frame
    // allocator and any additional page tables we might need to map the rest of
    // physical memory into the kernel memory region.
    kernel_heap_allocator(kernel_break_allocator &break_allocator,
                          page_frame_allocator &page_frames);

    // Allocates `size' bytes of memory, or returns NULL on failure.
    // The allocation may fail if it is not possible to allocate corresponding
    // physical page frames to back the allocation.
    void* malloc(size_t size) override;

    void* realloc(void *ptr, size_t new_size) override;
    void free(void *ptr) override;

private:
    kernel_break_allocator &break_allocator;
    page_frame_allocator &page_frames;

    // Returns true if we can secure page frames for an allocation.
    // For each unmapped page in the kernel heap which would be needed for an
    // allocation of a given length, check to see whether the corresponding page
    // frames are all available.
    // TODO: What if the heap collides with other allocations?
    bool can_secure_page_frames_for_allocation(size_t length);

    // Secure page frames for an allocation.
    // For each unmapped page in the kernel heap which would be needed for an
    // allocation of a given length, allocate the corresponding page frames.
    // Panics if the page frames cannot be allocated.
    void secure_page_frames_for_allocation(size_t length);

    // If unmapped, map the specified logical addresses to the corresponding
    // physical address.
    void map_unmapped_kernel_heap_memory(uintptr_t base_address, size_t length);

    // If unmapped, map the specified logical page to the corresponding
    // physical page frame.
    void map_unmapped_kernel_heap_page(uintptr_t page);

    // Returns true if the specified kernel virtual page is mapped.
    bool is_kernel_heap_page_mapped(uintptr_t page);

    // Maps the specified virtual address to the corresponding virtual address.
    // The mapping is made read-write for supervisor.
    // The physical and virtual addresses are related through the rules for
    // kernel logical addressing. This method establishes that mapping in the
    // MMU.
    void map_kernel_heap_page(uintptr_t page);
};
