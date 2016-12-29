#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct heap_block
{
    struct heap_block *prev;
    struct heap_block *next;
    size_t size;
    unsigned inuse;
} heap_block_t;
