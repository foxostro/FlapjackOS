#include <stdio.h>
#include <stdbool.h>
#include <check.h>

START_TEST(test_example)
{
    fail_if(false, "foo");
}
END_TEST

Suite* create_test_suite(void)
{
    Suite *suite = suite_create("test_suite_example");
    TCase *testCase = tcase_create("test_example");
    tcase_add_test(testCase, test_example);
    suite_add_tcase(suite, testCase);
    return suite;
}

int main(int __unused argc, char * __unused argv[])
{
    int numberOfFailedTests;
    Suite *suite = create_test_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    numberOfFailedTests = srunner_ntests_failed(runner);
    srunner_free(runner);
    return numberOfFailedTests;
}
