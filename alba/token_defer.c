#include "token.h"
#include "token_defer.h"
#include "memory.h"

enum result token_list_make_defer(struct token_list** tl)
{
    enum result r;
    r = malloc_safe(tl, sizeof(**tl));
    if (r == result_error) {
        return r;
    }
    token_list_init(*tl);
    return result_ok;
}

/*
* Append the new token to the end of the token list
*/
enum result token_list_add_defer(struct token_list* tl, struct token* t)
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
    r = string_copy(&t->value, &new_t->value);
    if (r == result_error) {
        cleanup(ds);
        return r;
    }
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

enum result token_list_slice_defer(struct token_list* tl, int start, int end, struct token_list** slice)
{
    struct defer_node* cleanup_on_error = NULL;

    enum result r = token_list_make_defer(slice);
    if (r == result_error) {
        return r;
    }
    r = defer(token_list_destroy_defer, slice, &cleanup_on_error);
    if (r == result_error) {
        token_list_destroy_defer(*slice);
        return r;
    }
    int i = 0;
    for (struct token_node* tn = tl->head; tn; tn = tn->next) {
        if (i >= start || start == -1) {
            if (i <= end || end == -1) {
                r = token_list_add_defer(*slice, tn->t);
                if (r == result_error) {
                    cleanup(cleanup_on_error);
                    return r;
                }
            }
        }
        i++;
    }
    cleanup_stack(cleanup_on_error);
    return result_ok;
}

void token_destroy_defer(struct token* t)
{
    token_reset(t);
    free(t);
}

struct token* token_list_pop_defer(struct token_list* tl)
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
        free(tn);
        return t;
    }
    return NULL;
}

/*
* frees memory of token value, token, and token node in token list
*/
void token_list_reset_defer(struct token_list* tl)
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

void token_list_destroy_defer(struct token_list* tl)
{
    token_list_reset_defer(tl);
    free(tl);
}

enum result token_list_print_defer(struct token_list* tl, char** token_name)
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
