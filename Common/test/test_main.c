#include <stdlib.h>
#include <stdio.h>
#include <check.h>

Suite* test_suite_string(void); // defined in test_string.c
Suite* test_suite_stdio(void); // defined in test_stdio.c

int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[])
{
    int numberOfFailedTests = 0;
    Suite* suite[] = {
        test_suite_string(),
        test_suite_stdio(),
    };
    for (int i = 0, n = sizeof(suite) / sizeof(*suite); i < n; ++i) {
        SRunner *runner = srunner_create(suite[i]);
        srunner_run_all(runner, CK_NORMAL);
        numberOfFailedTests += srunner_ntests_failed(runner);
        srunner_free(runner);   
    }
    return (numberOfFailedTests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
