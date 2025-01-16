#include <zinc/error_unit_test.h>
#include <zinc/utf8.h>

#include "test_lex_tools.h"
#include "zinc/unit_test.h"
#include "json/lex.h"
#include "zinc/unicode.h"

void test_lex_string()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"hello\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "hello", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_backslash()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\\\ \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " \\ ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_slash()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\/ \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " / ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_backspace()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\b \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " \b ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_form_feed()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\f \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " \f ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_newline()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\n \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " \n ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_carriage_return()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\r \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " \r ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_tab()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\t \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " \t ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_escape_unicode()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\" \\u0391 \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, " Α ", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 10, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_start_error_multibyte()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "θ");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid character: θ");
    Zinc_expect_int_equal(token->type, Json_token_type_eof, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 2, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_start_error_single_byte()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "|");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid character: |");
    Zinc_expect_int_equal(token->type, Json_token_type_eof, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_error_missing_escape_character()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "missing escape character");
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 2, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 3, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_error_invalid_escape_character_multibyte()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\θ\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid escape character: θ");
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 2, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 3, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_error_invalid_unicode_escape_not_finished()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\u039");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "unicode escape not finished");
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 6, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 7, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_error_unicode_escape_invalid_hex_digit()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\u039G\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid hex digit: G");
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 6, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 7, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_error_unicode_code_point_less_that_0x20()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\u0019\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "code point is less than \\u0020: \\u0019");
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 1, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 2, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_unicode_code_point_five_digits()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\u1fa89\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "🪉", "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 9, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_unicode_code_point_six_digits()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\u10ffff\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    char dest[5];
    Zinc_code_to_utf8(dest, 0x10ffff);
    int num = ZINC_NUM_BYTES(dest[0]);
    dest[num] = '\0';
    Zinc_expect_str(&token->value, dest, "value token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 10, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_unicode_error_code_point_too_large()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\u110000\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    struct Zinc_error* e = Zinc_expect_source_error(
        ld.el,
        "code point greater than \\u10FFFF: \\u110000"
    );
    Zinc_expect_size_t_equal(e->loc.start_pos, 1, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 9, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 2, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_string_error_invalid_escape_character_single_byte()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "\"\\|\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid escape character: |");
    Zinc_expect_int_equal(token->type, Json_token_type_string, "type token");
    Zinc_expect_str(&token->value, "", "value token");
    Zinc_expect_size_t_equal(e->loc.start_pos, 2, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 3, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_integer()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "315");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "315", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(token->number.integer, 315, "integer");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_integer_negative()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "-35");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "-35", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(token->number.integer, -35, "integer");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_fraction()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "5.13");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "5.13", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 5.13, "fp");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_fraction_leading_zero()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "0.5");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "0.5", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 0.5, "fp");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_exponent()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "1.5e2");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "1.5e2", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 1.5e2, "fp");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 5, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_exponent_positive()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "4.5e+2");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "4.5e+2", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 4.5e2, "fp");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_exponent_negative()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "4.5e-2");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "4.5e-2", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 4.5e-2, "fp");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_starts_with_period()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, ".1");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "number starts with period");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, ".1", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 0.1, "fp");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_starts_with_plus_sign()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "+1");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "number starts with plus sign");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "+1", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(token->number.integer, 1, "integer");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_minus_no_digits()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "-");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid number");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "0", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(token->number.integer, 0, "integer");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_plus_no_digits()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "+");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid number");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "0", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(token->number.integer, 0, "integer");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_leading_zero()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "01");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e =Zinc_expect_source_error(ld.el, "leading zero with no other digits or faction");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "1", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(token->number.integer, 1, "integer");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_leading_zero2()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "001");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e =Zinc_expect_source_error(ld.el, "leading zero with no other digits or faction");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "1", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(token->number.integer, 1, "integer");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_no_digits_in_fraction()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "1.");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "no digits in fraction");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "1.", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 1.0, "fp");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_exponent_multiple_signs()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "1e--1");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "exponent already has a sign");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "1e-1", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 1e-1, "fp");
    Zinc_expect_size_t_equal(e->loc.start_pos, 3, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 4, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_exponent_sign_after_digits()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "1e-1-");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "sign after exponent digits");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "1e-1", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 1e-1, "fp");
    Zinc_expect_size_t_equal(e->loc.start_pos, 4, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 5, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 5, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_number_error_exponent_no_digits()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "1e");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e =Zinc_expect_source_error(ld.el, "no digits in exponent");
    Zinc_expect_int_equal(token->type, Json_token_type_number, "type token");
    Zinc_expect_str(&token->value, "1e", "value token");
    Zinc_expect_int_equal(token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(token->number.fp, 1, "fp");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_left_square_bracket()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "[");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_left_square_bracket, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_right_square_bracket()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "]");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_right_square_bracket, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_comma()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, ",");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_comma, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_left_curly_brace()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "{");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_left_curly_brace, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_right_curly_brace()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "}");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_right_curly_brace, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_right_colon()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, ":");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_colon, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_true()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "true");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_true, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_false()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "false");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_false, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 5, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_null()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "null");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(ld.el);
    Zinc_expect_int_equal(token->type, Json_token_type_null, "type token");
    Zinc_expect_size_t_equal(token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(token->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_lex_word_error()
{
    Zinc_test_name(__func__);
    Json_lex_data ld;
    test_lex_setup(&ld, "abc");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(ld.el, "invalid word (abc), expecting true, false, or null");
    Zinc_expect_size_t_equal(e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(e->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    test_lex_teardown(&ld);
}

void test_match_tools_convert_char1()
{
    Zinc_test_name(__func__);
    Zinc_string_slice slice = {"A", 1};
    UChar32 cp;
    Json_convert_slice(slice, &cp);
    Zinc_expect_int_equal(cp, 0x41, "cp");
}

void test_match_tools_convert_char2()
{
    Zinc_test_name(__func__);
    Zinc_string_slice slice = {"θ", 2};
    UChar32 cp;
    Json_convert_slice(slice, &cp);
    Zinc_expect_int_equal(cp, 0x3B8, "cp");
}

void test_lex()
{
    test_lex_string();
    test_lex_string_escape_backslash();
    test_lex_string_escape_slash();
    test_lex_string_escape_backspace();
    test_lex_string_escape_form_feed();
    test_lex_string_escape_newline();
    test_lex_string_escape_carriage_return();
    test_lex_string_escape_tab();
    test_lex_string_escape_unicode();
    test_lex_start_error_multibyte();
    test_lex_start_error_single_byte();
    test_lex_string_error_missing_escape_character();
    test_lex_string_error_invalid_escape_character_multibyte();
    test_lex_string_error_invalid_unicode_escape_not_finished();
    test_lex_string_error_unicode_escape_invalid_hex_digit();
    test_lex_string_error_unicode_code_point_less_that_0x20();
    test_lex_string_error_invalid_escape_character_single_byte();
    test_lex_string_unicode_code_point_five_digits();
    test_lex_string_unicode_code_point_six_digits();
    test_lex_string_unicode_error_code_point_too_large();

    test_lex_number_integer();
    test_lex_number_integer_negative();
    test_lex_number_fraction();
    test_lex_number_fraction_leading_zero();
    test_lex_number_exponent();
    test_lex_number_exponent_positive();
    test_lex_number_exponent_negative();
    test_lex_number_error_starts_with_period();
    test_lex_number_error_starts_with_plus_sign();
    test_lex_number_error_minus_no_digits();
    test_lex_number_error_plus_no_digits();
    test_lex_number_error_leading_zero();
    test_lex_number_error_leading_zero2();
    test_lex_number_error_no_digits_in_fraction();
    test_lex_number_error_exponent_multiple_signs();
    test_lex_number_error_exponent_sign_after_digits();
    test_lex_number_error_exponent_no_digits();

    test_lex_left_square_bracket();
    test_lex_right_square_bracket();
    test_lex_comma();

    test_lex_left_curly_brace();
    test_lex_right_curly_brace();
    test_lex_right_colon();

    test_lex_true();
    test_lex_false();
    test_lex_null();
    test_lex_word_error();

    test_match_tools_convert_char1();
    test_match_tools_convert_char2();
 }