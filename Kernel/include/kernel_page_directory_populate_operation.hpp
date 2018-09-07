#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_PAGE_DIRECTORY_POPULATE_OPERATION_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_PAGE_DIRECTORY_POPULATE_OPERATION_HPP

#include <mmu.hpp>
#include <kernel_break_allocator.hpp>

// Operation to intialize page tables for the kernel section of virtual memory.
// These page tables are allocated from the kernel bootstrap heap once at boot
// time. These tables are updatd as memory is allocated for the kernel.
// Kernel virtual memory mappings are shared by all tasks.
class KernelPageDirectoryPopulateOperation {
public:
    KernelPageDirectoryPopulateOperation(KernelBreakAllocator &break_allocator);

    void populate_kernel_page_directory();

private:
    KernelBreakAllocator &break_allocator_;

    PageTable* allocate_kernel_page_tables();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_PAGE_DIRECTORY_POPULATE_OPERATION_HPP
