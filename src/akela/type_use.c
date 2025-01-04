#define AKELA_TYPE_USE_C
#include "type_use.h"
#include "zinc/memory.h"

void Type_use_init(Type_use* tu)
{
    tu->type = Type_use_type_def;
    tu->td = NULL;
    buffer_init(&tu->name);
    VectorInit(&tu->dim, sizeof(Type_dimension));
    tu->is_ref = false;
    tu->is_mut = false;
    tu->original_is_mut = false;
    tu->is_array = false;
    tu->is_slice = false;
    tu->context = Type_context_value;
    tu->lhs_allocation = NULL;
    tu->next = NULL;
    tu->prev = NULL;
    tu->head = NULL;
    tu->prev = NULL;
}

void Type_use_create(Type_use** tu)
{
    malloc_safe((void**)tu, sizeof(Type_use));
    Type_use_init(*tu);
}

void Type_use_destroy(Type_use* tu)
{
    if (tu) {
        Type_use* p = tu->head;
        while (p) {
            Type_use* temp = p;
            p = p->next;
            Type_use_destroy(temp);
        }
        buffer_destroy(&tu->name);
        VectorDestroy(&tu->dim);
        free(tu);
    }
}

void Type_use_add(Type_use* p, Type_use* c)
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

void Type_use_copy(Type_use* src, Type_use* dest)
{
    dest->type = src->type;
    dest->td = src->td;
    buffer_copy(&src->name, &dest->name);
    VectorCopy(&src->dim, &dest->dim);
    dest->is_ref = src->is_ref;
    dest->is_mut = src->is_mut;
    dest->original_is_mut = src->original_is_mut;
    dest->is_array = src->is_array;
    dest->is_slice = src->is_slice;
    dest->context = Type_context_value;         /* default to value */
    dest->lhs_allocation = NULL;                /* default to NULL */
}

Type_use* Type_use_clone(Type_use* tu)
{
    Type_use* new_tu = NULL;
    if (tu) {
        Type_use_create(&new_tu);
        Type_use_copy(tu, new_tu);

        Type_use* p = tu->head;
        while (p) {
            Type_use* new_p = Type_use_clone(p);
            Type_use_add(new_tu, new_p);
            p = p->next;
        }
    }
    return new_tu;
}

void Type_use_reduce_dimension(Type_use* tu)
{
    if (tu->dim.count > 0) {
        Type_dimension current;
        Type_dimension *first = (Type_dimension*)VECTOR_PTR(&tu->dim, 0);
        current = *first;

        if (!tu->original_is_mut || current.option == Array_element_const) {
            tu->is_mut = false;
        } else {
            tu->is_mut = true;
        }

        for (size_t i = 0; i < tu->dim.count - 1; i++) {
            Type_dimension* dest = (Type_dimension*)VECTOR_PTR(&tu->dim, i);
            Type_dimension* src = (Type_dimension*)VECTOR_PTR(&tu->dim, i + 1);
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