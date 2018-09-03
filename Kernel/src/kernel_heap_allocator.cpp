#include "kernel_heap_allocator.hpp"
#include "kernel_image_info.hpp"
#include "logical_addressing.hpp"
#include "mmu.hpp"

#include <cstring>

void kernel_heap_allocator::reset(void *bootstrap_heap_begin)
{
    kernel_break = (void*)round_address_up_to_next_page(bootstrap_heap_begin);
    kernel_heap_begin = kernel_break;

    cleanup_kernel_memory_map();
    populate_kernel_page_directory();
}

void* kernel_heap_allocator::malloc(size_t length)
{
    // TODO: reserve physical page frames required to serve the allocation.
    void *address = increment_break_pointer(length);
    map_unmapped_kernel_heap_memory((uintptr_t)address, length);
    return address;
}

void* kernel_heap_allocator::realloc(void *, size_t)
{
    // TODO: If the allocation is at the end of the heap then we can extend it.
    // Otherwise, allocate a new block of memory and copy data.
    assert(!"unimplemented");
}

void kernel_heap_allocator::free(void *)
{
    // Do nothing. We cannot free kernel heap memory, once allocated.
}

void* kernel_heap_allocator::get_bootstrap_heap_end()
{
    return (void*)BOOTSTRAP_HEAP_END;
}

void* kernel_heap_allocator::round_address_up_to_next_page(void* address)
{
    return (void*)(((uintptr_t)address & ~(PAGE_SIZE-1)) + PAGE_SIZE);
}

void kernel_heap_allocator::cleanup_kernel_memory_map()
{
    for (uintptr_t page = (uintptr_t)kernel_image_begin;
         page < (uintptr_t)kernel_rodata_end;
         page += PAGE_SIZE) {
        mark_page_readonly(page);
    }
}

void kernel_heap_allocator::mark_page_readonly(uintptr_t virtual_address)
{
    page_table_entry& pte = get_current_page_directory().get_pte(virtual_address);
    pte.set_writable(false);
    invlpg((void*)virtual_address);
}

void kernel_heap_allocator::populate_kernel_page_directory()
{
    page_table *pt = allocate_kernel_page_tables();
    page_directory& pd = get_current_page_directory();
    size_t index_of_first_open_pde = pd.index_for_virtual_address((uintptr_t)get_bootstrap_heap_end());

    for (size_t i = index_of_first_open_pde;
         i < page_directory::NUMBER_OF_PAGE_DIRECTORY_ENTRIES;
         ++i, ++pt) {

        page_directory_entry &pde = pd.entries[i];
        pde.set_page_table(pt);
    }
}

page_table* kernel_heap_allocator::allocate_kernel_page_tables()
{
    size_t number_of_page_tables = page_directory::NUMBER_OF_PAGE_DIRECTORY_ENTRIES - NUMBER_OF_BOOTSTRAP_PAGE_TABLES;
    size_t page_table_allocation_size = sizeof(page_table) * number_of_page_tables;
    void* page_table_pointer = increment_break_pointer(page_table_allocation_size);
    uintptr_t page_table_virtual_address = (uintptr_t)page_table_pointer;
    page_table *pt = (page_table*)page_table_virtual_address;
    memset((void*)pt, 0, page_table_allocation_size);
    return pt;
}

void* kernel_heap_allocator::increment_break_pointer(intptr_t increment)
{
    void *old_break_pointer = kernel_break;
    uintptr_t break_address = (uintptr_t)kernel_break;
    break_address += increment;
    kernel_break = (void*)break_address;
    assert(kernel_break >= kernel_heap_begin);
    return old_break_pointer;
}

void kernel_heap_allocator::map_unmapped_kernel_heap_memory(uintptr_t base_address,
                                                            size_t length)
{
    for (uintptr_t page = get_page_for_address(base_address);
         page < (base_address+length);
         page += PAGE_SIZE) {
        map_unmapped_kernel_heap_page(page);
    }
}

void kernel_heap_allocator::map_unmapped_kernel_heap_page(uintptr_t page)
{
    if (!is_kernel_heap_page_mapped(page)) {
        map_kernel_heap_page(page);
    }
}

bool kernel_heap_allocator::is_kernel_heap_page_mapped(uintptr_t virtual_address)
{
    return get_current_page_directory_const().is_address_mapped(virtual_address);
}

void kernel_heap_allocator::map_kernel_heap_page(uintptr_t page)
{
    assert_is_page_aligned(page);
    page_directory& pd = get_current_page_directory();
    pd.map_page(convert_logical_to_physical_address(page),
                page,
                PHYS_MAP_WRITABLE);
    memset((void*)page, 0, PAGE_SIZE);
}
