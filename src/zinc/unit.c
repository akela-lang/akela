#include <stdlib.h>
#include <string.h>

#include "test.h"

void Zinc_test_panic()
{
    fprintf(stderr, "Exiting because of assertion error.\n");
    exit(1);
}

void Zinc_test_assert_ok(Zinc_test* test, Zinc_result r, const char* message)
{
    test->check_count++;

    if (r == Zinc_result_ok) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\tok assertion error: %s: %s\n", message, Zinc_error_message);
    Zinc_test_panic();
}

void Zinc_test_assert_ptr(Zinc_test* test, void* p, const char* message)
{
    test->check_count++;

    if (p) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "\t%p != NULL assertion error: %s\n", p, message);
    Zinc_test_panic();
}

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

void Zinc_test_expect_string(Zinc_test* test, Zinc_string* a, const char* b, const char* message)
{
    test->check_count++;

    if (Zinc_string_compare_str(a, b)) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(
        stderr,
        "\tstrings not equal (%s):\n\t(%s)\n\t(%s)\n",
        message,
        Zinc_string_c_str(a),
        b);
}

void Zinc_test_expect_true(Zinc_test* test, int value, const char* message)
{
    test->check_count++;

    if (value) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "%d = true error: %s\n", value, message);
}

void Zinc_test_expect_false(Zinc_test* test, int value, const char* message)
{
    test->check_count++;

    if (!value) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "(%d) = (false) error: %s\n", value, message);
}

void Zinc_test_expect_strcmp(Zinc_test* test, const char* a, const char* b, const char* message)
{
    test->check_count++;
    if (strcmp(a, b) == 0) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "(%s) = (%s) strcmp error: %s\n", a, b, message);
}

void Zinc_test_expect_int_equal(Zinc_test* test, int a, int b, const char* message)
{
    test->check_count++;

    if (a == b) {
        test->check_passed++;
        return;
    }

    test->check_failed++;
    test->pass = false;
    Zinc_test_print_unseen(test);
    fprintf(stderr, "%d = %d error: %s\n", a, b, message);
}
