#include <stdlib.h>
#include "test.h"

void Zinc_test_expect_null(Zinc_test* test, void* p, const char* message)
{
    test->check_count++;

    if (p == NULL) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p == null error: %s\n", p, message);
}

void Zinc_test_expect_size_t_equal(Zinc_test* test, size_t a, size_t b, const char* message)
{
    test->check_count++;
    if (a == b) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t(%zu) = (%zu) error: %s\n", a, b, message);
}