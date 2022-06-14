#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include "result.h"
#include "token.h"
#include "scan.h"
#include "ustring.h"
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
}

enum result process_char_start(struct allocator* al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    struct char_value cv;
    set_char_values(&cv);
    *got_token = 0;

    if (u_isalpha(is->uc)) {
        *state = state_word;
        t->type = token_word;
        string_copy(al, &is->s, &t->value);
    } else if (u_isdigit(is->uc)) {
        *state = state_number;
        t->type = token_number;
        string_copy(al, &is->s, &t->value);
    } else if (is->uc == cv.equal) {
        t->type = token_equal;
        *got_token = 1;
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
    } else {
        char* a;
        enum result r = string2array(al, &is->s, &a);
        if (r == result_error) {
            return set_error("unrecogized character");
        }
        return set_error("unrecogized: %s", a);
    }
    return result_ok;
}

enum result process_char_word(struct allocator *al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    enum result r;
    struct char_value cv;
    set_char_values(&cv);

    if (u_isalpha(is->uc)) {
        r = string_copy(al, &is->s, &t->value);
        if (r == result_error) {
            return r;
        }
    } else if (u_isdigit(is->uc)) {
        r = string_copy(al, &is->s, &t->value);
        if (r == result_error) {
            return r;
        }
    } else {
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    }
    return result_ok;
}

enum result process_char_number(struct allocator *al, struct input_state* is, enum state_enum* state, int* got_token, struct token* t)
{
    struct char_value cv;
    set_char_values(&cv);

    if (u_isdigit(is->uc)) {
        string_copy(al, &is->s, &t->value);
    } else {
        *state = state_start;
        *got_token = 1;
        input_state_push_uchar(is);
    }
    return result_ok;
}

enum result scan_get_token(struct allocator *al, struct input_state* is, int* got_token, struct token** t)
{
    enum result r = result_ok;
    enum state_enum state = state_start;
    *got_token = 0;
    struct token* tf;       /* token for function */

    r = allocator_malloc(al, &tf, sizeof(struct token));
    if (r == result_error) {
        return r;
    }

    token_init(tf);

    while (get_uchar(al, is) != result_error && !is->done) {
        if (state == state_start) {
            r = process_char_start(al, is, &state, got_token, tf);
        } else if (state == state_word) {
            r = process_char_word(al, is, &state, got_token, tf);
        } else if (state == state_number) {
            r = process_char_number(al, is, &state, got_token, tf);
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
        state = state_start;
        *got_token = 1;
    }

function_success:
    *t = tf;
    return r;

function_error:
    return r;
}
