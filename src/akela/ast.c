#define AKELA_AST_C

#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/memory.h"
#include "zinc/list.h"

void Ake_ast_create(Ake_ast** n)
{
	malloc_safe((void**)n, sizeof(Ake_ast));
	Ake_ast_init(*n);
}

void Ake_ast_init(Ake_ast* n)
{
	n->type = Ake_ast_type_none;
	buffer_init(&n->value);
	n->tu = NULL;
    n->is_mut = false;
    location_init(&n->loc);
    n->sym = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_ast_destroy(Ake_ast* n)
{
    if (n) {
        Ake_ast* p = n->head;
        while (p) {
            Ake_ast* temp = p;
            p = p->next;
            Ake_ast_destroy(temp);
        }

        buffer_destroy(&n->value);
        Type_use_destroy(n->tu);
        free(n);
    }
}

void Ake_ast_add(Ake_ast* p, Ake_ast* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
    location_combine(&p->loc, &c->loc);
}

/* assume parent and child are not NULL */
void Ast_node_push(Ake_ast* parent, Ake_ast* child)
{
	Ake_ast* old_head = parent->head;
	if (old_head) {
		old_head->prev = child;
	}
	child->next = old_head;
	parent->head = child;
}

Ake_ast* Ast_node_get(Ake_ast* p, size_t pos)
{
	int i = 0;
	for (Ake_ast* n = p->head; n; n = n->next) {
		if (i == pos) {
			return n;
		}
		i++;
	}
	return NULL;
}

void Type_use_print(Type_use* tu);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_print(Ake_ast* n)
{
    char* names[Ake_ast_type_count];
    Ast_set_names(names);

    if (n == NULL) return;

	printf("<ast ");
    printf("[%p]", n);
    printf("%s", names[n->type]);

	for (Ake_ast* p = n->head; p; p = p->next) {
		printf(" <%p>", p);
	}
    printf(">");

    Type_use_print(n->tu);

    printf("\n");

    for (Ake_ast* p = n->head; p; p = p->next) {
        Ast_node_print(p);
	}
}

void Type_use_print(Type_use* tu)
{
    if (tu) {
        char* names[Type_use_count];
        Type_use_names(names);

        printf(" <tu ");
        printf("[%p]", tu);
        printf(" %s", names[tu->type]);
        if (tu->td) {
            buffer_finish(&tu->td->name);
            printf(" %s", tu->td->name.buf);
        }
        printf(">");

        Type_use* p = tu->head;
        while (p) {
            Type_use_print(p);
            p = p->next;
        }
    }
}

void Type_use_print_pointers(Type_use* tu, struct list* l);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_print_pointers(Ake_ast* root, struct list* l)
{
    char* names[Ake_ast_type_count];
    Ast_set_names(names);

    bool created = false;
    if (!l) {
        list_create(&l);
        created = true;
    }

    printf("(%p)\n", root);
//    if (list_has_item(l, root)) abort();
    list_add_item(l, root);

    Type_use_print_pointers(root->tu, l);

    for (Ake_ast* p = root->head; p; p = p->next) {
        Ast_node_print_pointers(p, l);
    }

    if (created) {
        list_destroy(l, NULL);
    }
}

void Type_use_print_pointers(Type_use* tu, struct list* l)
{
    if (tu) {
        printf("(%p)\n", tu);
//        if (list_has_item(l, tu)) abort();
        list_add_item(l, tu);

        Type_use* tu2 = tu->head;
        while (tu2) {
            Type_use_print_pointers(tu2, l);
            tu2 = tu2->next;
        }
    }
}
/* NOLINTNEXTLINE(misc-no-recursion) */
void Ast_node_copy(Ake_ast* src, Ake_ast* dest)
{
    dest->type = src->type;
    dest->tu = Type_use_clone(src->tu);
    dest->loc = src->loc;
    buffer_copy(&src->value, &dest->value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ast_node_clone(Ake_ast* n)
{
	Ake_ast* copy = NULL;

	if (n) {
        Ake_ast_create(&copy);
        Ast_node_copy(n, copy);

		Ake_ast* p = n->head;
		while (p) {
			Ake_ast* p_copy = NULL;
			p_copy = Ast_node_clone(p);
            Ake_ast_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ast_node_match(Ake_ast* a, Ake_ast* b)
{
	if (a && b) {
		if (a->type != b->type) {
			return false;
		}

		if (!buffer_compare(&a->value, &b->value)) {
			return false;
		}

        if (a->tu && b->tu) {
            if (a->tu->td != b->tu->td) {
                return false;
            }
        } else if (a->tu) {
            return false;
        } else if (b->tu) {
            return false;
        }

		Ake_ast* c = a->head;
		Ake_ast* d = b->head;
		do {
			if (!Ast_node_match(c, d)) {
				return false;
			}
			if (c) c = c->next;
			if (d) d = d->next;
		} while (c || d);
	} else if (!a && !b) {
		return true;
	} else {
		return false;
	}

	return true;
}

size_t Ast_node_count_children(Ake_ast* n)
{
    size_t count = 0;
    if (n) {
        Ake_ast* p = n->head;
        while (p) {
            count++;
            p = p->next;
        }
    }

    return count;
}
