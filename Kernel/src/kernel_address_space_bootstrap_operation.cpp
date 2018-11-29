#include "kernel_address_space_bootstrap_operation.hpp"
#include "logger.hpp"
#include "logical_addressing.hpp" // for KERNEL_MEMORY_REGION_SIZE
#include "panic.h"

KernelAddressSpaceBootstrapOperation::KernelAddressSpaceBootstrapOperation(size_t count, PageTable *empty_page_tables)
 : pd_(get_current_page_directory()),
   count_(count),
   next_page_table_(empty_page_tables)
{}

void KernelAddressSpaceBootstrapOperation::prepare_address_space()
{
    TRACE("Populating page directory with page tables...");
    for (size_t i = pd_.index_for_virtual_address(KERNEL_VIRTUAL_START_ADDR);
        i < PageDirectory::NUMBER_OF_PAGE_DIRECTORY_ENTRIES;
        ++i) {
        prepare_page_directory_entry(pd_.entries[i]);
    }
    TRACE("Done.");
}

void KernelAddressSpaceBootstrapOperation::prepare_page_directory_entry(PageDirectoryEntry& pde)
{
    uintptr_t page_table_physical_address = pde.get_page_table_physical_address();
    if (page_table_physical_address == 0) {
        PageTable* page_table = get_next_page_table();
        pde.set_page_table(page_table);
    }
}

PageTable* KernelAddressSpaceBootstrapOperation::get_next_page_table()
{
    assert(count_ > 0);
    PageTable* page_table = next_page_table_;
    next_page_table_++;
    count_--;
    return page_table;
}