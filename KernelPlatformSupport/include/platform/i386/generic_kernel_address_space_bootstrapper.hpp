#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GENERIC_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GENERIC_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP

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
template<typename PagingResolver>
class GenericKernelAddressSpaceBootstrapper {
public:
    using PageDirectory = typename PagingResolver::PageDirectory;
    using PageTable = typename PagingResolver::PageTable;
    using PageDirectoryEntry = decltype(PageDirectory::entries[0]);

    GenericKernelAddressSpaceBootstrapper()
     : count_(NUMBER_OF_PAGE_TABLES),
       next_page_table_(&page_tables_[0])
    {}

    template<typename MMU>
    void prepare_address_space(MMU &mmu)
    {
        resolver_.set_cr3(mmu.get_cr3());
        prepare_address_space_internal();
        mmu.reload();
    }
    
private:
    static constexpr size_t NUMBER_OF_PAGE_TABLES = KERNEL_MEMORY_REGION_SIZE / PAGE_SIZE / PageTable::COUNT;
    PageTable page_tables_[NUMBER_OF_PAGE_TABLES];
    size_t count_;
    PageTable* next_page_table_;
    PagingResolver resolver_;

    void prepare_address_space_internal()
    {
        memset(page_tables_, 0, sizeof(page_tables_));
        PageDirectory& pd = get_relevant_page_directory();
        resolver_.enumerate_page_directory_entries(pd, (uintptr_t)KERNEL_VIRTUAL_START_ADDR, KERNEL_MEMORY_REGION_SIZE, [&](PageDirectoryEntry& pde){
            prepare_page_directory_entry(pde);
        });
    }

    PageDirectory& get_relevant_page_directory()
    {
        PageDirectory* pd = resolver_.get_page_directory(KERNEL_VIRTUAL_START_ADDR);
        assert(pd && "expected a page directory for the kernel memory region");
        return *pd;
    }

    void prepare_page_directory_entry(PageDirectoryEntry& pde)
    {
        uintptr_t page_table_physical_address = pde.get_address();
        if (page_table_physical_address == 0) {
            PageTable* page_table = get_next_page_table();
            uintptr_t physical_address = convert_logical_to_physical_address((uintptr_t)page_table);
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

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_GENERIC_KERNEL_ADDRESS_SPACE_BOOTSTRAPPER_HPP