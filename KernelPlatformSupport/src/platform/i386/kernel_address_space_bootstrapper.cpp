#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <cassert>
#include <cstring> // for memset()
#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION_SIZE

namespace i386 {

KernelAddressSpaceBootstrapper::KernelAddressSpaceBootstrapper()
 : count_(NUMBER_OF_PAGE_TABLES),
   next_page_table_(&page_tables_[0])
{}

void KernelAddressSpaceBootstrapper::prepare_address_space(uint64_t cr3)
{
    // TODO: Get the current page directory from the cr3 parameter instead
    // of relying on global state via get_current_page_directory().
    PageDirectory& pd = get_current_page_directory();
    assert((uintptr_t)convert_physical_to_logical_address(cr3) == (uintptr_t)&pd);

    memset(page_tables_, 0, sizeof(page_tables_));

    // Populate the page directory with some page tables for the
    // the kernel higher-half.
    constexpr uintptr_t TWO_MEGS = 0x200000;
    uintptr_t linear_address = (uintptr_t)KERNEL_VIRTUAL_START_ADDR;
    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0;
         length -= TWO_MEGS) {

        size_t index = pd.index_for_virtual_address(linear_address);
        PageDirectoryEntry& pde = pd.entries[index];
        prepare_page_directory_entry(pde);

        linear_address += TWO_MEGS;
    }
}

void KernelAddressSpaceBootstrapper::prepare_page_directory_entry(PageDirectoryEntry& pde)
{
    uintptr_t page_table_physical_address = pde.get_page_table_physical_address();
    if (page_table_physical_address == 0) {
        PageTable* page_table = get_next_page_table();
        pde.set_page_table(page_table);
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