#include "flapjack_libc/stdlib.h"
#include "flapjack_libc/assert.h"
#include "flapjack_libc/new"
#include <common/memory_allocator.hpp>

static MemoryAllocator *g_allocator = nullptr;

extern "C"
void* MALLOC(size_t size)
{
    assert(g_allocator && "malloc() called without first specifying a global allocator.");
    return g_allocator->malloc(size);
}

extern "C"
void* REALLOC(void *ptr, size_t size)
{
    assert(g_allocator && "realloc() called without first specifying a global allocator.");
    return g_allocator->realloc(ptr, size);
}

extern "C"
void FREE(void *ptr)
{
    assert(g_allocator && "free() called without first specifying a global allocator.");
    return g_allocator->free(ptr);
}

// Sets the allocator to use for the malloc family of functions.
void set_global_allocator(MemoryAllocator *a)
{
    assert(a);
    g_allocator = a;
}
