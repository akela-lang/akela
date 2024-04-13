#include "zinc/unit_test.h"
#include "akela/lex_tools.h"
#include "zinc/utf8.h"

void test_lex_tools_is_word()
{
    test_name(__func__);

    expect_true(is_word("x", NUM_BYTES("x"[0])), "ascii letter");
    expect_false(is_word("1", NUM_BYTES("1"[0])), "not a letter");
    expect_true(is_word("Α", NUM_BYTES("Α"[0])), "0 greek letter");
    expect_true(is_word("π", NUM_BYTES("π"[0])), "1 greek letter");
    expect_true(is_word("ω", NUM_BYTES("ω"[0])), "2 greek letter");
    expect_false(is_word("я", NUM_BYTES("я"[0])), "cyrillic letter");
}

void test_lex_tools()
{
    test_lex_tools_is_word();
}