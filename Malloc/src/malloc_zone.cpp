#include <malloc/malloc_zone.hpp>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <new>

constexpr size_t ALIGN = 4;
constexpr size_t MIN_SPLIT_SIZE = sizeof(MallocBlock);

static size_t round_up_block_size(size_t size)
{
    size_t new_size = ((size + ALIGN - 1) / ALIGN) * ALIGN;

    if (new_size < size) { // check for overflow
        new_size = SIZE_MAX - 3;
        assert(new_size % ALIGN == 0);
    }

    return new_size;
}

static void consider_splitting_block(MallocBlock *block, size_t size)
{
    // Split the block if the remaining free space is big enough.
    size_t remaining_space = block->size - size;
    if (remaining_space > MIN_SPLIT_SIZE) {
        MallocBlock *new_block = (MallocBlock *)((uintptr_t)block + sizeof(MallocBlock) + size);

        assert((uintptr_t)new_block % ALIGN == 0);
        new_block->prev = block;
        new_block->next = block->next;
        new_block->size = remaining_space - sizeof(MallocBlock);
        new_block->inuse = false;

        block->size = size;
        if (block->next) {
            block->next->prev = new_block;
        }
        block->next = new_block;

        // If the next block is empty then merge the new free block with the
        // free block that follows it.
        MallocBlock *following = new_block->next;
        if (following && !following->inuse) {
            new_block->size += following->size + sizeof(MallocBlock);
            new_block->next = following->next;
            if (following->next) {
                following->next->prev = new_block;
            }
        }
    }
}

MallocZone* MallocZone::create(uintptr_t start, size_t size)
{
    assert(start);
    assert(size > sizeof(MallocZone));

    // If the buffer is not 4-byte aligned then make it so.
    if (start % 4 != 0) {
        start += (4 - start % 4);
    }

    MallocZone *zone = new ((void *)start) MallocZone;

    // The first block is placed at the address immediately after the header.
    uintptr_t first_block_addr = (uintptr_t)zone + sizeof(MallocZone);

    MallocBlock *first = zone->head_ = (MallocBlock *)first_block_addr;

    first->prev = nullptr;
    first->next = nullptr;
    first->size = size - sizeof(MallocZone) - sizeof(MallocBlock);
    first->inuse = false;

    return zone;
}

MallocZone::MallocZone()
 : head_(nullptr)
{}

void* MallocZone::malloc(size_t size)
{
    // Blocks for allocations are always multiples of four bytes in size.
    // This ensures that blocks are always aligned on four byte boundaries
    // given that the initial block is also aligned on a four byte boundary.
    size = round_up_block_size(size);

    MallocBlock *best = nullptr;

    // Get the smallest free block that is large enough to satisfy the request.
    for (MallocBlock *block = head_; block; block = block->next) {
        if (block->size >= size
            && !block->inuse
            && (!best || (block->size < best->size))) {
            best = block;
        }
    }

    if (!best) {
        return nullptr;
    }

    consider_splitting_block(best, size);

    best->inuse = true;
    return (void *)((uintptr_t)best + sizeof(MallocBlock));
}

void MallocZone::free(void *ptr)
{
    if (!ptr) {
        return; // do nothing
    }

    MallocBlock *block = (MallocBlock *)((uintptr_t)ptr - sizeof(MallocBlock));

    // Walk over the heap and see if we can find this allocation.
    // If we cannot find it then the calling code has an error in it.
#ifndef NDEBUG
    bool found_it = false;
    for (MallocBlock *iter = head_; iter; iter = iter->next) {
        if (iter == block) {
            found_it = true;
        }
    }
    assert(found_it);
    assert(block->inuse);
#endif

    block->inuse = false;

    MallocBlock *preceding = block->prev, *following = block->next;

    // If the preceding chunk is free then merge this one into it. This block
    // goes away and the preceding chunk expands to fill the hole.
    if (preceding && !preceding->inuse) {
        preceding->size += block->size + sizeof(MallocBlock);
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
        block->size += following->size + sizeof(MallocBlock);
        block->next = following->next;
        if (following->next) {
            following->next->prev = block;
        }
    }
}

void* MallocZone::realloc(void *ptr, size_t new_size)
{
    if (!ptr) {
        return malloc(new_size);
    }

    MallocBlock *block = (MallocBlock *)((uintptr_t)ptr - sizeof(MallocBlock));
    assert(block->inuse);

    // Walk over the heap and see if we can find this allocation.
    // If we cannot find it then the calling code has an error in it.
#ifndef NDEBUG
    bool found_it = false;
    for (MallocBlock *iter = head_; iter; iter = iter->next) {
        if (iter == block) {
            found_it = true;
        }
    }
    assert(found_it);
#endif

    if (new_size == 0) {
        free(ptr);
        return malloc(0);
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

    MallocBlock *following = block->next;

    // If this block is followed by a free block then would it satisfy our
    // requirements to take some of that free space by extending this block?
    if (following
        && !following->inuse
        && (block->size + following->size + sizeof(MallocBlock)) >= new_size) {

        // Remove the following block, extending this one so as to not leave a
        // hole in the zone.
        block->size = block->size + following->size + sizeof(MallocBlock);
        block->next = following->next;

        if (following->next) {
            following->next->prev = block;
        }

        // Split the remaining free space if there's enough of it.
        consider_splitting_block(block, new_size);

        return ptr;
    }

    // Can we allocate a new block of memory for the resized allocation?
    void *new_alloc = malloc(new_size);
    if (new_alloc) {
        memcpy(ptr, new_alloc, block->size);
        free(ptr);
        return new_alloc;
    }

    MallocBlock *preceding = block->prev;

    // If this block is preceded by a free block then would it satisfy our
    // requirements to merge into the preceding block?
    if (preceding
        && !preceding->inuse
        && (block->size + preceding->size + sizeof(MallocBlock)) >= new_size) {

        // Remove this block, extending the preceding one so as to not leave a
        // hole in the zone.
        preceding->next = following;
        if (following) {
            following->prev = preceding;
        }
        preceding->size = block->size + preceding->size + sizeof(MallocBlock);
        preceding->inuse = true;

        // Move the contents to the beginning of the new, combined block.
        new_alloc = (void *)((uintptr_t)preceding + sizeof(MallocBlock));
        memmove(ptr, new_alloc, block->size);

        // Split the remaining free space if there's enough of it.
        consider_splitting_block(preceding, new_size);

        return new_alloc;
    }

    return nullptr;
}
