#include "kernel_page_directory_populate_operation.hpp"
#include <cstring>

kernel_page_directory_populate_operation::kernel_page_directory_populate_operation(kernel_break_allocator &break_allocator)
 : break_allocator(break_allocator)
{}

void kernel_page_directory_populate_operation::populate_kernel_page_directory()
{
    page_table *pt = allocate_kernel_page_tables();
    page_directory& pd = get_current_page_directory();
    size_t index_of_first_open_pde = pd.index_for_virtual_address((uintptr_t)break_allocator.get_bootstrap_heap_end());

    for (size_t i = index_of_first_open_pde;
         i < page_directory::NUMBER_OF_PAGE_DIRECTORY_ENTRIES;
         ++i, ++pt) {

        page_directory_entry &pde = pd.entries[i];
        pde.set_page_table(pt);
    }
}

page_table* kernel_page_directory_populate_operation::allocate_kernel_page_tables()
{
    size_t number_of_page_tables = page_directory::NUMBER_OF_PAGE_DIRECTORY_ENTRIES - NUMBER_OF_BOOTSTRAP_PAGE_TABLES;
    size_t page_table_allocation_size = sizeof(page_table) * number_of_page_tables;
    break_allocator.align_break_on_next_page_boundary();
    void* page_table_pointer = break_allocator.malloc(page_table_allocation_size);
    uintptr_t page_table_virtual_address = (uintptr_t)page_table_pointer;
    page_table *pt = (page_table*)page_table_virtual_address;
    memset((void*)pt, 0, page_table_allocation_size);
    return pt;
}
