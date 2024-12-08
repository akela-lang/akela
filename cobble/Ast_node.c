#include "Ast_node.h"
#include <stdio.h>
#include <stdlib.h>
#include "zinc/memory.h"
#include "zinc/buffer.h"

void Ast_node_set_group(Ast_node* n);

void Ast_node_init(Ast_node* n)
{
    n->type = Ast_type_none;
    n->num = 0;
    location_init(&n->loc);
    n->is_root = false;
    n->is_group = false;
    n->group = 0;
    n->next = NULL;
    n->prev = NULL;
    n->head = NULL;
    n->tail = NULL;
    n->parent = NULL;
}

void Ast_node_create(Ast_node** n)
{
    malloc_safe((void**)n, sizeof(Ast_node));
    Ast_node_init(*n);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_destroy(Ast_node* n)
{
    if (n) {
        Ast_node* p = n->head;
        while (p) {
            Ast_node* temp = p;
            p = p->next;
            Ast_node_destroy(temp);
        }
        free(n);
    }
}

void Ast_node_add(Ast_node* p, Ast_node* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
    c->parent = p;
    location_combine(&p->loc, &c->loc);

    if (c->type == Ast_type_error) {
        p->type = Ast_type_error;
    }

    Ast_node_set_group(c);
}

Ast_node* Ast_node_get(Ast_node* n, int index)
{
    int count = 0;
    Ast_node* p = n->head;
    while (p) {
        if (count == index) {
            return p;
        }
        count++;
        p = p->next;
    }

    return NULL;
}

void Ast_node_set_root(Ast_node* n)
{
    n->is_root = true;
    if (n->type == Ast_type_group) {
        n->is_group = true;
        n->group = 1;
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_set_group(Ast_node* n)
{
    if (n->type == Ast_type_group) {
        n->is_group = true;
        Ast_node* p = n->parent;
        while (p) {
            if (p->is_group) {
                n->group = p->group + 1;
                break;
            }

            if (p->parent == NULL && p->is_root == false) {
                Ast_node_set_root(p);
                n->group = p->group + 1;
            }

            p = p->parent;
        }
    }
}