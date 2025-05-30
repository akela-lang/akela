#include "zinc/piece.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_piece_get(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "zero;one;two;three");

    Zinc_string x;
    Zinc_string_init(&x);

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 0, &x);
    Zinc_expect_string(test, &x, "zero", "zero");

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 1, &x);
    Zinc_expect_string(test, &x, "one", "one");

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 2, &x);
    Zinc_expect_string(test, &x, "two", "two");

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 3, &x);
    Zinc_expect_string(test, &x, "three", "three");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&x);
}

void Zinc_unit_piece(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_piece_get);
        return;
    }

    Zinc_test_perform(test);
}