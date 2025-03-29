#include "build_data.h"
#include "zinc/memory.h"
#include "value.h"

void Cent_ast_stack_push(Cent_ast_stack** top, Cent_ast* n)
{
    Cent_ast_stack* node = NULL;
    Zinc_malloc_safe((void**)&node, sizeof(Cent_ast_stack));
    node->n = n;
    node->next = *top;
    *top = node;
}

void Cent_ast_stack_pop(Cent_ast_stack** top)
{
    if (*top) {
        Cent_ast_stack* node = *top;
        *top = node->next;
        free(node);
    }
}

bool Cent_ast_stack_has_node(Cent_ast_stack* top, Cent_ast* n)
{
    bool found = false;
    Cent_ast_stack* node = top;
    while (node) {
        if (node->n == n) {
            found = true;
            break;
        }
        node = node->next;
    }
    return found;
}

void Cent_build_data_init(Cent_build_data *data, Zinc_error_list* errors)
{
    data->top = NULL;
    data->errors = errors;
}

void Cent_build_data_create(Cent_build_data** data, Zinc_error_list* errors)
{
    Zinc_malloc_safe((void**)data, sizeof(Cent_build_data));
    Cent_build_data_init(*data, errors);
}

void Cent_build_data_destroy(Cent_build_data* data)
{
    while (data->top) {
        Cent_ast_stack_pop(&data->top);
    }
}