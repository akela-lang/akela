#include "token.h"
#include "lex_tools.h"
#include "zinc/input_unicode.h"
#include "zinc/error.h"
#include <string.h>
#include "zinc/utf8.h"
#include <assert.h>
#include "zinc/unicode.h"

void Json_lex_start(Json_lex_data* jld, Json_token* t);
void Json_lex_string(Json_lex_data* jld, Json_token* t);
void Json_lex_string_escape(Json_lex_data* jld, Json_token* t);
void Json_lex_string_escape_unicode(Json_lex_data* jld, Json_token* t);
bool Json_is_number(char c[4], int num);
void Json_lex_number(Json_lex_data* jld, Json_token* t);
void Json_lex_number_integer(Json_lex_data* jld, Json_token* t);
void Json_lex_number_fraction(Json_lex_data* jld, Json_token* t);
void Json_lex_number_exponent(Json_lex_data* jld, Json_token* t);
bool Json_lex_word(Json_lex_data* jld, Json_token* t);

Json_token* Json_lex(Json_lex_data* jld)
{
    Json_token* t = NULL;
    Json_token_create(&t);
    Json_lex_start(jld, t);
    if (t->type == Json_token_type_number) {
        if (t->number_type == Json_number_type_integer) {
            buffer_finish(&t->value);
            t->number.integer = strtoll(t->value.buf, NULL, 10);
        } else if (t->number_type == Json_number_type_fp) {
            buffer_finish(&t->value);
            t->number.fp = strtod(t->value.buf, NULL);
        }
    }
    return t;
}

void Json_lex_start(Json_lex_data* jld, Json_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;

    while (true) {
        enum result r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            continue;
        }

        if (done) {
            t->type = Json_token_type_eof;
            t->loc = loc;
            t->loc.end_pos = t->loc.start_pos + 3;
            return;
        }

        if (num != 1) {
            struct buffer bf;
            buffer_init(&bf);
            buffer_add(&bf, c, num);
            error_list_set(jld->el, &loc, "invalid character: %b", &bf);
            buffer_destroy(&bf);
            continue;
        }

        if (c[0] == '"') {
            t->type = Json_token_type_string;
            t->loc = loc;
            Json_lex_string(jld, t);
            loc = InputUnicodeGetLocation(jld->input_obj, jld->input_vtable);
            t->loc.end_pos = loc.start_pos;
            return;
        }

        if (Json_is_number(c, num)) {
            t->type = Json_token_type_number;
            t->loc = loc;
            buffer_add_char(&t->value, c[0]);
            Json_lex_number(jld, t);
            loc = InputUnicodeGetLocation(jld->input_obj, jld->input_vtable);
            t->loc.end_pos = loc.start_pos;
            return;
        }

        if (num == 1 && isalpha(c[0])) {
            buffer_add_char(&t->value, c[0]);
            t->loc = loc;
            if (Json_lex_word(jld, t)) {
                loc = InputUnicodeGetLocation(jld->input_obj, jld->input_vtable);
                t->loc.end_pos = loc.start_pos;
                return;
            }
            continue;
        }

        if (c[0] == ' ') {
            continue;
        }

        if (c[0] == '\n') {
            continue;
        }

        if (c[0] == '\r') {
            continue;
        }

        if (c[0] == '\t') {
            continue;
        }

        if (c[0] == '[') {
            t->type = Json_token_type_left_square_bracket;
            t->loc = loc;
            return;
        }

        if (c[0] == ']') {
            t->type = Json_token_type_right_square_bracket;
            t->loc = loc;
            return;
        }

        if (c[0] == ',') {
            t->type = Json_token_type_comma;
            t->loc = loc;
            return;
        }

        if (c[0] == '{') {
            t->type = Json_token_type_left_curly_brace;
            t->loc = loc;
            return;
        }

        if (c[0] == '}') {
            t->type = Json_token_type_right_curly_brace;
            t->loc = loc;
            return;
        }

        if (c[0] == ':') {
            t->type = Json_token_type_colon;
            t->loc = loc;
            return;
        }

        error_list_set(jld->el, &loc, "invalid character: %c", c[0]);
    }
}

void Json_lex_string(Json_lex_data* jld, Json_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;

    while (true) {
        enum result r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            continue;
        }

        if (done) {
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            return;
        }

        if (num == 1 && c[0] == '"') {
            return;
        }

        if (num == 1 && c[0] == '\\') {
            Json_lex_string_escape(jld, t);
            continue;
        }

        UChar32 cp;
        Json_convert_char(c, num, &cp);
        if (cp < 0x20) {
            error_list_set(jld->el, &loc, "code point is less than \\u0020");
        } else if (cp > 0x10FFFF) {
            error_list_set(jld->el, &loc, "code point greater than \\u10FFFF");
        } else {
            buffer_add(&t->value, c, num);
        }
    }

}

void Json_lex_string_escape(Json_lex_data* jld, Json_token* t)
{
    enum result r;
    char c[4];
    int num;
    struct location loc;
    bool done;
    r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
    if (r == result_error) {
        error_list_set(jld->el, &loc, error_message);
        return;
    }

    if (done) {
        error_list_set(jld->el, &loc, "missing escape character");
        InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
        return;
    }

    if (num != 1) {
        struct buffer bf;
        buffer_init(&bf);
        buffer_add(&bf, c, num);
        error_list_set(jld->el, &loc, "invalid escape character: %b", &bf);
        buffer_destroy(&bf);
        return;
    }

    if (c[0] == '\\') {
        buffer_add(&t->value, c, num);
        return;
    }

    if (c[0] == '/') {
        buffer_add_char(&t->value, '/');
        return;
    }

    if (c[0] == 'b') {
        buffer_add_char(&t->value, '\b');
        return;
    }

    if (c[0] == 'f') {
        buffer_add_char(&t->value, '\f');
        return;
    }

    if (c[0] == 'n') {
        buffer_add_char(&t->value, '\n');
        return;
    }

    if (c[0] == 'r') {
        buffer_add_char(&t->value, '\r');
        return;
    }

    if (c[0] == 't') {
        buffer_add_char(&t->value, '\t');
        return;
    }

    if (c[0] == 'u') {
        Json_lex_string_escape_unicode(jld, t);
        return;
    }

    struct buffer bf;
    buffer_init(&bf);
    buffer_add(&bf, c, num);
    error_list_set(jld->el, &loc, "invalid escape character: %b", &bf);
    buffer_destroy(&bf);
}

void Json_lex_string_escape_unicode(Json_lex_data* jld, Json_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;
    enum result r;

    struct buffer bf;
    buffer_init(&bf);
    buffer_copy_str(&bf, "\\u");
    bool valid = true;
    struct location first_loc;
    first_loc = InputUnicodeGetLocation(jld->input_obj, jld->input_vtable);
    first_loc.start_pos -= 2;
    first_loc.col -= 2;

    /* first four hex digits */
    for (int i = 0; i < 4; i++) {
        r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);

        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
        } else {
            if (done) {
                error_list_set(jld->el, &loc, "unicode escape not finished");
                buffer_destroy(&bf);
                return;
            }
            if (is_hex_digit(c, num)) {
                buffer_add(&bf, c, num);
            } else {
                error_list_set(jld->el, &loc, "invalid hex digit: %c", c[0]);
                valid = false;
            }
        }
    }
    /* optional two hex digits */
    for (int i = 0; i < 2; i++) {
        r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            break;
        }
        if (done) {
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            break;
        }
        if (!is_hex_digit(c, num)) {
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            break;
        }
        buffer_add(&bf, c, num);
    }
    buffer_finish(&bf);

    struct location end_loc;
    end_loc = InputUnicodeGetLocation(jld->input_obj, jld->input_vtable);
    first_loc.end_pos = end_loc.start_pos;

    if (valid) {
        unsigned int cp = char4_to_hex(bf.buf + 2, (int)bf.size - 2);
        if (cp < 0x20) {
            error_list_set(jld->el, &first_loc, "code point is less than \\u0020: %s", bf.buf);
        } else if (cp > 0x10FFFF) {
            error_list_set(jld->el, &first_loc, "code point greater than \\u10FFFF: %s", bf.buf);
        } else {
            char dest[4];
            num = code_to_utf8(dest, cp);
            buffer_add(&t->value, dest, num);
        }
    }

    buffer_destroy(&bf);
}

bool Json_is_number(char c[4], int num)
{
    if (num != 1) return false;
    if (c[0] == '-') return true;
    if (c[0] == '+') return true;   /* error case if starting with + */
    if (c[0] == '.') return true;   /* error case if starting with . */
    if (isdigit(c[0])) return true;
    return false;
}

void Json_lex_number(Json_lex_data* jld, Json_token* t)
{
    t->number_type = Json_number_type_integer;

    char c[4];
    int num;
    struct location loc;
    bool done;
    enum result r;

    size_t digit_count = 0;
    char first_digit;
    struct location first_loc = InputUnicodeGetLocation(jld->input_obj, jld->input_vtable);
    first_loc.end_pos = first_loc.start_pos;
    first_loc.start_pos--;
    first_loc.col--;

    char first = t->value.buf[0];
    num = NUM_BYTES(first);

    if (num == 1 && first == '.') {
        error_list_set(jld->el, &first_loc, "number starts with period");
        Json_lex_number_fraction(jld, t);
        return;
    }

    if (num == 1 && first == '+') {
        error_list_set(jld->el, &first_loc, "number starts with plus sign");
    }

    if (isdigit(first)) {
        digit_count++;
        first_digit = first;
    }

    while (true) {
        r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            break;
        }

        if (done) {
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            break;
        }

        if (num == 1 && isdigit(c[0])) {
            if (digit_count == 1 && first_digit == '0') {
                t->value.size--;
                first_digit = c[0];
                digit_count--;
                error_list_set(jld->el, &first_loc, "leading zero with no other digits or faction");
            }
            buffer_add_char(&t->value, c[0]);
            digit_count++;
            if (digit_count == 1) {
                first_digit = c[0];
            }
            continue;
        }

        if (num == 1 && c[0] == '.') {
            buffer_add_char(&t->value, '.');
            Json_lex_number_fraction(jld, t);
            break;;
        }

        if (num == 1 && (c[0] == 'E' || c[0] == 'e')) {
            buffer_add_char(&t->value, c[0]);
            Json_lex_number_exponent(jld, t);
            break;
        }

        InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
        break;
    }

    if (t->value.size == 1 && first == '-') {
        error_list_set(jld->el, &first_loc, "invalid number");
        t->value.buf[0] = '0';
    }

    if (t->value.size == 1 && first == '+') {
        error_list_set(jld->el, &first_loc, "invalid number");
        t->value.buf[0] = '0';
    }
}

void Json_lex_number_fraction(Json_lex_data* jld, Json_token* t)
{
    t->number_type = Json_number_type_fp;

    char c[4];
    int num;
    struct location loc;
    bool done;
    enum result r;
    size_t digit_count = 0;

    while (true) {
        r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            break;
        }

        if (done) {
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            break;
        }

        if (num == 1 && isdigit(c[0])) {
            buffer_add_char(&t->value, c[0]);
            digit_count++;
            continue;
        }

        if (num == 1 && (c[0] == 'E' || c[0] == 'e')) {
            buffer_add_char(&t->value, c[0]);
            Json_lex_number_exponent(jld, t);
            break;
        }

        InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
        break;
    }

    if (digit_count == 0) {
        error_list_set(jld->el, &t->loc, "no digits in fraction");
    }
}

void Json_lex_number_exponent(Json_lex_data* jld, Json_token* t)
{
    t->number_type = Json_number_type_fp;

    char c[4];
    int num;
    struct location loc;
    bool done;
    enum result r;
    size_t digit_count = 0;
    bool has_sign = false;

    while (true) {
        r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            break;
        }

        if (done) {
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            break;
        }

        if (num == 1 && (c[0] == '+' || c[0] == '-')) {
            if (digit_count == 0) {
                if (!has_sign) {
                    buffer_add_char(&t->value, c[0]);
                    has_sign = true;
                    continue;
                } else {
                    error_list_set(jld->el, &loc, "exponent already has a sign");
                    continue;
                }
            } else {
                error_list_set(jld->el, &loc, "sign after exponent digits");
                continue;
            }
        }

        if (num == 1 && isdigit(c[0])) {
            buffer_add_char(&t->value, c[0]);
            digit_count++;
            continue;
        }

        InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
        break;
    }

    if (digit_count == 0) {
        error_list_set(jld->el, &t->loc, "no digits in exponent");
    }
}

bool Json_lex_word(Json_lex_data* jld, Json_token* t)
{
    char c[4];
    int num;
    struct location loc;
    bool done;
    enum result r;

    while (true) {
        r = InputUnicodeNext(jld->input_obj, jld->input_vtable, c, &num, &loc, &done);
        if (r == result_error) {
            error_list_set(jld->el, &loc, error_message);
            break;
        }

        if (done) {
            InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
            break;
        }

        if (num == 1 && isalpha(c[0])) {
            buffer_add_char(&t->value, c[0]);
            continue;
        }

        InputUnicodeRepeat(jld->input_obj, jld->input_vtable);
        break;
    }

    if (buffer_compare_str(&t->value, "true")) {
        t->type = Json_token_type_true;
        return true;
    }

    if (buffer_compare_str(&t->value, "false")) {
        t->type = Json_token_type_false;
        return true;
    }

    if (buffer_compare_str(&t->value, "null")) {
        t->type = Json_token_type_null;
        return true;
    }

    loc = InputUnicodeGetLocation(jld->input_obj, jld->input_vtable);
    t->loc.end_pos = loc.start_pos;

    error_list_set(jld->el, &t->loc, "invalid word (%b), expecting true, false, or null", &t->value);
    t->type = Json_token_type_none;
    buffer_clear(&t->value);
    return false;
}