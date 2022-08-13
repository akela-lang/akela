#define _TOKEN_C

#include <stdlib.h>
#include "zinc/memory.h"
#include "token.h"
#include "dag.h"

/* dynamic-output-none */
/* initialize-output t{} t{value{}} */
void token_init(struct token* t)
{
    t->type = token_none;
    buffer_init(&t->value);
    t->is_integer = false;
    t->is_float = false;
    t->line = 0;
    t->col = 0;
    t->byte_pos = 0;
    t->next = NULL;
    t->prev = NULL;
}

/* dynamic-destroy t{value{}} */
void token_destroy(struct token* t)
{
    /* destroy t{value{}} */
    if (t) {
        buffer_destroy(&t->value);
    }
}

/* dynamic-destroy t{value{}} */
void token_reset(struct token* t)
{
    t->type = token_none;
    /* destroy t{value{}} */
    buffer_reset(&t->value);
}

/* dynamic-output-none */
/* initialize-output tl{} */
void token_list_init(struct token_list* tl)
{
    tl->head = NULL;
    tl->tail = NULL;
}

/* dynamic-output-none */
struct token* get_token(struct token_list* tl, size_t pos)
{
    int i = 0;
    for (struct token* t = tl->head; t; t = t->next) {
        if (i == pos) {
            return t;
        }
        i++;
    }
    return NULL;
}

/* dynamic-output-none */
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

/* dynamic-output-none */
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

/* dynamic-output-none */
int token_list_count(struct token_list* tl)
{
    int i = 0;
    for (struct token* t = tl->head; t; t = t->next) {
        i++;
    }
    return i;
}

/* dynamic-output tl */
void token_list_make(struct token_list** tl)
{
    /* allocate tl */
    malloc_safe((void**)tl, sizeof(**tl));
    token_list_init(*tl);
}

/* dynamic-destroy tl{} */
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
/* dynamic-output-none */
/* dynamic-transfer t t{} -> tl{} */
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

/* partition-output tl{} -> return */
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
/* initialize-output tl{} */
void token_list_reset(struct token_list* tl)
{
    token_list_init(tl);
}

/* dynamic-output-none */
/* dynamic-temp a */
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

        /* allocate a */
        buffer2array(&t->value, &a);
        printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col,token_name[t->type], a);

        /* dynamic-destroy */
        free(a);

        t = t->next;
    }
    return result_ok;
}

/* dynamic-output-none */
/* dynamic-temp a */
enum result print_token(struct token* t)
{
    enum result r;

    char* token_name[token_count];
    r = token_name_init(token_name);
    if (r == result_error) {
        return r;
    }

    char* a;

    /* allocate a */
    buffer2array(&t->value, &a);
    printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col, token_name[t->type], a);

    /* destroy a */
    free(a);

    return r;
}
