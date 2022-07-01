#define _TOKEN_C

#include <stdlib.h>
#include "memory.h"
#include "token.h"

void token_init(struct token* t)
{
    t->type = token_none;
    t->line = 0;
    t->col = 0;
    buffer_init(&t->value);
}

void token_reset(struct token* t)
{
    t->type = token_none;
    buffer_reset(&t->value);
}

void token_list_init(struct token_list* tl)
{
    tl->head = NULL;
    tl->tail = NULL;
}

struct token* get_token(struct token_list* tl, size_t pos)
{
    int i = 0;
    for (struct token_node* tn = tl->head; tn; tn = tn->next) {
        if (i == pos) {
            return tn->t;
        }
        i++;
    }
    return NULL;
}

int token_find_first(struct token_list* tl, enum token_enum type)
{
    int i = 0;
    for (struct token_node* tn = tl->head; tn; tn = tn->next) {
        if (tn->t->type == type) {
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
    for (struct token_node* tn = tl->tail; tn; tn = tn->prev) {
        if (tn->t->type == type) {
            return count - 1 - i;
        }
        i++;
    }
    return -1;
}

int token_list_count(struct token_list* tl)
{
    int i = 0;
    for (struct token_node* tn = tl->head; tn; tn = tn->next) {
        i++;
    }
    return i;
}

enum result token_list_make(struct allocator *al, struct token_list** tl)
{
    enum result r;
    r = allocator_malloc(al, tl, sizeof(**tl));
    if (r == result_error) {
        return r;
    }
    token_list_init(*tl);
    return result_ok;
}

/*
* Append the new token to the end of the token list
*/
enum result token_list_add(struct allocator* al, struct token_list* tl, struct token* t)
{
    enum result r;
    struct defer_node* ds = NULL;

    struct token_node* tn;
    r = allocator_malloc(al, &tn, sizeof(struct token_node));
    if (r == result_error) {
        return r;
    }

    tn->t = t;

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

    return result_ok;
}

enum result token_list_slice(struct allocator *al, struct token_list* tl, int start, int end, struct token_list** slice)
{
    enum result r = token_list_make(al, slice);
    if (r == result_error) {
        return r;
    }
    int i = 0;
    for (struct token_node* tn = tl->head; tn; tn = tn->next) {
        if (i >= start || start == -1) {
            if (i <= end || end == -1) {
                r = token_list_add(al, *slice, tn->t);
                if (r == result_error) {
                    return r;
                }
            }
        }
        i++;
    }
    return result_ok;
}

struct token* token_list_pop(struct token_list* tl)
{
    struct token_node* tn;
    struct token_node* first;
    struct token* t = NULL;
    tn = tl->head;
    if (tn) {
        first = tn->next;
        if (first) {
            first->prev = NULL;
        }
        tl->head = first;
    }
    if (tn) {
        t = tn->t;
        return t;
    }
    return NULL;
}

/*
* frees memory of token value, token, and token node in token list
*/
void token_list_reset(struct token_list* tl)
{
    token_list_init(tl);
}

enum result token_list_print(struct allocator* al, struct token_list* tl)
{
    enum result r;

    char* token_name[token_count];
    r = token_name_init(&token_name);
    if (r == result_error) {
        return r;
    }

    struct token_node* tn = tl->head;
    while (tn) {
        char* a;
        r = buffer2array(al, &tn->t->value, &a);
        if (r == result_error) {
            return r;
        }
        printf("%zu, %zu: token: %s, value: %s\n", tn->t->line, tn->t->col,token_name[tn->t->type], a);
        tn = tn->next;
    }
    return result_ok;
}

enum result print_token(struct allocator* al, struct token* t)
{
    enum result r;

    char* token_name[token_count];
    r = token_name_init(&token_name);
    if (r == result_error) {
        return r;
    }

    char* a;
    r = buffer2array(al, &t->value, &a);
    if (r == result_error) {
        return r;
    }
    printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col, token_name[t->type], a);
}