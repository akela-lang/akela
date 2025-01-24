#include <string.h>
#include <worm/lex_data.h>
#include <zinc/input_unicode_string.h>

#include "zinc/unit_test.h"
#include "zinc/error.h"
#include "worm/lex.h"
#include "test_tools.h"

void test_lex_type_int32()
{
    Zinc_test_name(__func__);

    Worm_lex_data* ld = NULL;
    test_setup(&ld,
        "Scaler {}\n"
        ".Integer {.name=\"Int32\";.bit_count=32;.is_signed=true}\n"
    );

    Worm_token* t;

    /* line 1 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 1 0");
    Zinc_expect_string(&t->value, "Scaler", "string 1 0");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 1 1");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 1 2");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 1 3");

    /* line 2 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 0");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 1");
    Zinc_expect_string(&t->value, "Integer", "string 2 1");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 2 2");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 3");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 4");
    Zinc_expect_string(&t->value, "name", "string 2 4");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 5");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 2 6");
    Zinc_expect_string(&t->value, "Int32", "string 2 6");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 7");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "bit_count", "string 2 9");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_number, "type 2 11");
    Zinc_expect_string(&t->value, "32", "value 2 11");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 12");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "is_signed", "string 2 9");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_true, "type 2 11");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 2 12");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 2 12");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_eof, "type 2 12");

    test_teardown(ld);
}

void test_lex_type_nat32()
{
    Zinc_test_name(__func__);

    Worm_lex_data* ld = NULL;
    test_setup(&ld,
        "Scaler {}\n"
        ".Integer {.name=\"Nat32\";.bit_count=32;.is_signed=false}\n"
    );

    Worm_token* t;

    /* line 1 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 1 0");
    Zinc_expect_string(&t->value, "Scaler", "string 1 0");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 1 1");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 1 2");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 1 3");

    /* line 2 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 0");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 1");
    Zinc_expect_string(&t->value, "Integer", "string 2 1");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 2 2");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 3");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 4");
    Zinc_expect_string(&t->value, "name", "string 2 4");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 5");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 2 6");
    Zinc_expect_string(&t->value, "Nat32", "string 2 6");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 7");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "bit_count", "string 2 9");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_number, "type 2 11");
    Zinc_expect_string(&t->value, "32", "value 2 11");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 12");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "is_signed", "string 2 9");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_false, "type 2 11");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 2 12");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 2 12");

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_eof, "type 2 12");

    test_teardown(ld);
}

void test_lex_parse_const_int32()
{

}

void test_lex()
{
    test_lex_type_int32();
    test_lex_type_nat32();
}