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
