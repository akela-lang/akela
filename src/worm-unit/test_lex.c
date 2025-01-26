#include <string.h>
#include <worm/lex_data.h>
#include <zinc/input_unicode_string.h>

#include "zinc/unit_test.h"
#include "zinc/error.h"
#include "worm/lex.h"
#include "test_lex_tools.h"

void test_lex_type_int32()
{
    Zinc_test_name(__func__);

    Worm_lex_data* ld = NULL;
    test_lex_setup(&ld,
        "Scaler {}\n"
        ".Integer {.name=\"Int32\";.bit_count=32;.is_signed=true}\n"
    );

    Worm_token* t;

    /* line 1 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 1 0");
    Zinc_expect_string(&t->value, "Scaler", "string 1 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 1 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 1 2");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 1 3");
    Worm_token_destroy(t);
    free(t);

    /* line 2 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 1");
    Zinc_expect_string(&t->value, "Integer", "string 2 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 2 2");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 4");
    Zinc_expect_string(&t->value, "name", "string 2 4");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 2 6");
    Zinc_expect_string(&t->value, "Int32", "string 2 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 7");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "bit_count", "string 2 9");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_natural, "type 2 11");
    Zinc_expect_string(&t->value, "32", "value 2 11");
    Zinc_expect_unsigned_long_equal(t->number.natural, 32, "natural 2 11");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "is_signed", "string 2 9");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_true, "type 2 11");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_eof, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    test_lex_teardown(ld);
}

void test_lex_type_nat32()
{
    Zinc_test_name(__func__);

    Worm_lex_data* ld = NULL;
    test_lex_setup(&ld,
        "Scaler {}\n"
        ".Integer {.name=\"Nat32\";.bit_count=32;.is_signed=false}\n"
    );

    Worm_token* t;

    /* line 1 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 1 0");
    Zinc_expect_string(&t->value, "Scaler", "string 1 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 1 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 1 2");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 1 3");
    Worm_token_destroy(t);
    free(t);

    /* line 2 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 1");
    Zinc_expect_string(&t->value, "Integer", "string 2 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 2 2");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 4");
    Zinc_expect_string(&t->value, "name", "string 2 4");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 2 6");
    Zinc_expect_string(&t->value, "Nat32", "string 2 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 7");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "bit_count", "string 2 9");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_natural, "type 2 11");
    Zinc_expect_string(&t->value, "32", "value 2 11");
    Zinc_expect_unsigned_long_equal(t->number.natural, 32, "natural 2 11");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 9");
    Zinc_expect_string(&t->value, "is_signed", "string 2 9");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 2 10");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_false, "type 2 11");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_eof, "type 2 12");
    Worm_token_destroy(t);
    free(t);

    test_lex_teardown(ld);
}

void test_lex_parse_const_struct()
{
    Zinc_test_name(__func__);

    Worm_lex_data* ld = NULL;
    test_lex_setup(&ld,
        "Stmts {.type=\"struct {x: Real64; y: Real64}\"}\n"
        ".Const {}\n"
        "..Id {.value=\"a\"}\n"
        "..Type {.type=\"struct {x: Real64; y: Real64}\"}\n"
        "..Struct {.value=\"Point\";.type=\"struct {x: Real64; y: Real64}\"}\n"
        "...Field {.value=\"x\"}\n"
        "....Real {.value=\"10.1\";.real=10.1;.type=\"Real64\"}\n"
        "...Field {.value=\"y\"}\n"
        "....Real {.value=\"20.5\";.real=20.5;.type=\"Real64\"}\n"
        ".Id {.value=\"a\";.type=\"struct {x: Real64; y: Real64}\"}\n"
    );

    Worm_token* t = NULL;

    /* line 1 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 1 0");
    Zinc_expect_string(&t->value, "Stmts", "string 1 0");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 1 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 1 2");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 1 3");
    Zinc_expect_string(&t->value, "type", "value 1 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 1 4");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 1 5");
    Zinc_expect_string(&t->value, "struct {x: Real64; y: Real64}", "type 1 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 1 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 1 7");
    Worm_token_destroy(t);
    free(t);

    /* line 2 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 2 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 2 1");
    Zinc_expect_string(&t->value, "Const", "string 2 1");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 2 2");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 2 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 2 4");
    Worm_token_destroy(t);
    free(t);

    /* line 3 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 3 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 3 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 3 2");
    Zinc_expect_string(&t->value, "Id", "string 3 2");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 3 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 3 4");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 3 5");
    Zinc_expect_string(&t->value, "value", "string 3 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 3 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 3 7");
    Zinc_expect_string(&t->value, "a", "string 3 7");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 3 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 3 9");
    Worm_token_destroy(t);
    free(t);

    /* line 4 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 4 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 4 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 4 2");
    Zinc_expect_string(&t->value, "Type", "string 4 2");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 4 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 4 4");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 4 5");
    Zinc_expect_string(&t->value, "type", "string 4 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 4 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 4 7");
    Zinc_expect_string(&t->value, "struct {x: Real64; y: Real64}", "string 4 7");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 4 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 4 9");
    Worm_token_destroy(t);
    free(t);

    /* line 5 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 5 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 5 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 5 2");
    Zinc_expect_string(&t->value, "Struct", "string 5 2");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 5 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 5 4");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 5 5");
    Zinc_expect_string(&t->value, "value", "string 5 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 5 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 5 7");
    Zinc_expect_string(&t->value, "Point", "string 5 7");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 5 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 5 9");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 5 10");
    Zinc_expect_string(&t->value, "type", "string 5 10");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 5 11");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 5 12");
    Zinc_expect_string(&t->value, "struct {x: Real64; y: Real64}", "string 5 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 5 13");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 5 14");
    Worm_token_destroy(t);
    free(t);

    /* line 6 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 6 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 6 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 6 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 6 2");
    Zinc_expect_string(&t->value, "Field", "string 6 2");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 6 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 6 4");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 6 5");
    Zinc_expect_string(&t->value, "value", "string 6 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 6 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 6 7");
    Zinc_expect_string(&t->value, "x", "string 6 7");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 6 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 6 9");
    Worm_token_destroy(t);
    free(t);

    /* line 7 */
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 7 0");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 7 1");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 7 2");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 7 3");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 7 4");
    Zinc_expect_string(&t->value, "Real", "string 7 4");
    Worm_token_destroy(t);
    free(t);
    
    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_left_curly_brace, "type 7 5");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 7 6");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 7 7");
    Zinc_expect_string(&t->value, "value", "string 7 7");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 7 8");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 7 9");
    Zinc_expect_string(&t->value, "10.1", "string 7 9");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 7 10");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 7 11");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 7 12");
    Zinc_expect_string(&t->value, "real", "string 7 12");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 7 13");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_real, "type 7 14");
    Zinc_expect_double_equal(t->number.real, 10.1, "string 7 14");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_semicolon, "type 7 15");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_dot, "type 7 16");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_id, "type 7 17");
    Zinc_expect_string(&t->value, "type", "string 7 17");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_equal, "type 7 18");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_string, "type 7 19");
    Zinc_expect_string(&t->value, "Real64", "string 7 19");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_right_curly_brace, "type 7 20");
    Worm_token_destroy(t);
    free(t);

    t = Worm_lex(ld);
    Zinc_expect_int_equal(t->type, Worm_token_type_newline, "type 7 20");
    Worm_token_destroy(t);
    free(t);

    test_lex_teardown(ld);
}

void test_lex()
{
    test_lex_type_int32();
    test_lex_type_nat32();
    test_lex_parse_const_struct();
}
