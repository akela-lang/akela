#define AKELA_TYPE_USE_C
#include "type_use.h"
#include "zinc/memory.h"

void Ake_type_use_init(Ake_type_use* tu)
{
    tu->type = Ake_type_use_type_def;
    tu->td = NULL;
    Zinc_string_init(&tu->name);
    VectorInit(&tu->dim, sizeof(Ake_type_dimension));
    tu->is_ref = false;
    tu->is_mut = false;
    tu->original_is_mut = false;
    tu->is_array = false;
    tu->is_slice = false;
    tu->context = Ake_type_context_value;
    tu->lhs_allocation = NULL;
    tu->next = NULL;
    tu->prev = NULL;
    tu->head = NULL;
    tu->prev = NULL;
}

void Ake_type_use_create(Ake_type_use** tu)
{
    Zinc_malloc_safe((void**)tu, sizeof(Ake_type_use));
    Ake_type_use_init(*tu);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_type_use_destroy(Ake_type_use* tu)
{
    if (tu) {
        Ake_type_use* p = tu->head;
        while (p) {
            Ake_type_use* temp = p;
            p = p->next;
            Ake_type_use_destroy(temp);
        }
        Zinc_string_destroy(&tu->name);
        VectorDestroy(&tu->dim);
        free(tu);
    }
}

void Ake_type_use_add(Ake_type_use* p, Ake_type_use* c)
{
    if (p->head && p->tail) {
        p->tail->next = c;
        c->prev = p->tail;
        p->tail = c;
    } else {
        p->head = c;
        p->tail = c;
    }
}

void Ake_type_use_copy(Ake_type_use* src, Ake_type_use* dest)
{
    dest->type = src->type;
    dest->td = src->td;
    Zinc_string_copy(&src->name, &dest->name);
    VectorCopy(&src->dim, &dest->dim);
    dest->is_ref = src->is_ref;
    dest->is_mut = src->is_mut;
    dest->original_is_mut = src->original_is_mut;
    dest->is_array = src->is_array;
    dest->is_slice = src->is_slice;
    dest->context = Ake_type_context_value;         /* default to value */
    dest->lhs_allocation = NULL;                /* default to NULL */
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_type_use* Ake_type_use_clone(Ake_type_use* tu)
{
    Ake_type_use* new_tu = NULL;
    if (tu) {
        Ake_type_use_create(&new_tu);
        Ake_type_use_copy(tu, new_tu);

        Ake_type_use* p = tu->head;
        while (p) {
            Ake_type_use* new_p = Ake_type_use_clone(p);
            Ake_type_use_add(new_tu, new_p);
            p = p->next;
        }
    }
    return new_tu;
}

void Ake_type_use_reduce_dimension(Ake_type_use* tu)
{
    if (tu->dim.count > 0) {
        Ake_type_dimension current;
        Ake_type_dimension *first = (Ake_type_dimension*)VECTOR_PTR(&tu->dim, 0);
        current = *first;

        if (!tu->original_is_mut || current.option == Ake_array_element_const) {
            tu->is_mut = false;
        } else {
            tu->is_mut = true;
        }

        for (size_t i = 0; i < tu->dim.count - 1; i++) {
            Ake_type_dimension* dest = (Ake_type_dimension*)VECTOR_PTR(&tu->dim, i);
            Ake_type_dimension* src = (Ake_type_dimension*)VECTOR_PTR(&tu->dim, i + 1);
            *dest = *src;
        }
        if (tu->dim.count > 0) {
            tu->dim.count--;
        }
        if (tu->dim.count == 0) {
            tu->is_array = false;
            tu->is_slice = false;
        }
    }
}