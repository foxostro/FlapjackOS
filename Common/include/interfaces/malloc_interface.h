#pragma once

#include <stddef.h>

typedef struct malloc_interface {
    // Allocates a block of memory of the given size from the malloc zone.
    // May return NULL if the request cannot be satisfied.
    // If size is zero a new minimum-sized object is allocated.
    void* (*malloc)(struct malloc_interface *this, size_t size);

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
    void* (*realloc)(struct malloc_interface *this, void *ptr, size_t new_size);

    // Deallocates a memory allocation pointed to be ptr. If ptr is NULL then
    // no operation is performed.
    void (*free)(struct malloc_interface *this, void *ptr);
} malloc_interface_t;
