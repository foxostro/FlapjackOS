#pragma once

#include <mmu.hpp>
#include <kernel_break_allocator.hpp>

// Operation to intialize page tables for the kernel section of virtual memory.
// These page tables are allocated from the kernel bootstrap heap once at boot
// time. These tables are updatd as memory is allocated for the kernel.
// Kernel virtual memory mappings are shared by all tasks.
class kernel_page_directory_populate_operation {
public:
    kernel_page_directory_populate_operation(kernel_break_allocator &break_allocator);

    void populate_kernel_page_directory();

private:
    kernel_break_allocator &break_allocator;

    page_table* allocate_kernel_page_tables();
};
