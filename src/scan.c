#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include <stdlib.h>
#include "result.h"
#include "scan.h"
#include "ustring.h"
#include "defer.h"
#include "memory.h"

char* token_name[token_count];

enum result_enum token_name_init()
{
    for (int i = 0; i < token_count; i++) {
        token_name[i] = NULL;
    }

    token_name[token_none] = "none";
    token_name[token_number] = "number";
    token_name[token_word] = "word";
    token_name[token_equal] = "equal";
    token_name[token_plus] = "plus";
    token_name[token_minus] = "minus";
    token_name[token_mult] = "mult";
    token_name[token_divide] = "divide";
    token_name[token_left_paren] = "left parenthesis";
    token_name[token_right_paren] = "right parenthesis";

    for (int i = 0; i < token_count; i++) {
        if (token_name[i] == NULL) {
            return set_error("token name missing: %d\n", i);
        }
    }
    return ok_result;
}

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

enum result_enum token_list_add(struct token_list* tl, struct token* t)
{
    enum result_enum r;
    struct defer_node* ds = NULL;

    struct token* new_t;
    r = malloc_safe(&new_t, sizeof(struct token));
    if (r == error_result) {
        cleanup(ds);
        return r;
    }
    r = defer(free, new_t, &ds);
    if (r == error_result) {
        cleanup(ds);
        return r;
    }

    struct token_node* tn;
    r = malloc_safe(&tn, sizeof(struct token_node));
    if (r == error_result) {
        cleanup(ds);
        return r;
    }
    r = defer(free, tn, &ds);
    if (r == error_result) {
        cleanup(ds);
        return r;
    }

    new_t->type = t->type;
    string_init(&new_t->value);
    string_copy(&t->value, &new_t->value);
    tn->t = new_t;
    tn->next = NULL;
    tn->prev = tl->tail;
    tl->tail = tn;
    if (tl->head == NULL) {
        tl->head = tn;
    }

    return ok_result;
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

enum result_enum token_list_print(struct token_list* tl)
{
    enum result_enum r;
    struct token_node* tn = tl->head;
    while (tn) {
        char* a;
        r = string2array(&tn->t->value, &a);
        if (r == error_result) {
            return r;
        }
        printf("token: %s, value: %s\n", token_name[tn->t->type], a);
        free(a);
        tn = tn->next;
    }
    return ok_result;
}

enum result_enum process_char_start(UChar32 c2, char* a, size_t len, enum state_enum* state, struct token_list* tl, struct token* t)
{
    size_t pos2;
    size_t size;

    U_STRING_DECL(plus, "+", 1);
    U_STRING_INIT(plus, "+", 1);
    UChar32 plus_char;
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(plus, pos2, size, plus_char);

    U_STRING_DECL(space, " ", 1);
    U_STRING_INIT(space, " ", 1);
    UChar32 space_char;
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(space, pos2, size, space_char);

    U_STRING_DECL(minus, "-", 1);
    U_STRING_INIT(minus, "-", 1);
    UChar32 minus_char;
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(minus, pos2, size, minus_char);

    if (u_isalpha(c2)) {
        printf("found word\n");
        *state = state_word;
        t->type = token_word;
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else if (u_isdigit(c2)) {
        printf("found number\n");
    } else if (c2 == plus_char) {
        printf("found plus\n");
    } else if (c2 == minus_char) {
        printf("found minus\n");
    } else if (c2 == space_char) {
        printf("found space\n");
    } else {
        return set_error("unrecogized: %s", a);
    }
    return ok_result;
}

enum result_enum process_char_word(UChar32 c2, char* a, size_t len, enum state_enum* state, struct token_list* tl, struct token* t)
{
    enum result_enum r;
    size_t pos2;
    size_t size;
    
    U_STRING_DECL(plus, "+", 1);
    U_STRING_INIT(plus, "+", 1);
    UChar32 plus_char;
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(plus, pos2, size, plus_char);

    U_STRING_DECL(space, " ", 1);
    U_STRING_INIT(space, " ", 1);
    UChar32 space_char;
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(space, pos2, size, space_char);

    U_STRING_DECL(minus, "-", 1);
    U_STRING_INIT(minus, "-", 1);
    UChar32 minus_char;
    pos2 = 0;
    size = u_strlen(plus);
    U16_NEXT(minus, pos2, size, minus_char);

    if (u_isalpha(c2)) {
        printf("add to word\n");
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else if (u_isdigit(c2)) {
        printf("add to word\n");
        for (int i = 0; i < len; i++) {
            string_add_char(&t->value, a[i]);
        }
    } else if (c2 == space_char) {
        *state = state_start;
        r = token_list_add(tl, t);
        if (r == error_result) {
            return r;
        }
        return process_char_start(c2, a, len, state, tl, t);
    } else if (c2 == plus_char) {
        *state = state_start;
        r = token_list_add(tl, t);
        if (r == error_result) {
            return r;
        }
        return process_char_start(c2, a, len, state, tl, t);
    } else if (c2 == minus_char) {
        *state = state_start;
        r = token_list_add(tl, t);
        if (r == error_result) {
            return r;
        }
        return process_char_start(c2, a, len, state, tl, t);
    } else {
        return set_error("unexpected token in word: %s", a);
    }
    return ok_result;
}

enum result_enum scan(struct string* line, struct token_list* tl)
{
    enum result_enum r;
    size_t pos = 0;
    struct string s;
    struct defer_node* ds = NULL;
    enum state_enum state = state_start;
    struct token t;

    string_init(&s);
    r = defer(string_reset, &s, &ds);
    if (r == error_result) {
        cleanup(ds);
        return r;
    }

    token_init(&t);
    r = defer(token_reset, &t, &ds);
    if (r == error_result) {
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
    if (r == error_result) {
        cleanup(ds);
        return r;
    }

    UChar* dest;
    size_t dest_len;
    r = char2uchar(conv, line->buf, line->size, &dest, line->size, &dest_len);
    if (r == error_result) {
        cleanup(ds);
        return r;
    }
    r = defer(free, dest, &ds);
    if (r == error_result) {
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
        if (r == error_result) {
            cleanup(ds);
            return r;
        }
        r = defer(free, a, &ds);
        if (r == error_result) {
            cleanup(ds);
            return r;
        }

        r = ok_result;
        if (state == state_start) {
            r = process_char_start(c2, a, len, &state, tl, &t);
        } else if (state == state_word) {
            r = process_char_word(c2, a, len, &state, tl, &t);
        } else {
            cleanup(ds);
            r = set_error("unexpected state");
        }
        if (r == error_result) {
            cleanup(ds);
            return r;
        }
    }

    cleanup(ds);
    return ok_result;
}
