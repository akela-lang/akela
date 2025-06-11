#include "lex_tools.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "token.h"
#include "lex.h"
#include "zinc/memory.h"
#include "symbol.h"

bool Ake_lex_start(struct Ake_lex_state* ls,
               enum state_enum* state,
               bool* done,
               struct Ake_token* t);
bool Ake_lex_word(struct Ake_lex_state* ls,
              enum state_enum* state,
              bool* done,
              struct Ake_token* t);
bool Ake_lex_number(struct Ake_lex_state* ls,
                enum state_enum* state,
                bool* done,
                struct Ake_token* t);
bool Ake_lex_string(struct Ake_lex_state* ls,
                enum state_enum* state,
                bool* done,
                struct Ake_token* t);
bool Ake_lex_compound_operator(struct Ake_lex_state* ls,
                        enum state_enum* state,
                        bool* done,
                        struct Ake_token* t);

/**
 * lex in start state
 * @param ls lexer data
 * @param state current state
 * @param got_token if token is made
 * @param t the token
 * @return true if valid, otherwise false
 */
bool Ake_lex_start(struct Ake_lex_state* ls,
               enum state_enum* state,
               bool* done,
               struct Ake_token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct Zinc_location loc;
    enum Zinc_result r;

    while (true) {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
            break;
        }

        if (*done) {
            return Zinc_result_ok;
        }

        if (num == 1 && *c == '#') {
            /* comment */
            while (!(num == 1 && *c == '\n')) {
                r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
                if (r == Zinc_result_error) {
                    valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
                    break;
                }
                if (*done) {
                    break;
                }
            }
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        } else if (Ake_is_word(c, num)) {
            *state = Ake_state_id;
            t->type = Ake_token_id;
            for (int i = 0; i < num; i++) {
                Zinc_string_add_char(&t->value, c[i]);
            }
            t->loc = loc;
            return Ake_lex_word(ls, state, done, t);
        } else if (*c == '_') {
            *state = Ake_state_id_underscore;
            t->type = Ake_token_id;
            for (int i = 0; i < num; i++) {
                Zinc_string_add_char(&t->value, c[i]);
            }
            t->loc = loc;
            return Ake_lex_word(ls, state, done, t);
        } else if (Ake_is_num(c, num)) {
            *state = Ake_state_number_whole;
            t->type = Ake_token_number;
            t->is_integer = true;
            for (int i = 0; i < num; i++) {
                Zinc_string_add_char(&t->value, c[i]);
            }
            t->loc = loc;
            return Ake_lex_number(ls, state, done, t);
        } else if (*c == '"') {
            *state = Ake_state_string;
            t->type = Ake_token_string;
            t->loc = loc;
            return Ake_lex_string(ls, state, done, t);
        } else if (Ake_compound_operator_start(num, c)) {
            *state = Ake_state_compound_operator;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
            return Ake_lex_compound_operator(ls, state, done, t);
        } else if (*c == '+') {
            t->type = Ake_token_plus;
            t->loc = loc;
            break;
        } else if (*c == '-') {
            t->type = Ake_token_minus;
            t->loc = loc;
            break;
        } else if (*c == '*') {
            t->type = Ake_token_mult;
            t->loc = loc;
            break;
        } else if (*c == '/') {
            t->type = Ake_token_divide;
            t->loc = loc;
            break;
        } else if (*c == '^') {
            t->type = Ake_token_caret;
            t->loc = loc;
            break;
        } else if (*c == '(') {
            t->type = Ake_token_left_paren;
            t->loc = loc;
            break;
        } else if (*c == ')') {
            t->type = Ake_token_right_paren;
            t->loc = loc;
            break;
        } else if (*c == ' ') {
            /* nothing */
        } else if (*c == '\t') {
            /* nothing */
        } else if (*c == '\n') {
            t->type = Ake_token_newline;
            t->loc = loc;
            break;
        } else if (*c == ',') {
            t->type = Ake_token_comma;
            t->loc = loc;
            break;
        } else if (*c == ';') {
            t->type = Ake_token_semicolon;
            t->loc = loc;
            break;
        } else if (*c == '[') {
            t->type = Ake_token_left_square_bracket;
            t->loc = loc;
            break;
        } else if (*c == ']') {
            t->type = Ake_token_right_square_bracket;
            t->loc = loc;
            break;
        } else if (*c == '{') {
            t->type = Ake_token_left_curly_brace;
            t->loc = loc;
            break;
        } else if (*c == '}') {
            t->type = Ake_token_right_curly_brace;
            t->loc = loc;
            break;
        } else if (*c == '.') {
            t->type = Ake_token_dot;
            t->loc = loc;
            break;
        } else if (*c == ':') {
            t->type = Ake_token_colon;
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

            loc.end_pos = loc.start_pos + num;
            Zinc_error_list_set(ls->el, &loc, "Unrecognized character: %s", a);
            /* error test case: test_lex_error_unrecognized_character */
            valid = false;
            break;
        }
    }
    return valid;
}

/**
 * lex word
 * @param ls lexer data
 * @param state lexer state
 * @param got_token if got token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool Ake_lex_word(struct Ake_lex_state* ls,
              enum state_enum* state,
              bool* done,
              struct Ake_token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct Zinc_location loc;
    enum Zinc_result r;

    while (true) {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
            break;
        }

        if (*done) {
            struct Ake_symbol* sym = Ake_EnvironmentGet(ls->st, &t->value);
            if (sym && sym->type == Ake_symbol_type_reserved_word) {
                t->type = sym->tk_type;
            } else {
                t->type = Ake_token_id;
            }
            *state = Ake_state_start;
            break;
        }

        if (*state == Ake_state_id) {
            if (*c == '_') {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else if (Ake_is_word(c, num)) {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else if (Ake_is_num(c, num)) {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else {
                struct Ake_symbol* sym = Ake_EnvironmentGet(ls->st, &t->value);
                if (sym && sym->type == Ake_symbol_type_reserved_word) {
                    t->type = sym->tk_type;
                } else {
                    t->type = Ake_token_id;
                }
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        } else if (*state == Ake_state_id_underscore) {
            if (*c == '_') {
                Zinc_error_list_set(ls->el, &loc, "Must have a letter following underscore at start of id");
                valid = false;
                /* test case: test_lex_error_underscore_letter2 */
            } else if (Ake_is_num(c, num)) {
                Zinc_error_list_set(ls->el, &loc, "Must have a letter following underscore at start of id");
                valid = false;
                /* test case: test_lex_error_underscore_letter */
            } else if (Ake_is_word(c, num)) {
                *state = Ake_state_id;
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else {
                struct Ake_symbol* sym = Ake_EnvironmentGet(ls->st, &t->value);
                if (sym && sym->type == Ake_symbol_type_reserved_word) {
                    t->type = sym->tk_type;
                } else {
                    t->type = Ake_token_id;
                }
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        }
    }

    return valid;
}

/**
 * lex a number
 * @param ls lexer data
 * @param state current state
 * @param got_token if produced a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool Ake_lex_number(struct Ake_lex_state* ls,
                enum state_enum* state,
                bool* done,
                struct Ake_token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct Zinc_location loc;
    enum Zinc_result r;

    while (true) {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
            break;
        }

        if (*state == Ake_state_number_whole) {
            if (!*done && Ake_is_num(c, num)) {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else if (!*done && *c == '.') {
                *state = Ake_state_number_fraction_start;
                t->is_integer = false;
                t->is_float = true;
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else if (!*done && *c == 'e') {
                *state = Ake_state_number_exponent_start;
                t->is_integer = false;
                t->is_float = true;
                Zinc_string_add_char(&t->value, 'e');
            } else {
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        } else if (*state == Ake_state_number_fraction_start) {
            if (!*done && Ake_is_num(c, num)) {
                *state = Ake_state_number_fraction;
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else {
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        } else if (*state == Ake_state_number_fraction) {
            if (!*done && Ake_is_num(c, num)) {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else if (!*done && *c == 'e') {
                *state = Ake_state_number_exponent_start;
                Zinc_string_add_char(&t->value, 'e');
            } else {
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        } else if (*state == Ake_state_number_exponent_start) {
            if (!*done && Ake_is_num(c, num)) {
                *state = Ake_state_number_exponent;
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else if (!*done && (*c == '-' || *c == '+')) {
                *state = Ake_state_number_exponent_sign_start;
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else {
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        } else if (*state == Ake_state_number_exponent_sign_start) {
            if (!*done && Ake_is_num(c, num)) {
                *state = Ake_state_number_exponent;
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else {
                valid = Zinc_error_list_set(ls->el, &loc, "expected number after exponent sign");
                /* test case: test_lex_error_exponent_sign */
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        } else if (*state == Ake_state_number_exponent) {
            if (!*done && Ake_is_num(c, num)) {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            } else {
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
                break;
            }
        }

    }

    return valid;
}

/**
 * lex a string
 * @param ls lexer data
 * @param state current state
 * @param got_token if made a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool Ake_lex_string(
        struct Ake_lex_state* ls,
        enum state_enum* state,
        bool* done,
        struct Ake_token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct Zinc_location loc;
    enum Zinc_result r;

    while (true) {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (*done) {
            for (int i = 0; i < num; i++) {
                Zinc_string_add_char(&t->value, c[i]);
            }
            valid = Zinc_error_list_set(ls->el, &loc, "Unclosed string");
            break;
        }

        if (*state == Ake_state_string) {
            if (*c == '\\') {
                *state = Ake_state_string_backslash;
            } else if (*c == '"') {
                *state = Ake_state_start;
                t->loc = loc;
                break;
            } else {
                for (int i = 0; i < num; i++) {
                    Zinc_string_add_char(&t->value, c[i]);
                }
            }
        } else if (*state == Ake_state_string_backslash) {
            if (*c == '\\') {
                Zinc_string_add_char(&t->value, '\\');
            } else if (*c == 'n') {
                Zinc_string_add_char(&t->value, '\n');
            } else if (*c == 'r') {
                Zinc_string_add_char(&t->value, '\r');
            } else {
                char a[5];
                int i = 0;
                while (i < num) {
                    a[i] = c[i];
                    i++;
                }
                a[i] = '\0';
                valid = Zinc_error_list_set(ls->el, &loc, "Unrecognized escape sequence: %s", a);
                /* test case: test_lex_string_escape_error */
                break;
            }
            *state = Ake_state_string;
        }
    }

    return valid;
}

/**
 * lex a compound operator
 * @param ls lexer state
 * @param state current state
 * @param got_token if got a token
 * @param t the token
 * @return true if valid, otherwise false
 */
bool Ake_lex_compound_operator(
        struct Ake_lex_state* ls,
        enum state_enum* state,
        bool* done,
        struct Ake_token* t)
{
    bool valid = true;
    char c[4];
    int num;
    struct Zinc_location loc;
    enum Zinc_result r;

    r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
    if (r == Zinc_result_error) {
        valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
    }

    assert(!*done);

    if (num == 1 && *c == '=') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '=') {
            t->type = Ake_token_double_equal;
            *state = Ake_state_start;
        } else {
            t->type = Ake_token_equal;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }
    } else if (num == 1 && *c == '!') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '=') {
            t->type = Ake_token_not_equal;
            *state = Ake_state_start;
        } else {
            t->type = Ake_token_not;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }
    } else if (num == 1 && *c == '<') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '=') {
            t->type = Ake_token_less_than_or_equal;
            *state = Ake_state_start;
        } else {
            t->type = Ake_token_less_than;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }
    } else if (num == 1 && *c == '>') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '=') {
            t->type = Ake_token_greater_than_or_equal;
            *state = Ake_state_start;
        } else {
            t->type = Ake_token_greater_than;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }
    } else if (num == 1 && *c == '&') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '&') {
            t->type = Ake_token_and;
            *state = Ake_state_start;
        } else {
            t->type = Ake_token_ampersand;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }
    } else if (num == 1 && *c == '|') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '|') {
            t->type = Ake_token_or;
            *state = Ake_state_start;
        } else {
            t->type = Ake_token_vertical_bar;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }
    } else if (num == 1 && *c == '-') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '>') {
            t->type = Ake_token_arrow;
            *state = Ake_state_start;
        } else {
            t->type = Ake_token_minus;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }
    } else if (num == 1 && *c == '.') {
        r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
        if (r == Zinc_result_error) {
            valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
        }

        if (num == 1 && *c == '.') {
            r = Zinc_input_unicode_next(ls->input, ls->vtable, c, &num, &loc, done);
            if (r == Zinc_result_error) {
                valid = Zinc_error_list_set(ls->el, &loc, Zinc_error_message);
            }

            if (num == 1 && *c == '.') {
                t->type = Ake_token_ellipsis;
                *state = Ake_state_start;
            } else {
                t->type = Ake_token_range;
                *state = Ake_state_start;
                Zinc_input_unicode_repeat(ls->input, ls->vtable);
            }
        } else {
            t->type = Ake_token_dot;
            *state = Ake_state_start;
            Zinc_input_unicode_repeat(ls->input, ls->vtable);
        }

    } else {
        /* unrecognized compound operator */
        assert(false);
    }

    return valid;
}

/**
 * lex and produce a token
 * @param ls lexer data
 * @param t the token
 * @return true if valid, otherwise false
 */
bool Ake_lex(struct Ake_lex_state* ls, struct Ake_token** t)
{
    bool valid = true;
    enum state_enum state = Ake_state_start;
    *t = NULL;
    struct Ake_token* tf;
    bool done = false;

    Zinc_malloc_safe((void**)&tf, sizeof(struct Ake_token));
    Ake_token_init(tf);

    valid = Ake_lex_start(ls, &state, &done, tf);
    if (!valid) {
        Ake_token_destroy(tf);
        free(tf);
        return valid;
    }

    if (tf->type == Ake_token_none) {
        assert(valid);
        tf->type = Ake_token_eof;
        tf->loc = Zinc_input_unicode_get_location(ls->input, ls->vtable);
    }

    *t = tf;

    struct Zinc_location end = Zinc_input_unicode_get_location(ls->input, ls->vtable);
    (*t)->loc.end_pos = end.start_pos;
    if ((*t)->loc.end_pos == (*t)->loc.start_pos) {
        (*t)->loc.end_pos++;
    }

    return valid;
}
