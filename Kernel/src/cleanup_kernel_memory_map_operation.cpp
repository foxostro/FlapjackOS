#include "cleanup_kernel_memory_map_operation.hpp"
#include "mmu.hpp"
#include "kernel_image_info.hpp"

cleanup_kernel_memory_map_operation::cleanup_kernel_memory_map_operation() = default;

void cleanup_kernel_memory_map_operation::cleanup_kernel_memory_map()
{
    for (uintptr_t page = (uintptr_t)kernel_image_begin;
         page < (uintptr_t)kernel_rodata_end;
         page += PAGE_SIZE) {
        mark_page_readonly(page);
    }
}

void cleanup_kernel_memory_map_operation::mark_page_readonly(uintptr_t virtual_address)
{
    page_table_entry& pte = get_current_page_directory().get_pte(virtual_address);
    pte.set_writable(false);
    invlpg((void*)virtual_address);
}
