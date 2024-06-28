#include "type_use.h"
#include "zinc/memory.h"

void Type_use_init(Type_use* tu)
{
    tu->td = NULL;
    VectorInit(&tu->dim, sizeof(Type_dimension));
    tu->is_ref = false;
    tu->is_mut = false;
    tu->original_is_mut = false;
    tu->is_array = false;
    tu->is_slice = false;
    tu->context = Type_context_value;
    tu->proto = NULL;
}

void Type_use_create(Type_use** tu)
{
    malloc_safe((void**)tu, sizeof(Type_use));
    Type_use_init(*tu);
}

void Type_use_destroy(Type_use* tu)
{
    if (tu) {
        VectorDestroy(&tu->dim);
    }
}

Type_use* Type_use_copy(Type_use* tu)
{
    Type_use* new_tu = NULL;
    if (tu) {
        Type_use_create(&new_tu);
        new_tu->td = tu->td;
        VectorCopy(&tu->dim, &new_tu->dim);
        new_tu->is_ref = tu->is_ref;
        new_tu->is_mut = tu->is_mut;
        new_tu->original_is_mut = tu->original_is_mut;
        new_tu->is_array = tu->is_array;
        new_tu->is_slice = tu->is_slice;
        new_tu->context = Type_context_value;         /* default to value */
        new_tu->proto = Ast_node_copy(tu->proto);
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