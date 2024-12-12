#include <zinc/error_unit_test.h>
#include <zinc/utf8.h>

#include "test_lex_tools.h"
#include "zinc/unit_test.h"
#include "json/lex.h"

void test_lex_string()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"hello\"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "hello", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_backslash()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\\\ \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " \\ ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_slash()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\/ \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " / ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_backspace()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\b \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " \b ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_form_feed()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\f \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " \f ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_newline()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\n \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " \n ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_carriage_return()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\r \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " \r ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_tab()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\t \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " \t ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_escape_unicode()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\" \\u0391 \"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, " Α ", "value token");

    test_lex_teardown(&jld);
}

void test_lex_start_error_multibyte()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "θ");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid character: θ");
    expect_int_equal(token->type, Json_token_type_eof, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_start_error_single_byte()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "|");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid character: |");
    expect_int_equal(token->type, Json_token_type_eof, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_error_missing_escape_character()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "missing escape character");
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_error_invalid_escape_character_multibyte()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\θ\"");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid escape character: θ");
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_error_invalid_unicode_escape_not_finished()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\u039");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "unicode escape not finished");
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_error_unicode_escape_invalid_hex_digit()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\u039G\"");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid hex digit: G");
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_error_unicode_code_point_less_that_0x20()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\u0019\"");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "code point is less than \\u0020: \\u0019");
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_unicode_code_point_five_digits()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\u1fa89\"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "🪉", "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_unicode_code_point_six_digits()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\u10ffff\"");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    char dest[5];
    code_to_utf8(dest, 0x10ffff);
    int num = NUM_BYTES(dest[0]);
    dest[num] = '\0';
    expect_str(&token->value, dest, "value token");

    test_lex_teardown(&jld);
}

void test_lex_string_unicode_code_point_too_large()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\u110000\"");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_source_error(jld.el, "code point greater than \\u10FFFF: \\u110000");

    test_lex_teardown(&jld);
}

void test_lex_string_error_invalid_escape_character_single_byte()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"\\|\"");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid escape character: |");
    expect_int_equal(token->type, Json_token_type_string, "type token");
    expect_str(&token->value, "", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_integer()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "315");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "315", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_integer_negative()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "-35");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "-35", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_fraction()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "5.13");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "5.13", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_fraction_leading_zero()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "0.5");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "0.5", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_exponent()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "1.5e2");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "1.5e2", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_exponent_positive()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "4.5e+2");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "4.5e+2", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_exponent_negative()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "4.5e-2");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "4.5e-2", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_starts_with_period()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, ".1");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "number starts with period");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, ".1", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_starts_with_plus_sign()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "+1");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "number starts with plus sign");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "+1", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_minus_no_digits()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "-");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid number");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "0", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_plus_no_digits()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "+");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid number");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "0", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_leading_zero()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "01");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "leading zero with no other digits or faction");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "01", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_no_digits_in_fraction()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "1.");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "no digits in fraction");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "1.", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_exponent_multiple_signs()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "1e--1");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "exponent already has a sign");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "1e-1", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_exponent_sign_after_digits()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "1e-1-");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "sign after exponent digits");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "1e-1", "value token");

    test_lex_teardown(&jld);
}

void test_lex_number_error_exponent_no_digits()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "1e");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "no digits in exponent");
    expect_int_equal(token->type, Json_token_type_number, "type token");
    expect_str(&token->value, "1e", "value token");

    test_lex_teardown(&jld);
}

void test_lex_left_square_bracket()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "[");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_left_square_bracket, "type token");

    test_lex_teardown(&jld);
}

void test_lex_right_square_bracket()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "]");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_right_square_bracket, "type token");

    test_lex_teardown(&jld);
}

void test_lex_comma()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, ",");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_comma, "type token");

    test_lex_teardown(&jld);
}

void test_lex_left_curly_brace()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "{");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_left_curly_brace, "type token");

    test_lex_teardown(&jld);
}

void test_lex_right_curly_brace()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "}");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_right_curly_brace, "type token");

    test_lex_teardown(&jld);
}

void test_lex_right_colon()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, ":");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_colon, "type token");

    test_lex_teardown(&jld);
}

void test_lex_true()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "true");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_true, "type token");

    test_lex_teardown(&jld);
}

void test_lex_false()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "false");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_false, "type token");

    test_lex_teardown(&jld);
}

void test_lex_null()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "null");

    Json_token* token = Json_lex(&jld);
    expect_no_errors(jld.el);
    expect_int_equal(token->type, Json_token_type_null, "type token");

    test_lex_teardown(&jld);
}

void test_lex_word_error()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "abc");

    Json_token* token = Json_lex(&jld);
    expect_has_errors(jld.el);
    expect_source_error(jld.el, "invalid word: abc");

    test_lex_teardown(&jld);
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
    test_lex_string_unicode_code_point_too_large();

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
 }