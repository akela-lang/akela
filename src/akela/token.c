#define AKELA_TOKEN_C

#include <stdlib.h>
#include "zinc/memory.h"
#include "token.h"

void Ake_token_init(struct Ake_token* t)
{
    t->type = Ake_token_none;
    Zinc_string_init(&t->value);
    t->is_integer = false;
    t->is_float = false;
    Zinc_location_init(&t->loc);
    t->next = NULL;
    t->prev = NULL;
}

void Ake_token_destroy(struct Ake_token* t)
{
    /* destroy t{value{}} */
    if (t) {
        Zinc_string_destroy(&t->value);
    }
}

void Ake_token_reset(struct Ake_token* t)
{
    t->type = Ake_token_none;
    /* destroy t{value{}} */
    Zinc_string_reset(&t->value);
}

void Ake_token_list_init(struct Ake_token_list* tl)
{
    tl->head = NULL;
    tl->tail = NULL;
}

int Ake_token_find_first(struct Ake_token_list* tl, enum Ake_token_enum type)
{
    int i = 0;
    for (struct Ake_token* t = tl->head; t; t = t->next) {
        if (t->type == type) {
            return i;
        }
        i++;
    }
    return -1;
}

int Ake_token_find_last(struct Ake_token_list* tl, enum Ake_token_enum type)
{
    int count = Ake_token_list_count(tl);
    int i = 0;
    for (struct Ake_token* t = tl->tail; t; t = t->prev) {
        if (t->type == type) {
            return count - 1 - i;
        }
        i++;
    }
    return -1;
}

int Ake_token_list_count(struct Ake_token_list* tl)
{
    int i = 0;
    for (struct Ake_token* t = tl->head; t; t = t->next) {
        i++;
    }
    return i;
}

void Ake_token_list_make(struct Ake_token_list** tl)
{
    malloc_safe((void**)tl, sizeof(**tl));
    Ake_token_list_init(*tl);
}

void Ake_token_list_destroy(struct Ake_token_list* tl)
{
    struct Ake_token* t = tl->head;
    while (t) {
        struct Ake_token* temp = t;
        t = t->next;
        Ake_token_destroy(temp);
        free(temp);
    }
}

/* Append the new token to the end of the token list */
void Ake_token_list_add(struct Ake_token_list* tl, struct Ake_token* t)
{
    /* update previous */
    struct Ake_token* t_prev = tl->tail;
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

struct Ake_token* Ake_token_list_pop(struct Ake_token_list* tl)
{
    struct Ake_token* t;
    struct Ake_token* first;
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
void Ake_token_list_reset(struct Ake_token_list* tl)
{
    Ake_token_list_init(tl);
}

void Ake_token_list_print(struct Ake_token_list* tl)
{
    struct Ake_token* t = tl->head;
    while (t) {
        char* a;

        Zinc_string_create_str(&t->value, &a);
        printf("%zu, %zu: token: %s, value: %s\n", t->loc.line, t->loc.col, Ake_token_name(t->type), a);

        free(a);

        t = t->next;
    }
}

void Ake_print_token(struct Ake_token* t)
{
    char* a;
    Zinc_string_create_str(&t->value, &a);
    printf("%zu, %zu: token: %s, value: %s\n", t->loc.line, t->loc.col, Ake_token_name(t->type), a);
    free(a);
}

void Ake_get_token_location(struct Ake_token* t, struct Zinc_location* loc)
{
    *loc = t->loc;
}
