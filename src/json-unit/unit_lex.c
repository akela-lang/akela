#include <zinc/utf8.h>
#include "unit_lex_tools.h"
#include "json/lex.h"
#include "zinc/unicode.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Json_unit_lex_string(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"hello\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "hello", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_backslash(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\\\ \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " \\ ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_slash(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\/ \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " / ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_backspace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\b \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " \b ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_form_feed(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\f \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " \f ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_newline(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\n \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " \n ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_carriage_return(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\r \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " \r ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_tab(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\t \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " \t ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_escape_unicode(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\" \\u0391 \"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, " Α ", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 10, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_start_error_multibyte(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "θ");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid character: θ");
    Zinc_expect_int_equal(test, token->type, Json_token_type_eof, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 2, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_start_error_single_byte(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "|");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid character: |");
    Zinc_expect_int_equal(test, token->type, Json_token_type_eof, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_error_missing_escape_character(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "missing escape character");
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 2, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 3, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_error_invalid_escape_character_multibyte(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\θ\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid escape character: θ");
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 2, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 3, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_error_invalid_unicode_escape_not_finished(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\u039");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "unicode escape not finished");
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 6, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 7, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_error_unicode_escape_invalid_hex_digit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\u039G\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid hex digit: G");
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 6, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 7, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_error_unicode_code_point_less_that_0x20(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\u0019\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "code point is less than \\u0020: \\u0019");
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 1, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 7, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 2, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_unicode_code_point_five_digits(Zinc_test *test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\u1fa89\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "🪉", "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 9, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_unicode_code_point_six_digits(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\u10ffff\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    char dest[5];
    Zinc_code_to_utf8((unsigned char*)dest, 0x10ffff);
    int num = ZINC_NUM_BYTES(dest[0]);
    dest[num] = '\0';
    Zinc_expect_string(test, &token->value, dest, "value token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 10, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_unicode_error_code_point_too_large(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\u110000\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    struct Zinc_error* e = Zinc_expect_source_error(test,
        ld.el,
        "code point greater than \\u10FFFF: \\u110000"
    );
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 1, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 9, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 2, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_string_error_invalid_escape_character_single_byte(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "\"\\|\"");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid escape character: |");
    Zinc_expect_int_equal(test, token->type, Json_token_type_string, "type token");
    Zinc_expect_string(test, &token->value, "", "value token");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 2, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 3, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_integer(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "315");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "315", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(test, token->number.integer, 315, "integer");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_integer_negative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "-35");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "-35", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(test, token->number.integer, -35, "integer");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_fraction(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "5.13");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "5.13", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 5.13, "fp");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_fraction_leading_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "0.5");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "0.5", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 0.5, "fp");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_exponent(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "1.5e2");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "1.5e2", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 1.5e2, "fp");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 5, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_exponent_positive(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "4.5e+2");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "4.5e+2", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 4.5e2, "fp");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_exponent_negative(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "4.5e-2");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "4.5e-2", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 4.5e-2, "fp");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 6, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_starts_with_period(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, ".1");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "number starts with period");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, ".1", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 0.1, "fp");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_starts_with_plus_sign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "+1");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "number starts with plus sign");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "+1", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(test, token->number.integer, 1, "integer");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_minus_no_digits(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "-");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid number");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "0", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(test, token->number.integer, 0, "integer");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_plus_no_digits(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "+");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid number");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "0", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(test, token->number.integer, 0, "integer");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_leading_zero(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "01");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e =Zinc_expect_source_error(test, ld.el, "leading zero with no other digits or faction");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "1", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(test, token->number.integer, 1, "integer");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_leading_zero2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "001");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e =Zinc_expect_source_error(test, ld.el, "leading zero with no other digits or faction");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "1", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_integer, "number type");
    Zinc_expect_long_long_equal(test, token->number.integer, 1, "integer");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_no_digits_in_fraction(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "1.");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "no digits in fraction");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "1.", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 1.0, "fp");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_exponent_multiple_signs(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "1e--1");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "exponent already has a sign");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "1e-1", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 1e-1, "fp");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 3, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 4, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_exponent_sign_after_digits(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "1e-1-");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "sign after exponent digits");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "1e-1", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 1e-1, "fp");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 4, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 5, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 5, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_number_error_exponent_no_digits(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "1e");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e =Zinc_expect_source_error(test, ld.el, "no digits in exponent");
    Zinc_expect_int_equal(test, token->type, Json_token_type_number, "type token");
    Zinc_expect_string(test, &token->value, "1e", "value token");
    Zinc_expect_int_equal(test, token->number_type, Json_number_type_fp, "number type");
    Zinc_expect_double_equal(test, token->number.fp, 1, "fp");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_left_square_bracket(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "[");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_left_square_bracket, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_right_square_bracket(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "]");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_right_square_bracket, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_comma(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, ",");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_comma, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_left_curly_brace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "{");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_left_curly_brace, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_right_curly_brace(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "}");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_right_curly_brace, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_right_colon(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, ":");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_colon, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 1, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_true(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "true");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_true, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_false(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "false");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_false, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 5, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_null(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "null");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_no_errors(test, ld.el);
    Zinc_expect_int_equal(test, token->type, Json_token_type_null, "type token");
    Zinc_expect_size_t_equal(test, token->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, token->loc.end_pos, 4, "end pos token");
    Zinc_expect_size_t_equal(test, token->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, token->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_lex_word_error(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Json_lex_data ld;
    Zinc_unit_lex_setup(&ld, "abc");

    Json_token* token = Json_lex(&ld);
    Zinc_expect_has_errors(test, ld.el);
    struct Zinc_error* e = Zinc_expect_source_error(test, ld.el, "invalid word (abc), expecting true, false, or null");
    Zinc_expect_size_t_equal(test, e->loc.start_pos, 0, "start pos token");
    Zinc_expect_size_t_equal(test, e->loc.end_pos, 3, "end pos token");
    Zinc_expect_size_t_equal(test, e->loc.line, 1, "line token");
    Zinc_expect_size_t_equal(test, e->loc.col, 1, "col token");

    Json_token_destroy(token);
    free(token);
    Zinc_unit_lex_teardown(&ld);
}

void Json_unit_match_tools_convert_char1(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    uint32_t cp;
    int num = Zinc_utf8_to_utf32("A", &cp);
    Zinc_assert_true(test, num, "valid utf8");
    Zinc_expect_uint32_t_equal(test, cp, 0x41, "cp");
}

void Json_unit_match_tools_convert_char2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    uint32_t cp;
    int num = Zinc_utf8_to_utf32("θ", &cp);
    Zinc_assert_true(test, num, "valid utf8");
    Zinc_expect_uint32_t_equal(test, cp, 0x3B8, "cp");
}

void Zinc_unit_lex(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Json_unit_lex_string);
        Zinc_test_register(test, Json_unit_lex_string_escape_backslash);
        Zinc_test_register(test, Json_unit_lex_string_escape_slash);
        Zinc_test_register(test, Json_unit_lex_string_escape_backspace);
        Zinc_test_register(test, Json_unit_lex_string_escape_form_feed);
        Zinc_test_register(test, Json_unit_lex_string_escape_newline);
        Zinc_test_register(test, Json_unit_lex_string_escape_carriage_return);
        Zinc_test_register(test, Json_unit_lex_string_escape_tab);
        Zinc_test_register(test, Json_unit_lex_string_escape_unicode);
        Zinc_test_register(test, Json_unit_lex_start_error_multibyte);
        Zinc_test_register(test, Json_unit_lex_start_error_single_byte);
        Zinc_test_register(test, Json_unit_lex_string_error_missing_escape_character);
        Zinc_test_register(test, Json_unit_lex_string_error_invalid_escape_character_multibyte);
        Zinc_test_register(test, Json_unit_lex_string_error_invalid_unicode_escape_not_finished);
        Zinc_test_register(test, Json_unit_lex_string_error_unicode_escape_invalid_hex_digit);
        Zinc_test_register(test, Json_unit_lex_string_error_unicode_code_point_less_that_0x20);
        Zinc_test_register(test, Json_unit_lex_string_error_invalid_escape_character_single_byte);
        Zinc_test_register(test, Json_unit_lex_string_unicode_code_point_five_digits);
        Zinc_test_register(test, Json_unit_lex_string_unicode_code_point_six_digits);
        Zinc_test_register(test, Json_unit_lex_string_unicode_error_code_point_too_large);

        Zinc_test_register(test, Json_unit_lex_number_integer);
        Zinc_test_register(test, Json_unit_lex_number_integer_negative);
        Zinc_test_register(test, Json_unit_lex_number_fraction);
        Zinc_test_register(test, Json_unit_lex_number_fraction_leading_zero);
        Zinc_test_register(test, Json_unit_lex_number_exponent);
        Zinc_test_register(test, Json_unit_lex_number_exponent_positive);
        Zinc_test_register(test, Json_unit_lex_number_exponent_negative);
        Zinc_test_register(test, Json_unit_lex_number_error_starts_with_period);
        Zinc_test_register(test, Json_unit_lex_number_error_starts_with_plus_sign);
        Zinc_test_register(test, Json_unit_lex_number_error_minus_no_digits);
        Zinc_test_register(test, Json_unit_lex_number_error_plus_no_digits);
        Zinc_test_register(test, Json_unit_lex_number_error_leading_zero);
        Zinc_test_register(test, Json_unit_lex_number_error_leading_zero2);
        Zinc_test_register(test, Json_unit_lex_number_error_no_digits_in_fraction);
        Zinc_test_register(test, Json_unit_lex_number_error_exponent_multiple_signs);
        Zinc_test_register(test, Json_unit_lex_number_error_exponent_sign_after_digits);
        Zinc_test_register(test, Json_unit_lex_number_error_exponent_no_digits);

        Zinc_test_register(test, Json_unit_lex_left_square_bracket);
        Zinc_test_register(test, Json_unit_lex_right_square_bracket);
        Zinc_test_register(test, Json_unit_lex_comma);

        Zinc_test_register(test, Json_unit_lex_left_curly_brace);
        Zinc_test_register(test, Json_unit_lex_right_curly_brace);
        Zinc_test_register(test, Json_unit_lex_right_colon);

        Zinc_test_register(test, Json_unit_lex_true);
        Zinc_test_register(test, Json_unit_lex_false);
        Zinc_test_register(test, Json_unit_lex_null);
        Zinc_test_register(test, Json_unit_lex_word_error);

        Zinc_test_register(test, Json_unit_match_tools_convert_char1);
        Zinc_test_register(test, Json_unit_match_tools_convert_char2);

        return;
    }

    Zinc_test_perform(test);
}