#include <stdio.h>
#include <zinc/unit_test.h>

#include "zinc-unit/unit_string.h"

void Test_test(Zinc_test* test);

int main()
{
    Zinc_test test;
    Zinc_test_init(&test);

    test.dry_run = true;
    Test_test(&test);

    test.dry_run = false;
    Test_test(&test);

    Zinc_test_stat stat;
    Zinc_test_stat_init(&stat);

    Zinc_test_count(&test, &stat);
    Zinc_test_print(&stat);

    Zinc_test_destroy(&test);

    return 0;
}

void Test_test(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, "Test");

        Zinc_test_register(test, Zinc_unit_string);

        return;
    }

    Zinc_test_perform(test);
}