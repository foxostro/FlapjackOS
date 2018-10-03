#include "cleanup_kernel_memory_map_operation.hpp"
#include "mmu.hpp"
#include "kernel_image_info.hpp"
#include <logger.hpp>

CleanupKernelMemoryMapOperation::CleanupKernelMemoryMapOperation() = default;

void CleanupKernelMemoryMapOperation::cleanup_kernel_memory_map()
{
    mark_pages_readonly((uintptr_t)g_kernel_text_begin,
                        (uintptr_t)g_kernel_text_end);

    mark_pages_readonly((uintptr_t)g_kernel_rodata_begin,
                        (uintptr_t)g_kernel_rodata_end);
}

void CleanupKernelMemoryMapOperation::mark_pages_readonly(uintptr_t begin,
                                                          uintptr_t end)
{
    for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
        mark_page_readonly(page);
    }
}

void CleanupKernelMemoryMapOperation::mark_page_readonly(uintptr_t virtual_address)
{
    PageTableEntry& pte = get_current_page_directory().get_pte(virtual_address);
    pte.set_writable(false);
    invlpg((void*)virtual_address);
}
