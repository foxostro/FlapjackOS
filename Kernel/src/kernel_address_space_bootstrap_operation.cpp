#include "kernel_address_space_bootstrap_operation.hpp"
#include "logger.hpp"
#include <cstring>
#include <new>

KernelAddressSpaceBootstrapOperation::KernelAddressSpaceBootstrapOperation(KernelBreakAllocator &break_allocator)
 : break_allocator_(break_allocator),
   pd_(get_current_page_directory())
{}

void KernelAddressSpaceBootstrapOperation::prepare_address_space()
{
    TRACE("Preparing kernel address space...");

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
        configure_page_directory_entry(pde);
    }
}

void KernelAddressSpaceBootstrapOperation::configure_page_directory_entry(PageDirectoryEntry& pde)
{
    void* page_table_pointer = allocate_page_table_memory();
    ensure_page_table_is_mapped(page_table_pointer);
    set_page_table(pde, page_table_pointer);
}

void* KernelAddressSpaceBootstrapOperation::allocate_page_table_memory()
{
    break_allocator_.align_break_on_next_page_boundary();
    void* page_table_pointer = break_allocator_.malloc(sizeof(PageTable));
    return page_table_pointer;
}

void KernelAddressSpaceBootstrapOperation::ensure_page_table_is_mapped(void* page_table_pointer)
{
    uintptr_t page_table_vma = (uintptr_t)page_table_pointer;
    if (!pd_.is_address_mapped(page_table_vma)) {
        pd_.map_page(convert_logical_to_physical_address(page_table_vma),
                     page_table_vma,
                     PHYS_MAP_WRITABLE);
    }
}

void KernelAddressSpaceBootstrapOperation::set_page_table(PageDirectoryEntry& pde, void *page_table_pointer)
{
    memset(page_table_pointer, 0, sizeof(PageTable));
    PageTable* pt = new (page_table_pointer) PageTable;
    pde.set_page_table(pt);
}
