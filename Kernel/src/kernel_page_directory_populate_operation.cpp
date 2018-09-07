#include "kernel_page_directory_populate_operation.hpp"
#include <cstring>

KernelPageDirectoryPopulateOperation::KernelPageDirectoryPopulateOperation(KernelBreakAllocator &break_allocator)
 : break_allocator_(break_allocator)
{}

void KernelPageDirectoryPopulateOperation::populate_kernel_page_directory()
{
    PageTable *pt = allocate_kernel_page_tables();
    PageDirectory& pd = get_current_page_directory();
    size_t index_of_first_open_pde = pd.index_for_virtual_address((uintptr_t)break_allocator_.get_bootstrap_heap_end());

    for (size_t i = index_of_first_open_pde;
         i < PageDirectory::NUMBER_OF_PAGE_DIRECTORY_ENTRIES;
         ++i, ++pt) {

        PageDirectoryEntry &pde = pd.entries[i];
        pde.set_page_table(pt);
    }
}

PageTable* KernelPageDirectoryPopulateOperation::allocate_kernel_page_tables()
{
    size_t number_of_page_tables = PageDirectory::NUMBER_OF_PAGE_DIRECTORY_ENTRIES - NUMBER_OF_BOOTSTRAP_PAGE_TABLES;
    size_t page_table_allocation_size = sizeof(PageTable) * number_of_page_tables;
    break_allocator_.align_break_on_next_page_boundary();
    void* page_table_pointer = break_allocator_.malloc(page_table_allocation_size);
    uintptr_t page_table_virtual_address = (uintptr_t)page_table_pointer;
    PageTable *pt = (PageTable*)page_table_virtual_address;
    memset((void*)pt, 0, page_table_allocation_size);
    return pt;
}
