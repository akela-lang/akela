#include <string.h>

#include "zinc/unit_test.h"
#include "zinc/string_slice.h"

void test_string_slice_trim_left1()
{
    Zinc_test_name(__func__);

    char* title = "Test Suite";
    Zinc_string_slice title2 = Zinc_trim_left((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(title2.p, title, "p");
    Zinc_expect_size_t_equal(title2.size, strlen(title), "size");
}

void test_string_slice_trim_left2()
{
    Zinc_test_name(__func__);

    char* title = "  Test Suite";
    Zinc_string_slice title2 = Zinc_trim_left((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(title2.p, title+2, "p");
    Zinc_expect_size_t_equal(title2.size, strlen(title)-2, "size");
}

void test_string_slice_trim_right1()
{
    Zinc_test_name(__func__);

    char* title = "Test Suite";
    Zinc_string_slice title2 = Zinc_trim_right((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(title2.p, title, "p");
    Zinc_expect_size_t_equal(title2.size, strlen(title), "size");
}

void test_string_slice_trim_right2()
{
    Zinc_test_name(__func__);

    char* title = "Test Suite  ";
    Zinc_string_slice title2 = Zinc_trim_right((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(title2.p, title, "p");
    Zinc_expect_size_t_equal(title2.size, strlen(title) - 2, "size");
}

void test_string_slice_trim1()
{
    Zinc_test_name(__func__);

    char* title = "Test Suite";
    Zinc_string_slice title2 = Zinc_trim((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(title2.p, title, "p");
    Zinc_expect_size_t_equal(title2.size, strlen(title), "size");
}

void test_string_slice_trim2()
{
    Zinc_test_name(__func__);

    char* title = "  Test Suite  ";
    Zinc_string_slice title2 = Zinc_trim((Zinc_string_slice) {
        .p = title,
        .size = strlen(title),
    });

    Zinc_expect_ptr_equal(title2.p, title + 2, "p");
    Zinc_expect_size_t_equal(title2.size, strlen(title) - 4, "size");
}

void test_string_slice()
{
    test_string_slice_trim_left1();
    test_string_slice_trim_left2();
    test_string_slice_trim_right1();
    test_string_slice_trim_right2();
    test_string_slice_trim1();
    test_string_slice_trim2();
}