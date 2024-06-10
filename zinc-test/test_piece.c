#include "zinc/piece.h"
#include "zinc/unit_test.h"

void test_piece_get()
{
    test_name(__func__);

    struct buffer bf;
    buffer_init(&bf);
    buffer_copy_str(&bf, "zero;one;two;three");

    struct buffer x;
    buffer_init(&x);

    buffer_clear(&x);
    get_piece(&bf, ';', 0, &x);
    expect_str(&x, "zero", "zero");

    buffer_clear(&x);
    get_piece(&bf, ';', 1, &x);
    expect_str(&x, "one", "one");

    buffer_clear(&x);
    get_piece(&bf, ';', 2, &x);
    expect_str(&x, "two", "two");

    buffer_clear(&x);
    get_piece(&bf, ';', 3, &x);
    expect_str(&x, "three", "three");

    buffer_destroy(&bf);
    buffer_destroy(&x);
}

void test_piece()
{
    test_piece_get();
}