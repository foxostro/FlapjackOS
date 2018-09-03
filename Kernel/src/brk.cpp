#include "brk.hpp"
#include "kernel_image_info.hpp"
#include "logical_addressing.hpp"
#include "creg.h"

#include <cstring>

#define assert_is_page_aligned(address) assert(((address) & (PAGE_SIZE-1)) == 0)

constexpr uint32_t PHYS_MAP_PRESENT = (1 << 0);
constexpr uint32_t PHYS_MAP_WRITEABLE = (1 << 1);

// Get the index of the page directory associated with the given address.
static size_t page_directory_index_for_virtual_address(uintptr_t virtual_address)
{
    constexpr uint32_t PDE_INDEX_SHIFT = 22;
    return (uint32_t)virtual_address >> PDE_INDEX_SHIFT;
}

// Get the index of the page table associated with the given address.
static size_t page_table_index_for_virtual_address(uintptr_t virtual_address)
{
    constexpr uint32_t PTE_INDEX_SHIFT = 12;
    constexpr uint32_t PTE_INDEX_MASK = 0x03FF;
    return (uint32_t)virtual_address >> PTE_INDEX_SHIFT & PTE_INDEX_MASK;
}

static void invlpg(void* virtual_address)
{
    asm volatile("invlpg (%0)" : : "b"(virtual_address) : "memory");
}

static uintptr_t get_page_for_address(uintptr_t address)
{
    return address & ~(PAGE_SIZE-1);
}

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

size_t kernel_heap_allocator::get_available_bootstrap_heap_bytes()
{
    uintptr_t break_address = (uintptr_t)kernel_break;
    if (break_address < BOOTSTRAP_HEAP_END) {
        return BOOTSTRAP_HEAP_END - break_address;
    } else {
        return 0;
    }
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
        mark_page_readonly((void*)page);
    }
}

void kernel_heap_allocator::mark_page_readonly(void* virtual_address)
{
    size_t page_directory_index = page_directory_index_for_virtual_address((uintptr_t)virtual_address);
    size_t page_table_index = page_table_index_for_virtual_address((uintptr_t)virtual_address);
    uintptr_t page_table_physical_address = bootstrap_page_directory[page_directory_index] & ~(PAGE_SIZE-1);
    uint32_t* page_table = (uint32_t*)convert_physical_to_logical_address(page_table_physical_address);
    uint32_t& pte = page_table[page_table_index];
    pte = pte & ~PHYS_MAP_WRITEABLE;
    invlpg(virtual_address);
}

void kernel_heap_allocator::populate_kernel_page_directory()
{
    constexpr size_t PAGE_TABLE_SIZE = 4096;
    constexpr size_t NUMBER_OF_PAGE_DIRECTORY_ENTRIES = 1024;

    size_t number_of_page_tables = NUMBER_OF_PAGE_DIRECTORY_ENTRIES-NUMBER_OF_BOOTSTRAP_PAGE_TABLES;
    size_t page_table_allocation_size = PAGE_TABLE_SIZE * number_of_page_tables;
    void* page_table_pointer = increment_break_pointer(page_table_allocation_size);
    uintptr_t page_table_virtual_address = (uintptr_t)page_table_pointer;
    size_t index_of_first_open_pde = page_directory_index_for_virtual_address(KERNEL_VIRTUAL_START_ADDR + 0x400000*NUMBER_OF_BOOTSTRAP_PAGE_TABLES);

    for (size_t i = index_of_first_open_pde; i < NUMBER_OF_PAGE_DIRECTORY_ENTRIES; ++i) {
        memset((void*)page_table_virtual_address, 0, PAGE_TABLE_SIZE);
        uintptr_t page_table_physical_address = convert_logical_to_physical_address(page_table_virtual_address);
        bootstrap_page_directory[i] = page_table_physical_address | PHYS_MAP_PRESENT;
        page_table_virtual_address += PAGE_TABLE_SIZE;
    }
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
        assert((void*)page >= get_bootstrap_heap_end());
        map_page(convert_logical_to_physical_address(page), page);
    }
}

bool kernel_heap_allocator::is_kernel_heap_page_mapped(uintptr_t virtual_address)
{
    assert_is_page_aligned(virtual_address);
    const uint32_t* page_directory = (uint32_t*)convert_physical_to_logical_address(get_cr3());
    size_t page_directory_index = page_directory_index_for_virtual_address(virtual_address);
    size_t page_table_index = page_table_index_for_virtual_address(virtual_address);
    const uint32_t& pde = page_directory[page_directory_index];

    if ((pde & PHYS_MAP_PRESENT) != PHYS_MAP_PRESENT) {
        return false;
    }

    uintptr_t page_table_physical_address = pde & ~(PAGE_SIZE-1);
    const uint32_t* page_table = (uint32_t*)convert_physical_to_logical_address(page_table_physical_address);
    const uint32_t& pte = page_table[page_table_index];

    if ((pte & PHYS_MAP_PRESENT) != PHYS_MAP_PRESENT) {
        return false;
    }

    return true;
}

void kernel_heap_allocator::map_page(uintptr_t physical_address,
                                     uintptr_t virtual_address)
{
    assert_is_page_aligned(physical_address);
    assert_is_page_aligned(virtual_address);
    uint32_t* page_directory = (uint32_t*)convert_physical_to_logical_address(get_cr3());
    size_t page_directory_index = page_directory_index_for_virtual_address(virtual_address);
    size_t page_table_index = page_table_index_for_virtual_address(virtual_address);
    uint32_t& pde = page_directory[page_directory_index];
    pde = pde | PHYS_MAP_WRITEABLE | PHYS_MAP_PRESENT;
    uintptr_t page_table_physical_address = pde & ~(PAGE_SIZE-1);
    uint32_t* page_table = (uint32_t*)convert_physical_to_logical_address(page_table_physical_address);
    uint32_t& pte = page_table[page_table_index];
    pte = physical_address | PHYS_MAP_WRITEABLE | PHYS_MAP_PRESENT;
    invlpg((void*)virtual_address);
    memset((void*)virtual_address, 0, PAGE_SIZE);
}
