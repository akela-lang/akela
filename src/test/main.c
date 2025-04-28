#include <stdio.h>
#include <zinc/unit_test.h>

#include "zinc-unit/test_buffer.h"

int main()
{
    printf("Akela Tests\n");
    Zinc_test test;
    Zinc_test_init(&test);

    test.dry_run = true;
    Zinc_unit_buffer(&test);

    test.dry_run = false;
    Zinc_unit_buffer(&test);

    Zinc_test_stat stat;
    Zinc_test_stat_init(&stat);

    Zinc_test_count(&test, &stat);
    Zinc_test_print(&stat);

    Zinc_test_destroy(&test);

    return 0;
}