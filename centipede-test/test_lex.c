#include <string.h>
#include <zinc/error_unit_test.h>
#include <zinc/input_unicode_string.h>

#include "zinc/unit_test.h"
#include "centipede/lex.h"
#include "centipede/lex_data.h"
#include "test_lex_setup.h"

void test_lex_element()
{
    test_name(__func__);
    Cent_lex_data ld;
    Cent_token* t = NULL;

    test_lex_setup(&ld,
        "element Test_suite\n"
        "   properties\n"
        "       name: String required\n"
        "       solo: Bool\n"
        "       mute: Bool\n"
        "   end\n"
        "\n"
        "   children: Test\n"
        "end\n"
    );

    /* line 1 */
    t = lex(&ld);
    assert_ptr(t, "ptr 0");
    expect_int_equal(t->type, Cent_token_element, "type 0");

    t = lex(&ld);
    assert_ptr(t, "ptr 1");
    expect_int_equal(t->type, Cent_token_id, "type 1");
    expect_str(&t->value, "Test_suite", "value 1");

    t = lex(&ld);
    assert_ptr(t, "ptr 2");
    expect_int_equal(t->type, Cent_token_newline, "type 2");

    /* line 2 */
    t = lex(&ld);
    assert_ptr(t, "ptr 3");
    expect_int_equal(t->type, Cent_token_properties, "type 3");

    t = lex(&ld);
    assert_ptr(t, "ptr 4");
    expect_int_equal(t->type, Cent_token_newline, "type 4");

    /* line 3 */
    t = lex(&ld);
    assert_ptr(t, "ptr 5");
    expect_int_equal(t->type, Cent_token_id, "type 5");
    expect_str(&t->value, "name", "value 5");

    t = lex(&ld);
    assert_ptr(t, "ptr 6");
    expect_int_equal(t->type, Cent_token_colon, "type 6");

    t = lex(&ld);
    assert_ptr(t, "ptr 7");
    expect_int_equal(t->type, Cent_token_id, "type 7");
    expect_str(&t->value, "String", "value 7");

    t = lex(&ld);
    assert_ptr(t, "ptr 8");
    expect_int_equal(t->type, Cent_token_required, "type 8");

    t = lex(&ld);
    assert_ptr(t, "ptr 9");
    expect_int_equal(t->type, Cent_token_newline, "type 9");

    /* line 4 */
    t = lex(&ld);
    assert_ptr(t, "ptr 10");
    expect_int_equal(t->type, Cent_token_id, "type 10");
    expect_str(&t->value, "solo", "value 9");

    t = lex(&ld);
    assert_ptr(t, "ptr 11");
    expect_int_equal(t->type, Cent_token_colon, "type 11");

    t = lex(&ld);
    assert_ptr(t, "ptr 12");
    expect_int_equal(t->type, Cent_token_id, "type 12");
    expect_str(&t->value, "Bool", "value 12");

    t = lex(&ld);
    assert_ptr(t, "ptr 13");
    expect_int_equal(t->type, Cent_token_newline, "type 13");

    /* line 5 */
    t = lex(&ld);
    assert_ptr(t, "ptr 14");
    expect_int_equal(t->type, Cent_token_id, "type 14");
    expect_str(&t->value, "mute", "value 14");

    t = lex(&ld);
    assert_ptr(t, "ptr 15");
    expect_int_equal(t->type, Cent_token_colon, "type 15");

    t = lex(&ld);
    assert_ptr(t, "ptr 16");
    expect_int_equal(t->type, Cent_token_id, "type 16");
    expect_str(&t->value, "Bool", "value 16");

    t = lex(&ld);
    assert_ptr(t, "ptr 17");
    expect_int_equal(t->type, Cent_token_newline, "type 17");

    /* line 6 */
    t = lex(&ld);
    assert_ptr(t, "ptr 18");
    expect_int_equal(t->type, Cent_token_end, "type 18");

    t = lex(&ld);
    assert_ptr(t, "ptr 19");
    expect_int_equal(t->type, Cent_token_newline, "type 19");

    /* line 7 */
    t = lex(&ld);
    assert_ptr(t, "ptr 20");
    expect_int_equal(t->type, Cent_token_newline, "type 20");

    /* line 8 */
    t = lex(&ld);
    assert_ptr(t, "ptr 21");
    expect_int_equal(t->type, Cent_token_children, "type 21");

    t = lex(&ld);
    assert_ptr(t, "ptr 22");
    expect_int_equal(t->type, Cent_token_colon, "type 22");

    t = lex(&ld);
    assert_ptr(t, "ptr 23");
    expect_int_equal(t->type, Cent_token_id, "type 23");
    expect_str(&t->value, "Test", "value 23");

    t = lex(&ld);
    assert_ptr(t, "ptr 24");
    expect_int_equal(t->type, Cent_token_newline, "type 24");

    /* line 9 */
    t = lex(&ld);
    assert_ptr(t, "ptr 25");
    expect_int_equal(t->type, Cent_token_end, "type 25");

    t = lex(&ld);
    assert_ptr(t, "ptr 26");
    expect_int_equal(t->type, Cent_token_newline, "type 26");

    t = lex(&ld);
    assert_ptr(t, "ptr 27");
    expect_int_equal(t->type, Cent_token_eof, "type 27");

    expect_no_errors(ld.errors);

    test_lex_teardown(&ld);
}

void test_lex_enum()
{
    test_name(__func__);
    Cent_token* t = NULL;
    Cent_lex_data ld;

    test_lex_setup(&ld,
        "enum Symbol_type\n"
        "   Variable\n"
        "   Reserved_word\n"
        "   Type\n"
        "   Info\n"
        "end\n"
    );

    /* line 1 */
    t = lex(&ld);
    assert_ptr(t, "ptr 0");
    expect_int_equal(t->type, Cent_token_enum, "type 0");

    t = lex(&ld);
    assert_ptr(t, "ptr 1");
    expect_int_equal(t->type, Cent_token_id, "type 1");
    expect_str(&t->value, "Symbol_type", "value 1");

    t = lex(&ld);
    assert_ptr(t, "ptr 2");
    expect_int_equal(t->type, Cent_token_newline, "type 2");

    /* line 2 */
    t = lex(&ld);
    assert_ptr(t, "ptr 3");
    expect_int_equal(t->type, Cent_token_id, "type 3");
    expect_str(&t->value, "Variable", "value 3");

    t = lex(&ld);
    assert_ptr(t, "ptr 4");
    expect_int_equal(t->type, Cent_token_newline, "type 4");

    /* line 3 */
    t = lex(&ld);
    assert_ptr(t, "ptr 5");
    expect_int_equal(t->type, Cent_token_id, "type 5");
    expect_str(&t->value, "Reserved_word", "value 5");

    t = lex(&ld);
    assert_ptr(t, "ptr 6");
    expect_int_equal(t->type, Cent_token_newline, "type 6");

    /* line 4 */
    t = lex(&ld);
    assert_ptr(t, "ptr 7");
    expect_int_equal(t->type, Cent_token_id, "type 7");
    expect_str(&t->value, "Type", "value 7");

    t = lex(&ld);
    assert_ptr(t, "ptr 8");
    expect_int_equal(t->type, Cent_token_newline, "type 8");

    /* line 5 */
    t = lex(&ld);
    assert_ptr(t, "ptr 9");
    expect_int_equal(t->type, Cent_token_id, "type 9");
    expect_str(&t->value, "Info", "value 9");

    t = lex(&ld);
    assert_ptr(t, "ptr 10");
    expect_int_equal(t->type, Cent_token_newline, "type 10");

    /* line 6 */
    t = lex(&ld);
    assert_ptr(t, "ptr 11");
    expect_int_equal(t->type, Cent_token_end, "type 11");

    t = lex(&ld);
    assert_ptr(t, "ptr 12");
    expect_int_equal(t->type, Cent_token_newline, "type 12");

    t = lex(&ld);
    assert_ptr(t, "ptr 13");
    expect_int_equal(t->type, Cent_token_eof, "type 13");

    expect_no_errors(ld.errors);

    test_lex_teardown(&ld);
}

void test_lex_top_level_assign()
{
    test_name(__func__);
    Cent_token* t = NULL;
    Cent_lex_data ld;

    test_lex_setup(&ld,
        "# built-in element defs\n"
        "i32 = Type_def {\n"
        "   .type = Type_def_type::Integer\n"
        "   .name = \"i32\"\n"
        "   .bit_count = 32\n"
        "   .is_signed = true\n"
        "}\n"
    );

    /* line 2 */
    t = lex(&ld);
    assert_ptr(t, "ptr 0");
    expect_int_equal(t->type, Cent_token_id, "type 0");
    expect_str(&t->value, "i32", "value 0");

    expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex()
{
    test_lex_element();
    test_lex_enum();
    test_lex_top_level_assign();
}