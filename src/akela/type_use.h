#ifndef AKELA_TYPE_USE_H
#define AKELA_TYPE_USE_H

#include "type_def.h"
#include "zinc/vector.h"
#include <stdbool.h>
#include "zinc/memory.h"

typedef enum Ake_array_element_option {
    Ake_array_element_default,
    Ake_array_element_const,
} Ake_array_element_option;

typedef struct Ake_type_dimension {
    size_t size;
    Ake_array_element_option option;
} Ake_type_dimension;

typedef enum Ake_type_context {
    Ake_type_context_value,
    Ake_type_context_ptr,
} Ake_type_context;

typedef enum Ake_type_use_type {
    Ake_type_use_type_def,
    Ake_type_use_function_inputs,
    Ake_type_use_function_outputs,
    Ake_type_use_function_ellipsis,
    Ake_type_use_count,                 /* keep at end */
} Ake_type_use_type;

void static Ake_type_use_names(char const* names[])
{
    names[Ake_type_use_type_def] = "Type_use_type_def";
    names[Ake_type_use_function_inputs] = "Type_use_function_inputs";
    names[Ake_type_use_function_outputs] = "Type_use_function_outputs";
    names[Ake_type_use_function_ellipsis] = "Type_use_function_ellipsis";
}

typedef struct Ake_type_use {
    Ake_type_use_type type;
    Ake_type_def* td;
    Zinc_string name;
    Zinc_vector dim;
    bool is_ref;
    bool is_mut;
    bool original_is_mut;
    bool is_array;
    bool is_slice;
    Ake_type_context context;
    void* lhs_allocation;
    Ake_type_use* next;
    Ake_type_use* prev;
    Ake_type_use* head;
    Ake_type_use* tail;
} Ake_type_use;

#ifdef __cplusplus
extern "C" {
#endif

void Ake_type_use_init(Ake_type_use* tu);
void Ake_type_use_create(Ake_type_use** tu);
void Ake_type_use_destroy(Ake_type_use* tu);
void Ake_type_use_copy(Ake_type_use* a, Ake_type_use* b);
Ake_type_use* Ake_type_use_clone(Ake_type_use* tu);
void Ake_type_use_reduce_dimension(Ake_type_use* tu);
void Ake_type_use_add(Ake_type_use* p, Ake_type_use* c);

#ifdef __cplusplus
}
#endif

#endif