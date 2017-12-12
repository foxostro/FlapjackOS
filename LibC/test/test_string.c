#include "../include/string.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <check.h>
#include <assert.h>

START_TEST(test_foo)
{
    ck_assert(!"unimplemented");
}
END_TEST

static const struct { char *name; void *fn; } tests[] = {
    { "test_foo", test_foo },
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
