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
    void* (*malloc)(struct malloc_zone *self, size_t size);
    void* (*realloc)(struct malloc_zone *self, void *ptr, size_t new_size);
    void (*free)(struct malloc_zone *self, void *ptr);

    malloc_block_t *head;
} malloc_zone_t;

#ifdef __cplusplus
}
#endif