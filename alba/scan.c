#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include <stdbool.h>
#include "result.h"
#include "token.h"
#include "scan.h"
#include "buffer.h"
#include "input.h"
#include "source.h"

void scan_state_init(struct scan_state* sns, struct input_state* is, struct word_table* wt)
{
    sns->is = is;
    sns->wt = wt;
    sns->has_next = false;
    sns->state = state_start;
    sns->t = NULL;
}

void set_char_values(struct char_value* cv)
{
    size_t pos2;
    size_t size;

    U_STRING_DECL(equal, "=", 1);
    U_STRING_INIT(equal, "=", 1);
    pos2 = 0;
    size = u_strlen(equal);
    U16_NEXT(equal, pos2, size, cv->equal);

    U_STRING_DECL(plus, "+", 1);
    U_STRING_INIT(plus, "+", 1);
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(plus, pos2, size, cv->plus);

    U_STRING_DECL(space, " ", 1);
    U_STRING_INIT(space, " ", 1);
    pos2 = 0;
    size = u_strlen(space);
    U16_NEXT(space, pos2, size, cv->space);

    U_STRING_DECL(newline, "\n", 1);
    U_STRING_INIT(newline, "\n", 1);
    pos2 = 0;
    size = u_strlen(newline);
    U16_NEXT(newline, pos2, size, cv->newline);

    U_STRING_DECL(minus, "-", 1);
    U_STRING_INIT(minus, "-", 1);
    pos2 = 0;
    size = u_strlen(minus);
    U16_NEXT(minus, pos2, size, cv->minus);

    U_STRING_DECL(mult, "*", 1);
    U_STRING_INIT(mult, "*", 1);
    pos2 = 0;
    size = u_strlen(mult);
    U16_NEXT(mult, pos2, size, cv->mult);

    U_STRING_DECL(divide, "/", 1);
    U_STRING_INIT(divide, "/", 1);
    pos2 = 0;
    size = u_strlen(divide);
    U16_NEXT(divide, pos2, size, cv->divide);

    U_STRING_DECL(left_paren, "(", 1);
    U_STRING_INIT(left_paren, "(", 1);
    pos2 = 0;
    size = u_strlen(left_paren);
    U16_NEXT(left_paren, pos2, size, cv->left_paren);

    U_STRING_DECL(right_paren, ")", 1);
    U_STRING_INIT(right_paren, ")", 1);
    pos2 = 0;
    size = u_strlen(right_paren);
    U16_NEXT(right_paren, pos2, size, cv->right_paren);

    U_STRING_DECL(comma, ",", 1);
    U_STRING_INIT(comma, ",", 1);
    pos2 = 0;
    size = u_strlen(comma);
    U16_NEXT(comma, pos2, size, cv->comma);

    U_STRING_DECL(less_than, "<", 1);
    U_STRING_INIT(less_than, "<", 1);
    pos2 = 0;
    size = u_strlen(less_than);
    U16_NEXT(less_than, pos2, size, cv->less_than);

    U_STRING_DECL(greater_than, ">", 1);
    U_STRING_INIT(greater_than, ">", 1);
    pos2 = 0;
    size = u_strlen(greater_than);
    U16_NEXT(greater_than, pos2, size, cv->greater_than);

    U_STRING_DECL(underscore, "_", 1);
    U_STRING_INIT(underscore, "_", 1);
    pos2 = 0;
    size = u_strlen(underscore);
    U16_NEXT(underscore, pos2, size, cv->underscore);

    U_STRING_DECL(semicolon, ";", 1);
    U_STRING_INIT(semicolon, ";", 1);
    pos2 = 0;
    size = u_strlen(semicolon);
    U16_NEXT(semicolon, pos2, size, cv->semicolon);

    U_STRING_DECL(exclamation, "!", 1);
    U_STRING_INIT(exclamation, "!", 1);
    pos2 = 0;
    size = u_strlen(exclamation);
    U16_NEXT(exclamation, pos2, size, cv->exclamation);

    U_STRING_DECL(ampersand, "&", 1);
    U_STRING_INIT(ampersand, "&", 1);
    pos2 = 0;
    size = u_strlen(ampersand);
    U16_NEXT(ampersand, pos2, size, cv->ampersand);

    U_STRING_DECL(vertical_bar, "|", 1);
    U_STRING_INIT(vertical_bar, "|", 1);
    pos2 = 0;
    size = u_strlen(vertical_bar);
    U16_NEXT(vertical_bar, pos2, size, cv->vertical_bar);

    U_STRING_DECL(colon, ":", 1);
    U_STRING_INIT(colon, ":", 1);
    pos2 = 0;
    size = u_strlen(colon);
    U16_NEXT(colon, pos2, size, cv->colon);

    U_STRING_DECL(left_square_bracket, "[", 1);
    U_STRING_INIT(left_square_bracket, "[", 1);
    pos2 = 0;
    size = u_strlen(left_square_bracket);
    U16_NEXT(left_square_bracket, pos2, size, cv->left_square_bracket);

    U_STRING_DECL(right_square_bracket, "]", 1);
    U_STRING_INIT(right_square_bracket, "]", 1);
    pos2 = 0;
    size = u_strlen(right_square_bracket);
    U16_NEXT(right_square_bracket, pos2, size, cv->right_square_bracket);

    U_STRING_DECL(double_quote, "\"", 1);
    U_STRING_INIT(double_quote, "\"", 1);
    pos2 = 0;
    size = u_strlen(double_quote);
    U16_NEXT(double_quote, pos2, size, cv->double_quote);

    U_STRING_DECL(backslash, "\\", 1);
    U_STRING_INIT(backslash, "\\", 1);
    pos2 = 0;
    size = u_strlen(backslash);
    U16_NEXT(backslash, pos2, size, cv->backslash);
}

int compound_operator_start(UChar32 uc, struct char_value* cv)
{
    return uc == cv->equal || uc == cv->exclamation || uc == cv->less_than || uc == cv->greater_than || uc == cv->ampersand || uc == cv->vertical_bar || uc == cv->colon;
}

bool is_number_state(enum state_enum state)
{
    return state == state_number_whole || state == state_number_fraction || state == state_number_exponent_start || state == state_number_exponent;
}

enum result process_char_start(struct allocator* al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r;
    struct char_value cv;
    set_char_values(&cv);
    *got_token = 0;

    if (u_isalpha(is->uc)) {
        *state = state_id;
        t->type = token_id;
        t->line = is->line;
        t->col = is->col;
        r = buffer_copy(al, &is->bf, &t->value);
        if (r == result_error) {
            return r;
        }
    } else if (is->uc == cv.underscore) {
            *state = state_id_underscore;
            t->type = token_id;
            t->line = is->line;
            t->col = is->col;
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
    } else if (u_isdigit(is->uc)) {
        *state = state_number_whole;
        t->type = token_number;
        t->line = is->line;
        t->col = is->col;
        r = buffer_copy(al, &is->bf, &t->value);
        if (r == result_error) {
            return r;
        }
    } else if (is->uc == cv.double_quote) {
        *state = state_string;
        t->type = token_string;
        t->line = is->line;
        t->col = is->col;
    } else if (compound_operator_start(is->uc, &cv)) {
        *state = state_compound_operator;
        r = buffer_copy(al, &is->bf, &t->value);
        if (r == result_error) {
            return r;
        }
        t->line = is->line;
        t->col = is->col;
    } else if (is->uc == cv.plus) {
        t->type = token_plus;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.minus) {
        t->type = token_minus;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.mult) {
        t->type = token_mult;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.divide) {
        t->type = token_divide;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.left_paren) {
        t->type = token_left_paren;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.right_paren) {
        t->type = token_right_paren;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.space) {
        /* nothing */
    } else if (is->uc == cv.newline) {
        t->type = token_newline;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.comma) {
        t->type = token_comma;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.semicolon) {
        t->type = token_semicolon;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.left_square_bracket) {
        t->type = token_left_square_bracket;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else if (is->uc == cv.right_square_bracket) {
        t->type = token_right_square_bracket;
        t->line = is->line;
        t->col = is->col;
        *got_token = 1;
    } else {
        char* a;
        enum result r = buffer2array(al, &is->bf, &a);
        if (r == result_error) {
            return set_source_error(NULL, is, error_message);
        }
        return set_source_error(NULL, is, "Unrecognized character: %s", a);
    }
    return result_ok;
}

enum result process_char_word(struct allocator *al, struct input_state* is, struct word_table* wt, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r;
    struct char_value cv;
    set_char_values(&cv);

    if (*state == state_id) {
        if (is->uc == cv.underscore) {
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
        } else if (u_isalpha(is->uc)) {
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
        } else if (u_isdigit(is->uc)) {
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
        } else {
            struct word* w = word_table_get(wt, &t->value);
            if (w) {
                t->type = w->type;
            } else {
                r = word_table_add(al, wt, &t->value, t->type);
                if (r == result_error) return r;
            }
            *state = state_start;
            *got_token = 1;
            input_state_push_uchar(is);
        }
    } else if (*state == state_id_underscore) {
        if (is->uc == cv.underscore) {
            return set_source_error(NULL, is, "Must have a letter following underscore at start of id");
        } else if (u_isdigit(is->uc)) {
            return set_source_error(NULL, is, "Must have a letter following underscore at start of id");
        } else if (u_isalpha(is->uc)) {
            *state = state_id;
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
        } else {
            struct word* w = word_table_get(wt, &t->value);
            if (w) {
                t->type = w->type;
            } else {
                r = word_table_add(al, wt, &t->value, t->type);
                if (r == result_error) return r;
            }
            *state = state_start;
            *got_token = 1;
            input_state_push_uchar(is);
        }
    }

    return result_ok;
}

enum result process_char_number(struct allocator *al, struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r;
    struct input_state* is = sns->is;
    struct char_value cv;
    set_char_values(&cv);

    if (*state == state_number_whole) {
        if (u_isdigit(is->uc)) {
            buffer_copy(al, &is->bf, &t->value);
        } else if (is->uc == '.') {
            *state = state_number_fraction;
            buffer_copy(al, &is->bf, &t->value);
        } else if (is->uc == 'e') {
            *state = state_number_exponent_start;
            buffer_copy(al, &is->bf, &t->value);
        } else {
            t->type = token_number;
            *state = state_start;
            *got_token = 1;
            input_state_push_uchar(is);
        }
    } else if (*state == state_number_fraction) {
        if (u_isdigit(is->uc)) {
            buffer_copy(al, &is->bf, &t->value);
        } else if (is->uc == 'e') {
            *state = state_number_exponent_start;
            buffer_copy(al, &is->bf, &t->value);
        } else {
            t->type = token_number;
            *state = state_start;
            *got_token = 1;
            input_state_push_uchar(is);
        }
    } else if (*state == state_number_exponent_start) {
        if (u_isdigit(is->uc)) {
            buffer_copy(al, &is->bf, &t->value);
            *state = state_number_exponent;
        } else if (is->uc == '-') {
            *state = state_number_exponent;
            buffer_copy(al, &is->bf, &t->value);
        } else if (is->uc == '+') {
            *state = state_number_exponent;
            buffer_copy(al, &is->bf, &t->value);
        } else {
            /* emit float without exponent */
            t->type = token_number;
            t->value.size--;
            *got_token = 1;

            /* prepare next token with first letter e */
            *state = state_start;
            sns->has_next = true;
            sns->state = state_id;
            r = allocator_malloc(al, &sns->t, sizeof(struct token));
            if (r == result_error) return r;
            token_init(sns->t);
            sns->t->type = token_id;
            sns->t->line = is->line;
            sns->t->col = is->col - 1;
            r = buffer_add_char(al, &sns->t->value, 'e');
            if (r == result_error) return r;

            /* push last uchar */
            input_state_push_uchar(is);
        }
    } else if (*state == state_number_exponent) {
        if (u_isdigit(is->uc)) {
            buffer_copy(al, &is->bf, &t->value);
        } else {
            *state = state_start;
            *got_token = 1;
            input_state_push_uchar(is);
        }
    }
    return result_ok;
}

enum result process_char_string(struct allocator* al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r = result_ok;
    struct char_value cv;
    set_char_values(&cv);

    if (*state == state_string) {
        if (is->uc == cv.backslash) {
            *state = state_string_backslash;
        } else if (is->uc == cv.double_quote) {
            *state = state_start;
            *got_token = 1;
        } else {
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
        }
    } else if (*state == state_string_backslash) {
        if (is->uc == cv.backslash) {
            r = buffer_add_char(al, &t->value, '\\');
            if (r == result_error) {
                return r;
            }
        } else if (is->uc == 'n') {
            r = buffer_add_char(al, &t->value, '\n');
            if (r == result_error) {
                return r;
            }
        } else if (is->uc == 'r') {
            r = buffer_add_char(al, &t->value, '\r');
            if (r == result_error) {
                return r;
            }
        } else {
            char* a;
            r = buffer2array(al, &is->bf, &a);
            if (r == result_error) {
                return r;
            }
            return set_source_error(NULL, is, "Unrecognized escape sequence: %s", a);
        }
        *state = state_string;
    }

    return result_ok;
}

enum result process_compound_operator(struct allocator* al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r = result_ok;

    r = buffer_copy(al, &is->bf, &t->value);
    if (r == result_error) {
        return r;
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
        input_state_push_uchar(is);
    } else if (t->value.buf[0] == '!') {
        t->type = token_not;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    } else if (t->value.buf[0] == '<') {
        t->type = token_less_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    } else if (t->value.buf[0] == '>') {
        t->type = token_greater_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    } else if (t->value.buf[0] == '&') {
        t->type = token_ampersand;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    } else if (t->value.buf[0] == '|') {
        t->type = token_vertical_bar;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    } else if (t->value.buf[0] == ':') {
        t->type = token_colon;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    } else {
        char* a;
        r = buffer2array(al, &t->value, &a);
        if (r == result_error) {
            return r;
        }
        return set_source_error(NULL, is, "unrecognized compound operator: %s", a);
    }

    return r;
}

void check_for_operators(struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    if (t->value.buf[0] == '=') {
        t->type = token_equal;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
    } else if (t->value.buf[0] == '!') {
        t->type = token_not;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
    } else if (t->value.buf[0] == '<') {
        t->type = token_less_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
    } else if (t->value.buf[0] == '>') {
        t->type = token_greater_than;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
    } else if (t->value.buf[0] == '&') {
        t->type = token_ampersand;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
    } else if (t->value.buf[0] == '|') {
        t->type = token_vertical_bar;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
    } else if (t->value.buf[0] == ':') {
        t->type = token_colon;
        buffer_clear(&t->value);
        *state = state_start;
        *got_token = 1;
    }
}

enum result scan_process(struct allocator* al, struct scan_state* sns, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r = result_ok;
    struct input_state* is = sns->is;
    struct word_table* wt = sns->wt;

    if (*state == state_start) {
        r = process_char_start(al, is, state, got_token, t);
    } else if (*state == state_id || *state == state_id_underscore) {
        r = process_char_word(al, is, wt, state, got_token, t);
    } else if (is_number_state(*state)) {
        r = process_char_number(al, sns, state, got_token, t);
    } else if (*state == state_string || *state == state_string_backslash) {
        r = process_char_string(al, is, state, got_token, t);
    } else if (*state == state_compound_operator) {
        r = process_compound_operator(al, is, state, got_token, t);
    } else {
        r = set_source_error(NULL, is, "unexpected state");
    }
    return r;
}

enum result scan_get_token(struct allocator *al, struct scan_state* sns, int* got_token, struct token** t)
{
    enum result r = result_ok;
    enum state_enum state = state_start;
    *got_token = 0;
    struct token* tf;
    struct input_state* is = sns->is;
    struct word_table* wt = sns->wt;

    if (sns->has_next) {
        state = sns->state;
        tf = sns->t;
        sns->has_next = false;
        sns->state = state_start;
        sns->t = NULL;
    } else {
        r = allocator_malloc(al, &tf, sizeof(struct token));
        if (r == result_error) {
            return r;
        }
        token_init(tf);
    }


    while (get_uchar(al, is) != result_error && !is->done) {
        r = scan_process(al, sns, &state, got_token, tf);
        if (r == result_error) {
            return r;
        }
        if (*got_token) {
            break;
        }
    }

    if (r == result_error) {
        return r;
    }

    if (state != state_start) {
        r = scan_process(al, sns, &state, got_token, tf);
        if (r == result_error) return r;
    }

function_success:
    *t = tf;
    return r;

function_error:
    return r;
}
