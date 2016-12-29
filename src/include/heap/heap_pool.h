#pragma once

#include <heap/heap_block.h>

typedef struct heap_pool {
    heap_block_t *head;
} heap_pool_t;

// Initializes the heap pool using the specified region of memory.
// Returns the address of the heap_pool_t structure stored within that memory.
// Allocations from the heap pool will always be taken from this memory region.
heap_pool_t* heap_pool_init(void *start, size_t size);

// Allocates a block of memory of the given size from the heap pool.
// May return NULL if the request cannot be satisfied.
// If size is zero a new minimum-sized object is allocated.
void* heap_pool_malloc(heap_pool_t *this, size_t size);

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
void* heap_pool_realloc(heap_pool_t *this, void *ptr, size_t new_size);

// Deallocates a memory allocation pointed to be ptr. If ptr is NULL then
// no operation is performed.
void heap_pool_free(heap_pool_t *this, void *ptr);
