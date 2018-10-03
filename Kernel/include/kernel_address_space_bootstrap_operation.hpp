#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_ADDRESS_SPACE_BOOTSTRAP_OPERATION_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_ADDRESS_SPACE_BOOTSTRAP_OPERATION_HPP

#include <mmu.hpp>
#include <kernel_break_allocator.hpp>

// Operation to populate the MMU paging structures for kernel logical memory.
//
// This operation ensures the MMU paging structures are allocated in mapped
// memory such that actions to map pages in kernel logical memory do not require
// additional memory to be allocated.
//
// The hardware paging structures used to map kernel logical memory need to be
// allocated in memory themselves. If allocated in DATA or BSS, those objects
// will cause the kernel image to become larger than the 3MB limit imposed by
// the bootstrap code. So, there's a whole dance to bootstrap the address space.
class KernelAddressSpaceBootstrapOperation {
public:
    KernelAddressSpaceBootstrapOperation(KernelBreakAllocator &break_allocator);

    void prepare_address_space();

private:
    KernelBreakAllocator& break_allocator_;
    PageDirectory& pd_;

    void prepare_page_directory_entry(PageDirectoryEntry& pde);
    void configure_page_directory_entry(PageDirectoryEntry& pde);
    void* allocate_page_table_memory();
    void ensure_page_table_is_mapped(void* page_table_pointer);
    void set_page_table(PageDirectoryEntry& pde, void *page_table_pointer);
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_ADDRESS_SPACE_BOOTSTRAP_OPERATION_HPP
