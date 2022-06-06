#define _TOKEN_C

#include <stdlib.h>
#include "memory.h"
#include "token.h"

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
