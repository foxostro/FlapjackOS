#include <kernel_heap_allocator.hpp>
#include <logical_addressing.hpp>
#include <cstring>
#include <panic.h>

kernel_heap_allocator::kernel_heap_allocator(kernel_break_allocator &break_allocator,
                                             page_frame_allocator &page_frames)
 : break_allocator(break_allocator),
   page_frames(page_frames)
{}

void* kernel_heap_allocator::malloc(size_t length)
{
    if (!can_secure_page_frames_for_allocation(length)) {
        return nullptr;
    }
    secure_page_frames_for_allocation(length);
    void* pointer = break_allocator.malloc(length);
    map_unmapped_kernel_heap_memory((uintptr_t)pointer, length);
    return pointer;
}

void* kernel_heap_allocator::realloc(void *, size_t)
{
    return nullptr;
}

void kernel_heap_allocator::free(void *) {}

bool kernel_heap_allocator::can_secure_page_frames_for_allocation(size_t length)
{
    uintptr_t base_address = (uintptr_t)break_allocator.get_kernel_break();
    for (uintptr_t page = get_page_for_address(base_address);
         page < (base_address+length);
         page += PAGE_SIZE) {
        if (!is_kernel_heap_page_mapped(page)) {
            uintptr_t page_frame = convert_logical_to_physical_address(page);
            if (!page_frames.is_free(page_frame)) {
                return false;
            }
        }
    }
    return true;
}

void kernel_heap_allocator::secure_page_frames_for_allocation(size_t length)
{
    uintptr_t base_address = (uintptr_t)break_allocator.get_kernel_break();
    for (uintptr_t page = get_page_for_address(base_address);
         page < (base_address+length);
         page += PAGE_SIZE) {
        if (!is_kernel_heap_page_mapped(page)) {
            uintptr_t page_frame = convert_logical_to_physical_address(page);
            if (!page_frames.allocate(page_frame)) {
                panic("Unexpectedly failed to allocate page frame.");
            }
        }
    }
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
    uintptr_t page_frame = convert_logical_to_physical_address(page);
    page_directory& pd = get_current_page_directory();
    pd.map_page(page_frame, page, PHYS_MAP_WRITABLE);
    memset((void*)page, 0, PAGE_SIZE);
}
