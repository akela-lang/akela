#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include "result.h"
#include "token.h"
#include "scan.h"
#include "buffer.h"
#include "input.h"

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
}

int compound_operator_start(UChar32 uc, struct char_value* cv)
{
    return uc == cv->equal || uc == cv->exclamation || uc == cv->less_than || uc == cv->greater_than;
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
        r = buffer_copy(al, &is->bf, &t->value);
        if (r == result_error) {
            return r;
        }
    } else if (is->uc == cv.underscore) {
            *state = state_id_underscore;
            t->type = token_id;
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
    } else if (u_isdigit(is->uc)) {
        *state = state_number;
        t->type = token_number;
        r = buffer_copy(al, &is->bf, &t->value);
        if (r == result_error) {
            return r;
        }
    } else if (compound_operator_start(is->uc, &cv)) {
        *state = state_compound_operator;
        r = buffer_copy(al, &is->bf, &t->value);
        if (r == result_error) {
            return r;
        }
    } else if (is->uc == cv.plus) {
        t->type = token_plus;
        *got_token = 1;
    } else if (is->uc == cv.minus) {
        t->type = token_minus;
        *got_token = 1;
    } else if (is->uc == cv.mult) {
        t->type = token_mult;
        *got_token = 1;
    } else if (is->uc == cv.divide) {
        t->type = token_divide;
        *got_token = 1;
    } else if (is->uc == cv.left_paren) {
        t->type = token_left_paren;
        *got_token = 1;
    } else if (is->uc == cv.right_paren) {
        t->type = token_right_paren;
        *got_token = 1;
    } else if (is->uc == cv.space) {
        /* nothing */
    } else if (is->uc == cv.newline) {
        t->type = token_newline;
        *got_token = 1;
    } else if (is->uc == cv.comma) {
        t->type = token_comma;
        *got_token = 1;
    } else if (is->uc == cv.semicolon) {
        t->type = token_semicolon;
        *got_token = 1;
    } else {
        char* a;
        enum result r = buffer2array(al, &is->bf, &a);
        if (r == result_error) {
            return set_error("unrecogized character");
        }
        return set_error("unrecogized: %s", a);
    }
    return result_ok;
}

void check_reserved_words(struct token* t)
{
    if (buffer_str_compare(&t->value, "function")) {
        t->type = token_function;
    } else if (buffer_str_compare(&t->value, "end")) {
        t->type = token_end;
    } else if (buffer_str_compare(&t->value, "if")) {
        t->type = token_if;
    } else if (buffer_str_compare(&t->value, "elseif")) {
        t->type = token_elseif;
    } else if (buffer_str_compare(&t->value, "else")) {
        t->type = token_else;
    }
}

enum result process_char_word(struct allocator *al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
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
            check_reserved_words(t);
            *state = state_start;
            *got_token = 1;
            input_state_push_uchar(is);
        }
    } else if (*state == state_id_underscore) {
        if (is->uc == cv.underscore) {
            return set_error("Must have a letter following underscore at start of id");
        } else if (u_isdigit(is->uc)) {
            return set_error("Must have a letter following underscore at start of id");
        } else if (u_isalpha(is->uc)) {
            *state = state_id;
            r = buffer_copy(al, &is->bf, &t->value);
            if (r == result_error) {
                return r;
            }
        } else {
            check_reserved_words(t);
            *state = state_start;
            *got_token = 1;
            input_state_push_uchar(is);
        }
    }

    return result_ok;
}

enum result process_char_number(struct allocator *al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    struct char_value cv;
    set_char_values(&cv);

    if (u_isdigit(is->uc)) {
        buffer_copy(al, &is->bf, &t->value);
    } else {
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
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
    } else {
        char* a;
        r = buffer2array(al, &t->value, &a);
        if (r == result_error) {
            return r;
        }
        return set_error("unrecognized compound operator: %s", a);
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
    }
}

enum result scan_get_token(struct allocator *al, struct input_state* is, int* got_token, struct token** t)
{
    enum result r = result_ok;
    enum state_enum state = state_start;
    *got_token = 0;
    struct token* tf;

    r = allocator_malloc(al, &tf, sizeof(struct token));
    if (r == result_error) {
        return r;
    }

    token_init(tf);

    while (get_uchar(al, is) != result_error && !is->done) {
        if (state == state_start) {
            r = process_char_start(al, is, &state, got_token, tf);
        } else if (state == state_id || state == state_id_underscore) {
            r = process_char_word(al, is, &state, got_token, tf);
        } else if (state == state_number) {
            r = process_char_number(al, is, &state, got_token, tf);
        } else if (state == state_compound_operator) {
            r = process_compound_operator(al, is, &state, got_token, tf);
        } else {
            r = set_error("unexpected state");
        }
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

    if (state != state_start && tf->type != token_none) {
        if (state == state_id) {
            check_reserved_words(tf);
        }
        state = state_start;
        *got_token = 1;
    } else if (state == state_compound_operator) {
        check_for_operators(is, &state, got_token, tf);
    }

function_success:
    *t = tf;
    return r;

function_error:
    return r;
}
