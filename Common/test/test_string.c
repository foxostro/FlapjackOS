#include "flapjack_libc/string.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <check.h>
#include <assert.h>

START_TEST(test_strlen)
{
    const char str[] = "0123456789";
    size_t len = STRLEN(str);
    ck_assert_uint_eq(len, 10);
}
END_TEST

// When the string is NULL then STRLEN returns a length of zero.
START_TEST(test_strlen_null)
{
    size_t len = STRLEN(NULL);
    ck_assert_uint_eq(len, 0);
}
END_TEST

START_TEST(test_strnlen)
{
    const char str[] = "0123456789";
    size_t len = STRNLEN(str, sizeof(str));
    ck_assert_uint_eq(len, 10);
}
END_TEST

// When the string is NULL then STRNLEN returns a length of zero.
START_TEST(test_strnlen_null)
{
    size_t len = STRNLEN(NULL, 0);
    ck_assert_uint_eq(len, 0);
}
END_TEST

// STRNLEN will not exceed the specified character count while examining the
// string.
START_TEST(test_strnlen_exceed_the_count)
{
    const char str[] = "0123456789";
    size_t len = STRNLEN(str, 4);
    ck_assert_uint_eq(len, 4);
}
END_TEST

// Test that MEMCPY copies the entirety of the source buffer.
START_TEST(test_memcpy)
{
    const char src[] = "source string here";
    char dst[sizeof(src)] = {0};

    void *r = MEMCPY(dst, src, strlen(src));
    ck_assert_str_eq(src, dst);
    ck_assert_str_eq(src, "source string here");
    ck_assert_ptr_eq(r, dst);
}
END_TEST

// When size is zero then MEMCPY does nothing.
START_TEST(test_memcpy_zerolen)
{
    const char src[] = "source string here!!";
    char dst[] = "buffer buffer buffer";

    void *r = MEMCPY(dst, src, 0);
    ck_assert_str_eq(dst, "buffer buffer buffer");
    ck_assert_str_eq(src, "source string here!!");
    ck_assert_ptr_eq(r, dst);
}
END_TEST

// When size is zero and buffers are NULL then MEMCPY does nothing.
// So long as size is zero, we expect MEMCPY to do nothing and accept a NULL
// src and/or a NULL dst.
START_TEST(test_memcpy_zerolen_null)
{
    void *r = MEMCPY(NULL, NULL, 0);
    ck_assert_ptr_eq(r, NULL);
}
END_TEST

// Test that MEMMOVE copies the buffers correctly when buffers do not overlap.
START_TEST(test_memmove_non_overlapping)
{
    const char src[] = "source string here";
    char dst[sizeof(src)] = {0};

    void *r = MEMMOVE(dst, src, strlen(src));
    ck_assert_str_eq(src, dst);
    ck_assert_str_eq(src, "source string here");
    ck_assert_ptr_eq(r, dst);
}
END_TEST

// When the buffers are the same address then MEMMOVE does nothing.
START_TEST(test_memmove_same)
{
    const char expected[] = "source string here";
    char buffer[] = "source string here";
    const char *src = buffer;
    char *dst = buffer;

    void *r = MEMMOVE(dst, src, strlen(expected));
    ck_assert_str_eq(dst, expected);
    ck_assert_str_eq(src, expected);
    ck_assert_ptr_eq(r, dst);
}
END_TEST

// Test that MEMMOVE copies the buffers correctly when buffers are overlapping
// and the dst buffer comes before the source buffer.
START_TEST(test_memmove_overlapping_before)
{
    char buffer[] = "source string here!!";
    const char *src = buffer + 7;
    char *dst = buffer;

    ck_assert_str_eq(src, "string here!!");
    ck_assert_str_eq(dst, "source string here!!");
    void *r = MEMMOVE(dst, src, strlen("string here!!"));
    ck_assert_str_eq(dst, "string here!! here!!");
    ck_assert_str_eq(src, "here!! here!!");
    ck_assert_ptr_eq(r, dst);
}
END_TEST

// Test that MEMMOVE copies the buffers correctly when buffers are overlapping
// and the dst buffer comes after the source buffer.
START_TEST(test_memmove_overlapping_after)
{
    char buffer[] = "source string here!!";
    const char *src = buffer;
    char *dst = buffer + 7;

    ck_assert_str_eq(src, "source string here!!");
    ck_assert_str_eq(dst, "string here!!");
    void *r = MEMMOVE(dst, src, strlen("source string"));
    ck_assert_str_eq(dst, "source string");
    ck_assert_str_eq(src, "source source string");
    ck_assert_ptr_eq(r, dst);
}
END_TEST

// Test that MEMMOVE works correctly when size is zero.
START_TEST(test_memmove_zerolen)
{
    char buffer[] = "source string here!!";
    const char *src = buffer;
    char *dst = buffer + 7;

    ck_assert_str_eq(src, "source string here!!");
    ck_assert_str_eq(dst, "string here!!");
    void *r = MEMMOVE(dst, src, 0);
    ck_assert_str_eq(src, "source string here!!");
    ck_assert_str_eq(dst, "string here!!");
    ck_assert_ptr_eq(r, dst);
}
END_TEST

// When size is zero and buffers are NULL then MEMMOVE does nothing.
// So long as size is zero, we expect MEMMOVE to do nothing and accept a NULL
// src and/or a NULL dst.
START_TEST(test_memmove_zerolen_null)
{
    void *r = MEMMOVE(NULL, NULL, 0);
    ck_assert_ptr_eq(r, NULL);
}
END_TEST

// Test that MEMSET correctly sets the buffer values.
START_TEST(test_memset)
{
    char buffer[] = "buffer buffer buffer";
    void *r = MEMSET(buffer, 'X', strlen("buffer"));
    ck_assert_str_eq(buffer, "XXXXXX buffer buffer");
    ck_assert_ptr_eq(r, buffer);
}
END_TEST

// Test that MEMSET correctly sets the buffer values when length is zero.
START_TEST(test_memset_zerolen)
{
    char buffer[] = "buffer buffer buffer";
    void *r = MEMSET(buffer, 'X', 0);
    ck_assert_str_eq(buffer, "buffer buffer buffer");
    ck_assert_ptr_eq(r, buffer);
}
END_TEST

// When the length is zero and the buffer is NULL then MEMSET does nothing.
START_TEST(test_memset_zerolen_null)
{
    void *r = MEMSET(NULL, '?', 0);
    ck_assert_ptr_eq(r, NULL);
}
END_TEST

static const struct { char *name; void *fn; } tests[] = {
    { "test_strlen", test_strlen },
    { "test_strlen_null", test_strlen_null },
    { "test_strnlen", test_strnlen },
    { "test_strnlen_null", test_strnlen_null },
    { "test_strnlen_exceed_the_count", test_strnlen_exceed_the_count },
    { "test_memcpy", test_memcpy },
    { "test_memcpy_zerolen", test_memcpy_zerolen },
    { "test_memcpy_zerolen_null", test_memcpy_zerolen_null },
    { "test_memmove_non_overlapping", test_memmove_non_overlapping },
    { "test_memmove_same", test_memmove_same },
    { "test_memmove_overlapping_before", test_memmove_overlapping_before },
    { "test_memmove_overlapping_after", test_memmove_overlapping_after },
    { "test_memmove_zerolen", test_memmove_zerolen },
    { "test_memmove_zerolen_null", test_memmove_zerolen_null },
    { "test_memset", test_memset },
    { "test_memset_zerolen", test_memset_zerolen },
    { "test_memset_zerolen_null", test_memset_zerolen_null },
};
static const size_t num_tests = sizeof(tests) / sizeof(tests[0]);

Suite* test_suite_string()
{
    Suite *suite = suite_create(__FUNCTION__);

    for (size_t i = 0; i < num_tests; ++i) {
        TCase *testCase = tcase_create(tests[i].name);
        _tcase_add_test(testCase, tests[i].fn, tests[i].name, 0, 0, 0, 1);
        suite_add_tcase(suite, testCase);        
    }

    return suite;
}
