#include "zinc/piece.h"
#include "zinc/unit_test.h"

void test_piece_get()
{
    test_name(__func__);

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "zero;one;two;three");

    struct Zinc_string x;
    Zinc_string_init(&x);

    Zinc_string_clear(&x);
    get_piece(&bf, ';', 0, &x);
    expect_str(&x, "zero", "zero");

    Zinc_string_clear(&x);
    get_piece(&bf, ';', 1, &x);
    expect_str(&x, "one", "one");

    Zinc_string_clear(&x);
    get_piece(&bf, ';', 2, &x);
    expect_str(&x, "two", "two");

    Zinc_string_clear(&x);
    get_piece(&bf, ';', 3, &x);
    expect_str(&x, "three", "three");

    Zinc_string_destroy(&bf);
    Zinc_string_destroy(&x);
}

void test_piece()
{
    test_piece_get();
}