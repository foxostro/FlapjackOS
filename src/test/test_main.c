#include <stdio.h>
#include <check.h>

Suite* test_suite_printf(void); // defined in test_printf.c

int main(int __unused argc, char * __unused argv[])
{
    int numberOfFailedTests;
    Suite *suite = test_suite_printf();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    numberOfFailedTests = srunner_ntests_failed(runner);
    srunner_free(runner);
    return numberOfFailedTests;
}
