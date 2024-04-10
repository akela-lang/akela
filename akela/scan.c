#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "input.h"
#include "source.h"
#include "uconv.h"
#include "zinc/utf8.h"
#include <ctype.h>

/**
 * initialize scanner state
 * @param sns scanner data
 * @param lc lookahead buffer
 * @param el error list
 * @param st symbol table
 */
void scan_state_init(struct scan_state* sns, void* input_obj, InputCharVTable* input_vtable, struct error_list* el, struct symbol_table* st)
{
    sns->input_obj = input_obj;
    sns->input_vtable = input_vtable;
    sns->el = el;
    sns->st = st;
}

/**
 * check if compound operator
 * @param uc utf32 character
 * @return true if a compound operator, otherwise false
 */
bool compound_operator_start(int num, const char c[4])
{
    return *c == '=' || *c == '!' || *c == '<' || *c == '>' || *c == '&' || *c == '|' || *c == ':';
}

bool get_uc_char(struct scan_state* sns, char c[4], int* num, struct location* loc)
{
    bool done;
    done = InputCharNext(sns->input_obj, sns->input_vtable, c, loc);
    if (done) {
        num = 0;
        return done;
    }
    *num = NUM_BYTES(*c);
    for (int i = 1; i < *num; i++) {
        struct location loc2;
        done = InputCharNext(sns->input_obj, sns->input_vtable, c + i, &loc2);
        if (done) {
            break;
        }
        loc->size++;
    }
    return done;
}

bool is_word(const char c[4])
{
    return isalpha(*c);
}

bool is_num(const char c[4])
{
    return isdigit(*c);
}

/**
 * scan in start state
 * @param sns scanner data
 * @param state current state
 * @param got_token if token is made
 * @param t the token
 * @return true if valid, otherwise false
 */
bool process_char_start(struct scan_state* sns,
            enum state_enum* state,
            int* got_token,
            bool* done,
            struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    *got_token = 0;
    struct location loc;

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
    } else if (*c == '_') {
        *state = state_id_underscore;
        t->type = token_id;
        for (int i = 0; i < num; i++) {
            buffer_add_char(&t->value, c[i]);
        }
        t->loc = loc;
    } else if (is_num(c)) {
        *state = state_number_whole;
        t->type = token_number;
        t->is_integer = true;
        for (int i = 0; i < num; i++) {
            buffer_add_char(&t->value, c[i]);
        }
        t->loc = loc;
    } else if (*c == '"') {
        *state = state_string;
        t->type = token_string;
        t->loc = loc;
    } else if (compound_operator_start(num, c)) {
        *state = state_compound_operator;
        for (int i = 0; i < num; i++) {
            buffer_add_char(&t->value, c[i]);
        }
        t->loc = loc;
    } else if (*c == '+') {
        t->type = token_plus;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '-') {
        t->type = token_minus;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '*') {
        t->type = token_mult;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '/') {
        t->type = token_divide;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '^') {
        t->type = token_caret;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '(') {
        t->type = token_left_paren;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == ')') {
        t->type = token_right_paren;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == ' ') {
        /* nothing */
    } else if (*c == '\t') {
        /* nothing */
    } else if (*c == '\n') {
        t->type = token_newline;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == ',') {
        t->type = token_comma;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == ';') {
        t->type = token_semicolon;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '[') {
        t->type = token_left_square_bracket;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == ']') {
        t->type = token_right_square_bracket;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '{') {
        t->type = token_left_curly_brace;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '}') {
        t->type = token_right_curly_brace;
        t->loc = loc;
        *got_token = 1;
    } else if (*c == '.') {
        t->type = token_dot;
        t->loc = loc;
        *got_token = 1;
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
bool process_char_word(struct scan_state* sns,
        enum state_enum* state,
        int* got_token,
        bool* done,
        struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct location loc;
    *done = get_uc_char(sns, c, &num, &loc);

    if (*done) {
        struct symbol* sym = environment_get(sns->st->top, &t->value);
        assert(!sym || sym->tk_type != token_none);
        if (sym) {
            t->type = sym->tk_type;
        }
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        return valid;
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
            *got_token = 1;
            t->loc.size = t->value.size;
            InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
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
            *got_token = 1;
            t->loc.size = t->value.size;
            InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
        }
    }

    return valid;
}

/**
 * check if the state is a number state
 * @param state the state
 * @return true if number state, otherwise false
 */
bool is_number_state(enum state_enum state)
{
    return state == state_number_whole
        || state == state_number_fraction_start
        || state == state_number_fraction
        || state == state_number_exponent_start
        || state == state_number_exponent_sign_start
        || state == state_number_exponent;
}

/**
 * scan a number
 * @param sns scanner data
 * @param state current state
 * @param got_token if produced a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool process_char_number(struct scan_state* sns,
        enum state_enum* state,
        int* got_token,
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
                    *got_token = 1;
                    break;
                }
            } else {
                *state = state_start;
                *got_token = 1;
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
                *got_token = 1;
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
                *got_token = 1;
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
                *got_token = true;
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
                *got_token = true;
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
                *got_token = 1;
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
bool process_char_string(
        struct scan_state* sns,
        enum state_enum* state,
        int* got_token,
        bool* done,
        struct token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct location loc;
    *done = get_uc_char(sns, c, &num, &loc);
    if (*done) {
        for (int i = 0; i < num; i++) {
            buffer_add_char(&t->value, c[i]);
        }
        t->loc.size += num;
        *got_token = 1;
        valid = set_source_error(sns->el, &loc, "Unclosed string");
        return valid;
    }

    if (*state == state_string) {
        if (*c == '\\') {
            *state = state_string_backslash;
            t->loc.size += num;
        } else if (*c == '"') {
            *state = state_start;
            *got_token = 1;
            t->loc = loc;
            t->loc.size += num;
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
        }
        *state = state_string;
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
bool process_compound_operator(
        struct scan_state* sns,
        enum state_enum* state,
        int* got_token,
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
        *got_token = 1;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "!=")) {
        t->type = token_not_equal;
        *state = state_start;
        *got_token = 1;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "<=")) {
        t->type = token_less_than_or_equal;
        *state = state_start;
        *got_token = 1;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, ">=")) {
        t->type = token_greater_than_or_equal;
        *state = state_start;
        *got_token = 1;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "&&")) {
        t->type = token_and;
        *state = state_start;
        *got_token = 1;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "||")) {
        t->type = token_or;
        *state = state_start;
        *got_token = 1;
        t->loc.size += num;
    } else if (buffer_compare_str(&t->value, "::")) {
        t->type = token_double_colon;
        *state = state_start;
        *got_token = 1;
        t->loc.size += num;
    } else if (t->value.buf[0] == '=') {
        t->type = token_equal;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '!') {
        t->type = token_not;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '<') {
        t->type = token_less_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '>') {
        t->type = token_greater_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '&') {
        t->type = token_ampersand;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == '|') {
        t->type = token_vertical_bar;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else if (t->value.buf[0] == ':') {
        t->type = token_colon;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = 1;
        InputCharSeek(sns->input_obj, sns->input_vtable, &loc);
    } else {
        /* unrecognized compound operator */
        assert(false);
    }

    return valid;
}

/**
 * scan at starting state
 * @param sns scanner data
 * @param state current state
 * @param got_token if got a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool scan_process(struct scan_state* sns,
        enum state_enum* state,
        int* got_token,
        bool* done,
        struct token* t)
{
    bool valid = true;

    if (*state == state_start) {
        valid = process_char_start(sns, state, got_token, done, t);
    } else if (*state == state_id || *state == state_id_underscore) {
        valid = process_char_word(sns, state, got_token, done, t);
    } else if (is_number_state(*state)) {
        valid = process_char_number(sns, state, got_token, done, t);
    } else if (*state == state_string || *state == state_string_backslash) {
        valid = process_char_string(sns, state, got_token, done, t);
    } else if (*state == state_compound_operator) {
        valid = process_compound_operator(sns, state, got_token, done, t);
    } else {
        /* unexpected state */
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
bool scan_get_token(struct scan_state* sns, struct token** t)
{
    bool valid = true;
    enum state_enum state = state_start;
    int got_token = 0;
    *t = NULL;
    struct token* tf;
    bool done = false;

    malloc_safe((void**)&tf, sizeof(struct token));
    token_init(tf);

    while (!got_token && !done) {
        valid = scan_process(sns, &state, &got_token, &done, tf);
        if (!valid) {
            token_destroy(tf);
            free(tf);
            return valid;
        }
    }

    if (state != state_start) {
        valid = scan_process(sns, &state, &got_token, &done, tf);
        if (!valid) {
            token_destroy(tf);
            free(tf);
            return valid;
        }
    }

    if (!got_token && tf->type == token_none) {
        assert(valid);
        got_token = true;
        tf->type = token_eof;
        tf->loc = InputCharGetLocation(sns->input_obj, sns->input_vtable);
        tf->loc.size = 3;
    }

    assert(got_token);
    *t = tf;
    return valid;
}
