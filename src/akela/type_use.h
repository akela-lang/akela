#ifndef AKELA_TYPE_USE_H
#define AKELA_TYPE_USE_H

#include "type_def.h"
#include "zinc/vector.h"
#include <stdbool.h>
#include "zinc/memory.h"

typedef enum Array_element_option {
    Array_element_default,
    Array_element_const,
} Array_element_option;

typedef struct Type_dimension {
    size_t size;
    Array_element_option option;
} Type_dimension;

typedef enum Type_context {
    Type_context_value,
    Type_context_ptr,
} Type_context;

typedef enum Type_use_type {
    Type_use_type_def,
    Type_use_function_inputs,
    Type_use_function_outputs,
    Type_use_function_ellipsis,
    Type_use_count,                 /* keep at end */
} Type_use_type;

#ifdef AKELA_TYPE_USE_C
void Type_use_names(char* names[])
{
    names[Type_use_type_def] = "Type_use_type_def";
    names[Type_use_function_inputs] = "Type_use_function_inputs";
    names[Type_use_function_outputs] = "Type_use_function_outputs";
    names[Type_use_function_ellipsis] = "Type_use_function_ellipsis";
}
#else
void Type_use_names(char* names[]);
#endif

typedef struct Type_use {
    Type_use_type type;
    struct Ake_type_def* td;
    struct buffer name;
    Vector dim;
    bool is_ref;
    bool is_mut;
    bool original_is_mut;
    bool is_array;
    bool is_slice;
    Type_context context;
    void* lhs_allocation;
    struct Type_use* next;
    struct Type_use* prev;
    struct Type_use* head;
    struct Type_use* tail;
} Type_use;

#ifdef __cplusplus
extern "C" {
#endif

void Type_use_init(Type_use* tu);
void Type_use_create(Type_use** tu);
void Type_use_destroy(Type_use* tu);
void Type_use_copy(Type_use* a, Type_use* b);
Type_use* Type_use_clone(Type_use* tu);
void Type_use_reduce_dimension(Type_use* tu);
void Type_use_add(Type_use* p, Type_use* c);

#ifdef __cplusplus
}
#endif

#endif