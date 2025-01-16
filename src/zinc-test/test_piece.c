#include "zinc/piece.h"
#include "zinc/unit_test.h"

void test_piece_get()
{
    Zinc_test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "zero;one;two;three");

    struct Zinc_string x;
    Zinc_string_init(&x);

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 0, &x);
    Zinc_expect_str(&x, "zero", "zero");

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 1, &x);
    Zinc_expect_str(&x, "one", "one");

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 2, &x);
    Zinc_expect_str(&x, "two", "two");

    Zinc_string_clear(&x);
    Zinc_get_piece(&bf, ';', 3, &x);
    Zinc_expect_str(&x, "three", "three");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&x);
}

void test_piece()
{
    test_piece_get();
}