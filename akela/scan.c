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

/**
 * initialize scanner state
 * @param sns scanner data
 * @param lc lookahead buffer
 * @param el error list
 * @param st symbol table
 */
void scan_state_init(struct scan_state* sns, struct lookahead_char* lc, struct error_list* el, struct symbol_table* st)
{
    sns->lc = lc;
    sns->el = el;
    sns->st = st;
}

/**
 * check if compound operator
 * @param uc utf32 character
 * @return true if a compound operator, otherwise false
 */
bool compound_operator_start(UChar32 uc)
{
    return uc == '=' || uc == '!' || uc == '<' || uc == '>' || uc == '&' || uc == '|' || uc == ':';
}

/**
 * get scanner location
 * @param sns scanner state
 * @param loc location pointer
 */
void get_scan_location(struct scan_state* sns, struct location* loc)
{
    loc->line = sns->lc->line;
    loc->col = sns->lc->col;
    loc->byte_pos = sns->lc->byte_pos;
}

/**
 * scan in start state
 * @param sns scanner data
 * @param state current state
 * @param got_token if token is made
 * @param t the token
 * @return true if valid, otherwise false
 */
bool process_char_start(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    bool valid = true;
    struct lookahead_char* lc = sns->lc;
    *got_token = 0;
    UChar32 uc;
    if (lc->la_size > 0) {
        uc = lc->la0_32;
    } else {
        return result_ok;
    }

    if (u_isalpha(uc)) {
        *state = state_id;
        t->type = token_id;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = t->value.size;
    } else if (uc == '_') {
        *state = state_id_underscore;
        t->type = token_id;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = t->value.size;
    } else if (u_isdigit(uc)) {
        *state = state_number_whole;
        t->type = token_number;
        t->is_integer = true;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = t->value.size;
    } else if (uc == '"') {
        *state = state_string;
        t->type = token_string;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
    } else if (compound_operator_start(uc)) {
        *state = state_compound_operator;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = t->value.size;
    } else if (uc == '+') {
        t->type = token_plus;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '-') {
        t->type = token_minus;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '*') {
        t->type = token_mult;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '/') {
        t->type = token_divide;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '^') {
        t->type = token_caret;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '(') {
        t->type = token_left_paren;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == ')') {
        t->type = token_right_paren;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == ' ') {
        /* nothing */
    } else if (uc == '\t') {
        /* nothing */
    } else if (uc == '\n') {
        t->type = token_newline;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == ',') {
        t->type = token_comma;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == ';') {
        t->type = token_semicolon;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '[') {
        t->type = token_left_square_bracket;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == ']') {
        t->type = token_right_square_bracket;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '{') {
        t->type = token_left_curly_brace;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '}') {
        t->type = token_right_curly_brace;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else if (uc == '.') {
        t->type = token_dot;
        t->loc.line = lc->line;
        t->loc.col = lc->col;
        t->loc.byte_pos = lc->byte_pos;
        t->loc.size = 1;
        *got_token = 1;
    } else {
        char a[5];
        int i = 0;
        while (i < NUM_BYTES(lc->la0_8[0])) {
            a[i] = lc->la0_8[i];
            i++;
        }
        a[i] = '\0';

        struct location loc;
        get_scan_location(sns, &loc);
        /* allocate sns{el{}} */
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
bool process_char_word(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    bool valid = true;
    struct lookahead_char* lc = sns->lc;
    UChar32 uc;
    if (lc->la_size > 0) {
        uc = lc->la0_32;
    } else {
        uc = EOF;
    }

    if (*state == state_id) {
        if (uc == '_') {
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (u_isalpha(uc)) {
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (u_isdigit(uc)) {
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
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
            lookahead_char_push(lc);
        }
    } else if (*state == state_id_underscore) {
        if (uc == '_') {
            struct location loc;
            get_scan_location(sns, &loc);
            /* allocate sns{el{}} */
            set_source_error(sns->el, &loc, "Must have a letter following underscore at start of id");
            valid = false;
            /* test case: test_scan_error_underscore_letter2 */
        } else if (u_isdigit(uc)) {
            struct location loc;
            get_scan_location(sns, &loc);
            /* allocate sns{el{}} */
            set_source_error(sns->el, &loc, "Must have a letter following underscore at start of id");
            valid = false;
            /* test case: test_scan_error_underscore_letter */
        } else if (u_isalpha(uc)) {
            *state = state_id;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
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
            lookahead_char_push(lc);
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
bool process_char_number(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    bool valid = true;
    struct lookahead_char* lc = sns->lc;
    UChar32 uc;
    if (lc->la_size > 0) {
        uc = lc->la0_32;
    } else {
        uc = EOF;
    }

    if (*state == state_number_whole) {
        if (u_isdigit(uc)) {
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (uc == '.') {
            *state = state_number_fraction_start;
            t->is_integer = false;
            t->is_float = true;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (uc == 'e') {
            /* lookahead another character */
            UChar32 uc2 = lc->la1_32;
            if (lc->la_size >= 2 && (u_isdigit(uc2) || uc2 == '-' || uc2 == '+')) {
                /* e is part of exponent */
                *state = state_number_exponent_start;
                t->is_integer = false;
                t->is_float = true;
                /* allocate t{} */
                buffer_add_char(&t->value, 'e');
            } else {
                /* number is done and e will be part of an id */
                *state = state_start;
                *got_token = 1;
                lookahead_char_push(lc);
            }
        } else {
            *state = state_start;
            *got_token = 1;
            t->loc.size = t->value.size;
            lookahead_char_push(lc);
        }
    } else if (*state == state_number_fraction_start) {
        if (u_isdigit(uc)) {
            *state = state_number_fraction;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else {
            *state = state_start;
            *got_token = 1;
            t->loc.size = t->value.size;
            lookahead_char_push(lc);
        }
    } else if (*state == state_number_fraction) {
        if (u_isdigit(uc)) {
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (uc == 'e') {
            /* lookahead another character */
            UChar32 uc2 = lc->la1_32;
            if (lc->la_size >= 2 && (u_isdigit(uc2) || uc2 == '-' || uc2 == '+')) {
                /* e is part of exponent */
                *state = state_number_exponent_start;
                /* allocate t{} */
                buffer_add_char(&t->value, 'e');
            } else {
                /* number is done and e will be part of an id */
                *state = state_start;
                *got_token = 1;
                t->loc.size = t->value.size;
                lookahead_char_push(lc);
            }
        } else {
            *state = state_start;
            *got_token = 1;
            t->loc.size = t->value.size;
            lookahead_char_push(lc);
        }
    } else if (*state == state_number_exponent_start) {
        if (u_isdigit(uc)) {
            *state = state_number_exponent;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (uc == '-' || uc == '+') {
            *state = state_number_exponent_sign_start;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else {
            /* shound not happen because of lookahead: expecting <digit>, <->, or <+> after e in number */
            assert(false);
        }
    } else if (*state == state_number_exponent_sign_start) {
        if (u_isdigit(uc)) {
            *state = state_number_exponent;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else {
            struct location loc;
            get_scan_location(sns, &loc);
            valid = set_source_error(sns->el, &loc, "invalid number");
            /* test case: test_scan_error_exponent_sign */
        }
    } else if (*state == state_number_exponent) {
        if (u_isdigit(uc)) {
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else {
            *state = state_start;
            *got_token = 1;
            t->loc.size = t->value.size;
            lookahead_char_push(lc);
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
bool process_char_string(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    bool valid = true;
    struct lookahead_char* lc = sns->lc;
    UChar32 uc;
    if (lc->la_size > 0) {
        uc = lc->la0_32;
    } else {
        uc = EOF;
    }

    if (*state == state_string) {
        if (uc == '\\') {
            *state = state_string_backslash;
        } else if (uc == '"') {
            *state = state_start;
            *got_token = 1;
            t->loc.size = t->value.size;
        } else {
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        }
    } else if (*state == state_string_backslash) {
        if (uc == '\\') {
            /* allocate t{} */
            buffer_add_char(&t->value, '\\');
        } else if (uc == 'n') {
            /* allocate t{} */
            buffer_add_char(&t->value, '\n');
        } else if (uc == 'r') {
            /* allocate t{} */
            buffer_add_char(&t->value, '\r');
        } else {
            char a[5];
            int i = 0;
            while (i < NUM_BYTES(lc->la0_8[0])) {
                a[i] = lc->la0_8[i];
                i++;
            }
            a[i] = '\0';
            struct location loc;
            get_scan_location(sns, &loc);
            /* allocate sns{el{}} */
            valid = false;
            set_source_error(sns->el, &loc, "Unrecognized escape sequence: %s", a);
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
bool process_compound_operator(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    bool valid = true;
    struct lookahead_char* lc = sns->lc;
    
    if (lc->la_size > 0) {
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
    }

    if (buffer_compare_str(&t->value, "==")) {
        t->type = token_double_equal;
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
    } else if (buffer_compare_str(&t->value, "!=")) {
        t->type = token_not_equal;
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
    } else if (buffer_compare_str(&t->value, "<=")) {
        t->type = token_less_than_or_equal;
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
    } else if (buffer_compare_str(&t->value, ">=")) {
        t->type = token_greater_than_or_equal;
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
    } else if (buffer_compare_str(&t->value, "&&")) {
        t->type = token_and;
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
    } else if (buffer_compare_str(&t->value, "||")) {
        t->type = token_or;
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
    } else if (buffer_compare_str(&t->value, "::")) {
        t->type = token_double_colon;
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
    } else if (t->value.buf[0] == '=') {
        t->type = token_equal;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '!') {
        t->type = token_not;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '<') {
        t->type = token_less_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '>') {
        t->type = token_greater_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '&') {
        t->type = token_ampersand;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '|') {
        t->type = token_vertical_bar;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == ':') {
        t->type = token_colon;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        t->loc.size = t->value.size;
        lookahead_char_push(lc);
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
bool scan_process(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    bool valid = true;

    if (*state == state_start) {
        valid = process_char_start(sns, state, got_token, t);
    } else if (*state == state_id || *state == state_id_underscore) {
        valid = process_char_word(sns, state, got_token, t);
    } else if (is_number_state(*state)) {
        valid = process_char_number(sns, state, got_token, t);
    } else if (*state == state_string || *state == state_string_backslash) {
        valid = process_char_string(sns, state, got_token, t);
    } else if (*state == state_compound_operator) {
        valid = process_compound_operator(sns, state, got_token, t);
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

    /* allocate tf */
    malloc_safe((void**)&tf, sizeof(struct token));
    token_init(tf);

    while (!got_token && !lookahead_char_done(sns->lc)) {
        if (lookahead_char_need_preping(sns->lc)) {

            /* resource use sns{lc{conv}} */
            valid = lookahead_char_prep(sns->lc);
            if (!valid) {
                /* destroy tf{} tf */
                token_destroy(tf);
                free(tf);
                return valid;
            }
        }
        if (lookahead_char_need_loading(sns->lc)) lookahead_char_load(sns->lc);

        /* allocate sns{wt{} el{}} tf{} */
        valid = scan_process(sns, &state, &got_token, tf);
        if (!valid) {
            /* destroy tf{} tf */
            token_destroy(tf);
            free(tf);
            return valid;
        }

        lookahead_char_pop(sns->lc);
    }

    if (state != state_start) {
        /* allocate sns{wt{} el{}} tf{} */
        valid = scan_process(sns, &state, &got_token, tf);
        if (!valid) {
            /* destroy tf{} tf */
            token_destroy(tf);
            free(tf);
            return valid;
        }

        lookahead_char_pop(sns->lc);
    }

    if (!got_token && tf->type == token_none && lookahead_char_done(sns->lc)) {
        assert(valid);
        got_token = true;
        tf->type = token_eof;
        tf->loc.line = sns->lc->line;
        tf->loc.col = sns->lc->col;
        tf->loc.size = 0;
        tf->loc.byte_pos = sns->lc->byte_pos;
    }

    assert(got_token);
    /* allocate tf tf{} -> t t{} */
    *t = tf;
    return valid;
}
