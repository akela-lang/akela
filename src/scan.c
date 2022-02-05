#define _SCAN_C

#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include "result.h"
#include "scan.h"
#include "ustring.h"
#include "defer.h"
#include "memory.h"

void token_init(struct token* t)
{
    t->type = token_none;
    string_init(&t->value);
}

void token_reset(struct token* t)
{
    t->type = token_none;
    string_reset(&t->value);
}

void token_list_init(struct token_list* tl)
{
    tl->head = NULL;
    tl->tail = NULL;
}

/*
* Append the new token to the end of the token list
*/
enum result token_list_add(struct token_list* tl, struct token* t)
{
    enum result r;
    struct defer_node* ds = NULL;

    struct token* new_t;
    r = malloc_safe(&new_t, sizeof(struct token));
    if (r == result_error) {
        cleanup(ds);
        return r;
    }
    r = defer(free, new_t, &ds);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }

    struct token_node* tn;
    r = malloc_safe(&tn, sizeof(struct token_node));
    if (r == result_error) {
        cleanup(ds);
        return r;
    }
    r = defer(free, tn, &ds);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }

    new_t->type = t->type;
    string_init(&new_t->value);
    string_copy(&t->value, &new_t->value);
    tn->t = new_t;

    /* update previous */
    struct token_node* tn_prev = tl->tail;
    if (tn_prev != NULL) {
        tn_prev->next = tn;
    }

    /* update new node */
    tn->next = NULL;
    tn->prev = tn_prev;

    /* update list */
    if (tl->head == NULL) {
        tl->head = tn;
    }
    tl->tail = tn;

    cleanup_stack(ds);
    return result_ok;
}

/*
* frees memory of token value, token, and token node in token list
*/
void token_list_reset(struct token_list* tl)
{
    struct token_node* tn = tl->head;
    while (tn) {
        token_reset(tn->t);
        struct token_node* temp = tn;
        tn = tn->next;
        free(temp);
    }
    token_list_init(tl);
}

enum result token_list_print(struct token_list* tl, char** token_name)
{
    enum result r;
    struct token_node* tn = tl->head;
    while (tn) {
        char* a;
        r = string2array(&tn->t->value, &a);
        if (r == result_error) {
            return r;
        }
        printf("token: %s, value: %s\n", token_name[tn->t->type], a);
        free(a);
        tn = tn->next;
    }
    return result_ok;
}

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
        token_list_add(tl, t);
        token_reset(t);
    } else if (c2 == cv.minus) {
        t->type = token_minus;
        token_list_add(tl, t);
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
        r = token_list_add(tl, t);
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
        r = token_list_add(tl, t);
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
        r = token_list_add(tl, &t);
        if (r == result_error) {
            cleanup(ds);
            return r;
        }
    }

    cleanup(ds);
    return result_ok;
}

struct token* get_token(struct token_node* head, size_t pos)
{
    int i = 0;
    for (struct token_node* tn = head; tn; tn = tn->next) {
        if (i == pos) {
            return tn->t;
        }
        i++;
    }
    return NULL;
}
