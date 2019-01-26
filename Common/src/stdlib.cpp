#include "flapjack_libc/stdlib.h"
#include "flapjack_libc/string.h"
#include "flapjack_libc/assert.h"
#include <common/memory_allocator.hpp>

static MemoryAllocator *g_allocator = nullptr;

extern "C"
void* MALLOC(size_t size)
{
    assert(g_allocator && "malloc() called without first specifying a global allocator.");
    return g_allocator->malloc(size);
}

extern "C"
void* CALLOC(size_t count, size_t size)
{
    assert(g_allocator && "calloc() called without first specifying a global allocator.");
    void* result = g_allocator->malloc(count * size);
    MEMSET(result, 0, count * size);
    return result;
}

extern "C"
void* MEMALIGN(size_t size, size_t align)
{
    assert(g_allocator && "memalign() called without first specifying a global allocator.");
    return g_allocator->memalign(size, align);
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
