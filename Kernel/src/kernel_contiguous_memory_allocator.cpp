#include <kernel_contiguous_memory_allocator.hpp>
#include <logical_addressing.hpp>
#include <panic.h>

#include <cstring>

KernelContiguousMemoryAllocator::KernelContiguousMemoryAllocator(KernelBreakAllocator &break_allocator,
                                                                 PageFrameAllocator &page_frames)
 : break_allocator_(break_allocator),
   page_frames_(page_frames)
{}

void* KernelContiguousMemoryAllocator::malloc(size_t length)
{
    if (!can_secure_page_frames_for_allocation(length)) {
        return nullptr;
    }
    secure_page_frames_for_allocation(length);
    void* pointer = break_allocator_.malloc(length);
    map_unmapped_kernel_heap_memory((uintptr_t)pointer, length);
    return pointer;
}

void* KernelContiguousMemoryAllocator::realloc(void *, size_t)
{
    return nullptr;
}

void KernelContiguousMemoryAllocator::free(void *) {}

bool KernelContiguousMemoryAllocator::can_secure_page_frames_for_allocation(size_t length)
{
    uintptr_t base_address = (uintptr_t)break_allocator_.get_kernel_break();
    for (uintptr_t page = get_page_for_address(base_address);
         page < (base_address+length);
         page += PAGE_SIZE) {
        if (!is_kernel_heap_page_mapped(page)) {
            uintptr_t page_frame = convert_logical_to_physical_address(page);
            if (!page_frames_.is_free(page_frame)) {
                return false;
            }
        }
    }
    return true;
}

void KernelContiguousMemoryAllocator::secure_page_frames_for_allocation(size_t length)
{
    uintptr_t base_address = (uintptr_t)break_allocator_.get_kernel_break();
    for (uintptr_t page = get_page_for_address(base_address);
         page < (base_address+length);
         page += PAGE_SIZE) {
        if (!is_kernel_heap_page_mapped(page)) {
            uintptr_t page_frame = convert_logical_to_physical_address(page);
            if (!page_frames_.allocate(page_frame)) {
                panic("Unexpectedly failed to allocate page frame.");
            }
        }
    }
}

void KernelContiguousMemoryAllocator::map_unmapped_kernel_heap_memory(uintptr_t base_address,
                                                                      size_t length)
{
    for (uintptr_t page = get_page_for_address(base_address);
         page < (base_address+length);
         page += PAGE_SIZE) {
        map_unmapped_kernel_heap_page(page);
    }
}

void KernelContiguousMemoryAllocator::map_unmapped_kernel_heap_page(uintptr_t page)
{
    if (!is_kernel_heap_page_mapped(page)) {
        map_kernel_heap_page(page);
    }
}

bool KernelContiguousMemoryAllocator::is_kernel_heap_page_mapped(uintptr_t virtual_address)
{
    return get_current_page_directory_const().is_address_mapped(virtual_address);
}

void KernelContiguousMemoryAllocator::map_kernel_heap_page(uintptr_t page)
{
    assert_is_page_aligned(page);
    uintptr_t page_frame = convert_logical_to_physical_address(page);
    PageDirectory& pd = get_current_page_directory();
    pd.map_page(page_frame, page, PHYS_MAP_WRITABLE);
    memset((void*)page, 0, PAGE_SIZE);
}
