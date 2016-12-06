#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <check.h>

#include "kprintf.h"

#define MIN(a, b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

// ksnprintf returns zero when the buffer is null
START_TEST(test_null_buffer)
{
    int c = ksnprintf(NULL, 1000, NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

// ksnprintf returns zero when the format string is null
START_TEST(test_null_format)
{
    char buf[32];
    int c = ksnprintf(buf, sizeof(buf), NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

// ksnprintf returns zero when the size is zero
START_TEST(test_zero_length)
{
    int c = ksnprintf(NULL, 0, NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

// ksnprintf copies the format string verbatim when there are no args
// (assuming enough space in the output buffer)
START_TEST(test_return_no_args_0)
{
    char fmt[] = "test test";
    char buf[32];
    int c = ksnprintf(buf, sizeof(buf), fmt);
    ck_assert_int_eq(c, strnlen(fmt, sizeof(fmt)));
    ck_assert(!strcmp(buf, fmt));
}
END_TEST

// ksnprintf copies the format string verbatim when there are no args
// If there's not enough space it truncates the string in the output buffer.
// The return value is the number of characters that would have been output had
// the buffer not been size restricted.
START_TEST(test_return_no_args_1)
{
    const char fmt[] = "test test";
    char buf[5];
    int c = ksnprintf(buf, sizeof(buf), fmt);
    ck_assert_int_eq(c, strnlen(fmt, sizeof(fmt) - 1));
    ck_assert_str_eq(buf, "test");
}
END_TEST

START_TEST(test_return_char_arg)
{
    static const char expected_str[] = "test a";
    char buf[32];
    int c = ksnprintf(buf, sizeof(buf), "test %c", 'a');
    ck_assert_int_eq(c, strnlen(expected_str, sizeof(expected_str) - 1));
    ck_assert_str_eq(buf, expected_str);
}
END_TEST

START_TEST(test_return_format_percent)
{
    static const char expected_str[] = "test %";
    char buf[32];
    int c = ksnprintf(buf, sizeof(buf), "test %%");
    ck_assert_int_eq(c, strnlen(expected_str, sizeof(expected_str) - 1));
    ck_assert_str_eq(buf, expected_str);
}
END_TEST

// ksnprintf can format integer arguments
START_TEST(test_one_int_arg)
{
    static const char expected_str[] = "test 123";
    char buf[32];
    int c = ksnprintf(buf, sizeof(buf), "test %d", 123);
    ck_assert_int_eq(c, strnlen(expected_str, sizeof(expected_str) - 1));
    ck_assert_str_eq(buf, expected_str);
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
    { "test_one_int_arg", test_one_int_arg },
};
static const size_t num_tests = sizeof(tests) / sizeof(tests[0]);

Suite* test_suite_printf(void)
{
    Suite *suite = suite_create("test_suite_printf");

    for(size_t i = 0; i < num_tests; ++i)
    {
        TCase *testCase = tcase_create(tests[i].name);
        tcase_add_test(testCase, tests[i].fn);
        suite_add_tcase(suite, testCase);        
    }

    return suite;
}
