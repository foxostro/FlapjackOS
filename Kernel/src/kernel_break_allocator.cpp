#include "kernel_break_allocator.hpp"
#include "kernel_image_info.hpp"
#include "page_size.hpp"
#include <cassert>

void KernelBreakAllocator::reset(void *bootstrap_heap_begin)
{
    kernel_break_ = bootstrap_heap_begin;
    kernel_heap_begin_ = kernel_break_;
}

void* KernelBreakAllocator::malloc(size_t length)
{
    void *old_break_pointer = kernel_break_;
    uintptr_t break_address = (uintptr_t)kernel_break_;
    break_address += length;
    kernel_break_ = (void*)break_address;
    assert(kernel_break_ >= kernel_heap_begin_);
    return old_break_pointer;
}

void* KernelBreakAllocator::realloc(void *, size_t)
{
    return nullptr;
}

void KernelBreakAllocator::free(void *) {}

void KernelBreakAllocator::align_break_on_next_page_boundary()
{
    kernel_break_ = (void*)round_address_up_to_next_page(kernel_break_);
}

void* KernelBreakAllocator::get_kernel_break()
{
    return kernel_break_;
}
