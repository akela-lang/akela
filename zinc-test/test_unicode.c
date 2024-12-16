#include "zinc/unicode.h"
#include "zinc/unit_test.h"

void test_unicode_num32_to_hex_str()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    num32_to_hex_str(0x61, &bf);
    expect_str(&bf, "61", "str bf");

    buffer_destroy(&bf);
}

void test_unicode_num32_to_hex_str2()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);

    num32_to_hex_str(0x10ffff, &bf);
    expect_str(&bf, "10ffff", "str bf");

    buffer_destroy(&bf);
}

void test_unicode()
{
    test_unicode_num32_to_hex_str();
    test_unicode_num32_to_hex_str2();
}