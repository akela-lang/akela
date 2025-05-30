#include "zinc/unicode.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_unicode_num32_to_hex_str(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_num32_to_hex_str(0x61, &bf);
    Zinc_expect_string(test, &bf, "61", "str bf");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_unicode_num32_to_hex_str2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string bf;
    Zinc_string_init(&bf);

    Zinc_num32_to_hex_str(0x10ffff, &bf);
    Zinc_expect_string(test, &bf, "10ffff", "str bf");

    Zinc_string_destroy(&bf);
}

void Zinc_unit_unicode(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_unicode_num32_to_hex_str);
        Zinc_test_register(test, Zinc_unit_unicode_num32_to_hex_str2);

        return;
    }

    Zinc_test_perform(test);
}