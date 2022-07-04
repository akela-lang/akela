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
    t->next = NULL;
    t->prev = NULL;
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
    for (struct token* t = tl->head; t; t = t->next) {
        if (i == pos) {
            return t;
        }
        i++;
    }
    return NULL;
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

    return result_ok;
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

        t->next = NULL;
        t->prev = NULL;
    }
    return t;
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
    r = token_name_init(token_name);
    if (r == result_error) {
        return r;
    }

    struct token* t = tl->head;
    while (t) {
        char* a;
        r = buffer2array(al, &t->value, &a);
        if (r == result_error) {
            return r;
        }
        printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col,token_name[t->type], a);
        t = t->next;
    }
    return result_ok;
}

enum result print_token(struct allocator* al, struct token* t)
{
    enum result r;

    char* token_name[token_count];
    r = token_name_init(token_name);
    if (r == result_error) {
        return r;
    }

    char* a;
    r = buffer2array(al, &t->value, &a);
    if (r == result_error) {
        return r;
    }
    printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col, token_name[t->type], a);

    return r;
}

unsigned int hash_buffer(struct buffer* bf, unsigned int size)
{
    unsigned int val = 0;

    for (int i = 0; i < bf->size; i++) {
        val += (unsigned int)bf->buf[i];
        val %= size;
    }

    return val;
}

/*
* initialize word table
*/
enum result word_table_init(struct allocator* al, struct word_table* wt, unsigned int size)
{
    enum result r;
    wt->size = size;
    r = allocator_malloc(al, &wt->buckets, sizeof(struct token_list) * size);
    if (r == result_error) {
        return r;
    }

    for (int i = 0; i < size; i++) {
        token_list_init(&wt->buckets[i]);
    }

    return result_ok;
}

/*
* add token to word table
* assume word is not in word table so call word_table_get before if not sure
*/
void word_table_add(struct word_table* wt, struct token* t)
{
    unsigned int val = hash_buffer(&t->value, wt->size);

    /* add to beginning of bucket */
    struct token* next = wt->buckets[val].head;
    t->next = next;
    if (next) {
        next->prev = t;
    }
    wt->buckets[val].head = t;
    if (!wt->buckets[val].tail) {
        wt->buckets[val].tail = t;
    }
}

/*
* get token based on word
* return token if found
* otherwise, return NULL
*/
struct token* word_table_get(struct word_table* wt, struct buffer* bf)
{
    struct token* t;

    unsigned int val = hash_buffer(bf, wt->size);
    
    t = wt->buckets[val].head;
    while (t) {
        if (buffer_compare(&t->value, bf)) {
            return t;
        }
        t = t->next;
    }

    return NULL;
}
