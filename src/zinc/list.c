#include "list.h"
#include <stdlib.h>
#include "memory.h"

void Zinc_list_node_init(Zinc_list_node* ln)
{
    ln->item = NULL;
    ln->next = NULL;
    ln->prev = NULL;
}

void Zinc_list_init(Zinc_list* l)
{
    l->head = NULL;
    l->tail = NULL;
}

void Zinc_list_create(Zinc_list** l)
{
    Zinc_malloc_safe((void**)l, sizeof(Zinc_list));
    Zinc_list_init(*l);
}

void Zinc_list_add(Zinc_list* l, Zinc_list_node* ln)
{
    if (l->head && l->tail) {
        ln->prev = l->tail->next;
        l->tail->next = ln;
        l->tail = ln;
    } else {
        l->head = l->tail = ln;
    }
}

void Zinc_list_add_item(Zinc_list* l, void* item)
{
    Zinc_list_node* ln = NULL;
    Zinc_malloc_safe((void**)&ln, sizeof(Zinc_list_node));
    Zinc_list_node_init(ln);
    ln->item = item;
    Zinc_list_add(l, ln);
}

bool Zinc_list_has_item(Zinc_list* l, void* item)
{
    Zinc_list_node* ln = l->head;
    while (ln) {
        if (ln->item == item) {
            return true;
        }
        ln = ln->next;
    }

    return false;
}

Zinc_list_node* list_remove_first(Zinc_list* l)
{
    if (l->head && l->tail) {
        /* has a node */
        if (l->head == l->tail) {
            /* one node in list */
            struct Zinc_list_node* n = l->head;
            l->head = NULL;
            l->tail = NULL;
            return n;
        } else {
            /* more than one node in list */
            struct Zinc_list_node* n = l->head;
            l->head = n->next;
            n->next->prev = NULL;
            n->next = NULL;
            n->prev = NULL;
            return n;
        }
    }
    return NULL;
}

void* list_remove_first_item(Zinc_list* l)
{
    Zinc_list_node* n = list_remove_first(l);
    if (n) {
        void* item = n->item;
        free(n);
        return item;
    } else {
        return NULL;
    }
}

void Zinc_list_destroy(Zinc_list* l, Zinc_list_node_destroy lnd)
{
    Zinc_list_node* ln = l->head;
    while (ln) {
        Zinc_list_node* temp = ln;
        ln = ln->next;
        if (lnd) {
            lnd(temp->item);
        }
        free(temp);
    }
}

void* Zinc_list_get(Zinc_list* l, int index)
{
    int i = 0;
    Zinc_list_node* ln = l->head;
    while (ln) {
        if (i == index) {
            return ln->item;
        }
        i++;
        ln = ln->next;
    }
    return NULL;
}