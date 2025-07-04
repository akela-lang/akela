#include "lex_data.h"
#include "token.h"
#include "zinc/unicode.h"
#include "zinc/utf8.h"
#include <assert.h>
#include "zinc/input_unicode.h"
#include "zinc/error.h"

void lex_start(Cent_lex_data* ld, Cent_token* t);
void lex_id(Cent_lex_data* ld, Cent_token* t);
void Cent_lex_string(Cent_lex_data* ld, Cent_token* t);
void Cent_lex_string_escape(Cent_lex_data* ld, Cent_token* t);
void Cent_lex_string_escape_unicode(Cent_lex_data* ld, Cent_token* t);
void Cent_lex_number_fraction(Cent_lex_data* ld, Cent_token* t);
void Cent_lex_number_exponent(Cent_lex_data* ld, Cent_token* t);
bool Cent_is_number(char c[4], int num);
void Cent_lex_integer(Cent_lex_data* ld, Cent_token* t);
void Cent_lex_modifier(Cent_lex_data* ld, Cent_token* t);

Cent_token* lex(Cent_lex_data* ld)
{
    Cent_token* t = NULL;
    Cent_token_create(&t);

    lex_start(ld, t);
    if (t->type == Cent_token_id) {
        if (t->value.size >= 1 && t->value.buf[0] == '@') {
            Cent_builtin_type* bt = Cent_lex_get_builtin(ld, &t->value);
            if (bt) {
                t->builtin_type = *bt;
            } else {
                Zinc_error_list_set(ld->errors, &t->loc, "invalid builtin id: %bf", &t->value);
            }
        } else {
            Cent_token_type* p = Cent_lex_get_reserved_word(ld, &t->value);
            if (p) {
                t->type = *p;
            }
        }
    }

    if (t->type == Cent_token_integer) {
        Zinc_string_finish(&t->value);
        t->data.integer = strtoll(t->value.buf, NULL, 10);
    } else if (t->type == Cent_token_natural) {
        Zinc_string_finish(&t->value);
        t->data.natural = strtoull(t->value.buf, NULL, 10);
    } else if (t->type == Cent_token_real) {
        Zinc_string_finish(&t->value);
        t->data.real = strtod(t->value.buf, NULL);
    }

    return t;
}

void lex_start(Cent_lex_data* ld, Cent_token* t)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;

    while (true) {
        enum Zinc_result r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);

        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            t->type = Cent_token_eof;
            t->loc = loc;
            return;
        }

        if (num == 1) {
            if (c[0] == ' ') {
                continue;
            }

            if (c[0] == '\n') {
                t->type = Cent_token_newline;
                t->loc = loc;
                return;
            }

            if (c[0] == ':') {
                t->type = Cent_token_colon;
                t->loc = loc;

                r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
                if (r == Zinc_result_error) {
                    fprintf(stderr, "%s\n", Zinc_error_message);
                    exit(1);
                }

                if (done) {
                    Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
                    return;
                }

                if (c[0] == ':') {
                    t->type = Cent_token_double_colon;
                    t->loc.end = loc.end;
                }

                return;
            }

            if (c[0] == ';') {
                t->type = Cent_token_semicolon;
                t->loc = loc;
                return;
            }

            if (c[0] == '=') {
                t->type = Cent_token_equal;
                t->loc = loc;
                return;
            }

            if (c[0] == '{') {
                t->type = Cent_token_left_curly_brace;
                t->loc = loc;
                return;
            }

            if (c[0] == '}') {
                t->type = Cent_token_right_curly_brace;
                t->loc = loc;
                return;
            }

            if (c[0] == '(') {
                t->type = Cent_token_left_paren;
                t->loc = loc;
                return;
            }

            if (c[0] == ')') {
                t->type = Cent_token_right_paren;
                t->loc = loc;
                return;
            }

            if (c[0] == ',') {
                t->type = Cent_token_comma;
                t->loc = loc;
                return;
            }

            if (c[0] == '.') {
                t->type = Cent_token_dot;
                t->loc = loc;
                return;
            }

            if (c[0] == '*') {
                t->type = Cent_token_asterisk;
                t->loc = loc;
                return;
            }

            if (c[0] == '#') {
                while (true) {
                    r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
                    if (r == Zinc_result_error) {
                        fprintf(stderr, "%s\n", Zinc_error_message);
                        exit(1);
                    }
                    if (done) {
                        t->type = Cent_token_eof;
                        t->loc = loc;
                        return;
                    }
                    if (num == 1 && c[0] == '\n') {
                        Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
                        break;
                    }
                }

                continue;
            }

            if (isalpha(c[0]) || c[0] == '_' || c[0] == '@') {
                t->type = Cent_token_id;
                t->loc = loc;
                Zinc_string_add(&t->value, c, num);
                lex_id(ld, t);
                return;
            }

            if (c[0] == '"') {
                t->type = Cent_token_string;
                t->loc = loc;
                Cent_lex_string(ld, t);
                return;
            }

            if (Cent_is_number(c, num)) {
                t->type = Cent_token_integer;
                t->loc = loc;
                Zinc_string_add(&t->value, c, num);
                Cent_lex_integer(ld, t);
                return;
            }

            if (c[0] == '`') {
                t->type = Cent_token_modifier;
                t->loc = loc;
                Cent_lex_modifier(ld, t);
                return;
            }
        }

        struct Zinc_string bf;
        Zinc_string_init(&bf);
        Zinc_string_add(&bf, c, num);
        loc.end = loc.start + num;
        Zinc_error_list_set(ld->errors, &loc, "invalid character: %bf", &bf);
        Zinc_string_destroy(&bf);
    }
}

void lex_id(Cent_lex_data* ld, Cent_token* t)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;

    while (true) {
        enum Zinc_result r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);

        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
            return;
        }

        if (num == 1) {
            if (isalpha(c[0]) || isdigit(c[0]) || c[0] == '_' || c[0] == '.') {
                Zinc_string_add(&t->value, c, num);
                continue;
            }
        }

        loc = Zinc_input_unicode_get_location(ld->input, ld->input_vtable);
        t->loc.end = loc.start;
        Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
        return;
    }
}

void Cent_lex_string(Cent_lex_data* ld, Cent_token* t)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;

    while (true) {
        enum Zinc_result r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
            return;
        }

        if (num == 1 && c[0] == '"') {
            return;
        }

        if (num == 1 && c[0] == '\\') {
            Cent_lex_string_escape(ld, t);
            continue;
        }

        uint32_t cp;
        int num2 = Zinc_utf8_to_utf32(c, &cp);
        if (!num2) {
            Zinc_error_list_set(ld->errors, &loc, "invalid utf-8 character");
        } else if (cp < 0x20) {
            Zinc_error_list_set(ld->errors, &loc, "code point is less than \\u0020");
        } else if (cp > 0x10FFFF) {
            Zinc_error_list_set(ld->errors, &loc, "code point greater than \\u10FFFF");
        } else {
            Zinc_string_add(&t->value, c, num);
        }
    }

}

void Cent_lex_string_escape(Cent_lex_data* ld, Cent_token* t)
{
    enum Zinc_result r;
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
    if (r == Zinc_result_error) {
        fprintf(stderr, "%s\n", Zinc_error_message);
        exit(1);
    }

    if (done) {
        Zinc_error_list_set(ld->errors, &loc, "missing escape character");
        Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
        return;
    }

    if (num != 1) {
        struct Zinc_string bf;
        Zinc_string_init(&bf);
        Zinc_string_add(&bf, c, num);
        Zinc_error_list_set(ld->errors, &loc, "invalid escape character: %bf", &bf);
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
        Cent_lex_string_escape_unicode(ld, t);
        return;
    }

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add(&bf, c, num);
    Zinc_error_list_set(ld->errors, &loc, "invalid escape character: %bf", &bf);
    Zinc_string_destroy(&bf);
}

void Cent_lex_string_escape_unicode(Cent_lex_data* ld, Cent_token* t)
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
    first_loc = Zinc_input_unicode_get_location(ld->input, ld->input_vtable);
    first_loc.start -= 2;
    first_loc.col -= 2;

    /* first four hex digits */
    for (int i = 0; i < 4; i++) {
        r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);

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
        r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }
        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
            break;
        }
        if (!Zinc_is_hex_digit(c, num)) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
            break;
        }
        Zinc_string_add(&bf, c, num);
    }
    Zinc_string_finish(&bf);

    struct Zinc_location end_loc;
    end_loc = Zinc_input_unicode_get_location(ld->input, ld->input_vtable);
    first_loc.end = end_loc.start;

    if (valid) {
        unsigned int cp = Zinc_char4_to_hex(bf.buf + 2, (int)bf.size - 2);
        if (cp < 0x20) {
            Zinc_error_list_set(ld->errors, &first_loc, "code point is less than \\u0020: %s", bf.buf);
        } else if (cp > 0x10FFFF) {
            Zinc_error_list_set(ld->errors, &first_loc, "code point greater than \\u10FFFF: %s", bf.buf);
        } else {
            char dest[4];
            num = Zinc_code_to_utf8((unsigned char*)dest, cp);
            Zinc_string_add(&t->value, dest, num);
        }
    }

    Zinc_string_destroy(&bf);
}

bool Cent_is_number(char c[4], int num)
{
    if (num != 1) return false;
    if (c[0] == '-') return true;
    if (c[0] == '+') return true;   /* error case if starting with + */
    if (c[0] == '.') return true;   /* error case if starting with . */
    if (isdigit(c[0])) return true;
    return false;
}

void Cent_lex_integer(Cent_lex_data* ld, Cent_token* t)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;

    size_t digit_count = 0;
    char first_digit;
    struct Zinc_location first_loc = Zinc_input_unicode_get_location(ld->input, ld->input_vtable);
    first_loc.end = first_loc.start;
    first_loc.start--;
    first_loc.col--;

    char first = t->value.buf[0];
    num = ZINC_NUM_BYTES(first);

    if (first == '-') {
        t->type = Cent_token_integer;
    } else {
        t->type = Cent_token_natural;
    }

    if (num == 1 && first == '.') {
        Zinc_error_list_set(ld->errors, &first_loc, "number starts with period");
        Cent_lex_number_fraction(ld, t);
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
        r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
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
            Cent_lex_number_fraction(ld, t);
            break;;
        }

        if (num == 1 && (c[0] == 'E' || c[0] == 'e')) {
            Zinc_string_add_char(&t->value, c[0]);
            Cent_lex_number_exponent(ld, t);
            break;
        }

        Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
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

void Cent_lex_number_fraction(Cent_lex_data* ld, Cent_token* t)
{
    t->type = Cent_token_real;

    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;
    size_t digit_count = 0;

    while (true) {
        r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
            break;
        }

        if (num == 1 && isdigit(c[0])) {
            Zinc_string_add_char(&t->value, c[0]);
            digit_count++;
            continue;
        }

        if (num == 1 && (c[0] == 'E' || c[0] == 'e')) {
            Zinc_string_add_char(&t->value, c[0]);
            Cent_lex_number_exponent(ld, t);
            break;
        }

        Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
        break;
    }

    if (digit_count == 0) {
        Zinc_error_list_set(ld->errors, &t->loc, "no digits in fraction");
    }
}

void Cent_lex_number_exponent(Cent_lex_data* ld, Cent_token* t)
{
    t->type = Cent_token_real;

    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;
    size_t digit_count = 0;
    bool has_sign = false;

    while (true) {
        r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
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

        Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
        break;
    }

    if (digit_count == 0) {
        Zinc_error_list_set(ld->errors, &t->loc, "no digits in exponent");
    }
}

void Cent_lex_modifier(Cent_lex_data* ld, Cent_token* t)
{
    char c[4];
    int num;
    struct Zinc_location loc;
    bool done;
    enum Zinc_result r;

    while (true) {
        r = Zinc_input_unicode_next(ld->input, ld->input_vtable, c, &num, &loc, &done);
        if (r == Zinc_result_error) {
            fprintf(stderr, "%s\n", Zinc_error_message);
            exit(1);
        }

        if (done) {
            Zinc_input_unicode_repeat(ld->input, ld->input_vtable);
            break;
        }

        if (num == 1 && c[0] == '`') {
            t->loc.end = loc.end;
            return;
        }

        Zinc_string_add(&t->value, c, num);
    }
}
