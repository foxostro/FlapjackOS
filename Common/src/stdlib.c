#include "flapjack_libc/stdlib.h"
#include "flapjack_libc/assert.h"
#include <interfaces/malloc_interface.h>

static malloc_interface_t *g_allocator = NULL;

void* MALLOC(size_t size)
{
    assert(g_allocator);
    return g_allocator->malloc(g_allocator, size);
}

void* REALLOC(void *ptr, size_t size)
{
    assert(g_allocator);
    return g_allocator->realloc(g_allocator, ptr, size);
}

void FREE(void *ptr)
{
    assert(g_allocator);
    return g_allocator->free(g_allocator, ptr);
}

// Sets the allocator to use for the malloc family of functions.
void set_global_allocator(malloc_interface_t *allocator)
{
    g_allocator = allocator;
}
