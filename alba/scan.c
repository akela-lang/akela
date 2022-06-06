#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include "result.h"
#include "token.h"
#include "scan.h"
#include "ustring.h"
#include "defer.h"

void set_char_values(struct char_value* cv)
{
    size_t pos2;
    size_t size;

    U_STRING_DECL(plus, "+", 1);
    U_STRING_INIT(plus, "+", 1);
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(plus, pos2, size, cv->plus);

    U_STRING_DECL(space, " ", 1);
    U_STRING_INIT(space, " ", 1);
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(space, pos2, size, cv->space);

    U_STRING_DECL(minus, "-", 1);
    U_STRING_INIT(minus, "-", 1);
    pos2 = 0;
    size = u_strlen(plus);
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

enum result process_char_start(UChar32 c2, char* a, size_t len, enum state_enum* state, struct token_list* tl, struct token* t)
{
    struct char_value cv;
    set_char_values(&cv);

    if (u_isalpha(c2)) {
        *state = state_word;
        t->type = token_word;
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else if (u_isdigit(c2)) {
        *state = state_number;
        t->type = token_number;
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else if (c2 == cv.plus) {
        t->type = token_plus;
        token_list_add_defer(tl, t);
        token_reset(t);
    } else if (c2 == cv.minus) {
        t->type = token_minus;
        token_list_add_defer(tl, t);
        token_reset(t);
    } else if (c2 == cv.mult) {
        t->type = token_mult;
        token_list_add_defer(tl, t);
        token_reset(t);
    } else if (c2 == cv.divide) {
        t->type = token_divide;
        token_list_add_defer(tl, t);
        token_reset(t);
    } else if (c2 == cv.left_paren) {
        t->type = token_left_paren;
        token_list_add_defer(tl, t);
        token_reset(t);
    } else if (c2 == cv.right_paren) {
        t->type = token_right_paren;
        token_list_add_defer(tl, t);
        token_reset(t);
    } else if (c2 == cv.space) {
        /* nothing */
    } else {
        return set_error("unrecogized: %s", a);
    }
    return result_ok;
}

enum result process_char_word(UChar32 c2, char* a, size_t len, enum state_enum* state, struct token_list* tl, struct token* t)
{
    enum result r;
    struct char_value cv;
    set_char_values(&cv);

    if (u_isalpha(c2)) {
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else if (u_isdigit(c2)) {
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else {
        *state = state_start;
        r = token_list_add_defer(tl, t);
        if (r == result_error) {
            return r;
        }
        token_reset(t);
        return process_char_start(c2, a, len, state, tl, t);
    }
    return result_ok;
}

enum result process_char_number(UChar32 c2, char* a, size_t len, enum state_enum* state, struct token_list* tl, struct token* t)
{
    enum result r;
    struct char_value cv;
    set_char_values(&cv);

    if (u_isdigit(c2)) {
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else {
        *state = state_start;
        r = token_list_add_defer(tl, t);
        if (r == result_error) {
            return r;
        }
        token_reset(t);
        return process_char_start(c2, a, len, state, tl, t);
    }
    return result_ok;
}

enum result scan(struct string* line, struct token_list* tl)
{
    enum result r;
    size_t pos = 0;
    struct string s;
    struct defer_node* ds = NULL;
    enum state_enum state = state_start;
    struct token t;

    string_init(&s);
    r = defer(string_reset, &s, &ds);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }

    token_init(&t);
    r = defer(token_reset, &t, &ds);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }

    UErrorCode err;
    UConverter* conv = ucnv_open("utf8", &err);
    if (U_FAILURE(err)) {
        cleanup(ds);
        return set_error("utf error");
    }
    r = defer(ucnv_close, conv, &ds);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }

    UChar* dest;
    size_t dest_len;
    r = char2uchar(conv, line->buf, line->size, &dest, line->size, &dest_len);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }
    r = defer(free, dest, &ds);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }

    while (pos < dest_len) {
        UChar32 c2;
        size_t old_pos = pos;
        U16_NEXT(dest, pos, line->size, c2);
        size_t char_len = pos - old_pos;
        char* a;
        size_t len;
        r = uchar2char(conv, dest + old_pos, char_len, &a, 4, &len);
        if (r == result_error) {
            cleanup(ds);
            return r;
        }
        r = defer(free, a, &ds);
        if (r == result_error) {
            cleanup(ds);
            return r;
        }

        r = result_ok;
        if (state == state_start) {
            r = process_char_start(c2, a, len, &state, tl, &t);
        } else if (state == state_word) {
            r = process_char_word(c2, a, len, &state, tl, &t);
        } else if (state == state_number) {
            r = process_char_word(c2, a, len, &state, tl, &t);
        } else {
            cleanup(ds);
            r = set_error("unexpected state");
        }
        if (r == result_error) {
            cleanup(ds);
            return r;
        }
    }

    if (state != state_start && t.type != token_none) {
        state = state_start;
        r = token_list_add_defer(tl, &t);
        if (r == result_error) {
            cleanup(ds);
            return r;
        }
    }

    cleanup(ds);
    return result_ok;
}
