#include "kernel_break_allocator.hpp"
#include "kernel_image_info.hpp"

void kernel_break_allocator::reset(void *bootstrap_heap_begin)
{
    kernel_break = bootstrap_heap_begin;
    kernel_heap_begin = kernel_break;
}

void* kernel_break_allocator::malloc(size_t length)
{
    void *old_break_pointer = kernel_break;
    uintptr_t break_address = (uintptr_t)kernel_break;
    break_address += length;
    kernel_break = (void*)break_address;
    assert(kernel_break >= kernel_heap_begin);
    return old_break_pointer;
}

void* kernel_break_allocator::realloc(void *, size_t)
{
    return nullptr;
}

void kernel_break_allocator::free(void *) {}

void kernel_break_allocator::align_break_on_next_page_boundary()
{
    kernel_break = (void*)round_address_up_to_next_page(kernel_break);
}

void* kernel_break_allocator::get_bootstrap_heap_end()
{
    return (void*)BOOTSTRAP_HEAP_END;
}

void* kernel_break_allocator::get_kernel_break()
{
    return kernel_break;
}

bool kernel_break_allocator::is_frame_beyond_bootstrap_heap(uintptr_t page_frame)
{
    uintptr_t bootstrap_heap_end = (uintptr_t)get_bootstrap_heap_end();
    bool result = (page_frame >= (bootstrap_heap_end - KERNEL_VIRTUAL_START_ADDR));
    return result;
}
