#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <common/memory_allocator.hpp>

struct page_table;

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
    void reset(void *bootstrap_heap_begin);

    // Allocates `size' bytes of memory, or returns NULL on failure.
    // The allocation may fail if it is not possible to allocate corresponding
    // physical page frames to back the allocation.
    void* malloc(size_t size) override;

    // Tries to change the size of the allocation pointed to by ptr to size,
    // and returns the address of the new allocation. This may move the
    // allocation to a different part of the heap, copying as much of the old
    // data pointed to by ptr as will fit in the new allocation, and freeing the
    // old allocation.
    // 
    // When extending an allocation, no guarantee is made as to the value of
    // memory in the extended region.
    // 
    // If ptr is NULL then realloc() is identical to a call to malloc().
    // 
    // If size is zero and ptr is not NULL, a new minimum-sized object is
    // allocated and the original object is freed.
    void* realloc(void *ptr, size_t new_size) override;

    // Deallocates a memory allocation pointed to be ptr. If ptr is NULL then
    // no operation is performed.
    void free(void *ptr) override;

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
    void mark_page_readonly(uintptr_t virtual_address);

    // Allocate page tables from kernel heap and use them to populate the kernel
    // page directory. The bootstrap page tables have been configured to ensure
    // that enough memory is mapped into the kernel virtual memory space to
    // provide the 4MB of page tables that we need to do this.
    // NOTE: This won't work correctly when using a kernel virtual memory region
    // that is larger than 1GB.
    void populate_kernel_page_directory();

    // Allocate page tables necessary to map the kernel virtual memory region.
    // These page tables are allocated once at init time and then are never
    // replaced, only updated.
    page_table* allocate_kernel_page_tables();

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
    bool is_kernel_heap_page_mapped(uintptr_t page);

    // Maps the specified virtual address to the corresponding virtual address.
    // The mapping is made read-write for supervisor.
    // The physical and virtual addresses are related through the rules for
    // kernel logical addressing. This method establishes that mapping in the
    // MMU.
    void map_kernel_heap_page(uintptr_t page);
};
