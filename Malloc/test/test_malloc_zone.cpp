#include "catch.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <cassert>
#include <cstring>

#include <malloc/malloc_zone.hpp>

constexpr unsigned VERBOSE = 0;
constexpr unsigned SMALL = 64;

static uint8_t s_buffer[1024];

TEST_CASE("test_init", "[Malloc]")
{
    void *buffer = s_buffer + 3; // break alignment intentionally for test
    size_t size = sizeof(s_buffer) - 3;

    memset(buffer, 0, size);
    MallocZone *zone = MallocZone::create((uintptr_t)buffer, size);

    REQUIRE(zone != nullptr);

    // alignment
    REQUIRE((uintptr_t)s_buffer % 4 == 0);
    REQUIRE((uintptr_t)buffer % 4 == 3);
    REQUIRE((uintptr_t)zone % 4 == 0); // four byte alignment

    // zone initially contains one large empty block
    REQUIRE(zone->head() != nullptr);
    REQUIRE(zone->head()->prev == nullptr);
    REQUIRE(zone->head()->next == nullptr);
    REQUIRE(zone->head()->size < size);
    REQUIRE(!zone->head()->inuse);
}

// Starting with an empty zone, we cannot satisfy an allocation request larger
// than the size of the zone itself.
TEST_CASE("test_malloc_really_big", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));
    void *allocation = zone->malloc(SIZE_MAX);
    REQUIRE(allocation == nullptr);
}

// Starting with an empty zone, we should be able to satisfy a small request.
TEST_CASE("test_malloc_one_small_request", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));
    void *allocation = zone->malloc(64);
    REQUIRE(allocation != nullptr);
    REQUIRE((uintptr_t)allocation % 4 == 0);
}

// Starting with an empty zone, we should be able to satisfy a request for a
// size zero block. This returns a minimum size heap block.
TEST_CASE("test_malloc_one_smallest_request", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));
    void *allocation = zone->malloc(SMALL);
    REQUIRE(allocation != nullptr);
    REQUIRE((uintptr_t)allocation % 4 == 0);
}

// Starting with an empty zone, we should be able to satisfy a request the size
// of the zone's free space.
TEST_CASE("test_malloc_whole_thing", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));
    void *allocation = zone->malloc(zone->head()->size);
    REQUIRE(allocation != nullptr);
    REQUIRE((uintptr_t)allocation % 4 == 0);
}

// Starting with an empty zone, we should be able to several small requests.
TEST_CASE("test_malloc_several_small", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    size_t size = SMALL, i = 0;
    size_t ex = (sizeof(s_buffer) - sizeof(MallocZone)) / (sizeof(MallocBlock) + size);

    while (true) {
        void *allocation = zone->malloc(size);
        REQUIRE((uintptr_t)allocation % 4 == 0);
        if (!allocation) {
            break;
        }
        ++i;
    }

    REQUIRE(i >= ex);
}

// We should be able to allocate one, free one, and then allocate another.
TEST_CASE("test_malloc_one_free_one", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));
    void *alloc = zone->malloc(zone->head()->size);
    REQUIRE(alloc != nullptr);
    REQUIRE(zone->malloc(zone->head()->size) == nullptr);
    zone->free(alloc);
    alloc = zone->malloc(zone->head()->size);
    REQUIRE(alloc != nullptr);
}

// Freeing allocations should release memory to the zone for future allocations.
TEST_CASE("test_malloc_several_free_one", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));
    void *allocation = zone->malloc(SMALL);
    while (zone->malloc(SMALL));
    zone->free(allocation);
    allocation = zone->malloc(SMALL);
    REQUIRE(allocation != nullptr);
}

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the zone. Free two and then see whether we can
// use the coalesced free space to allocate a new block. This tests merging of
// a preceding free block.
TEST_CASE("test_coalesce_0", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(SMALL);
    REQUIRE(a);

    void *b = zone->malloc(SMALL);
    REQUIRE(b);

    void *c = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - 2*(SMALL+sizeof(MallocBlock)) - sizeof(MallocBlock));
    REQUIRE(c);

    void *d = zone->malloc(SMALL*2);
    REQUIRE(!d); // expected to fail

    zone->free(a);
    zone->free(b); // merge with preceding free block

    void *e = zone->malloc(SMALL*2);
    REQUIRE(e); // should succeed now
    REQUIRE(a == e); // using the same block as `a'
}

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the zone. Free two and then see whether we can
// use the coalesced free space to allocate a new block. This tests merging of
// a following free block.
TEST_CASE("test_coalesce_1", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(SMALL);
    REQUIRE(a);

    void *b = zone->malloc(SMALL);
    REQUIRE(b);

    void *c = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - 2*(SMALL+sizeof(MallocBlock)) - sizeof(MallocBlock));
    REQUIRE(c);

    void *d = zone->malloc(SMALL*2);
    REQUIRE(!d); // expected to fail

    zone->free(b);
    zone->free(a); // merge with the following free block

    void *e = zone->malloc(SMALL*2);
    REQUIRE(e); // should succeed now
    REQUIRE(a == e); // using the same block as `a'
}

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the zone. Free three and then see whether we
// can use the coalesced free space to allocate a new block. This tests merging
// a preceding and following free block in one step.
TEST_CASE("test_coalesce_2", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(SMALL);
    REQUIRE(a);

    void *b = zone->malloc(SMALL);
    REQUIRE(b);

    void *c = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - 2*(SMALL+sizeof(MallocBlock)) - sizeof(MallocBlock));
    REQUIRE(c);

    void *d = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - sizeof(MallocBlock));
    REQUIRE(!d); // expected to fail

    zone->free(c);
    zone->free(a);
    zone->free(b); // preceding and following both merged here

    void *e = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - sizeof(MallocBlock));
    REQUIRE(e); // should succeed now
    REQUIRE(a == e); // using the same block as `a'
}

// Realloc extends a live allocation into the free space following it.
// If the block already has capacity then there's no need to change anything.
TEST_CASE("test_realloc_extend_0", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(1);
    REQUIRE(a);
    REQUIRE(zone->head()->next != nullptr);

    MallocBlock *block1 = zone->head();
    MallocBlock *block2 = zone->head()->next;

    size_t size1 = block1->size;
    size_t Size2_ = block2->size;

    void *b = zone->realloc(a, 2);
    REQUIRE(b);
    REQUIRE(a == b);

    REQUIRE(zone->head() == block1);
    REQUIRE(zone->head()->next == block2);
    REQUIRE(block1->size == size1);
    REQUIRE(block2->size == Size2_);
}

// If realloc cannot extend the current allocation into the following block
// then it allocates another chunk of memory for the new allocation and moves
// it there.
TEST_CASE("test_realloc_extend_1", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(0);
    REQUIRE(a);
    REQUIRE(zone->head()->next != nullptr);

    void *b = zone->realloc(a, SMALL);
    REQUIRE(b);
    REQUIRE(a == b);

    MallocBlock *block1 = zone->head();
    REQUIRE(block1);

    MallocBlock *block2 = zone->head()->next;
    REQUIRE(block2);

    REQUIRE(block1->size >= SMALL);
    REQUIRE(block2->size >= sizeof(s_buffer) - sizeof(MallocZone) - 2*sizeof(MallocBlock) - SMALL);
}

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation.
TEST_CASE("test_realloc_relocate_0", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(SMALL);
    REQUIRE(a);

    void *b = zone->malloc(SMALL);
    REQUIRE(b);

    void *c = zone->realloc(a, 2*SMALL);
    REQUIRE(c);
    REQUIRE(a != c);

    size_t count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        ++count;
    }
    REQUIRE(count == 4);

    size_t expected_size[] = {
        SMALL,
        SMALL,
        2*SMALL,
        sizeof(s_buffer) - sizeof(MallocZone) - 4*SMALL - 4*sizeof(MallocBlock)
    };
    size_t expected_inuse[] = {
        false,
        true,
        true,
        false
    };
    count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        REQUIRE(block->size == expected_size[count]);
        REQUIRE(block->inuse == expected_inuse[count]);
        ++count;
    }
}

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation. However, if the second
// allocation fails then realloc returns NULL.
TEST_CASE("test_realloc_relocate_1", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(SMALL);
    REQUIRE(a);

    void *b = zone->malloc(SMALL);
    REQUIRE(b);

    void *c = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - 2*(SMALL+sizeof(MallocBlock)) - sizeof(MallocBlock));
    REQUIRE(c);

    void *d = zone->realloc(a, 2*SMALL);
    REQUIRE(!d);

    size_t count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        ++count;
    }
    REQUIRE(count == 3);

    size_t expected_size[] = {
        SMALL,
        SMALL,
        sizeof(s_buffer) - sizeof(MallocZone) - 2*(SMALL+sizeof(MallocBlock)) - sizeof(MallocBlock)
    };
    size_t expected_inuse[] = {
        true,
        true,
        true
    };
    count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        REQUIRE(block->size == expected_size[count]);
        REQUIRE(block->inuse == expected_inuse[count]);
        ++count;
    }
}

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation. In the case where the
// preceding block has space then we merge the block into the preceding block
// and move the allocation. This allows realloc to succeed in the case where
// the only space large enough is the combined space of the preceding block
// and the current block.
TEST_CASE("test_realloc_relocate_2", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(SMALL);
    REQUIRE(a);

    void *b = zone->malloc(SMALL);
    REQUIRE(b);

    void *c = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - 2*(SMALL+sizeof(MallocBlock)) - sizeof(MallocBlock));
    REQUIRE(c);

    zone->free(a);

#if VERBOSE
    printf("Before:\n");
    for (malloc_block *block = zone->head(); block; block = block->next) {
        printf("size = %zu ; inuse = %d\n", block->size, block->inuse);
    }
#endif

    void *d = zone->realloc(b, 2*SMALL);
    REQUIRE(d);
    REQUIRE(a == d);

#if VERBOSE
    printf("After:\n");
    for (malloc_block *block = zone->head(); block; block = block->next) {
        printf("size = %zu ; inuse = %d\n", block->size, block->inuse);
    }
#endif

    size_t count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        ++count;
    }
    REQUIRE(count == 2);

    size_t expected_size[] = {
        2*SMALL + sizeof(MallocBlock),
        sizeof(s_buffer) - sizeof(MallocZone) - 3*sizeof(MallocBlock) - 2*SMALL
    };
    size_t expected_inuse[] = {
        true,
        true,
    };
    count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        REQUIRE(block->size == expected_size[count]);
        REQUIRE(block->inuse == expected_inuse[count]);
        ++count;
    }
}

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation. In the case where the
// preceding block has space then we merge the block into the preceding block
// and move the allocation. This allows realloc to succeed in the case where
// the only space large enough is the combined space of the preceding block
// and the current block.
TEST_CASE("test_realloc_shrink", "[Malloc]")
{
    memset(s_buffer, 0, sizeof(s_buffer));
    MallocZone *zone = MallocZone::create((uintptr_t)s_buffer, sizeof(s_buffer));

    void *a = zone->malloc(sizeof(s_buffer) - sizeof(MallocZone) - sizeof(MallocBlock));
    REQUIRE(a);

    void *d = zone->realloc(a, SMALL);
    REQUIRE(d);
    REQUIRE(a == d);

    size_t count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        ++count;
    }
    REQUIRE(count == 2);

    size_t expected_size[] = {
        SMALL,
        sizeof(s_buffer) - sizeof(MallocZone) - 2*sizeof(MallocBlock) - SMALL
    };
    size_t expected_inuse[] = {
        true,
        false,
    };
    count = 0;
    for (MallocBlock *block = zone->head(); block; block = block->next) {
        REQUIRE(block->size == expected_size[count]);
        REQUIRE(block->inuse == expected_inuse[count]);
        ++count;
    }
}
