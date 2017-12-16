#include "flapjack_libc/stdio.h"

#include <stdbool.h>
#include <stddef.h>
#include <check.h>
#include <stdio.h>
#include <assert.h>

// SNPRINTF returns zero when the buffer is null
START_TEST(test_null_buffer)
{
    int c = SNPRINTF(NULL, 1000, NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

// SNPRINTF returns zero when the format string is null
START_TEST(test_null_format)
{
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

// SNPRINTF returns zero when the size is zero
START_TEST(test_zero_length)
{
    int c = SNPRINTF(NULL, 0, NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

// SNPRINTF copies the format string verbatim when there are no args
// (assuming enough space in the output buffer)
START_TEST(test_return_no_args_0)
{
    const char fmt[] = "test test";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), fmt);
    ck_assert_int_eq(c, strlen(fmt));
    ck_assert(!strcmp(buf, fmt));

    // ensure behavior is the same as libc
    char libcbuf[32] = {0};
    int d = snprintf(libcbuf, sizeof(libcbuf), fmt);
    ck_assert_str_eq(buf, libcbuf);
    ck_assert_int_eq(c, d);
}
END_TEST

// SNPRINTF copies the format string verbatim when there are no args
// If there's not enough space it truncates the string in the output buffer.
// The return value is the number of characters that would have been output had
// the buffer not been size restricted.
START_TEST(test_return_no_args_1)
{
    const char fmt[] = "test test";
    char buf[5];
    int c = SNPRINTF(buf, sizeof(buf), fmt);
    ck_assert_int_eq(c, strlen(fmt));
    ck_assert_str_eq(buf, "test");

    // ensure behavior is the same as libc
    char libcbuf[5];
    int d = snprintf(libcbuf, sizeof(libcbuf), fmt);
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

START_TEST(test_return_char_arg)
{
    static const char expected[] = "test a";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %c", 'a');
    ck_assert_int_eq(c, strlen(expected));
    ck_assert_str_eq(buf, expected);

    // ensure behavior is the same as libc
    char libcbuf[32];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %c", 'a');
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

START_TEST(test_return_format_percent)
{
    static const char expected[] = "test %";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %%");
    ck_assert_int_eq(c, strlen(expected));
    ck_assert_str_eq(buf, expected);

    // ensure behavior is the same as libc
    char libcbuf[32];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %%");
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

// SNPRINTF can format string arguments
START_TEST(test_string_arg_0)
{
    static const char expected[] = "test foo";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %s", "foo");
    ck_assert_str_eq(buf, expected);
    ck_assert_int_eq(c, strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[32];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %s", "foo");
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

// SNPRINTF can format string arguments
// If the string is NULL then we format as "NULL".
START_TEST(test_string_arg_1)
{
    static const char expected[] = "test (null)";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %s", NULL);
    ck_assert_str_eq(buf, expected);
    ck_assert_int_eq(c, strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[32];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %s", NULL);
    ck_assert_str_eq(buf, libcbuf);
    ck_assert_int_eq(c, d);
}
END_TEST

// SNPRINTF can format string arguments
// If the output buffer cannot contain the entire string then it truncates.
START_TEST(test_string_arg_2)
{
    static const char expected[] = "test";
    char buf[5];
    int c = SNPRINTF(buf, sizeof(buf), "test %s", NULL);
    ck_assert_str_eq(buf, expected);
    ck_assert_int_eq(c, strlen("test (null)"));

    // ensure behavior is the same as libc
    char libcbuf[5];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %s", NULL);
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

// SNPRINTF can format integer arguments
START_TEST(test_int_arg_0)
{
    static const char expected[] = "test -123";
    char buf[16];
    int c = SNPRINTF(buf, sizeof(buf), "test %d", -123);
    ck_assert_int_eq(c, strlen(expected));
    ck_assert_str_eq(buf, expected);

    // ensure behavior is the same as libc
    char libcbuf[16];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %d", -123);
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

// SNPRINTF can format integer arguments
// If the output buffer cannot contain the entire string then it truncates.
START_TEST(test_int_arg_1)
{
    static const char expected[] = "test 1"; // truncated by small buffer
    char buf[7];
    int c = SNPRINTF(buf, sizeof(buf), "test %d", 12345);
    ck_assert_str_eq(buf, expected);

    // The buffer wasn't long enough to hold the entire output string. This
    // return value is the number of characters (preceding the null terminator)
    // that would have been output had the buffer not been size restricted.
    ck_assert_int_eq(c, strlen("test 12345"));

    // ensure behavior is the same as libc
    char libcbuf[7];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %d", 12345);
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

// SNPRINTF can format hexadecimal integer arguments
START_TEST(test_hex_arg_0)
{
    static const char expected[] = "0xdeadbeef";
    char buf[16];
    int c = SNPRINTF(buf, sizeof(buf), "0x%x", 0xdeadbeef);
    ck_assert_str_eq(buf, expected);
    ck_assert_int_eq(c, strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[16];
    int d = snprintf(libcbuf, sizeof(libcbuf), "0x%x", 0xdeadbeef);
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

// SNPRINTF can format hexadecimal integer arguments
START_TEST(test_hex_arg_1)
{
    static const char expected[] = "0xCAFE10";
    char buf[16];
    int c = SNPRINTF(buf, sizeof(buf), "0x%X", 0xcafe10);
    ck_assert_str_eq(buf, expected);
    ck_assert_int_eq(c, strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[16];
    int d = snprintf(libcbuf, sizeof(libcbuf), "0x%X", 0xcafe10);
    ck_assert_int_eq(c, d);
    ck_assert_str_eq(buf, libcbuf);
}
END_TEST

// SNPRINTF can format pointers arguments too
START_TEST(test_ptr_arg)
{
    static const size_t N = 3;
    static const void *values[N] = {
        (void *)0xdeadbeef,
        (void *)0x00010000,
        (void *)0xffffffff
    };

    // We expect our %p to left-pad the pointer addresses with zeroes. Since we
    // build unit tests for a 64-bit host architecture, we need sixteen
    // characters for the address portion of the output.
    // This doesn't match the exact behavior of the libc on my system, but I
    // like it anyway.
    assert(sizeof(void*) == 8);
    static const void *expected[N] = {
        "ptr is 0x00000000deadbeef",
        "ptr is 0x0000000000010000",
        "ptr is 0x00000000ffffffff"
    };

    char buf[128];

    for (size_t i = 0; i < N; ++i) {
        int r = SNPRINTF(buf, sizeof(buf), "ptr is %p", values[i]);
        ck_assert_str_eq(buf, expected[i]);
        ck_assert_int_eq(r, strlen(expected[i]));
    }
}
END_TEST

static const struct { char *name; void *fn; } tests[] = {
    { "test_null_buffer", test_null_buffer },
    { "test_null_format", test_null_format },
    { "test_zero_length", test_zero_length },
    { "test_return_no_args_0", test_return_no_args_0 },
    { "test_return_no_args_1", test_return_no_args_1 },
    { "test_return_char_arg", test_return_char_arg },
    { "test_return_format_percent", test_return_format_percent },
    { "test_string_arg_0", test_string_arg_0 },
    { "test_string_arg_1", test_string_arg_1 },
    { "test_string_arg_2", test_string_arg_2 },
    { "test_int_arg_0", test_int_arg_0 },
    { "test_int_arg_1", test_int_arg_1 },
    { "test_hex_arg_0", test_hex_arg_0 },
    { "test_hex_arg_1", test_hex_arg_1 },
    { "test_ptr_arg", test_ptr_arg },
};
static const size_t num_tests = sizeof(tests) / sizeof(tests[0]);

Suite* test_suite_stdio(void)
{
    Suite *suite = suite_create(__FUNCTION__);

    for (size_t i = 0; i < num_tests; ++i) {
        TCase *testCase = tcase_create(tests[i].name);
        _tcase_add_test(testCase, tests[i].fn, tests[i].name, 0, 0, 0, 1);
        suite_add_tcase(suite, testCase);        
    }

    return suite;
}
