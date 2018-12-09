#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <cassert>
#include <cstring> // for memset()
#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION_SIZE

// AFOX_TODO: Can this class and x86_64::KernelAddressSpaceBootstrapper be consolidated into one?

namespace i386 {

KernelAddressSpaceBootstrapper::KernelAddressSpaceBootstrapper()
 : count_(NUMBER_OF_PAGE_TABLES),
   next_page_table_(&page_tables_[0])
{}

void KernelAddressSpaceBootstrapper::prepare_address_space_internal()
{
    constexpr uintptr_t TWO_MEGS = 0x200000;
    memset(page_tables_, 0, sizeof(page_tables_));
    PageDirectory& pd = get_relevant_page_directory();

    uintptr_t linear_address = (uintptr_t)KERNEL_VIRTUAL_START_ADDR;
    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0;
         length -= TWO_MEGS) {

        PageDirectoryEntry* pde = _resolver.get_page_directory_entry(&pd, linear_address);
        assert(pde);
        prepare_page_directory_entry(*pde);

        linear_address += TWO_MEGS;
    }
}

PageDirectory& KernelAddressSpaceBootstrapper::get_relevant_page_directory()
{
    PageDirectory* pd = _resolver.get_page_directory();
    assert(pd && "expected a page directory for the kernel memory region");
    return *pd;
}

void KernelAddressSpaceBootstrapper::prepare_page_directory_entry(PageDirectoryEntry& pde)
{
    uintptr_t page_table_physical_address = pde.get_address();
    if (page_table_physical_address == 0) {
        PageTable* page_table = get_next_page_table();
        uintptr_t physical_address = convert_logical_to_physical_address((uintptr_t)page_table);
        pde.set_address(physical_address);
    }
}

PageTable* KernelAddressSpaceBootstrapper::get_next_page_table()
{
    assert(count_ > 0);
    PageTable* page_table = next_page_table_;
    next_page_table_++;
    count_--;
    return page_table;
}

} // namespace i386