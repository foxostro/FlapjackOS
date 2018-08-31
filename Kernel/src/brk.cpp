#include "brk.hpp"
#include "kernel_image_info.hpp"

// The kernel heap region follows the end of the kernel image.
// The bootstrap assembly procedure will have ensured that we have enough
// physical memory mapped into the address space to setup the page frame
// allocator and any additional page tables we might need to map the rest of
// physical memory into the kernel memory region.
static void* s_kernel_break = (void*)kernel_image_end;

// The initial value of the kernel break pointer.
// We can use this to compute the current size of the kernel heap.
static const void* KERNEL_HEAP_BEGIN = (void*)s_kernel_break;

// Set the kernel break pointer.
// The kernel break pointer is the address of the end of the kernel heap. This
// address must be greater than the beginning of the kernel heap.
void brk(void *break_pointer)
{
    assert(break_pointer >= KERNEL_HEAP_BEGIN);
    s_kernel_break = break_pointer;
}

// Increase or decrease the kernel break pointer by the specified amount.
// Returns a pointer to the base of the new memory allocation.
// The kernel break pointer is the address of the end of the kernel heap. This
// address must be greater than the beginning of the kernel heap.
// Specifying an increment of zero causes this to return the value of the
// current break pointer.
void* sbrk(intptr_t increment)
{
    void *old_break_pointer = s_kernel_break;
    uintptr_t break_address = (uintptr_t)s_kernel_break;
    break_address += increment;
    s_kernel_break = (void*)break_address;
    assert(s_kernel_break >= KERNEL_HEAP_BEGIN);
    return old_break_pointer;
}

// Returns the number of byte remaining in the kernel's bootstrap heap.
// This is the region of the kernel heap which was mapped into virtual memory
// by the bootstrap assembly start code.
size_t get_available_bootstrap_heap_bytes()
{
    uintptr_t break_address = (uintptr_t)s_kernel_break;
    if (break_address < BOOTSTRAP_HEAP_END) {
        return BOOTSTRAP_HEAP_END - break_address;
    } else {
        return 0;
    }
}

void* get_bootstrap_heap_end()
{
    return (void*)BOOTSTRAP_HEAP_END;
}
