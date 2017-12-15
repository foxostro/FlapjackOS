#include <malloc/malloc_zone.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#define ALIGN 4
#define MIN_SPLIT_SIZE (sizeof(malloc_block_t))

static inline size_t round_up_block_size(size_t size)
{
    size_t new_size = ((size + ALIGN - 1) / ALIGN) * ALIGN;

    if (new_size < size) { // check for overflow
        new_size = SIZE_MAX - 3;
        assert(new_size % ALIGN == 0);
    }

    return new_size;
}

static void consider_splitting_block(malloc_block_t *block, size_t size)
{
    // Split the block if the remaining free space is big enough.
    size_t remaining_space = block->size - size;
    if (remaining_space > MIN_SPLIT_SIZE) {
        malloc_block_t *new_block = (malloc_block_t *)((void *)block + sizeof(malloc_block_t) + size);

        assert((uintptr_t)new_block % ALIGN == 0);
        new_block->prev = block;
        new_block->next = block->next;
        new_block->size = remaining_space - sizeof(malloc_block_t);
        new_block->inuse = false;

        block->size = size;
        if (block->next) {
            block->next->prev = new_block;
        }
        block->next = new_block;

        // If the next block is empty then merge the new free block with the
        // free block that follows it.
        malloc_block_t *following = new_block->next;
        if (following && !following->inuse) {
            new_block->size += following->size + sizeof(malloc_block_t);
            new_block->next = following->next;
            if (following->next) {
                following->next->prev = new_block;
            }
        }
    }
}

malloc_zone_t* malloc_zone_init(void *start, size_t size)
{
    assert(start);
    assert(size > sizeof(malloc_zone_t));

    malloc_zone_t *zone = start + (4 - (uintptr_t)start % 4); // 4 byte alignment

    // The first block is placed at the address immediately after the header.
    malloc_block_t *first = zone->head = (void *)zone + sizeof(malloc_zone_t);

    first->prev = NULL;
    first->next = NULL;
    first->size = size - sizeof(malloc_zone_t) - sizeof(malloc_block_t);
    first->inuse = false;

    return zone;
}

void* malloc_zone_malloc(malloc_zone_t *this, size_t size)
{
    assert(this);

    // Blocks for allocations are always multiples of four bytes in size.
    // This ensures that blocks are always aligned on four byte boundaries
    // given that the initial block is also aligned on a four byte boundary.
    size = round_up_block_size(size);

    malloc_block_t *best = NULL;

    // Get the smallest free block that is large enough to satisfy the request.
    for (malloc_block_t *block = this->head; block; block = block->next) {
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
    return (void *)best + sizeof(malloc_block_t);
}

void malloc_zone_free(malloc_zone_t *this, void *ptr)
{
    assert(this);

    if (!ptr) {
        return; // do nothing
    }

    malloc_block_t *block = ptr - sizeof(malloc_block_t);

    // Walk over the heap and see if we can find this allocation.
    // If we cannot find it then the calling code has an error in it.
#ifndef NDEBUG
    bool found_it = false;
    for (malloc_block_t *iter = this->head; iter; iter = iter->next) {
        if (iter == block) {
            found_it = true;
        }
    }
    assert(found_it);
    assert(block->inuse);
#endif

    block->inuse = false;

    malloc_block_t *preceding = block->prev, *following = block->next;

    // If the preceding chunk is free then merge this one into it. This block
    // goes away and the preceding chunk expands to fill the hole.
    if (preceding && !preceding->inuse) {
        preceding->size += block->size + sizeof(malloc_block_t);
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
        block->size += following->size + sizeof(malloc_block_t);
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
void* malloc_zone_realloc(malloc_zone_t *this, void *ptr, size_t new_size)
{
    assert(this);

    if (!ptr) {
        return malloc_zone_malloc(this, new_size);
    }

    malloc_block_t *block = ptr - sizeof(malloc_block_t);
    assert(block->inuse);

    // Walk over the heap and see if we can find this allocation.
    // If we cannot find it then the calling code has an error in it.
#ifndef NDEBUG
    bool found_it = false;
    for (malloc_block_t *iter = this->head; iter; iter = iter->next) {
        if (iter == block) {
            found_it = true;
        }
    }
    assert(found_it);
#endif

    if (new_size == 0) {
        malloc_zone_free(this, ptr);
        return malloc_zone_malloc(this, 0);
    }

    // Blocks for allocations are always multiples of four bytes in size.
    // This ensures that blocks are always aligned on four byte boundaries
    // given that the initial block is also aligned on a four byte boundary.
    new_size = round_up_block_size(new_size);

    // The block is already large enough to accomodate the new size.
    if (block->size >= new_size) {
        consider_splitting_block(block, new_size);
        return ptr;
    }

    malloc_block_t *following = block->next;

    // If this block is followed by a free block then would it satisfy our
    // requirements to take some of that free space by extending this block?
    if (following
        && !following->inuse
        && (block->size + following->size + sizeof(malloc_block_t)) >= new_size) {

        // Remove the following block, extending this one so as to not leave a
        // hole in the zone.
        block->size = block->size + following->size + sizeof(malloc_block_t);
        block->next = following->next;

        if (following->next) {
            following->next->prev = block;
        }

        // Split the remaining free space if there's enough of it.
        consider_splitting_block(block, new_size);

        return ptr;
    }

    // Can we allocate a new block of memory for the resized allocation?
    void *new_alloc = malloc_zone_malloc(this, new_size);
    if (new_alloc) {
        memcpy(ptr, new_alloc, block->size);
        malloc_zone_free(this, ptr);
        return new_alloc;
    }

    malloc_block_t *preceding = block->prev;

    // If this block is preceded by a free block then would it satisfy our
    // requirements to merge into the preceding block?
    if (preceding
        && !preceding->inuse
        && (block->size + preceding->size + sizeof(malloc_block_t)) >= new_size) {

        // Remove this block, extending the preceding one so as to not leave a
        // hole in the zone.
        preceding->next = following;
        if (following) {
            following->prev = preceding;
        }
        preceding->size = block->size + preceding->size + sizeof(malloc_block_t);
        preceding->inuse = true;

        // Move the contents to the beginning of the new, combined block.
        new_alloc = (void *)preceding + sizeof(malloc_block_t);
        memmove(ptr, new_alloc, block->size);

        // Split the remaining free space if there's enough of it.
        consider_splitting_block(preceding, new_size);

        return new_alloc;
    }

    return NULL;
}
