#define _TOKEN_C

#include <stdlib.h>
#include "zinc/memory.h"
#include "token.h"

void token_init(struct token* t)
{
    t->type = token_none;
    buffer_init(&t->value);
    t->is_integer = false;
    t->is_float = false;
    location_init(&t->loc);
    t->next = NULL;
    t->prev = NULL;
}

void token_destroy(struct token* t)
{
    /* destroy t{value{}} */
    if (t) {
        buffer_destroy(&t->value);
    }
}

void token_reset(struct token* t)
{
    t->type = token_none;
    /* destroy t{value{}} */
    buffer_reset(&t->value);
}

void token_list_init(struct token_list* tl)
{
    tl->head = NULL;
    tl->tail = NULL;
}

int token_find_first(struct token_list* tl, enum token_enum type)
{
    int i = 0;
    for (struct token* t = tl->head; t; t = t->next) {
        if (t->type == type) {
            return i;
        }
        i++;
    }
    return -1;
}

int token_find_last(struct token_list* tl, enum token_enum type)
{
    int count = token_list_count(tl);
    int i = 0;
    for (struct token* t = tl->tail; t; t = t->prev) {
        if (t->type == type) {
            return count - 1 - i;
        }
        i++;
    }
    return -1;
}

int token_list_count(struct token_list* tl)
{
    int i = 0;
    for (struct token* t = tl->head; t; t = t->next) {
        i++;
    }
    return i;
}

void token_list_make(struct token_list** tl)
{
    malloc_safe((void**)tl, sizeof(**tl));
    token_list_init(*tl);
}

void token_list_destroy(struct token_list* tl)
{
    struct token* t = tl->head;
    while (t) {
        struct token* temp = t;
        t = t->next;
        token_destroy(temp);
        free(temp);
    }
}

/* Append the new token to the end of the token list */
void token_list_add(struct token_list* tl, struct token* t)
{
    /* update previous */
    struct token* t_prev = tl->tail;
    if (t_prev != NULL) {
        t_prev->next = t;
    }

    /* update new node */
    t->next = NULL;
    t->prev = t_prev;

    /* update list */
    if (tl->head == NULL) {
        tl->head = t;
    }
    tl->tail = t;
}

struct token* token_list_pop(struct token_list* tl)
{
    struct token* t;
    struct token* first;
    t = tl->head;
    if (t) {
        first = t->next;
        if (first) {
            first->prev = NULL;
        }
        tl->head = first;
        if (!first) {
            tl->tail = first;
        }

        t->next = NULL;
        t->prev = NULL;
    }
    return t;
}

/* reset tl */
void token_list_reset(struct token_list* tl)
{
    token_list_init(tl);
}

enum result token_list_print(struct token_list* tl)
{
    enum result r;

    char* token_name[token_count];
    r = token_name_init(token_name);
    if (r == result_error) {
        return r;
    }

    struct token* t = tl->head;
    while (t) {
        char* a;

        buffer2array(&t->value, &a);
        printf("%zu, %zu: token: %s, value: %s\n", t->loc.line, t->loc.col, token_name[t->type], a);

        free(a);

        t = t->next;
    }
    return result_ok;
}

enum result print_token(struct token* t)
{
    enum result r;

    char* token_name[token_count];
    r = token_name_init(token_name);
    if (r == result_error) {
        return r;
    }

    char* a;

    buffer2array(&t->value, &a);
    printf("%zu, %zu: token: %s, value: %s\n", t->loc.line, t->loc.col, token_name[t->type], a);

    free(a);

    return r;
}

void get_token_location(struct token* t, struct location* loc)
{
    *loc = t->loc;
}
