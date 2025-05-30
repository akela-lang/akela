#include <string.h>
#include "zinc/string_slice.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_string_slice_trim_left1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char* title = "Test Suite";
    Zinc_string_slice title2 = Zinc_trim_left((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(test, title2.p, title, "p");
    Zinc_expect_size_t_equal(test, title2.size, strlen(title), "size");
}

void Zinc_unit_string_slice_trim_left2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char* title = "  Test Suite";
    Zinc_string_slice title2 = Zinc_trim_left((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(test, title2.p, title+2, "p");
    Zinc_expect_size_t_equal(test, title2.size, strlen(title)-2, "size");
}

void Zinc_unit_string_slice_trim_right1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char* title = "Test Suite";
    Zinc_string_slice title2 = Zinc_trim_right((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(test, title2.p, title, "p");
    Zinc_expect_size_t_equal(test, title2.size, strlen(title), "size");
}

void Zinc_unit_string_slice_trim_right2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char* title = "Test Suite  ";
    Zinc_string_slice title2 = Zinc_trim_right((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(test, title2.p, title, "p");
    Zinc_expect_size_t_equal(test, title2.size, strlen(title) - 2, "size");
}

void Zinc_unit_string_slice_trim1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char* title = "Test Suite";
    Zinc_string_slice title2 = Zinc_trim((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(test, title2.p, title, "p");
    Zinc_expect_size_t_equal(test, title2.size, strlen(title), "size");
}

void Zinc_unit_string_slice_trim2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    char* title = "  Test Suite  ";
    Zinc_string_slice title2 = Zinc_trim((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(test, title2.p, title + 2, "p");
    Zinc_expect_size_t_equal(test, title2.size, strlen(title) - 4, "size");
}

void Zinc_unit_string_slice(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_string_slice_trim_left1);
        Zinc_test_register(test, Zinc_unit_string_slice_trim_left2);
        Zinc_test_register(test, Zinc_unit_string_slice_trim_right1);
        Zinc_test_register(test, Zinc_unit_string_slice_trim_right2);
        Zinc_test_register(test, Zinc_unit_string_slice_trim1);
        Zinc_test_register(test, Zinc_unit_string_slice_trim2);

        return;
    }

    Zinc_test_perform(test);
}