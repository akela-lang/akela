#include "zinc/unicode.h"
#include "zinc/unit_test.h"

void test_unicode_num32_to_hex_str()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_num32_to_hex_str(0x61, &bf);
    expect_str(&bf, "61", "str bf");

    Zinc_string_destroy(&bf);
}

void test_unicode_num32_to_hex_str2()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_num32_to_hex_str(0x10ffff, &bf);
    expect_str(&bf, "10ffff", "str bf");

    Zinc_string_destroy(&bf);
}

void test_unicode()
{
    test_unicode_num32_to_hex_str();
    test_unicode_num32_to_hex_str2();
}