#include <heap/heap_pool.h>
#include <assert.h>
#include <stdint.h>
#include <strings.h>
#include <stdio.h>

#define ALIGN 4
#define MIN_SPLIT_SIZE 16

static inline size_t round_up_block_size(size_t size)
{
    size_t new_size = ((size + ALIGN - 1) / ALIGN) * ALIGN;

    if (new_size < size) { // check for overflow
        new_size = SIZE_MAX - 3;
        assert(new_size % ALIGN == 0);
    }

    return new_size;
}

static void consider_splitting_block(heap_block_t *best, size_t size)
{
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
}

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
    size = round_up_block_size(size);

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

    consider_splitting_block(best, size);

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
    assert(block->inuse);
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
void* heap_pool_realloc(heap_pool_t *this, void *ptr, size_t new_size)
{
    assert(this);

    if (!ptr) {
        return heap_pool_malloc(this, new_size);
    }

    if (new_size == 0) {
        heap_pool_free(this, ptr);
        return heap_pool_malloc(this, 0);
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
    assert(block->inuse);
#endif

    // The block is already large enough to accomodate the new size.
    // TODO: actually try to shrink the allocation
    if (block->size >= new_size) {
        return ptr;
    }

    // Blocks for allocations are always multiples of four bytes in size.
    // This ensures that blocks are always aligned on four byte boundaries
    // given that the initial block is also aligned on a four byte boundary.
    new_size = round_up_block_size(new_size);

    heap_block_t *following = block->next;

    // If this block is followed by a free block then would it satisfy our
    // requirements to take some of that free space by extending this block?
    if (following
        && !following->inuse
        && (block->size + following->size + sizeof(heap_block_t)) >= new_size) {

        // Remove the following block, extending this one so as to not leave a
        // hole in the pool.
        block->size = block->size + following->size + sizeof(heap_block_t);
        block->next = following->next;

        if (following->next) {
            following->next->prev = block;
        }

        // Split the remaining free space if there's enough of it.
        consider_splitting_block(block, new_size);

        return ptr;
    }

    return NULL;
}
