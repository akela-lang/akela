#include "token.h"
#include "lex_data.h"
#include "zinc/input_unicode.h"
#include "zinc/result.h"
#include <unicode/ucnv.h>
#include "json/lex_tools.h"
#include "zinc/unicode.h"
#include "zinc/utf8.h"

void Worm_lex_start(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_id(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_string(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_string_escape(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_string_escape_unicode(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_number(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_number_fraction(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_number_exponent(Worm_lex_data* ld, Worm_token* t);
void Worm_lex_modifier(Worm_lex_data* ld, Worm_token* t);

Worm_token* Worm_lex(Worm_lex_data* ld)
{
    Worm_token* t = NULL;
    Worm_token_create(&t);
    Worm_lex_start(ld, t);
    return t;
}

void Worm_lex_start(Worm_lex_data* ld, Worm_token* t)
{
    while (true) {
        char c[5];
        int num;
        bool done;
        Zinc_location loc;
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            t->type = Worm_token_type_eof;
            t->loc = loc;
            return;
        }

        if (num > 1) {
            c[num] = '\0';
            Zinc_location loc2 = Zinc_input_unicode_get_location(ld->input, ld->vtable);
            loc.end_pos = loc2.start_pos;
            Zinc_error_list_set(ld->errors, &loc, "invalid character: %s", c);
            continue;
        }

        if (c[0] == '_' || isalpha(c[0])) {
            Zinc_string_add(&t->value, c, num);
            Worm_lex_id(ld, t);
            return;
        }

        if (c[0] == '"') {
            Worm_lex_string(ld, t);
            return;
        }

        if (isdigit(c[0])) {
            Zinc_string_add(&t->value, c, num);
            Worm_lex_number(ld, t);
            return;
        }

        if (c[0] == ' ') {
            continue;
        }

        if (c[0] == '\t') {
            continue;
        }

        if (c[0] == '.') {
            t->type = Worm_token_type_dot;
            t->loc = loc;
            return;
        }

        if (c[0] == '{') {
            t->type == Worm_token_type_left_curly_brace;
            t->loc = loc;
            return;
        }

        if (c[0] == '}') {
            t->type = Worm_token_type_right_curly_brace;
            t->loc = loc;
            return;
        }

        if (c[0] == '=') {
            t->type = Worm_token_type_equal;
            t->loc = loc;
            return;
        }

        if (c[0] == ';') {
            t->type = Worm_token_type_semicolon;
            t->loc = loc;
            return;
        }

        if (c[0] == '\n') {
            t->type = Worm_token_type_newline;
            t->loc = loc;
            return;
        }

        if (c[0] == '`') {
            t->type = Worm_token_type_modifier;
            t->loc = loc;
            Worm_lex_modifier(ld, t);
            return;
        }

        c[num] = '\0';
        Zinc_error_list_set(ld->errors, &loc, "invalid character: %s", c);
    }
}

void Worm_lex_id(Worm_lex_data* ld, Worm_token* t)
{
    while (true) {
        char c[5];
        int num;
        bool done;
        Zinc_location loc;
        Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            t->type = Worm_token_type_eof;
            t->loc = loc;
            return;
        }

        if (t->value.buf[0] == '_' && t->value.size == 1 &&  c[0] == '_') {
            c[num] = '\0';
            Zinc_error_list_set(ld->errors, &loc, "first two characters are underscore");
            continue;
        }

        if (isalpha(c[0]) || isdigit(c[0]) || c[0] == '_') {
            Zinc_string_add(&t->value, c, num);
            continue;
        }

        break;
    }
}

void Worm_lex_string(Worm_lex_data* ld, Worm_token* t)
{
    char c[5];
    int num;
    Zinc_location loc;
    bool done;

    while (true) {
        enum Zinc_result r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            return;
        }

        if (num == 1 && c[0] == '"') {
            return;
        }

        if (num == 1 && c[0] == '\\') {
            Worm_lex_string_escape(ld, t);
            continue;
        }

        UChar32 cp;
        Json_convert_char(c, num, &cp);
        if (cp < 0x20) {
            Zinc_error_list_set(ld->errors, &loc, "code point is less than \\u0020");
        } else if (cp > 0x10FFFF) {
            Zinc_error_list_set(ld->errors, &loc, "code point greater than \\u10FFFF");
        } else {
            Zinc_string_add(&t->value, c, num);
        }
    }

}

void Worm_lex_string_escape(Worm_lex_data* ld, Worm_token* t)
{
    Zinc_result r;
    char c[4];
    int num;
    Zinc_location loc;
    bool done;
    r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
    if (r == Zinc_result_error) {
        fprintf(stderr, "%s\n", Zinc_error_message);
        exit(1);
    }

    if (done) {
        Zinc_error_list_set(ld->errors, &loc, "missing escape character");
        Zinc_input_unicode_repeat(ld->input, ld->vtable);
        return;
    }

    if (num != 1) {
        struct Zinc_string bf;
        Zinc_string_init(&bf);
        Zinc_string_add(&bf, c, num);
        Zinc_error_list_set(ld->errors, &loc, "invalid escape character: %b", &bf);
        Zinc_string_destroy(&bf);
        return;
    }

    if (c[0] == '\\') {
        Zinc_string_add(&t->value, c, num);
        return;
    }

    if (c[0] == '/') {
        Zinc_string_add_char(&t->value, '/');
        return;
    }

    if (c[0] == 'b') {
        Zinc_string_add_char(&t->value, '\b');
        return;
    }

    if (c[0] == 'f') {
        Zinc_string_add_char(&t->value, '\f');
        return;
    }

    if (c[0] == 'n') {
        Zinc_string_add_char(&t->value, '\n');
        return;
    }

    if (c[0] == 'r') {
        Zinc_string_add_char(&t->value, '\r');
        return;
    }

    if (c[0] == 't') {
        Zinc_string_add_char(&t->value, '\t');
        return;
    }

    if (c[0] == 'u') {
        Worm_lex_string_escape_unicode(ld, t);
        return;
    }

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add(&bf, c, num);
    Zinc_error_list_set(ld->errors, &loc, "invalid escape character: %b", &bf);
    Zinc_string_destroy(&bf);
}

void Worm_lex_string_escape_unicode(Worm_lex_data* ld, Worm_token* t)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "\\u");
    bool valid = true;
    struct Zinc_location first_loc;
    first_loc = Zinc_input_unicode_get_location(ld->input, ld->vtable);
    first_loc.start_pos -= 2;
    first_loc.col -= 2;

    /* first four hex digits */
    for (int i = 0; i < 4; i++) {
        r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);

        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }
        if (done) {
            Zinc_error_list_set(ld->errors, &loc, "unicode escape not finished");
            Zinc_string_destroy(&bf);
            return;
        }
        if (Zinc_is_hex_digit(c, num)) {
            Zinc_string_add(&bf, c, num);
        } else {
            Zinc_error_list_set(ld->errors, &loc, "invalid hex digit: %c", c[0]);
            valid = false;
        }
    }
    /* optional two hex digits */
    for (int i = 0; i < 2; i++) {
        r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }
        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }
        if (!Zinc_is_hex_digit(c, num)) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }
        Zinc_string_add(&bf, c, num);
    }
    Zinc_string_finish(&bf);

    struct Zinc_location end_loc;
    end_loc = Zinc_input_unicode_get_location(ld->input, ld->vtable);
    first_loc.end_pos = end_loc.start_pos;

    if (valid) {
        unsigned int cp = Zinc_char4_to_hex(bf.buf + 2, (int)bf.size - 2);
        if (cp < 0x20) {
            Zinc_error_list_set(ld->errors, &first_loc, "code point is less than \\u0020: %s", bf.buf);
        } else if (cp > 0x10FFFF) {
            Zinc_error_list_set(ld->errors, &first_loc, "code point greater than \\u10FFFF: %s", bf.buf);
        } else {
            char dest[4];
            num = Zinc_code_to_utf8(dest, cp);
            Zinc_string_add(&t->value, dest, num);
        }
    }

    Zinc_string_destroy(&bf);
}

bool Worm_is_number(char c[4], int num)
{
    if (num != 1) return false;
    if (c[0] == '-') return true;
    if (c[0] == '+') return true;   /* error case if starting with + */
    if (c[0] == '.') return true;   /* error case if starting with . */
    if (isdigit(c[0])) return true;
    return false;
}

void Worm_lex_number(Worm_lex_data* ld, Worm_token* t)
{
    t->number_type = Worm_number_type_int;

    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;

    size_t digit_count = 0;
    char first_digit;
    struct Zinc_location first_loc = Zinc_input_unicode_get_location(ld->input, ld->vtable);
    first_loc.end_pos = first_loc.start_pos;
    first_loc.start_pos--;
    first_loc.col--;

    char first = t->value.buf[0];
    num = ZINC_NUM_BYTES(first);

    if (num == 1 && first == '.') {
        Zinc_error_list_set(ld->errors, &first_loc, "number starts with period");
        Worm_lex_number_fraction(ld, t);
        return;
    }

    if (num == 1 && first == '+') {
        Zinc_error_list_set(ld->errors, &first_loc, "number starts with plus sign");
    }

    if (isdigit(first)) {
        digit_count++;
        first_digit = first;
    }

    while (true) {
        r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        if (num == 1 && isdigit(c[0])) {
            if (digit_count == 1 && first_digit == '0') {
                t->value.size--;
                first_digit = c[0];
                digit_count--;
                Zinc_error_list_set(ld->errors, &first_loc, "leading zero with no other digits or faction");
            }
            Zinc_string_add_char(&t->value, c[0]);
            digit_count++;
            if (digit_count == 1) {
                first_digit = c[0];
            }
            continue;
        }

        if (num == 1 && c[0] == '.') {
            Zinc_string_add_char(&t->value, '.');
            Worm_lex_number_fraction(ld, t);
            break;;
        }

        if (num == 1 && (c[0] == 'E' || c[0] == 'e')) {
            Zinc_string_add_char(&t->value, c[0]);
            Worm_lex_number_exponent(ld, t);
            break;
        }

        Zinc_input_unicode_repeat(ld->input, ld->vtable);
        break;
    }

    if (t->value.size == 1 && first == '-') {
        Zinc_error_list_set(ld->errors, &first_loc, "invalid number");
        t->value.buf[0] = '0';
    }

    if (t->value.size == 1 && first == '+') {
        Zinc_error_list_set(ld->errors, &first_loc, "invalid number");
        t->value.buf[0] = '0';
    }
}

void Worm_lex_number_fraction(Worm_lex_data* ld, Worm_token* t)
{
    t->number_type = Worm_number_type_real;

    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;
    size_t digit_count = 0;

    while (true) {
        r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        if (num == 1 && isdigit(c[0])) {
            Zinc_string_add_char(&t->value, c[0]);
            digit_count++;
            continue;
        }

        if (num == 1 && (c[0] == 'E' || c[0] == 'e')) {
            Zinc_string_add_char(&t->value, c[0]);
            Worm_lex_number_exponent(ld, t);
            break;
        }

        Zinc_input_unicode_repeat(ld->input, ld->vtable);
        break;
    }

    if (digit_count == 0) {
        Zinc_error_list_set(ld->errors, &t->loc, "no digits in fraction");
    }
}

void Worm_lex_number_exponent(Worm_lex_data* ld, Worm_token* t)
{
    t->number_type = Worm_number_type_real;

    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;
    size_t digit_count = 0;
    bool has_sign = false;

    while (true) {
        r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        if (num == 1 && (c[0] == '+' || c[0] == '-')) {
            if (digit_count == 0) {
                if (!has_sign) {
                    Zinc_string_add_char(&t->value, c[0]);
                    has_sign = true;
                    continue;
                } else {
                    Zinc_error_list_set(ld->errors, &loc, "exponent already has a sign");
                    continue;
                }
            } else {
                Zinc_error_list_set(ld->errors, &loc, "sign after exponent digits");
                continue;
            }
        }

        if (num == 1 && isdigit(c[0])) {
            Zinc_string_add_char(&t->value, c[0]);
            digit_count++;
            continue;
        }

        Zinc_input_unicode_repeat(ld->input, ld->vtable);
        break;
    }

    if (digit_count == 0) {
        Zinc_error_list_set(ld->errors, &t->loc, "no digits in exponent");
    }
}

void Worm_lex_modifier(Worm_lex_data* ld, Worm_token* t)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;

    while (true) {
        r = Zinc_input_unicode_next(ld->input, ld->vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->vtable);
            break;
        }

        if (num == 1 && c[0] == '`') {
            t->loc.end_pos = loc.end_pos;
            return;
        }

        Zinc_string_add(&t->value, c, num);
    }
}