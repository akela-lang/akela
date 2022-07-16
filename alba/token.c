#define _TOKEN_C

#include <stdlib.h>
#include "zinc/memory.h"
#include "token.h"

/* static-output */
/* initialize-output t{} t{value{}} */
void token_init(struct token* t)
{
    t->type = token_none;
    t->line = 0;
    t->col = 0;
    t->byte_pos = 0;
    buffer_init(&t->value);
    t->next = NULL;
    t->prev = NULL;
}

/* dynamic-destroy t{value{}} */
void token_destroy(struct token* t)
{
    /* destroy t{value{}} */
    buffer_destroy(&t->value);
}

/* dynamic-destroy t{value{}} */
void token_reset(struct token* t)
{
    t->type = token_none;
    /* destroy t{value{}} */
    buffer_reset(&t->value);
}

/* static-output */
/* initialize-output tl{} */
void token_list_init(struct token_list* tl)
{
    tl->head = NULL;
    tl->tail = NULL;
}

/* static-output */
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

/* static-output */
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

/* static-output */
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

/* static-output */
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
    malloc_safe(tl, sizeof(**tl));
    token_list_init(*tl);
}

/* Append the new token to the end of the token list */
/* static-output */
/* dynamic-transfer t t{} -> tl{} */
enum result token_list_add(struct token_list* tl, struct token* t)
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

/* static-output */
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
        r = buffer2array(&t->value, &a);
        if (r == result_error) {
            return r;
        }
        printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col,token_name[t->type], a);

        /* dynamic-destroy */
        free(a);

        t = t->next;
    }
    return result_ok;
}

/* static-output */
/* dynamic-temp a */
enum result print_token(struct allocator* al, struct token* t)
{
    enum result r;

    char* token_name[token_count];
    r = token_name_init(token_name);
    if (r == result_error) {
        return r;
    }

    char* a;

    /* allocate a */
    r = buffer2array(&t->value, &a);
    if (r == result_error) {
        return r;
    }
    printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col, token_name[t->type], a);

    /* destroy a */
    free(a);

    return r;
}

/* static-output */
unsigned int hash_buffer(struct buffer* bf, unsigned int size)
{
    unsigned int val = 0;

    for (int i = 0; i < bf->size; i++) {
        val += (unsigned int)bf->buf[i];
        val %= size;
    }

    return val;
}

/* static-output */
/* initialize-output w{} w{value{}} */
void word_init(struct word* w)
{
    w->type = token_none;
    /* init w{value{}} */
    buffer_init(&w->value);
    w->next = NULL;
    w->prev = NULL;
}

/* static-output */
/* initialize-output wl{} */
void word_list_init(struct word_list* wl)
{
    wl->head = NULL;
    wl->tail = NULL;
}

/* initialize word table */
/* dynamic-output wt{} */
enum result word_table_init(struct word_table* wt, unsigned int size)
{
    enum result r;
    wt->size = size;

    /* allocate wt{} */
    malloc_safe(&wt->buckets, sizeof(struct word_list) * size);

    for (int i = 0; i < size; i++) {
        /* initialize wt{} */
        word_list_init(&wt->buckets[i]);
    }

    /* allocate wt{} */
    r = word_table_init_reserved(wt);
    if (r == result_error) {
        return r;
    }

    return result_ok;
}

/* add word to word table */
/* assume word is not in word table so call word_table_get before if not sure */
/* dynamic-output wt{} */
enum result word_table_add(struct word_table* wt, struct buffer* bf, enum token_enum type)
{
    enum result r;
    unsigned int val = hash_buffer(bf, wt->size);

    /* allocate w */
    struct word* w;
    malloc_safe(&w, sizeof(struct word));
    word_init(w);
    w->type = type;

    /* allocate w{} */
    r = buffer_copy(bf, &w->value);
    if (r == result_error) return r;

    /* add to beginning of bucket */
    /* allocate w w{} -> wt{} */
    struct word* next = wt->buckets[val].head;
    w->next = next;
    if (next) {
        next->prev = w;
    }
    wt->buckets[val].head = w;
    if (!wt->buckets[val].tail) {
        wt->buckets[val].tail = w;
    }

    return result_ok;
}

/* get token based on word */
/* return token if found */
/* otherwise, return NULL */
/* static-output */
struct word* word_table_get(struct word_table* wt, struct buffer* bf)
{
    struct word* w;

    unsigned int val = hash_buffer(bf, wt->size);
    
    w = wt->buckets[val].head;
    while (w) {
        if (buffer_compare(&w->value, bf)) {
            return w;
        }
        w = w->next;
    }

    return NULL;
}

/* dynamic-output wt{} */
/* dynamic-temp bf{} */
enum result word_table_add_reserved(struct word_table* wt, char* name, enum dag_type type)
{
    enum result r;
    struct buffer bf;

    /* initialize bf{} */
    buffer_init(&bf);
    char* p = name;
    while (*p) {

        /* allocate bf{} */
        r = buffer_add_char(&bf, *p);
        if (r == result_error) return r;
        p++;
    }

    /* allocate wt{} */
    r = word_table_add(wt, &bf, type);
    if (r == result_error) return r;

    /* destroy bf{} */
    buffer_destroy(&bf);

    return result_ok;
}

/* dynamic-output wt{} */
enum result word_table_init_reserved(struct word_table* wt)
{
    enum result r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "function", token_function);
    if (r == result_error) return r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "end", token_end);
    if (r == result_error) return r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "if", token_if);
    if (r == result_error) return r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "elseif", token_elseif);
    if (r == result_error) return r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "else", token_else);
    if (r == result_error) return r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "while", token_while);
    if (r == result_error) return r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "for", token_for);
    if (r == result_error) return r;

    /* allocate wt{} */
    r = word_table_add_reserved(wt, "in", token_in);
    if (r == result_error) return r;

    return result_ok;
}
