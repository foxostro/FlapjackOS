#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_ADDRESS_SPACE_BOOTSTRAP_OPERATION_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_ADDRESS_SPACE_BOOTSTRAP_OPERATION_HPP

#include <mmu.hpp>
#include <type_traits> // for std::aligned_storage

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
    KernelAddressSpaceBootstrapOperation(size_t count, PageTable* empty_page_tables);

    void prepare_address_space();

private:
    PageDirectory& pd_;
    size_t count_;
    PageTable* next_page_table_;

    void prepare_page_directory_entry(PageDirectoryEntry& pde);
    PageTable* get_next_page_table();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_ADDRESS_SPACE_BOOTSTRAP_OPERATION_HPP