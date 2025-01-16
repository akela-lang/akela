#include "zinc/unit_test.h"
#include "akela/lex_tools.h"
#include "zinc/utf8.h"

void test_lex_tools_is_word()
{
    Zinc_test_name(__func__);

    Zinc_expect_true(Ake_is_word("x  ", NUM_BYTES("x"[0])), "ascii letter");
    Zinc_expect_false(Ake_is_word("1  ", NUM_BYTES("1"[0])), "not a letter");
    Zinc_expect_true(Ake_is_word("Α  ", NUM_BYTES("Α"[0])), "0 greek letter");
    Zinc_expect_true(Ake_is_word("π  ", NUM_BYTES("π"[0])), "1 greek letter");
    Zinc_expect_true(Ake_is_word("ω  ", NUM_BYTES("ω"[0])), "2 greek letter");
    Zinc_expect_false(Ake_is_word("я  ", NUM_BYTES("я"[0])), "cyrillic letter");
}

void test_lex_tools()
{
    test_lex_tools_is_word();
}