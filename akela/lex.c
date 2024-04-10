#include "lex_tools.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "zinc/result.h"
#include "token.h"
#include "lex.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "input.h"
#include "source.h"
#include "uconv.h"
#include "zinc/utf8.h"
#include <ctype.h>

bool lex_start(struct scan_state* sns,
               enum state_enum* state,
               bool* done,
               struct token* t);
bool lex_word(struct scan_state* sns,
              enum state_enum* state,
              bool* done,
              struct token* t);
bool lex_number(struct scan_state* sns,
                enum state_enum* state,
                bool* done,
                struct token* t);
bool lex_string(struct scan_state* sns,
                enum state_enum* state,
                bool* done,
                struct token* t);
bool lex_compound_operator(struct scan_state* sns,
                        enum state_enum* state,
                        bool* done,
                        struct token* t);

/**
 * scan in start state
 * @param sns scanner data
 * @param state current state
 * @param got_token if token is made
 * @param t the token
 * @return true if valid, otherwise false
 */
bool lex_start(struct scan_state* sns,
               enum state_enum* state,
               bool* done,
               struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct location loc;

    while (true) {
        *done = get_uc_char(sns, c, &num, &loc);
        if (*done) {
            return result_ok;
        }

        if (is_word(c)) {
            *state = state_id;
            t->type = token_id;
            for (int i = 0; i < num; i++) {
                buffer_add_char(&t->value, c[i]);
            }
            t->loc = loc;
            return lex_word(sns, state, done, t);
        } else if (*c == '_') {
            *state = state_id_underscore;
            t->type = token_id;
            for (int i = 0; i < num; i++) {
                buffer_add_char(&t->value, c[i]);
            }
            t->loc = loc;
            return lex_word(sns, state, done, t);
        } else if (is_num(c)) {
            *state = state_number_whole;
            t->type = token_number;
            t->is_integer = true;
            for (int i = 0; i < num; i++) {
                buffer_add_char(&t->value, c[i]);
            }
            t->loc = loc;
            return lex_number(sns, state, done, t);
        } else if (*c == '"') {
            *state = state_string;
            t->type = token_string;
            t->loc = loc;
            return lex_string(sns, state, done, t);
        } else if (compound_operator_start(num, c)) {
            *state = state_compound_operator;
            for (int i = 0; i < num; i++) {
                buffer_add_char(&t->value, c[i]);
            }
            t->loc = loc;
            return lex_compound_operator(sns, state, done, t);
        } else if (*c == '+') {
            t->type = token_plus;
            t->loc = loc;
            break;
        } else if (*c == '-') {
            t->type = token_minus;
            t->loc = loc;
            break;
        } else if (*c == '*') {
            t->type = token_mult;
            t->loc = loc;
            break;
        } else if (*c == '/') {
            t->type = token_divide;
            t->loc = loc;
            break;
        } else if (*c == '^') {
            t->type = token_caret;
            t->loc = loc;
            break;
        } else if (*c == '(') {
            t->type = token_left_paren;
            t->loc = loc;
            break;
        } else if (*c == ')') {
            t->type = token_right_paren;
            t->loc = loc;
            break;
        } else if (*c == ' ') {
            /* nothing */
        } else if (*c == '\t') {
            /* nothing */
        } else if (*c == '\n') {
            t->type = token_newline;
            t->loc = loc;
            break;
        } else if (*c == ',') {
            t->type = token_comma;
            t->loc = loc;
            break;
        } else if (*c == ';') {
            t->type = token_semicolon;
            t->loc = loc;
            break;
        } else if (*c == '[') {
            t->type = token_left_square_bracket;
            t->loc = loc;
            break;
        } else if (*c == ']') {
            t->type = token_right_square_bracket;
            t->loc = loc;
            break;
        } else if (*c == '{') {
            t->type = token_left_curly_brace;
            t->loc = loc;
            break;
        } else if (*c == '}') {
            t->type = token_right_curly_brace;
            t->loc = loc;
            break;
        } else if (*c == '.') {
            t->type = token_dot;
            t->loc = loc;
            break;
        } else {
            char a[5];
            int i = 0;
            while (i < num) {
                a[i] = c[i];
                i++;
            }
            a[i] = '\0';

            set_source_error(sns->el, &loc, "Unrecognized character: %s", a);
            /* error test case: test_scan_error_unrecognized_character */
            valid = false;
            break;
        }
    }
    return valid;
}

/**
 * scan word
 * @param sns scanner data
 * @param state scanner state
 * @param got_token if got token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool lex_word(struct scan_state* sns,
              enum state_enum* state,
              bool* done,
              struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct location loc;

    while (true) {
        *done = get_uc_char(sns, c, &num, &loc);

        if (*done) {
            struct symbol* sym = environment_get(sns->st->top, &t->value);
            assert(!sym || sym->tk_type != token_none);
            if (sym) {
                t->type = sym->tk_type;
            }
            *state = state_start;
            t->loc.size = t->value.size;
            break;
        }

        if (*state == state_id) {
            if (*c == '_') {
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else if (is_word(c)) {
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else if (is_num(c)) {
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else {
                struct symbol* sym = environment_get(sns->st->top, &t->value);
                assert(!sym || sym->tk_type != token_none);
                if (sym) {
                    t->type = sym->tk_type;
                }
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        } else if (*state == state_id_underscore) {
            if (*c == '_') {
                set_source_error(sns->el, &loc, "Must have a letter following underscore at start of id");
                valid = false;
                /* test case: test_scan_error_underscore_letter2 */
            } else if (is_num(c)) {
                set_source_error(sns->el, &loc, "Must have a letter following underscore at start of id");
                valid = false;
                /* test case: test_scan_error_underscore_letter */
            } else if (is_word(c)) {
                *state = state_id;
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else {
                struct symbol* sym = environment_get(sns->st->top, &t->value);
                assert(!sym || sym->tk_type != token_none);
                if (sym) {
                    t->type = sym->tk_type;
                }
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        }
    }

    return valid;
}

/**
 * scan a number
 * @param sns scanner data
 * @param state current state
 * @param got_token if produced a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool lex_number(struct scan_state* sns,
                enum state_enum* state,
                bool* done,
                struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct location loc;

    while (true) {
        *done = get_uc_char(sns, c, &num, &loc);

        if (*state == state_number_whole) {
            if (!*done && is_num(c)) {
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else if (!*done && *c == '.') {
                *state = state_number_fraction_start;
                t->is_integer = false;
                t->is_float = true;
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else if (!*done && *c == 'e') {
                /* lookahead another character */
                char peek_c[4];
                int peek_num;
                bool peek_done;
                struct location peek_loc;
                peek_done = get_uc_char(sns, peek_c, &peek_num, &peek_loc);
                if (!peek_done && (is_num(peek_c) || *peek_c == '-' || *peek_c == '+')) {
                    /* e is part of exponent */
                    *state = state_number_exponent_start;
                    t->is_integer = false;
                    t->is_float = true;
                    buffer_add_char(&t->value, 'e');
                    InputCharSeek(sns->input_obj, sns->input_vtable, &peek_loc);
                } else {
                    /* number is done and e will be part of an id */
                    InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                    *state = state_start;
                    break;
                }
            } else {
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        } else if (*state == state_number_fraction_start) {
            if (!*done && is_num(c)) {
                *state = state_number_fraction;
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else {
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        } else if (*state == state_number_fraction) {
            if (!*done && is_num(c)) {
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else if (!*done && *c == 'e') {
                *state = state_number_exponent_start;
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else {
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        } else if (*state == state_number_exponent_start) {
            if (!*done && is_num(c)) {
                *state = state_number_exponent;
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else if (!*done && (*c == '-' || *c == '+')) {
                *state = state_number_exponent_sign_start;
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else {
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        } else if (*state == state_number_exponent_sign_start) {
            if (!*done && is_num(c)) {
                *state = state_number_exponent;
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else {
                valid = set_source_error(sns->el, &loc, "expected number after exponent sign");
                /* test case: test_scan_error_exponent_sign */
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        } else if (*state == state_number_exponent) {
            if (!*done && is_num(c)) {
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
            } else {
                *state = state_start;
                t->loc.size = t->value.size;
                InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
                break;
            }
        }

    }

    return valid;
}

/**
 * scan a string
 * @param sns scanner data
 * @param state current state
 * @param got_token if made a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool lex_string(
        struct scan_state* sns,
        enum state_enum* state,
        bool* done,
        struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct location loc;

    while (true) {
        *done = get_uc_char(sns, c, &num, &loc);
        if (*done) {
            for (int i = 0; i < num; i++) {
                buffer_add_char(&t->value, c[i]);
            }
            t->loc.size += num;
            valid = set_source_error(sns->el, &loc, "Unclosed string");
            break;
        }

        if (*state == state_string) {
            if (*c == '\\') {
                *state = state_string_backslash;
                t->loc.size += num;
            } else if (*c == '"') {
                *state = state_start;
                t->loc = loc;
                t->loc.size += num;
                break;
            } else {
                for (int i = 0; i < num; i++) {
                    buffer_add_char(&t->value, c[i]);
                }
                t->loc.size += num;
            }
        } else if (*state == state_string_backslash) {
            if (*c == '\\') {
                buffer_add_char(&t->value, '\\');
                t->loc.size += num;
            } else if (*c == 'n') {
                buffer_add_char(&t->value, '\n');
                t->loc.size += num;
            } else if (*c == 'r') {
                buffer_add_char(&t->value, '\r');
                t->loc.size += num;
            } else {
                char a[5];
                int i = 0;
                while (i < num) {
                    a[i] = c[i];
                    i++;
                }
                a[i] = '\0';
                valid = set_source_error(sns->el, &loc, "Unrecognized escape sequence: %s", a);
                /* test case: test_scan_string_escape_error */
                break;
            }
            *state = state_string;
        }
    }

    return valid;
}

/**
 * scan a compound operator
 * @param sns scanner state
 * @param state current state
 * @param got_token if got a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool lex_compound_operator(
        struct scan_state* sns,
        enum state_enum* state,
        bool* done,
        struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct location loc;
    *done = get_uc_char(sns, c, &num, &loc);

    if (!*done && num > 0) {
        for (int i = 0; i < num; i++) {
            buffer_add_char(&t->value, c[i]);
        }
    }

    if (buffer_compare_str(&t->value, "==")) {
        t->type = token_double_equal;
        *state = state_start;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "!=")) {
        t->type = token_not_equal;
        *state = state_start;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "<=")) {
        t->type = token_less_than_or_equal;
        *state = state_start;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, ">=")) {
        t->type = token_greater_than_or_equal;
        *state = state_start;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "&&")) {
        t->type = token_and;
        *state = state_start;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "||")) {
        t->type = token_or;
        *state = state_start;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "::")) {
        t->type = token_double_colon;
        *state = state_start;
        t->loc.size += num;
    } else if (t->value.buf[0] == '=') {
        t->type = token_equal;
        buffer_clear(&t->value);
        *state = state_start;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '!') {
        t->type = token_not;
        buffer_clear(&t->value);
        *state = state_start;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '<') {
        t->type = token_less_than;
        buffer_clear(&t->value);
        *state = state_start;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '>') {
        t->type = token_greater_than;
        buffer_clear(&t->value);
        *state = state_start;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '&') {
        t->type = token_ampersand;
        buffer_clear(&t->value);
        *state = state_start;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '|') {
        t->type = token_vertical_bar;
        buffer_clear(&t->value);
        *state = state_start;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == ':') {
        t->type = token_colon;
        buffer_clear(&t->value);
        *state = state_start;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else {
        /* unrecognized compound operator */
        assert(false);
    }

    return valid;
}

/**
 * scan and produce a token
 * @param sns scanner data
 * @param t the token
 * @return true if valid, otherwise false
 */
bool lex(struct scan_state* sns, struct token** t)
{
    bool valid = true;
    enum state_enum state = state_start;
    *t = NULL;
    struct token* tf;
    bool done = false;

    malloc_safe((void**)&tf, sizeof(struct token));
    token_init(tf);

    valid = lex_start(sns, &state, &done, tf);
    if (!valid) {
        token_destroy(tf);
        free(tf);
        return valid;
    }

    if (tf->type == token_none) {
        assert(valid);
        tf->type = token_eof;
        tf->loc = InputCharGetLocation(sns->input_obj, sns->input_vtable);
        tf->loc.size = 3;
    }

    *t = tf;
    return valid;
}
