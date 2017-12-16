#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <check.h>
#include <assert.h>
#include <string.h>

#include <malloc/malloc_zone.h>

#define VERBOSE 0
#define SMALL 64

static uint8_t s_buffer[1024];

START_TEST(test_init)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    void *buffer = s_buffer + 3; // break alignment intentionally for test
    size_t size = sizeof(s_buffer) - 3;

    memset(buffer, 0, size);
    malloc_zone_t *zone = allocator->init(buffer, size);

    ck_assert_ptr_ne(zone, NULL);

    // alignment
    ck_assert_uint_eq((uintptr_t)s_buffer % 4, 0);
    ck_assert_uint_eq((uintptr_t)buffer % 4, 3);
    ck_assert_uint_eq((uintptr_t)zone % 4, 0); // four byte alignment

    // zone initially contains one large empty block
    ck_assert_ptr_ne(zone->head, NULL);
    ck_assert_ptr_eq(zone->head->prev, NULL);
    ck_assert_ptr_eq(zone->head->next, NULL);
    ck_assert_uint_lt(zone->head->size, size);
    ck_assert(!zone->head->inuse);
}
END_TEST

// Starting with an empty zone, we cannot satisfy an allocation request larger
// than the size of the zone itself.
START_TEST(test_malloc_really_big)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));
    void *allocation = allocator->malloc(zone, SIZE_MAX);
    ck_assert_ptr_eq(allocation, NULL);
}
END_TEST

// Starting with an empty zone, we should be able to satisfy a small request.
START_TEST(test_malloc_one_small_request)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));
    void *allocation = allocator->malloc(zone, 64);
    ck_assert_ptr_ne(allocation, NULL);
    ck_assert_uint_eq((uintptr_t)allocation % 4, 0);
}
END_TEST

// Starting with an empty zone, we should be able to satisfy a request for a
// size zero block. This returns a minimum size heap block.
START_TEST(test_malloc_one_smallest_request)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));
    void *allocation = allocator->malloc(zone, SMALL);
    ck_assert_ptr_ne(allocation, NULL);
    ck_assert_uint_eq((uintptr_t)allocation % 4, 0);
}
END_TEST

// Starting with an empty zone, we should be able to satisfy a request the size
// of the zone's free space.
START_TEST(test_malloc_whole_thing)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));
    void *allocation = allocator->malloc(zone, zone->head->size);
    ck_assert_ptr_ne(allocation, NULL);
    ck_assert_uint_eq((uintptr_t)allocation % 4, 0);
}
END_TEST

// Starting with an empty zone, we should be able to several small requests.
START_TEST(test_malloc_several_small)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    size_t size = SMALL, i = 0;
    size_t ex = (sizeof(s_buffer) - sizeof(malloc_zone_t)) / (sizeof(malloc_block_t) + size);

    while (true) {
        void *allocation = allocator->malloc(zone, size);
        ck_assert_uint_eq((uintptr_t)allocation % 4, 0);
        if (!allocation) {
            break;
        }
        ++i;
    }

    ck_assert_uint_ge(i, ex);
}
END_TEST

// We should be able to allocate one, free one, and then allocate another.
START_TEST(test_malloc_one_free_one)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));
    void *alloc = allocator->malloc(zone, zone->head->size);
    ck_assert_ptr_ne(alloc, NULL);
    ck_assert_ptr_eq(allocator->malloc(zone, zone->head->size), NULL);
    allocator->free(zone, alloc);
    alloc = allocator->malloc(zone, zone->head->size);
    ck_assert_ptr_ne(alloc, NULL);
}
END_TEST

// Freeing allocations should release memory to the zone for future allocations.
START_TEST(test_malloc_several_free_one)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));
    void *allocation = allocator->malloc(zone, SMALL);
    while (allocator->malloc(zone, SMALL));
    allocator->free(zone, allocation);
    allocation = allocator->malloc(zone, SMALL);
    ck_assert_ptr_ne(allocation, NULL);
}
END_TEST

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the zone. Free two and then see whether we can
// use the coalesced free space to allocate a new block. This tests merging of
// a preceding free block.
START_TEST(test_coalesce_0)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, SMALL);
    ck_assert(a);

    void *b = allocator->malloc(zone, SMALL);
    ck_assert(b);

    void *c = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*(SMALL+sizeof(malloc_block_t)) - sizeof(malloc_block_t));
    ck_assert(c);

    void *d = allocator->malloc(zone, SMALL*2);
    ck_assert(!d); // expected to fail

    allocator->free(zone, a);
    allocator->free(zone, b); // merge with preceding free block

    void *e = allocator->malloc(zone, SMALL*2);
    ck_assert(e); // should succeed now
    ck_assert_ptr_eq(a, e); // using the same block as `a'
}
END_TEST

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the zone. Free two and then see whether we can
// use the coalesced free space to allocate a new block. This tests merging of
// a following free block.
START_TEST(test_coalesce_1)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, SMALL);
    ck_assert(a);

    void *b = allocator->malloc(zone, SMALL);
    ck_assert(b);

    void *c = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*(SMALL+sizeof(malloc_block_t)) - sizeof(malloc_block_t));
    ck_assert(c);

    void *d = allocator->malloc(zone, SMALL*2);
    ck_assert(!d); // expected to fail

    allocator->free(zone, b);
    allocator->free(zone, a); // merge with the following free block

    void *e = allocator->malloc(zone, SMALL*2);
    ck_assert(e); // should succeed now
    ck_assert_ptr_eq(a, e); // using the same block as `a'
}
END_TEST

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the zone. Free three and then see whether we
// can use the coalesced free space to allocate a new block. This tests merging
// a preceding and following free block in one step.
START_TEST(test_coalesce_2)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, SMALL);
    ck_assert(a);

    void *b = allocator->malloc(zone, SMALL);
    ck_assert(b);

    void *c = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*(SMALL+sizeof(malloc_block_t)) - sizeof(malloc_block_t));
    ck_assert(c);

    void *d = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - sizeof(malloc_block_t));
    ck_assert(!d); // expected to fail

    allocator->free(zone, c);
    allocator->free(zone, a);
    allocator->free(zone, b); // preceding and following both merged here

    void *e = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - sizeof(malloc_block_t));
    ck_assert(e); // should succeed now
    ck_assert_ptr_eq(a, e); // using the same block as `a'
}
END_TEST

// Realloc extends a live allocation into the free space following it.
// If the block already has capacity then there's no need to change anything.
START_TEST(test_realloc_extend_0)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, 1);
    ck_assert(a);
    ck_assert_ptr_ne(zone->head->next, NULL);

    malloc_block_t *block1 = zone->head;
    malloc_block_t *block2 = zone->head->next;

    size_t size1 = block1->size;
    size_t size2 = block2->size;

    void *b = allocator->realloc(zone, a, 2);
    ck_assert(b);
    ck_assert_ptr_eq(a, b);

    ck_assert_ptr_eq(zone->head, block1);
    ck_assert_ptr_eq(zone->head->next, block2);
    ck_assert_uint_eq(block1->size, size1);
    ck_assert_uint_eq(block2->size, size2);
}
END_TEST

// If realloc cannot extend the current allocation into the following block
// then it allocates another chunk of memory for the new allocation and moves
// it there.
START_TEST(test_realloc_extend_1)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, 0);
    ck_assert(a);
    ck_assert_ptr_ne(zone->head->next, NULL);

    void *b = allocator->realloc(zone, a, SMALL);
    ck_assert(b);
    ck_assert_ptr_eq(a, b);

    malloc_block_t *block1 = zone->head;
    ck_assert(block1);

    malloc_block_t *block2 = zone->head->next;
    ck_assert(block2);

    ck_assert_uint_ge(block1->size, SMALL);
    ck_assert_uint_ge(block2->size, sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*sizeof(malloc_block_t) - SMALL);
}
END_TEST

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation.
START_TEST(test_realloc_relocate_0)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, SMALL);
    ck_assert(a);

    void *b = allocator->malloc(zone, SMALL);
    ck_assert(b);

    void *c = allocator->realloc(zone, a, 2*SMALL);
    ck_assert(c);
    ck_assert_ptr_ne(a, c);

    size_t count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ++count;
    }
    ck_assert_uint_eq(count, 4);

    size_t expected_size[] = {
        SMALL,
        SMALL,
        2*SMALL,
        sizeof(s_buffer) - sizeof(malloc_zone_t) - 4*SMALL - 4*sizeof(malloc_block_t)
    };
    size_t expected_inuse[] = {
        false,
        true,
        true,
        false
    };
    count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ck_assert_uint_eq(block->size, expected_size[count]);
        ck_assert_uint_eq(block->inuse, expected_inuse[count]);
        ++count;
    }
}
END_TEST

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation. However, if the second
// allocation fails then realloc returns NULL.
START_TEST(test_realloc_relocate_1)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, SMALL);
    ck_assert(a);

    void *b = allocator->malloc(zone, SMALL);
    ck_assert(b);

    void *c = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*(SMALL+sizeof(malloc_block_t)) - sizeof(malloc_block_t));
    ck_assert(c);

    void *d = allocator->realloc(zone, a, 2*SMALL);
    ck_assert(!d);

    size_t count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ++count;
    }
    ck_assert_uint_eq(count, 3);

    size_t expected_size[] = {
        SMALL,
        SMALL,
        sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*(SMALL+sizeof(malloc_block_t)) - sizeof(malloc_block_t)
    };
    size_t expected_inuse[] = {
        true,
        true,
        true
    };
    count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ck_assert_uint_eq(block->size, expected_size[count]);
        ck_assert_uint_eq(block->inuse, expected_inuse[count]);
        ++count;
    }
}
END_TEST

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation. In the case where the
// preceding block has space then we merge the block into the preceding block
// and move the allocation. This allows realloc to succeed in the case where
// the only space large enough is the combined space of the preceding block
// and the current block.
START_TEST(test_realloc_relocate_2)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);

    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, SMALL);
    ck_assert(a);

    void *b = allocator->malloc(zone, SMALL);
    ck_assert(b);

    void *c = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*(SMALL+sizeof(malloc_block_t)) - sizeof(malloc_block_t));
    ck_assert(c);

    allocator->free(zone, a);

#if VERBOSE
    printf("Before:\n");
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        printf("size = %zu ; inuse = %d\n", block->size, block->inuse);
    }
#endif

    void *d = allocator->realloc(zone, b, 2*SMALL);
    ck_assert(d);
    ck_assert_ptr_eq(a, d);

#if VERBOSE
    printf("After:\n");
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        printf("size = %zu ; inuse = %d\n", block->size, block->inuse);
    }
#endif

    size_t count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ++count;
    }
    ck_assert_uint_eq(count, 2);

    size_t expected_size[] = {
        2*SMALL + sizeof(malloc_block_t),
        sizeof(s_buffer) - sizeof(malloc_zone_t) - 3*sizeof(malloc_block_t) - 2*SMALL
    };
    size_t expected_inuse[] = {
        true,
        true,
    };
    count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ck_assert_uint_eq(block->size, expected_size[count]);
        ck_assert_uint_eq(block->inuse, expected_inuse[count]);
        ++count;
    }
}
END_TEST

// Realloc tries to extend an allocation in place. If it cannot then it
// allocates a new block and moves the allocation. In the case where the
// preceding block has space then we merge the block into the preceding block
// and move the allocation. This allows realloc to succeed in the case where
// the only space large enough is the combined space of the preceding block
// and the current block.
START_TEST(test_realloc_shrink)
{
    malloc_interface_t _allocator;
    malloc_interface_t *allocator = &_allocator;
    get_malloc_interface(allocator);
    
    memset(s_buffer, 0, sizeof(s_buffer));
    malloc_zone_t *zone = allocator->init(s_buffer, sizeof(s_buffer));

    void *a = allocator->malloc(zone, sizeof(s_buffer) - sizeof(malloc_zone_t) - sizeof(malloc_block_t));
    ck_assert(a);

    void *d = allocator->realloc(zone, a, SMALL);
    ck_assert(d);
    ck_assert_ptr_eq(a, d);

    size_t count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ++count;
    }
    ck_assert_uint_eq(count, 2);

    size_t expected_size[] = {
        SMALL,
        sizeof(s_buffer) - sizeof(malloc_zone_t) - 2*sizeof(malloc_block_t) - SMALL
    };
    size_t expected_inuse[] = {
        true,
        false,
    };
    count = 0;
    for (malloc_block_t *block = zone->head; block; block = block->next) {
        ck_assert_uint_eq(block->size, expected_size[count]);
        ck_assert_uint_eq(block->inuse, expected_inuse[count]);
        ++count;
    }
}
END_TEST

static const struct { char *name; void *fn; } tests[] = {
    { "test_init", test_init },
    { "test_malloc_really_big", test_malloc_really_big },
    { "test_malloc_one_small_request", test_malloc_one_small_request },
    { "test_malloc_one_smallest_request", test_malloc_one_smallest_request },
    { "test_malloc_whole_thing", test_malloc_whole_thing },
    { "test_malloc_several_small", test_malloc_several_small },
    { "test_malloc_one_free_one", test_malloc_one_free_one },
    { "test_malloc_several_free_one", test_malloc_several_free_one },
    { "test_coalesce_0", test_coalesce_0 },
    { "test_coalesce_1", test_coalesce_1 },
    { "test_coalesce_2", test_coalesce_2 },
    { "test_realloc_extend_0", test_realloc_extend_0 },
    { "test_realloc_extend_1", test_realloc_extend_1 },
    { "test_realloc_relocate_0", test_realloc_relocate_0 },
    { "test_realloc_relocate_1", test_realloc_relocate_1 },
    { "test_realloc_relocate_2", test_realloc_relocate_2 },
    { "test_realloc_shrink", test_realloc_shrink },
};
static const size_t num_tests = sizeof(tests) / sizeof(tests[0]);

Suite* test_suite_malloc_zone(void)
{
    Suite *suite = suite_create(__FUNCTION__);

    for (size_t i = 0; i < num_tests; ++i) {
        TCase *testCase = tcase_create(tests[i].name);
        _tcase_add_test(testCase, tests[i].fn, tests[i].name, 0, 0, 0, 1);
        suite_add_tcase(suite, testCase);        
    }

    return suite;
}
