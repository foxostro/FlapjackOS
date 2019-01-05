#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP

#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION_SIZE

namespace i386 {

// Populate the MMU paging structures for kernel logical memory.
// Also owns the statically allocated storage for those paging structures.
//
// This object ensures the MMU paging structures are allocated in mapped memory
// such that actions to map pages in kernel logical memory do not require
// additional memory to be allocated.
//
// The hardware paging structures used to map kernel logical memory need to be
// allocated in memory themselves. If allocated in DATA or BSS, those objects
// will cause the kernel image to become larger than the limit imposed by the
// bootstrap code. So, there's a whole dance to bootstrap the address space.
template<typename PAGING_RESOLVER>
class GenericKernelAddressSpaceBootstrapper {
public:
    using PageDirectory = typename PAGING_RESOLVER::PageDirectory;
    using PageTable = typename PAGING_RESOLVER::PageTable;
    using PageDirectoryEntry = decltype(PageDirectory::entries[0]);
    using PagingResolver = PAGING_RESOLVER;

    GenericKernelAddressSpaceBootstrapper(HardwareMemoryManagementUnit &mmu)
     : count_(NUMBER_OF_PAGE_TABLES),
       next_page_table_(&page_tables_[0]),
       resolver_(mmu),
       mmu_(mmu)
    {}

    void prepare_address_space()
    {
        resolver_.set_cr3(mmu_.get_cr3());
        prepare_address_space_internal();
        mmu_.reload();
    }

public:
    static constexpr size_t NUMBER_OF_PAGE_TABLES = KERNEL_MEMORY_REGION_SIZE / PAGE_SIZE / PageTable::COUNT;
    PageTable page_tables_[NUMBER_OF_PAGE_TABLES];

private:
    size_t count_;
    PageTable* next_page_table_;
    PagingResolver resolver_;
    HardwareMemoryManagementUnit &mmu_;

    void prepare_address_space_internal()
    {
        memset(page_tables_, 0, sizeof(page_tables_));
        resolver_.enumerate_page_directory_entries(mmu_.get_kernel_virtual_start_address(), KERNEL_MEMORY_REGION_SIZE, [&](PageDirectoryEntry& pde){
            prepare_page_directory_entry(pde);
        });
    }

    void prepare_page_directory_entry(PageDirectoryEntry& pde)
    {
        auto page_table_physical_address = pde.get_address();
        if (page_table_physical_address == 0) {
            PageTable* page_table = get_next_page_table();
            auto physical_address = mmu_.convert_logical_to_physical_address((uintptr_t)page_table);
            pde.set_address(physical_address);
            pde.set_readwrite(true);
            pde.set_present(true);
        }
    }

    PageTable* get_next_page_table()
    {
        assert(count_ > 0);
        PageTable* page_table = next_page_table_;
        next_page_table_++;
        count_--;
        return page_table;
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP