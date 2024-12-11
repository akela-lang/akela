#include <zinc/error_unit_test.h>

#include "test_lex_tools.h"
#include "zinc/unit_test.h"
#include "json/lex.h"

void test_lex_string()
{
    test_name(__func__);
    Json_lex_data jld;
    test_lex_setup(&jld, "\"hello\"");

    Json_token* token = Json_lex(&jld);
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
 }