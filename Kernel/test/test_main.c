#include <stdlib.h>
#include <stdio.h>
#include <check.h>

void lgdt(__attribute__((unused)) void *gdt, __attribute__((unused)) unsigned limit)
{
    // satisfy the linker
}

Suite* test_suite_gdt(); // defined in test_gdt.c
Suite* test_suite_idt(); // defined in test_idt.c

// TODO: need tests for string functions in string.h

int main(__attribute__((unused)) int argc, __attribute__((unused)) char* argv[])
{
    int numberOfFailedTests = 0;
    Suite* suite[] = {
        test_suite_gdt(),
        test_suite_idt()
    };
    for (int i = 0, n = sizeof(suite) / sizeof(*suite); i < n; ++i) {
        SRunner *runner = srunner_create(suite[i]);
        srunner_run_all(runner, CK_NORMAL);
        numberOfFailedTests += srunner_ntests_failed(runner);
        srunner_free(runner);   
    }
    return (numberOfFailedTests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
