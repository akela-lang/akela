#define _TOKEN_C

#include <stdlib.h>
#include "zinc/memory.h"
#include "token.h"

/* dynamic-output-none */
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
    malloc_safe(tl, sizeof(**tl));
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
    buffer2array(&t->value, &a);
    printf("%zu, %zu: token: %s, value: %s\n", t->line, t->col, token_name[t->type], a);

    /* destroy a */
    free(a);

    return r;
}

/* dynamic-output-none */
unsigned int hash_buffer(struct buffer* bf, unsigned int size)
{
    unsigned int val = 0;

    for (int i = 0; i < bf->size; i++) {
        val += (unsigned int)bf->buf[i];
        val %= size;
    }

    return val;
}

/* dynamic-output-none */
/* initialize-output w{} w{value{}} */
void word_init(struct word* w)
{
    w->type = token_none;
    /* init w{value{}} */
    buffer_init(&w->value);
    w->next = NULL;
    w->prev = NULL;
}

/* dynamic-destroy w{} */
void word_destroy(struct word* w)
{
    buffer_destroy(&w->value);
}

/* dynamic-output-none */
/* initialize-output wl{} */
void word_list_init(struct word_list* wl)
{
    wl->head = NULL;
    wl->tail = NULL;
}

void word_list_destroy(struct word_list* wl)
{
    struct word* p = wl->head;
    while (p) {
        struct word* temp = p;
        p = p->next;
        word_destroy(temp);
        free(temp);
    }
}

/* initialize word table */
/* dynamic-output wt{} */
void word_table_init(struct word_table* wt, unsigned int size)
{
    wt->size = size;

    /* allocate wt{} */
    malloc_safe(&wt->buckets, sizeof(struct word_list) * size);

    for (int i = 0; i < size; i++) {
        /* initialize wt{} */
        word_list_init(&wt->buckets[i]);
    }

    /* allocate wt{} */
    word_table_init_reserved(wt);
}

/* dynamic-destroy wt{} */
void word_table_destroy(struct word_table* wt)
{
    for (int i = 0; i < wt->size; i++) {
        word_list_destroy(&wt->buckets[i]);
    }
    free(wt->buckets);
}

/* add word to word table */
/* assume word is not in word table so call word_table_get before if not sure */
/* dynamic-output wt{} */
void word_table_add(struct word_table* wt, struct buffer* bf, enum token_enum type)
{
    unsigned int val = hash_buffer(bf, wt->size);

    /* allocate w */
    struct word* w;
    malloc_safe(&w, sizeof(struct word));
    word_init(w);
    w->type = type;

    /* allocate w{} */
    buffer_copy(bf, &w->value);

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
}

/* get token based on word */
/* return token if found */
/* otherwise, return NULL */
/* dynamic-output-none */
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
void word_table_add_reserved(struct word_table* wt, char* name, enum dag_type type)
{
    struct buffer bf;

    /* initialize bf{} */
    buffer_init(&bf);
    char* p = name;
    while (*p) {

        /* allocate bf{} */
        buffer_add_char(&bf, *p);
        p++;
    }

    /* allocate wt{} */
    word_table_add(wt, &bf, type);

    /* destroy bf{} */
    buffer_destroy(&bf);
}

/* dynamic-output wt{} */
void word_table_init_reserved(struct word_table* wt)
{
    /* allocate wt{} */
    word_table_add_reserved(wt, "function", token_function);
    word_table_add_reserved(wt, "end", token_end);
    word_table_add_reserved(wt, "if", token_if);
    word_table_add_reserved(wt, "elseif", token_elseif);
    word_table_add_reserved(wt, "else", token_else);
    word_table_add_reserved(wt, "while", token_while);
    word_table_add_reserved(wt, "for", token_for);
    word_table_add_reserved(wt, "in", token_in);
}
