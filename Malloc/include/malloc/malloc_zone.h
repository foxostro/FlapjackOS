#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct malloc_block
{
    struct malloc_block *prev;
    struct malloc_block *next;
    size_t size;
    unsigned inuse;
} malloc_block_t;

typedef struct malloc_zone {
    malloc_block_t *head;
} malloc_zone_t;

typedef struct {
    // Initializes the malloc zone using the specified region of memory.
    // Returns the address of the malloc_zone_t structure stored within that
    // memory.
    // Allocations from the zone will always be taken from this memory region.
    malloc_zone_t* (*init)(void *start, size_t size);

    // Allocates a block of memory of the given size from the malloc zone.
    // May return NULL if the request cannot be satisfied.
    // If size is zero a new minimum-sized object is allocated.
    void* (*malloc)(malloc_zone_t *this, size_t size);

    // Tries to change the size of the allocation pointed to by ptr to size,
    // and returns the address of the new allocation. This may move the
    // allocation to a different part of the heap, copying as much of the old
    // data pointed to by ptr as will fit in the new allocation, and freeing the
    // old allocation.
    // 
    // When extending an allocation, no guarantee is made as to the value of
    // memory in the extended region.
    // 
    // If ptr is NULL, realloc() is identical to a call to malloc().
    // 
    // If size is zero and ptr is not NULL, a new minimum-sized object is
    // allocated and the original object is freed.
    void* (*realloc)(malloc_zone_t *this, void *ptr, size_t new_size);

    // Deallocates a memory allocation pointed to be ptr. If ptr is NULL then
    // no operation is performed.
    void (*free)(malloc_zone_t *this, void *ptr);
} malloc_interface_t;

// Returns the interface for the memory allocator.
void get_malloc_interface(malloc_interface_t *interface);
