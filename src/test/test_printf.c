#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <check.h>

#include "kprintf.h"

START_TEST(test_null_string)
{
    int c = ksnprintf(NULL, 1000, NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

START_TEST(test_zero_length)
{
    int c = ksnprintf(NULL, 0, NULL);
    ck_assert_int_eq(c, 0);
}
END_TEST

START_TEST(test_return_no_args)
{
    char fmt[] = "test test";
    char buf[32];
    int c = ksnprintf(buf, sizeof(buf), fmt);
    ck_assert_int_eq(c, strlen(fmt));
    ck_assert(!strcmp(buf, fmt));
}
END_TEST

START_TEST(test_one_int_arg)
{
    static const char expected_str[] = "test 123";
    char buf[32];
    int c = ksnprintf(buf, sizeof(buf), "test %d", 123);
    ck_assert_int_eq(c, strlen(expected_str));
    ck_assert(!strcmp(buf, expected_str));
}
END_TEST

static const struct { char *name; void *fn; } tests[] = {
    { "test_null_string", test_null_string },
    { "test_zero_length", test_zero_length },
    { "test_return_no_args", test_return_no_args },
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
