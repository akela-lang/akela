#include <string.h>
#include <zinc/error_unit_test.h>
#include <zinc/input_unicode_string.h>

#include "zinc/unit_test.h"
#include "centipede/lex.h"
#include "centipede/lex_data.h"
#include "test_lex_setup.h"

void test_lex_element()
{
    Zinc_test_name(__func__);
    Cent_lex_data ld;
    Cent_token* t = NULL;

    test_lex_setup(&ld,
        "element Test_suite {\n"
        "   properties {\n"
        "       name: String `required`,\n"
        "       solo: Bool,\n"
        "       mute: Bool,\n"
        "   }\n"
        "   children {\n"
        "       Test,\n"
        "   }\n"
        "}\n"
    );

    /* line 1 */
    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_element, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 1.2");
    Zinc_expect_string(&t->value, "Test_suite", "value 1.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_left_curly_brace, "type 1.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 1.4");
    Cent_token_destroy(t);
    free(t);

    /* line 2 */
    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_properties, "type 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_left_curly_brace, "type 2.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 2.3");
    Cent_token_destroy(t);
    free(t);

    /* line 3 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 3.1");
    Zinc_expect_string(&t->value, "name", "value 3.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.2");
    Zinc_expect_int_equal(t->type, Cent_token_colon, "type 3.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.3");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 3.3");
    Zinc_expect_string(&t->value, "String", "value 3.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.4");
    Zinc_expect_int_equal(t->type, Cent_token_modifier, "type 3.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.5");
    Zinc_expect_int_equal(t->type, Cent_token_comma, "type 3.5");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 3.6");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 4.1");
    Zinc_expect_string(&t->value, "solo", "value 4.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_colon, "type 4.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 4.3");
    Zinc_expect_string(&t->value, "Bool", "value 4.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_comma, "type 4.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 3.5");
    Cent_token_destroy(t);
    free(t);

    /* line 5 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 5.1");
    Zinc_expect_string(&t->value, "mute", "value 5.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.2");
    Zinc_expect_int_equal(t->type, Cent_token_colon, "type 5.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.3");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 5.3");
    Zinc_expect_string(&t->value, "Bool", "value 5.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.4");
    Zinc_expect_int_equal(t->type, Cent_token_comma, "type 5.4");
    Cent_token_destroy(t);
    free(t);
    
    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 5.5");
    Cent_token_destroy(t);
    free(t);

    /* line 6 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 6.1");
    Zinc_expect_int_equal(t->type, Cent_token_right_curly_brace, "type 6.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 6.2");
    Cent_token_destroy(t);
    free(t);

    /* line 7 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 7.1");
    Zinc_expect_int_equal(t->type, Cent_token_children, "type 7.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 7.2");
    Zinc_expect_int_equal(t->type, Cent_token_left_curly_brace, "type 7.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 7.3");
    Cent_token_destroy(t);
    free(t);

    /* line 8 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 8.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 8.1");
    Zinc_expect_string(&t->value, "Test", "value 8.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 8.2");
    Zinc_expect_int_equal(t->type, Cent_token_comma, "type 8.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 8.3");
    Cent_token_destroy(t);
    free(t);

    /* line 9 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 9.1");
    Zinc_expect_int_equal(t->type, Cent_token_right_curly_brace, "type 9.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 9.2");
    Cent_token_destroy(t);
    free(t);

    /* line 10 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 10.1");
    Zinc_expect_int_equal(t->type, Cent_token_right_curly_brace, "type 10.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 10.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 27");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 27");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);

    test_lex_teardown(&ld);
}

void test_lex_enum()
{
    Zinc_test_name(__func__);
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
    Zinc_expect_int_equal(t->type, Cent_token_enum, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 1.2");
    Zinc_expect_string(&t->value, "Symbol_type", "value 1.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 1.3");
    Cent_token_destroy(t);
    free(t);

    /* line 2 */
    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 2.1");
    Zinc_expect_string(&t->value, "Variable", "value 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 2.2");
    Cent_token_destroy(t);
    free(t);

    /* line 3 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 3.1");
    Zinc_expect_string(&t->value, "Type", "value 3.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 3.1");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 4.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 4.1");
    Zinc_expect_string(&t->value, "Info", "value 4.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 4.2");
    Cent_token_destroy(t);
    free(t);

    /* line 5 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.1");
    Zinc_expect_int_equal(t->type, Cent_token_end, "type 5.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 5.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.3");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 5.3");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);

    test_lex_teardown(&ld);
}

void test_lex_top_level_assign()
{
    Zinc_test_name(__func__);
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
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    /* line 2 */
    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 2.1");
    Zinc_expect_string(&t->value, "i32", "value 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 2.2");
    Zinc_expect_int_equal(t->type, Cent_token_equal, "type 2.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 2.3");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 2.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 2.4");
    Zinc_expect_int_equal(t->type, Cent_token_left_curly_brace, "type 2.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 2.5");
    Cent_token_destroy(t);
    free(t);

    /* line 3 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.1");
    Zinc_expect_int_equal(t->type, Cent_token_dot, "type 3.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.2");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 3.2");
    Zinc_expect_string(&t->value, "type", "value 3.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.3");
    Zinc_expect_int_equal(t->type, Cent_token_equal, "type 3.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.4");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 3.4");
    Zinc_expect_string(&t->value, "Type_def_type", "value 3.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.5");
    Zinc_expect_int_equal(t->type, Cent_token_double_colon, "type 3.5");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 3.6");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 3.6");
    Zinc_expect_string(&t->value, "Integer", "value 3.6");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 3.7");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 4.1");
    Zinc_expect_int_equal(t->type, Cent_token_dot, "type 4.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 4.2");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 4.2");
    Zinc_expect_string(&t->value, "name", "value 4.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 4.3");
    Zinc_expect_int_equal(t->type, Cent_token_equal, "type 4.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 4.4");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 4.4");
    Zinc_expect_string(&t->value, "i32", "value 4.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 4.5");
    Cent_token_destroy(t);
    free(t);

    /* line 5 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.1");
    Zinc_expect_int_equal(t->type, Cent_token_dot, "type 5.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.2");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 5.2");
    Zinc_expect_string(&t->value, "bit_count", "value 5.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.3");
    Zinc_expect_int_equal(t->type, Cent_token_equal, "type 5.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 5.4");
    Zinc_expect_int_equal(t->type, Cent_token_natural, "type 5.4");
    Zinc_expect_string(&t->value, "32", "value 5.4");
    Zinc_expect_int64_t_equal(t->data.natural, 32, "integer 5.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 5.5");
    Cent_token_destroy(t);
    free(t);

    /* line 6 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 6.1");
    Zinc_expect_int_equal(t->type, Cent_token_dot, "type 6.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 6.2");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 6.2");
    Zinc_expect_string(&t->value, "is_signed", "value 6.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 6.3");
    Zinc_expect_int_equal(t->type, Cent_token_equal, "type 6.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 6.4");
    Zinc_expect_int_equal(t->type, Cent_token_true, "type 6.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 6.5");
    Cent_token_destroy(t);
    free(t);

    /* line 7 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 7.1");
    Zinc_expect_int_equal(t->type, Cent_token_right_curly_brace, "type 7.1");
    Cent_token_destroy(t);
    free(t);
    
    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 7.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr eof");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type eof");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_builtin()
{
    Zinc_test_name(__func__);
    Cent_token* t = NULL;
    Cent_lex_data ld;

    test_lex_setup(&ld,
        "@file_name\n"
        "abc\n"
    );

    /* line 1 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 1.1");
    Zinc_expect_int_equal(t->builtin_type, Cent_builtin_type_file_name, "builtin type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    /* line 4 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 2.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 2.1");
    Zinc_expect_int_equal(t->builtin_type, Cent_builtin_type_none, "builtin type 2.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 2.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr eof");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type eof");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_builtin_error()
{
    Zinc_test_name(__func__);
    Cent_token* t = NULL;
    Cent_lex_data ld;

    test_lex_setup(&ld,
        "@abc\n"
    );

    /* line 1 */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_id, "type 1.1");
    Zinc_expect_int_equal(t->builtin_type, Cent_builtin_type_none, "builtin type 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_expect_int_equal(t->type, Cent_token_newline, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    /* eof */
    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr eof");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type eof");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_has_errors(ld.errors);
    Zinc_expect_source_error(ld.errors, "invalid builtin id: @abc");
    test_lex_teardown(&ld);
}

void test_lex_escape_backslash()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\\\\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "\\", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_escape_forward_slash()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\/\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "/", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_escape_backspace()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\b\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "\b", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_escape_form_feed()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\f\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "\f", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_escape_newline()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\n\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "\n", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_escape_carriage_return()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\r\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "\r", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_escape_tab()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\t\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "\t", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_escape_unicode()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "\"\\u03b8\"");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_string, "type 1.1");
    Zinc_expect_string(&t->value, "θ", "value 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_semicolon()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "1;2;3");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_natural, "type 1.1");
    Zinc_expect_uint64_t_equal(t->data.natural, 1, "natural 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_semicolon, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.3");
    Zinc_expect_int_equal(t->type, Cent_token_natural, "type 1.3");
    Zinc_expect_uint64_t_equal(t->data.natural, 2, "integer 1.3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.4");
    Zinc_expect_int_equal(t->type, Cent_token_semicolon, "type 1.4");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.5");
    Zinc_expect_int_equal(t->type, Cent_token_natural, "type 1.5");
    Zinc_expect_uint64_t_equal(t->data.natural, 3, "integer 1.5");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.6");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.6");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_error_invalid_character()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "~");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.1");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_has_errors(ld.errors);
    struct Zinc_error* e = Zinc_expect_source_error(ld.errors, "invalid character: ~");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos e");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos e");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line e");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col e");
    test_lex_teardown(&ld);
}

void test_lex_number_integer()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "2918");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_uint64_t_equal(t->data.integer, 2918, "integer 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_number_fraction()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "5.123");

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_real, "type 1.1");
    Zinc_expect_double_equal(t->data.real, 5.123, "real 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex_number_exponent()
{
    Zinc_test_name(__func__);

    Cent_token* t = NULL;
    Cent_lex_data ld;
    test_lex_setup(&ld, "5.123e3");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.1");
    Zinc_expect_int_equal(t->type, Cent_token_real, "type 1.1");
    Zinc_expect_double_equal(t->data.real, 5.123e3, "integer 1.1");
    Cent_token_destroy(t);
    free(t);

    t = lex(&ld);
    Zinc_assert_ptr(t, "ptr 1.2");
    Zinc_expect_int_equal(t->type, Cent_token_eof, "type 1.2");
    Cent_token_destroy(t);
    free(t);

    Zinc_expect_no_errors(ld.errors);
    test_lex_teardown(&ld);
}

void test_lex()
{
    test_lex_element();
    test_lex_enum();
    test_lex_top_level_assign();
    test_lex_builtin();
    test_lex_builtin_error();

    test_lex_escape_backslash();
    test_lex_escape_forward_slash();
    test_lex_escape_backspace();
    test_lex_escape_form_feed();
    test_lex_escape_newline();
    test_lex_escape_carriage_return();
    test_lex_escape_tab();

    test_lex_escape_unicode();

    test_lex_semicolon();

    test_lex_error_invalid_character();

    test_lex_number_integer();
    test_lex_number_fraction();
    test_lex_number_exponent();
}
