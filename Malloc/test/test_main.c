#include <stdlib.h>
#include <stdio.h>
#include <check.h>

Suite* test_suite_malloc_zone(); // defined in test_malloc_zone.c

int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[])
{
    int numberOfFailedTests = 0;
    Suite* suite[] = {
        test_suite_malloc_zone()
    };
    for (int i = 0, n = sizeof(suite) / sizeof(*suite); i < n; ++i) {
        SRunner *runner = srunner_create(suite[i]);
        srunner_run_all(runner, CK_NORMAL);
        numberOfFailedTests += srunner_ntests_failed(runner);
        srunner_free(runner);   
    }
    return (numberOfFailedTests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}