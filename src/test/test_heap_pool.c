#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <check.h>
#include <assert.h>

#include <heap/heap_pool.h>

#define SMALL 64

static uint8_t s_buffer[1024];

START_TEST(test_init)
{
    void *buffer = s_buffer + 3; // break alignment intentionally for test
    size_t size = sizeof(s_buffer) - 3;

    bzero(buffer, size);
    heap_pool_t *pool = heap_pool_init(buffer, size);

    ck_assert_ptr_ne(pool, NULL);

    // alignment
    ck_assert_uint_eq((uintptr_t)s_buffer % 4, 0);
    ck_assert_uint_eq((uintptr_t)buffer % 4, 3);
    ck_assert_uint_eq((uintptr_t)pool % 4, 0); // four byte alignment

    // pool initially contains one large empty block
    ck_assert_ptr_ne(pool->head, NULL);
    ck_assert_ptr_eq(pool->head->prev, NULL);
    ck_assert_ptr_eq(pool->head->next, NULL);
    ck_assert_uint_lt(pool->head->size, size);
    ck_assert(!pool->head->inuse);
}
END_TEST

// Starting with an empty pool, we cannot satisfy an allocation request larger
// than the size of the pool itself.
START_TEST(test_malloc_really_big)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));
    void *allocation = heap_pool_malloc(pool, SIZE_MAX);
    ck_assert_ptr_eq(allocation, NULL);
}
END_TEST

// Starting with an empty pool, we should be able to satisfy a small request.
START_TEST(test_malloc_one_small_request)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));
    void *allocation = heap_pool_malloc(pool, 64);
    ck_assert_ptr_ne(allocation, NULL);
    ck_assert_uint_eq((uintptr_t)allocation % 4, 0);
}
END_TEST

// Starting with an empty pool, we should be able to satisfy a request for a
// size zero block. This returns a minimum size heap block.
START_TEST(test_malloc_one_smallest_request)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));
    void *allocation = heap_pool_malloc(pool, SMALL);
    ck_assert_ptr_ne(allocation, NULL);
    ck_assert_uint_eq((uintptr_t)allocation % 4, 0);
}
END_TEST

// Starting with an empty pool, we should be able to satisfy a request the size
// of the pool's free space.
START_TEST(test_malloc_whole_thing)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));
    void *allocation = heap_pool_malloc(pool, pool->head->size);
    ck_assert_ptr_ne(allocation, NULL);
    ck_assert_uint_eq((uintptr_t)allocation % 4, 0);
}
END_TEST

// Starting with an empty pool, we should be able to several small requests.
START_TEST(test_malloc_several_small)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));

    size_t size = SMALL, i = 0;
    size_t ex = (sizeof(s_buffer) - sizeof(heap_pool_t)) / (sizeof(heap_block_t) + size);

    while (true) {
        void *allocation = heap_pool_malloc(pool, size);
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
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));
    void *alloc = heap_pool_malloc(pool, pool->head->size);
    ck_assert_ptr_ne(alloc, NULL);
    ck_assert_ptr_eq(heap_pool_malloc(pool, pool->head->size), NULL);
    heap_pool_free(pool, alloc);
    alloc = heap_pool_malloc(pool, pool->head->size);
    ck_assert_ptr_ne(alloc, NULL);
}
END_TEST

// Freeing allocations should release memory to the pool for future allocations.
START_TEST(test_malloc_several_free_one)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));
    void *allocation = heap_pool_malloc(pool, SMALL);
    while (heap_pool_malloc(pool, SMALL));
    heap_pool_free(pool, allocation);
    allocation = heap_pool_malloc(pool, SMALL);
    ck_assert_ptr_ne(allocation, NULL);
}
END_TEST

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the pool. Free two and then see whether we can
// use the coalesced free space to allocate a new block. This tests merging of
// a preceding free block.
START_TEST(test_coalesce_0)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));

    void *a = heap_pool_malloc(pool, SMALL);
    ck_assert(a);

    void *b = heap_pool_malloc(pool, SMALL);
    ck_assert(b);

    void *c = heap_pool_malloc(pool, sizeof(s_buffer) - sizeof(heap_pool_t) - 2*(SMALL+sizeof(heap_block_t)) - sizeof(heap_block_t));
    ck_assert(c);

    void *d = heap_pool_malloc(pool, SMALL*2);
    ck_assert(!d); // expected to fail

    heap_pool_free(pool, a);
    heap_pool_free(pool, b); // merge with preceding free block

    void *e = heap_pool_malloc(pool, SMALL*2);
    ck_assert(e); // should succeed now
    ck_assert(a == e); // using the same block as `a'
}
END_TEST

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the pool. Free two and then see whether we can
// use the coalesced free space to allocate a new block. This tests merging of
// a following free block.
START_TEST(test_coalesce_1)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));

    void *a = heap_pool_malloc(pool, SMALL);
    ck_assert(a);

    void *b = heap_pool_malloc(pool, SMALL);
    ck_assert(b);

    void *c = heap_pool_malloc(pool, sizeof(s_buffer) - sizeof(heap_pool_t) - 2*(SMALL+sizeof(heap_block_t)) - sizeof(heap_block_t));
    ck_assert(c);

    void *d = heap_pool_malloc(pool, SMALL*2);
    ck_assert(!d); // expected to fail

    heap_pool_free(pool, b);
    heap_pool_free(pool, a); // merge with the following free block

    void *e = heap_pool_malloc(pool, SMALL*2);
    ck_assert(e); // should succeed now
    ck_assert(a == e); // using the same block as `a'
}
END_TEST

// Free blocks should coalesce to avoid fragmentation.
// Allocate three blocks to fill the pool. Free three and then see whether we
// can use the coalesced free space to allocate a new block. This tests merging
// a preceding and following free block in one step.
START_TEST(test_coalesce_2)
{
    bzero(s_buffer, sizeof(s_buffer));
    heap_pool_t *pool = heap_pool_init(s_buffer, sizeof(s_buffer));

    void *a = heap_pool_malloc(pool, SMALL);
    ck_assert(a);

    void *b = heap_pool_malloc(pool, SMALL);
    ck_assert(b);

    void *c = heap_pool_malloc(pool, sizeof(s_buffer) - sizeof(heap_pool_t) - 2*(SMALL+sizeof(heap_block_t)) - sizeof(heap_block_t));
    ck_assert(c);

    void *d = heap_pool_malloc(pool, sizeof(s_buffer) - sizeof(heap_pool_t) - sizeof(heap_block_t));
    ck_assert(!d); // expected to fail

    heap_pool_free(pool, c);
    heap_pool_free(pool, a);
    heap_pool_free(pool, b); // preceding and following both merged here

    void *e = heap_pool_malloc(pool, sizeof(s_buffer) - sizeof(heap_pool_t) - sizeof(heap_block_t));
    assert(e); // should succeed now
    ck_assert(a == e); // using the same block as `a'
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
};
static const size_t num_tests = sizeof(tests) / sizeof(tests[0]);

Suite* test_suite_heap_pool()
{
    Suite *suite = suite_create("test_suite_heap_pool");

    for (size_t i = 0; i < num_tests; ++i) {
        TCase *testCase = tcase_create(tests[i].name);
        _tcase_add_test(testCase, tests[i].fn, tests[i].name, 0, 0, 0, 1);
        suite_add_tcase(suite, testCase);        
    }

    return suite;
}
