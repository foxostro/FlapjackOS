#include <heap/heap_pool.h>
#include <assert.h>
#include <stdint.h>
#include <strings.h>
#include <stdio.h>

#define ALIGN 4
#define MIN_SPLIT_SIZE 16

heap_pool_t* heap_pool_init(void *start, size_t size)
{
    assert(start);
    assert(size > sizeof(heap_pool_t));
    bzero(start, size);

    heap_pool_t *pool = start + (4 - (uintptr_t)start % 4); // 4 byte alignment

    // The first block is placed at the address immediately after the header.
    heap_block_t *first = pool->head = (void *)pool + sizeof(heap_pool_t);

    first->prev = NULL;
    first->next = NULL;
    first->size = size - sizeof(heap_pool_t) - sizeof(heap_block_t);
    first->inuse = false;

    return pool;
}

void* heap_pool_malloc(heap_pool_t *this, size_t size)
{
    assert(this);

    // Blocks for allocations are always multiples of four bytes in size.
    // This ensures that blocks are always aligned on four byte boundaries
    // given that the initial block is also aligned on a four byte boundary.
    size_t new_size;

    if (size == 0) {
        new_size = ALIGN;
    } else {
        new_size = ((size + ALIGN - 1) / ALIGN) * ALIGN;
        if (new_size < size) { // check for overflow
            size = SIZE_MAX - 3;
            assert(size % ALIGN == 0);
        } else {
            size = new_size;
        }
    }

    heap_block_t *best = NULL;

    // Get the smallest free block that is large enough to satisfy the request.
    for (heap_block_t *block = this->head; block; block = block->next) {
        if (block->size >= size
            && !block->inuse
            && (!best || (block->size < best->size))) {
            best = block;
        }
    }

    if (!best) {
        return NULL;
    }

    // Split the block if the remaining free space is big enough.
    size_t remaining_space = best->size - size;
    if (remaining_space >= MIN_SPLIT_SIZE) {
        heap_block_t *new_block = (heap_block_t *)((void *)best + sizeof(heap_block_t) + size);

        assert((uintptr_t)new_block % ALIGN == 0);
        new_block->prev = best;
        new_block->next = best->next;
        new_block->size = remaining_space - sizeof(heap_block_t);
        new_block->inuse = false;

        best->size = size;
        if (best->next) {
            best->next->prev = new_block;
        }
        best->next = new_block;
    }

    best->inuse = true;
    return (void *)best + sizeof(heap_block_t);
}

void heap_pool_free(heap_pool_t *this, void *ptr)
{
    assert(this);

    if (!ptr) {
        return; // do nothing
    }

    heap_block_t *block = ptr - sizeof(heap_block_t);

    // Walk over the heap and see if we can find this allocation.
    // If we cannot find it then the calling code has an error in it.
#ifndef NDEBUG
    bool found_it = false;
    for (heap_block_t *iter = this->head; iter; iter = iter->next) {
        if (iter == block) {
            found_it = true;
        }
    }
    assert(found_it);
#endif

    block->inuse = false;

    heap_block_t *preceding = block->prev, *following = block->next;

    // If the preceding chunk is free then merge this one into it. This block
    // goes away and the preceding chunk expands to fill the hole.
    if (preceding && !preceding->inuse) {
        preceding->size += block->size + sizeof(heap_block_t);
        preceding->next = following;
        if (following) {
            following->prev = preceding;
        }

        // Reset these pointers for a possible merge with `following', below.
        block = preceding;
        preceding = block->prev;
    }

    // If the following chunk is free then merge it into this one.
    // The following block goes away and this chunk expands to fill the hole.
    if (following && !following->inuse) {
        block->size += following->size + sizeof(heap_block_t);
        block->next = following->next;
        if (following->next) {
            following->next->prev = block;
        }
    }
}

void* heap_pool_realloc(heap_pool_t *this, void *ptr, size_t new_size)
{
    assert(this);
    (void)this;
    (void)ptr;
    (void)new_size;
    return NULL;
}
