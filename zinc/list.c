#include "list.h"
#include <stdlib.h>
#include "memory.h"

void list_node_init(struct list_node* ln)
{
    ln->item = NULL;
    ln->next = NULL;
    ln->prev = NULL;
}

void list_init(struct list* l)
{
    l->head = NULL;
    l->tail = NULL;
}

void list_create(struct list** l)
{
    malloc_safe((void**)l, sizeof(struct list));
    list_init(*l);
}

void list_add(struct list* l, struct list_node* ln)
{
    if (l->head && l->tail) {
        ln->prev = l->tail->next;
        l->tail->next = ln;
        l->tail = ln;
    } else {
        l->head = l->tail = ln;
    }
}

void list_add_item(struct list* l, void* item)
{
    struct list_node* ln = NULL;
    malloc_safe((void**)&ln, sizeof(struct list_node));
    list_node_init(ln);
    ln->item = item;
    list_add(l, ln);
}

bool list_has_item(struct list* l, void* item)
{
    struct list_node* ln = l->head;
    while (ln) {
        if (ln->item == item) {
            return true;
        }
        ln = ln->next;
    }

    return false;
}

struct list_node* list_remove_first(struct list* l)
{
    if (l->head && l->tail) {
        /* has a node */
        if (l->head == l->tail) {
            /* one node in list */
            struct list_node* n = l->head;
            l->head = NULL;
            l->tail = NULL;
            return n;
        } else {
            /* more than one node in list */
            struct list_node* n = l->head;
            l->head = n->next;
            n->next->prev = NULL;
            n->next = NULL;
            n->prev = NULL;
            return n;
        }
    }
    return NULL;
}

void* list_remove_first_item(struct list* l)
{
    struct list_node* n = list_remove_first(l);
    if (n) {
        void* item = n->item;
        free(n);
        return item;
    } else {
        return NULL;
    }
}

void list_destroy(struct list* l, list_node_destroy lnd)
{
    struct list_node* ln = l->head;
    while (ln) {
        struct list_node* temp = ln;
        ln = ln->next;
        if (lnd) {
            lnd(temp->item);
        }
        free(temp);
    }
}

void* list_get(struct list* l, int index)
{
    int i = 0;
    struct list_node* ln = l->head;
    while (ln) {
        if (i == index) {
            return ln->item;
        }
        i++;
        ln = ln->next;
    }
    return NULL;
}