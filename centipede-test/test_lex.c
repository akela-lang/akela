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
        "       name: String `required`\n"
        "       solo: Bool\n"
        "       mute: Bool\n"
        "   end\n"
        "   children {\n"
        "       Test\n"
        "   end\n"
        "end\n"
    );

    /* line 1 */
    t = lex(&ld);
    assert_ptr(t, "ptr 1.1");
    expect_int_equal(t->type, Cent_token_element, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 1.2");
    expect_int_equal(t->type, Cent_token_id, "type 1.2");
    expect_str(&t->value, "Test_suite", "value 1.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 1.3");
    expect_int_equal(t->type, Cent_token_newline, "type 1.3");
    Cent_token_destroy(t);
    free(t);

    /* line 2 */
    t = lex(&ld);
    assert_ptr(t, "ptr 2.1");
    expect_int_equal(t->type, Cent_token_properties, "type 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 2.1");
    expect_int_equal(t->type, Cent_token_newline, "type 2.1");
    Cent_token_destroy(t);
    free(t);

    /* line 3 */
    t = lex(&ld);
    assert_ptr(t, "ptr 3.1");
    expect_int_equal(t->type, Cent_token_id, "type 3.1");
    expect_str(&t->value, "name", "value 3.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.2");
    expect_int_equal(t->type, Cent_token_colon, "type 3.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.3");
    expect_int_equal(t->type, Cent_token_id, "type 3.3");
    expect_str(&t->value, "String", "value 3.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.4");
    expect_int_equal(t->type, Cent_token_modifier, "type 3.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.5");
    expect_int_equal(t->type, Cent_token_newline, "type 3.5");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    assert_ptr(t, "ptr 4.1");
    expect_int_equal(t->type, Cent_token_id, "type 4.1");
    expect_str(&t->value, "solo", "value 4.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.2");
    expect_int_equal(t->type, Cent_token_colon, "type 4.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.3");
    expect_int_equal(t->type, Cent_token_id, "type 4.3");
    expect_str(&t->value, "Bool", "value 4.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.4");
    expect_int_equal(t->type, Cent_token_newline, "type 4.4");
    Cent_token_destroy(t);
    free(t);

    /* line 5 */
    t = lex(&ld);
    assert_ptr(t, "ptr 5.1");
    expect_int_equal(t->type, Cent_token_id, "type 5.1");
    expect_str(&t->value, "mute", "value 5.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.2");
    expect_int_equal(t->type, Cent_token_colon, "type 5.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.3");
    expect_int_equal(t->type, Cent_token_id, "type 5.3");
    expect_str(&t->value, "Bool", "value 5.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.4");
    expect_int_equal(t->type, Cent_token_newline, "type 5.4");
    Cent_token_destroy(t);
    free(t);

    /* line 6 */
    t = lex(&ld);
    assert_ptr(t, "ptr 6.1");
    expect_int_equal(t->type, Cent_token_end, "type 6.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 6.2");
    expect_int_equal(t->type, Cent_token_newline, "type 6.2");
    Cent_token_destroy(t);
    free(t);

    /* line 7 */
    t = lex(&ld);
    assert_ptr(t, "ptr 7.1");
    expect_int_equal(t->type, Cent_token_children, "type 7.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 7.2");
    expect_int_equal(t->type, Cent_token_left_curly_brace, "type 7.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 7.3");
    expect_int_equal(t->type, Cent_token_newline, "type 7.3");
    Cent_token_destroy(t);
    free(t);

    /* line 8 */
    t = lex(&ld);
    assert_ptr(t, "ptr 8.1");
    expect_int_equal(t->type, Cent_token_id, "type 8.1");
    expect_str(&t->value, "Test", "value 8.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 8.2");
    expect_int_equal(t->type, Cent_token_newline, "type 8.2");
    Cent_token_destroy(t);
    free(t);

    /* line 9 */
    t = lex(&ld);
    assert_ptr(t, "ptr 9.1");
    expect_int_equal(t->type, Cent_token_end, "type 9.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 9.2");
    expect_int_equal(t->type, Cent_token_newline, "type 9.2");
    Cent_token_destroy(t);
    free(t);

    /* line 10 */
    t = lex(&ld);
    assert_ptr(t, "ptr 10.1");
    expect_int_equal(t->type, Cent_token_end, "type 10.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 10.2");
    expect_int_equal(t->type, Cent_token_newline, "type 10.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    assert_ptr(t, "ptr 27");
    expect_int_equal(t->type, Cent_token_eof, "type 27");
    Cent_token_destroy(t);
    free(t);

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
        "   Type\n"
        "   Info\n"
        "end\n"
    );

    /* line 1 */
    t = lex(&ld);
    assert_ptr(t, "ptr 1.1");
    expect_int_equal(t->type, Cent_token_enum, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 1.2");
    expect_int_equal(t->type, Cent_token_id, "type 1.2");
    expect_str(&t->value, "Symbol_type", "value 1.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 1.3");
    expect_int_equal(t->type, Cent_token_newline, "type 1.3");
    Cent_token_destroy(t);
    free(t);

    /* line 2 */
    t = lex(&ld);
    assert_ptr(t, "ptr 2.1");
    expect_int_equal(t->type, Cent_token_id, "type 2.1");
    expect_str(&t->value, "Variable", "value 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 2.2");
    expect_int_equal(t->type, Cent_token_newline, "type 2.2");
    Cent_token_destroy(t);
    free(t);

    /* line 3 */
    t = lex(&ld);
    assert_ptr(t, "ptr 3.1");
    expect_int_equal(t->type, Cent_token_id, "type 3.1");
    expect_str(&t->value, "Type", "value 3.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.2");
    expect_int_equal(t->type, Cent_token_newline, "type 3.2");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    assert_ptr(t, "ptr 4.1");
    expect_int_equal(t->type, Cent_token_id, "type 4.1");
    expect_str(&t->value, "Info", "value 4.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.2");
    expect_int_equal(t->type, Cent_token_newline, "type 4.2");
    Cent_token_destroy(t);
    free(t);

    /* line 5 */
    t = lex(&ld);
    assert_ptr(t, "ptr 5.1");
    expect_int_equal(t->type, Cent_token_end, "type 5.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.2");
    expect_int_equal(t->type, Cent_token_newline, "type 5.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    assert_ptr(t, "ptr 5.3");
    expect_int_equal(t->type, Cent_token_eof, "type 5.3");
    Cent_token_destroy(t);
    free(t);

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

    /* line 1 */
    t = lex(&ld);
    assert_ptr(t, "ptr 1.1");
    expect_int_equal(t->type, Cent_token_newline, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    /* line 2 */
    t = lex(&ld);
    assert_ptr(t, "ptr 2.1");
    expect_int_equal(t->type, Cent_token_id, "type 2.1");
    expect_str(&t->value, "i32", "value 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 2.2");
    expect_int_equal(t->type, Cent_token_equal, "type 2.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 2.3");
    expect_int_equal(t->type, Cent_token_id, "type 2.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 2.4");
    expect_int_equal(t->type, Cent_token_left_curly_brace, "type 2.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 2.5");
    expect_int_equal(t->type, Cent_token_newline, "type 2.5");
    Cent_token_destroy(t);
    free(t);

    /* line 3 */
    t = lex(&ld);
    assert_ptr(t, "ptr 3.1");
    expect_int_equal(t->type, Cent_token_dot, "type 3.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.2");
    expect_int_equal(t->type, Cent_token_id, "type 3.2");
    expect_str(&t->value, "type", "value 3.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.3");
    expect_int_equal(t->type, Cent_token_equal, "type 3.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.4");
    expect_int_equal(t->type, Cent_token_id, "type 3.4");
    expect_str(&t->value, "Type_def_type", "value 3.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.5");
    expect_int_equal(t->type, Cent_token_double_colon, "type 3.5");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.6");
    expect_int_equal(t->type, Cent_token_id, "type 3.6");
    expect_str(&t->value, "Integer", "value 3.6");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.7");
    expect_int_equal(t->type, Cent_token_newline, "type 3.7");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    assert_ptr(t, "ptr 4.1");
    expect_int_equal(t->type, Cent_token_dot, "type 4.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.2");
    expect_int_equal(t->type, Cent_token_id, "type 4.2");
    expect_str(&t->value, "name", "value 4.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.3");
    expect_int_equal(t->type, Cent_token_equal, "type 4.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.4");
    expect_int_equal(t->type, Cent_token_string, "type 4.4");
    expect_str(&t->value, "i32", "value 4.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.5");
    expect_int_equal(t->type, Cent_token_newline, "type 4.5");
    Cent_token_destroy(t);
    free(t);

    /* line 5 */
    t = lex(&ld);
    assert_ptr(t, "ptr 5.1");
    expect_int_equal(t->type, Cent_token_dot, "type 5.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.2");
    expect_int_equal(t->type, Cent_token_id, "type 5.2");
    expect_str(&t->value, "bit_count", "value 5.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.3");
    expect_int_equal(t->type, Cent_token_equal, "type 5.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.4");
    expect_int_equal(t->type, Cent_token_number, "type 5.4");
    expect_str(&t->value, "32", "value 5.4");
    expect_int_equal(t->number_type, Cent_number_type_integer, "number type 5.4");
    expect_long_long_equal(t->number_value.integer, 32, "integer 5.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.5");
    expect_int_equal(t->type, Cent_token_newline, "type 5.5");
    Cent_token_destroy(t);
    free(t);

    /* line 6 */
    t = lex(&ld);
    assert_ptr(t, "ptr 6.1");
    expect_int_equal(t->type, Cent_token_dot, "type 6.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 6.2");
    expect_int_equal(t->type, Cent_token_id, "type 6.2");
    expect_str(&t->value, "is_signed", "value 6.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 6.3");
    expect_int_equal(t->type, Cent_token_equal, "type 6.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 6.4");
    expect_int_equal(t->type, Cent_token_true, "type 6.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 6.5");
    expect_int_equal(t->type, Cent_token_newline, "type 6.5");
    Cent_token_destroy(t);
    free(t);

    /* line 7 */
    t = lex(&ld);
    assert_ptr(t, "ptr 7.1");
    expect_int_equal(t->type, Cent_token_right_curly_brace, "type 7.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 7.2");
    expect_int_equal(t->type, Cent_token_newline, "type 7.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    assert_ptr(t, "ptr eof");
    expect_int_equal(t->type, Cent_token_eof, "type eof");
    Cent_token_destroy(t);
    free(t);

    expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_builtin()
{
    test_name(__func__);
    Cent_token* t = NULL;
    Cent_lex_data ld;

    test_lex_setup(&ld,
        "@child_of\n"
        "@property_of\n"
        "@top\n"
        "@file_name\n"
        "abc\n"
    );

    /* line 1 */
    t = lex(&ld);
    assert_ptr(t, "ptr 1.1");
    expect_int_equal(t->type, Cent_token_id, "type 1.1");
    expect_int_equal(t->builtin_type, Cent_builtin_type_child_of, "builtin type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 1.2");
    expect_int_equal(t->type, Cent_token_newline, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    /* line 2 */
    t = lex(&ld);
    assert_ptr(t, "ptr 2.1");
    expect_int_equal(t->type, Cent_token_id, "type 2.1");
    expect_int_equal(t->builtin_type, Cent_builtin_type_property_of, "builtin type 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 2.2");
    expect_int_equal(t->type, Cent_token_newline, "type 2.2");
    Cent_token_destroy(t);
    free(t);

    /* line 3 */
    t = lex(&ld);
    assert_ptr(t, "ptr 3.1");
    expect_int_equal(t->type, Cent_token_id, "type 3.1");
    expect_int_equal(t->builtin_type, Cent_builtin_type_top, "builtin type 3.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 3.2");
    expect_int_equal(t->type, Cent_token_newline, "type 3.1");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    assert_ptr(t, "ptr 4.1");
    expect_int_equal(t->type, Cent_token_id, "type 4.1");
    expect_int_equal(t->builtin_type, Cent_builtin_type_file_name, "builtin type 4.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 4.2");
    expect_int_equal(t->type, Cent_token_newline, "type 4.2");
    Cent_token_destroy(t);
    free(t);

    /* line 5 */
    t = lex(&ld);
    assert_ptr(t, "ptr 5.1");
    expect_int_equal(t->type, Cent_token_id, "type 5.1");
    expect_int_equal(t->builtin_type, Cent_builtin_type_none, "builtin type 5.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 5.2");
    expect_int_equal(t->type, Cent_token_newline, "type 5.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    assert_ptr(t, "ptr eof");
    expect_int_equal(t->type, Cent_token_eof, "type eof");
    Cent_token_destroy(t);
    free(t);

    expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_builtin_error()
{
    test_name(__func__);
    Cent_token* t = NULL;
    Cent_lex_data ld;

    test_lex_setup(&ld,
        "@abc\n"
    );

    /* line 1 */
    t = lex(&ld);
    assert_ptr(t, "ptr 1.1");
    expect_int_equal(t->type, Cent_token_id, "type 1.1");
    expect_int_equal(t->builtin_type, Cent_builtin_type_none, "builtin type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    assert_ptr(t, "ptr 1.2");
    expect_int_equal(t->type, Cent_token_newline, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    assert_ptr(t, "ptr eof");
    expect_int_equal(t->type, Cent_token_eof, "type eof");
    Cent_token_destroy(t);
    free(t);

    expect_has_errors(ld.errors);
    expect_source_error(ld.errors, "invalid builtin id: @abc");
    test_lex_teardown(&ld);
}

void test_lex()
{
    test_lex_element();
    test_lex_enum();
    test_lex_top_level_assign();
    test_lex_builtin();
    test_lex_builtin_error();
}