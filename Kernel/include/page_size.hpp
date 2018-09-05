#pragma once

#include <cassert>

// The size of a page of memory.
constexpr size_t PAGE_SIZE = 4096;

#define assert_is_page_aligned(address) assert(((uintptr_t)(address) & (PAGE_SIZE-1)) == 0)

inline void* round_address_up_to_next_page(void* address)
{
    return (void*)(((uintptr_t)address & ~(PAGE_SIZE-1)) + PAGE_SIZE);
}
