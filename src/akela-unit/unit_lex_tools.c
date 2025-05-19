#include "akela/lex_tools.h"
#include "zinc/utf8.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_lex_tools_is_word(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Zinc_test_expect_true(test, Ake_is_word("x  ", ZINC_NUM_BYTES("x"[0])), "ascii letter");
    Zinc_test_expect_false(test, Ake_is_word("1  ", ZINC_NUM_BYTES("1"[0])), "not a letter");
    Zinc_test_expect_true(test, Ake_is_word("Α  ", ZINC_NUM_BYTES("Α"[0])), "0 greek letter");
    Zinc_test_expect_true(test, Ake_is_word("π  ", ZINC_NUM_BYTES("π"[0])), "1 greek letter");
    Zinc_test_expect_true(test, Ake_is_word("ω  ", ZINC_NUM_BYTES("ω"[0])), "2 greek letter");
    Zinc_test_expect_false(test, Ake_is_word("я  ", ZINC_NUM_BYTES("я"[0])), "cyrillic letter");
}

void AkeUnit_lex_tools(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_lex_tools_is_word);

        return;
    }

    Zinc_test_perform(test);
}