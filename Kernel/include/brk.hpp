#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <common/memory_allocator.hpp>

class kernel_heap_allocator : public memory_allocator {
public:
    // The kernel heap region follows the end of the kernel image.
    // The bootstrap assembly procedure will have ensured that we have enough
    // physical memory mapped into the address space to setup the page frame
    // allocator and any additional page tables we might need to map the rest of
    // physical memory into the kernel memory region.
    void reset(void *bootstrap_heap_begin);

    void* malloc(size_t size) override;

    void* realloc(void *ptr, size_t new_size) override;

    void free(void *ptr) override;

    // Returns the number of byte remaining in the kernel's bootstrap heap.
    // The bootstrap heap is the region of the kernel heap which was mapped into
    // virtual memory by the bootstrap assembly start code.
    size_t get_available_bootstrap_heap_bytes();

    // Gets the address of the end of the kernel's bootstrap heap.
    // The bootstrap heap is the region of the kernel heap which was mapped into
    // virtual memory by the bootstrap assembly start code.
    void* get_bootstrap_heap_end();

private:
    void* kernel_heap_begin;
    void* kernel_break;

    // Given a memory address, round up to the next page boundary.
    void* round_address_up_to_next_page(void* address);

    // Ensure the text and rodata sections are marked readonly.
    // The assembly boot code was sloppy and marked these as writable.
    void cleanup_kernel_memory_map();

    // Modify the kernel page tables to mark the page associated with the given
    // virtual address as readonly.
    // The address must be in the kernel memory region.
    void mark_page_readonly(void* virtual_address);

    // Allocate page tables from kernel heap and use them to populate the kernel
    // page directory. The bootstrap page tables have been configured to ensure
    // that enough memory is mapped into the kernel virtual memory space to
    // provide the 4MB of page tables that we need to do this.
    // NOTE: This won't work correctly when using a kernel virtual memory region
    // that is larger than 1GB.
    void populate_kernel_page_directory();

    // Increase or decrease the kernel break pointer by the specified amount.
    // Returns a pointer to the base of the new memory allocation.
    // The kernel break pointer is the address of the end of the kernel heap. This
    // address must be greater than the beginning of the kernel heap.
    // Specifying an increment of zero causes this to return the value of the
    // current break pointer.
    void* increment_break_pointer(intptr_t increment);

    // If unmapped, map the specified logical addresses to the corresponding
    // physical address.
    void map_unmapped_kernel_heap_memory(uintptr_t base_address, size_t length);

    // If unmapped, map the specified logical page to the corresponding
    // physical page frame.
    void map_unmapped_kernel_heap_page(uintptr_t page);

    // Returns true if the specified kernel virtual page is mapped.
    bool is_kernel_heap_page_mapped(uintptr_t virtual_address);

    // Maps the specified physical address to the specified virtual address.
    // The mapping is made read-write for supervisor.
    void map_page(uintptr_t physical_address, uintptr_t virtual_address);
};
