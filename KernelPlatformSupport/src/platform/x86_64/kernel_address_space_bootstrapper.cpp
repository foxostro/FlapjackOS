#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <cassert>
#include <cstring> // for memset()
#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION_SIZE

namespace x86_64 {

KernelAddressSpaceBootstrapper::KernelAddressSpaceBootstrapper()
 : count_(NUMBER_OF_PAGE_TABLES),
   next_page_table_(&page_tables_[0])
{}

void KernelAddressSpaceBootstrapper::prepare_address_space_internal()
{
    memset(page_tables_, 0, sizeof(page_tables_));
    PageDirectory& pd = get_relevant_page_directory();
    resolver_.enumerate_page_directory_entries(pd, (uintptr_t)KERNEL_VIRTUAL_START_ADDR, KERNEL_MEMORY_REGION_SIZE, [&](PageDirectoryEntry& pde){
        prepare_page_directory_entry(pde);
    });
}

PageDirectory& KernelAddressSpaceBootstrapper::get_relevant_page_directory()
{
    PageDirectory* pd = resolver_.get_page_directory(KERNEL_VIRTUAL_START_ADDR);
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
        pde.set_readwrite(true);
        pde.set_present(true);
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