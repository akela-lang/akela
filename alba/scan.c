#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "scan.h"
#include "zinc/buffer.h"
#include "zinc/memory.h"
#include "input.h"
#include "source.h"
#include "uconv.h"
#include "zinc/utf8.h"

/* dynamic-output-none */
/* initialize-output sns{} */
void scan_state_init(struct scan_state* sns, struct lookahead_char* lc, struct word_table* wt, struct compile_error_list* el)
{
    sns->lc = lc;
    sns->wt = wt;
    sns->el = el;
}

/* dynamic-output-none */
int compound_operator_start(UChar32 uc)
{
    return uc == '=' || uc == '!' || uc == '<' || uc == '>' || uc == '&' || uc == '|' || uc == ':';
}

/* dynamic-output-none */
bool is_number_state(enum state_enum state)
{
    return state == state_number_whole || state == state_number_fraction || state == state_number_exponent_start || state == state_number_exponent;
}

/* dynamic-output-none */
void get_scan_location(struct scan_state* sns, struct location* loc)
{
    loc->line = sns->lc->line;
    loc->col = sns->lc->col;
    loc->byte_pos = sns->lc->byte_pos;
}

/* dynamic-output sns{el{}} t{} */
enum result process_char_start(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    struct lookahead_char* lc = sns->lc;
    *got_token = 0;
    UChar32 uc;
    if (lc->la_size > 0) {
        uc = lc->la0_32;
    } else {
        uc = EOF;
        return result_ok;
    }

    if (u_isalpha(uc)) {
        *state = state_id;
        t->type = token_id;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
    } else if (uc == '_') {
        *state = state_id_underscore;
        t->type = token_id;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
    } else if (u_isdigit(uc)) {
        *state = state_number_whole;
        t->type = token_number;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
    } else if (uc == '"') {
        *state = state_string;
        t->type = token_string;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
    } else if (compound_operator_start(uc)) {
        *state = state_compound_operator;
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
    } else if (uc == '+') {
        t->type = token_plus;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == '-') {
        t->type = token_minus;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == '*') {
        t->type = token_mult;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == '/') {
        t->type = token_divide;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == '(') {
        t->type = token_left_paren;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == ')') {
        t->type = token_right_paren;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == ' ') {
        /* nothing */
    } else if (uc == '\n') {
        t->type = token_newline;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == ',') {
        t->type = token_comma;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == ';') {
        t->type = token_semicolon;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == '[') {
        t->type = token_left_square_bracket;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
        *got_token = 1;
    } else if (uc == ']') {
        t->type = token_right_square_bracket;
        t->line = lc->line;
        t->col = lc->col;
        t->byte_pos = lc->byte_pos;
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
        return set_source_error(sns->el, &loc, "Unrecognized character: %s", a);
    }
    return result_ok;
}

/* dynamic-output sns{wt{} el{}} t{} */
enum result process_char_word(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    struct lookahead_char* lc = sns->lc;
    struct word_table* wt = sns->wt;
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
            struct word* w = word_table_get(wt, &t->value);
            if (w) {
                t->type = w->type;
            } else {
                /* allocate wt{} */
                word_table_add(wt, &t->value, t->type);
            }
            *state = state_start;
            *got_token = 1;
            lookahead_char_push(lc);
        }
    } else if (*state == state_id_underscore) {
        if (uc == '_') {
            struct location loc;
            get_scan_location(sns, &loc);
            /* allocate sns{el{}} */
            return set_source_error(sns->el, &loc, "Must have a letter following underscore at start of id");
        } else if (u_isdigit(uc)) {
            struct location loc;
            get_scan_location(sns, &loc);
            /* allocate sns{el{}} */
            return set_source_error(sns->el, &loc, "Must have a letter following underscore at start of id");
        } else if (u_isalpha(uc)) {
            *state = state_id;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else {
            struct word* w = word_table_get(wt, &t->value);
            if (w) {
                t->type = w->type;
            } else {
                /* allocate wt{} */
                word_table_add(wt, &t->value, t->type);
            }
            *state = state_start;
            *got_token = 1;
            lookahead_char_push(lc);
        }
    }

    return result_ok;
}

/* dynamic-output t{} */
enum result process_char_number(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
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
            *state = state_number_fraction;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (uc == 'e') {
            *state = state_number_exponent_start;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else {
            *state = state_start;
            *got_token = 1;
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
                lookahead_char_push(lc);
            }
        } else {
            *state = state_start;
            *got_token = 1;
            lookahead_char_push(lc);
        }
    } else if (*state == state_number_exponent_start) {
        if (u_isdigit(uc)) {
            *state = state_number_exponent;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (uc == '-') {
            *state = state_number_exponent;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                /* allocate t{} */
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else if (uc == '+') {
            *state = state_number_exponent;
            for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
                buffer_add_char(&t->value, lc->la0_8[i]);
            }
        } else {
            /* shound not happen because of lookahead */
            return set_error(NULL, lc, "expecting <digit>, <->, or <+> after e in number");
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
            lookahead_char_push(lc);
        }
    }
    return result_ok;
}

/* dynamic-output sns{el{}} t{} */
enum result process_char_string(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
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
            return set_source_error(sns->el, &loc, "Unrecognized escape sequence: %s", a);
        }
        *state = state_string;
    }

    return result_ok;
}

/* dynamic-output sns{el{}} t{} */
enum result process_compound_operator(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    struct lookahead_char* lc = sns->lc;
    enum result r = result_ok;

    if (lc->la_size > 0) {
        for (int i = 0; i < NUM_BYTES(lc->la0_8[0]); i++) {
            /* allocate t{} */
            buffer_add_char(&t->value, lc->la0_8[i]);
        }
    }

    if (buffer_str_compare(&t->value, "==")) {
        t->type = token_double_equal;
        *state = state_start;
        *got_token = 1;
    } else if (buffer_str_compare(&t->value, "!=")) {
        t->type = token_not_equal;
        *state = state_start;
        *got_token = 1;
    } else if (buffer_str_compare(&t->value, "<=")) {
        t->type = token_less_than_or_equal;
        *state = state_start;
        *got_token = 1;
    } else if (buffer_str_compare(&t->value, ">=")) {
        t->type = token_greater_than_or_equal;
        *state = state_start;
        *got_token = 1;
    } else if (buffer_str_compare(&t->value, "&&")) {
        t->type = token_and;
        *state = state_start;
        *got_token = 1;
    } else if (buffer_str_compare(&t->value, "||")) {
        t->type = token_or;
        *state = state_start;
        *got_token = 1;
    } else if (buffer_str_compare(&t->value, "::")) {
        t->type = token_double_colon;
        *state = state_start;
        *got_token = 1;
    } else if (t->value.buf[0] == '=') {
        t->type = token_equal;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '!') {
        t->type = token_not;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '<') {
        t->type = token_less_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '>') {
        t->type = token_greater_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '&') {
        t->type = token_ampersand;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == '|') {
        t->type = token_vertical_bar;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        lookahead_char_push(lc);
    } else if (t->value.buf[0] == ':') {
        t->type = token_colon;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        lookahead_char_push(lc);
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
        return set_source_error(sns->el, &loc, "unrecognized compound operator: %s", a);
    }

    return r;
}

/* dyanmic-output sns{wt{} el{}} t{} */
enum result scan_process(struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r = result_ok;

    if (*state == state_start) {
        r = process_char_start(sns, state, got_token, t);
    } else if (*state == state_id || *state == state_id_underscore) {
        r = process_char_word(sns, state, got_token, t);
    } else if (is_number_state(*state)) {
        r = process_char_number(sns, state, got_token, t);
    } else if (*state == state_string || *state == state_string_backslash) {
        r = process_char_string(sns, state, got_token, t);
    } else if (*state == state_compound_operator) {
        r = process_compound_operator(sns, state, got_token, t);
    } else {
        r = set_error("unexpected state");
    }
    return r;
}

/* dynamic-output sns{wt{} el{}} t t{} */
/* dynamic-temp: tf tf{} */
enum result scan_get_token(struct scan_state* sns, int* got_token, struct token** t)
{
    enum result r = result_ok;
    enum state_enum state = state_start;
    *got_token = 0;
    struct token* tf;

    /* allocate tf */
    malloc_safe(&tf, sizeof(struct token));
    token_init(tf);

    while (!*got_token && !lookahead_char_done(sns->lc)) {
        if (lookahead_char_need_preping(sns->lc)) {

            /* resource use sns{lc{conv}} */
            r = lookahead_char_prep(sns->lc);
            if (r == result_error) {
                /* destroy tf{} tf */
                token_destroy(tf);
                free(tf);
            }
        }
        if (lookahead_char_need_loading(sns->lc)) lookahead_char_load(sns->lc);

        /* allocate sns{wt{} el{}} tf{} */
        r = scan_process(sns, &state, got_token, tf);
        if (r == result_error) {
            /* destroy tf{} tf */
            token_destroy(tf);
            free(tf);
            return r;
        }

        lookahead_char_pop(sns->lc);
    }

    if (state != state_start) {
        /* allocate sns{wt{} el{}} tf{} */
        r = scan_process(sns, &state, got_token, tf);
        if (r == result_error) {
            /* destroy tf{} tf */
            token_destroy(tf);
            free(tf);
            return r;
        }

        lookahead_char_pop(sns->lc);
    }

    /* allocate tf tf{} -> t t{} */
    *t = tf;
    return r;
}
