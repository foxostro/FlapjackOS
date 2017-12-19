#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <malloc/malloc_zone.h>

typedef struct malloc_block
{
    struct malloc_block *prev;
    struct malloc_block *next;
    size_t size;
    unsigned inuse;
} malloc_block_t;

typedef struct malloc_zone {
    void* (*malloc)(struct malloc_zone *this, size_t size);
    void* (*realloc)(struct malloc_zone *this, void *ptr, size_t new_size);
    void (*free)(struct malloc_zone *this, void *ptr);

    malloc_block_t *head;
} malloc_zone_t;

_Static_assert(offsetof(malloc_zone_t, malloc) == offsetof(malloc_interface_t, malloc),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(malloc_zone_t, realloc) == offsetof(malloc_interface_t, realloc),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(malloc_zone_t, free) == offsetof(malloc_interface_t, free),
    "Incorrect offset prevents correct casting behavior when casting between types.");

#ifdef __cplusplus
}
#endif