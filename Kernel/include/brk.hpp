#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

// Set the kernel break pointer.
// The kernel break pointer is the address of the end of the kernel heap. This
// address must be greater than the beginning of the kernel heap.
void brk(void *break_pointer);

// Increase or decrease the kernel break pointer by the specified amount.
// Returns a pointer to the base of the new memory allocation.
// The kernel break pointer is the address of the end of the kernel heap. This
// address must be greater than the beginning of the kernel heap.
// Specifying an increment of zero causes this to return the value of the
// current break pointer.
void* sbrk(intptr_t increment);

// Returns the number of byte remaining in the kernel's bootstrap heap.
// The bootstrap heap is the region of the kernel heap which was mapped into
// virtual memory by the bootstrap assembly start code.
size_t get_available_bootstrap_heap_bytes();

// Gets the address of the end of the kernel's bootstrap heap.
// The bootstrap heap is the region of the kernel heap which was mapped into
// virtual memory by the bootstrap assembly start code.
void* get_bootstrap_heap_end();

// Convenience function to get the current kernel break pointer.
inline void* get_break_pointer()
{
    return sbrk(0);
}

// Convenience function to safely allocate memory from the bootstrap heap.
// This will panic if the allocation would move the break pointer beyond the
// end of the bootstrap heap.
inline void* bootstrap_sbrk(intptr_t increment)
{
    assert((increment > 0) && (get_available_bootstrap_heap_bytes() >= (uintptr_t)increment));
    return sbrk(increment);
}