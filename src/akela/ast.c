#define AKELA_AST_C

#include <stdlib.h>
#include <stdbool.h>
#include "ast.h"
#include "zinc/memory.h"
#include "zinc/list.h"

void Ake_ast_create(Ake_ast** n)
{
	Zinc_malloc_safe((void**)n, sizeof(Ake_ast));
	Ake_ast_init(*n);
}

void Ake_ast_init(Ake_ast* n)
{
	n->type = Ake_ast_type_none;
	Zinc_string_init(&n->value);
	n->tu = NULL;
    n->is_mut = false;
    Zinc_location_init(&n->loc);
	n->env = NULL;
	n->next = NULL;
	n->prev = NULL;
	n->head = NULL;
	n->tail = NULL;
	n->parent = NULL;
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

        Zinc_string_destroy(&n->value);
        Ake_type_use_destroy(n->tu);

    	Ake_EnvironmentDestroy(n->env);
    	free(n->env);

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
    Zinc_location_combine(&p->loc, &c->loc);
	c->parent = p;
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

void Ake_type_use_print(Ake_type_use* tu);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_ast_print(Ake_ast* n)
{
    if (n == NULL) return;

	printf("<ast ");
    printf("[%p]", n);
    printf("%s", Ast_type_name(n->type));

	for (Ake_ast* p = n->head; p; p = p->next) {
		printf(" <%p>", p);
	}
    printf(">");

    Ake_type_use_print(n->tu);

    printf("\n");

    for (Ake_ast* p = n->head; p; p = p->next) {
        Ake_ast_print(p);
	}
}

void Ake_type_use_print(Ake_type_use* tu)
{
    if (tu) {
        char const* names[Ake_type_use_count];
        Ake_type_use_names(names);

        printf(" <tu ");
        printf("[%p]", tu);
        printf(" %s", names[tu->type]);
        if (tu->td) {
            Zinc_string_finish(&tu->td->name);
            printf(" %s", tu->td->name.buf);
        }
        printf(">");

        Ake_type_use* p = tu->head;
        while (p) {
            Ake_type_use_print(p);
            p = p->next;
        }
    }
}

void Ake_type_use_print_pointers(Ake_type_use* tu, struct Zinc_list* l);

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_ast_print_pointers(Ake_ast* root, struct Zinc_list* l)
{
    bool created = false;
    if (!l) {
        Zinc_list_create(&l);
        created = true;
    }

    printf("(%p)\n", root);
//    if (list_has_item(l, root)) abort();
    Zinc_list_add_item(l, root);

    Ake_type_use_print_pointers(root->tu, l);

    for (Ake_ast* p = root->head; p; p = p->next) {
        Ake_ast_print_pointers(p, l);
    }

    if (created) {
        Zinc_list_destroy(l, NULL);
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_type_use_print_pointers(Ake_type_use* tu, struct Zinc_list* l)
{
    if (tu) {
        printf("(%p)\n", tu);
//        if (list_has_item(l, tu)) abort();
        Zinc_list_add_item(l, tu);

        Ake_type_use* tu2 = tu->head;
        while (tu2) {
            Ake_type_use_print_pointers(tu2, l);
            tu2 = tu2->next;
        }
    }
}
/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_ast_copy(Ake_ast* src, Ake_ast* dest)
{
    dest->type = src->type;
    dest->tu = Ake_type_use_clone(src->tu);
    dest->loc = src->loc;
    Zinc_string_copy(&src->value, &dest->value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_ast_clone(Ake_ast* n)
{
	Ake_ast* copy = NULL;

	if (n) {
        Ake_ast_create(&copy);
        Ake_ast_copy(n, copy);

		Ake_ast* p = n->head;
		while (p) {
			Ake_ast* p_copy = NULL;
			p_copy = Ake_ast_clone(p);
            Ake_ast_add(copy, p_copy);
			p = p->next;
		}
	}

	return copy;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_ast_match(Ake_ast* a, Ake_ast* b)
{
	if (a && b) {
		if (a->type != b->type) {
			return false;
		}

		if (!Zinc_string_compare(&a->value, &b->value)) {
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
			if (!Ake_ast_match(c, d)) {
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

size_t Ake_ast_count_children(Ake_ast* n)
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
