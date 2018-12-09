#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <cassert>
#include <cstring> // for memset()

namespace x86_64 {

KernelAddressSpaceBootstrapper::KernelAddressSpaceBootstrapper()
 : count_(NUMBER_OF_PAGE_TABLES),
   next_page_table_(&page_tables_[0])
{}

void KernelAddressSpaceBootstrapper::prepare_address_space_internal()
{
    constexpr uintptr_t TWO_MEGS = 0x200000;
    memset(page_tables_, 0, sizeof(page_tables_));
    PageDirectory& pd = get_relevant_page_directory();
    for (uintptr_t linear_address = KERNEL_VIRTUAL_START_ADDR;
         linear_address < KERNEL_MEMORY_REGION_SIZE;
         linear_address += TWO_MEGS) {
        PageDirectoryEntry* pde = _resolver.get_page_directory_entry(&pd, linear_address);
        assert(pde);
        prepare_page_directory_entry(*pde);
    }
}

PageDirectory& KernelAddressSpaceBootstrapper::get_relevant_page_directory()
{
    PageDirectory* pd = _resolver.get_page_directory(KERNEL_VIRTUAL_START_ADDR);
    assert(pd && "expected a page directory for the kernel memory region");
    return *pd;
}

void KernelAddressSpaceBootstrapper::prepare_page_directory_entry(PageDirectoryEntry& pde)
{
    uintptr_t page_table_physical_address = pde.get_address();
    if (page_table_physical_address == 0) {
        PageTable* page_table = get_next_page_table();
        pde.set_address((uintptr_t)page_table);
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

} // namespace x86_64