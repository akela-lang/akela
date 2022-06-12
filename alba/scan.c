#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include "result.h"
#include "token.h"
#include "scan.h"
#include "ustring.h"

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

enum result process_char_start(struct allocator* al, struct string* s, UChar32 c2, enum state_enum* state, struct token_list* tl, struct token* t)
{
    struct char_value cv;
    set_char_values(&cv);

    if (u_isalpha(c2)) {
        *state = state_word;
        t->type = token_word;
        string_copy(al, s, &t->value);
    } else if (u_isdigit(c2)) {
        *state = state_number;
        t->type = token_number;
        string_copy(al, s, &t->value);
    } else if (c2 == cv.equal) {
        t->type = token_equal;
        token_list_add(al, tl, t);
        token_reset(t);
    } else if (c2 == cv.plus) {
        t->type = token_plus;
        token_list_add(al, tl, t);
        token_reset(t);
    } else if (c2 == cv.minus) {
        t->type = token_minus;
        token_list_add(al, tl, t);
        token_reset(t);
    } else if (c2 == cv.mult) {
        t->type = token_mult;
        token_list_add(al, tl, t);
        token_reset(t);
    } else if (c2 == cv.divide) {
        t->type = token_divide;
        token_list_add(al, tl, t);
        token_reset(t);
    } else if (c2 == cv.left_paren) {
        t->type = token_left_paren;
        token_list_add(al, tl, t);
        token_reset(t);
    } else if (c2 == cv.right_paren) {
        t->type = token_right_paren;
        token_list_add(al, tl, t);
        token_reset(t);
    } else if (c2 == cv.space) {
        /* nothing */
    } else if (c2 == cv.newline) {
        t->type = token_newline;
        token_list_add(al, tl, t);
        token_reset(t);
    } else {
        char* a;
        enum result r = string2array(al, s, &a);
        if (r == result_error) {
            return set_error("unrecogized character");
        }
        return set_error("unrecogized: %s", a);
    }
    return result_ok;
}

enum result process_char_word(struct allocator *al, struct string* s, UChar32 c2, enum state_enum* state, struct token_list* tl, struct token* t)
{
    enum result r;
    struct char_value cv;
    set_char_values(&cv);

    if (u_isalpha(c2)) {
        string_copy(al, s, &t->value);
    } else if (u_isdigit(c2)) {
        string_copy(al, s, &t->value);
    } else {
        *state = state_start;
        r = token_list_add(al, tl, t);
        if (r == result_error) {
            return r;
        }
        token_reset(t);
        return process_char_start(al, s, c2, state, tl, t);
    }
    return result_ok;
}

enum result process_char_number(struct allocator *al, struct string* s, UChar32 c2, enum state_enum* state, struct token_list* tl, struct token* t)
{
    enum result r;
    struct char_value cv;
    set_char_values(&cv);

    if (u_isdigit(c2)) {
        string_copy(al, s, &t->value);
    } else {
        *state = state_start;
        r = token_list_add(al, tl, t);
        if (r == result_error) {
            return r;
        }
        token_reset(t);
        return process_char_start(al, s, c2, state, tl, t);
    }
    return result_ok;
}

enum result scan(struct allocator *al, struct string* line, struct token_list* tl)
{
    enum result r = result_ok;
    size_t pos = 0;
    enum state_enum state = state_start;
    struct token t;

    token_init(&t);

    UConverter* conv = NULL;
    r = conv_open(&conv);
    if (r == result_error) {
        goto cleanup;
    }

    struct string_data sd;
    string_data_init(line, &sd);
    io_getchar f = string_getchar;
    io_data d = &sd;

    UChar32 uc;
    int done;
    struct string s;
    string_init(&s);

    while (get_uchar(al, f, d, &s, conv, &uc, &done) != result_error && !done) {
        if (state == state_start) {
            r = process_char_start(al, &s, uc, &state, tl, &t);
        } else if (state == state_word) {
            r = process_char_word(al, &s, uc, &state, tl, &t);
        } else if (state == state_number) {
            r = process_char_number(al, &s, uc, &state, tl, &t);
        } else {
            r = set_error("unexpected state");
        }
        if (r == result_error) {
            return r;
        }
    }

    if (state != state_start && t.type != token_none) {
        state = state_start;
        r = token_list_add(al, tl, &t);
        if (r == result_error) {
            return r;
        }
    }

cleanup:
    conv_close(conv);
    return r;
}
