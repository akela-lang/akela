#ifndef AKELA_TYPE_USE_H
#define AKELA_TYPE_USE_H

#include "type_def.h"
#include "zinc/vector.h"
#include <stdbool.h>

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

typedef struct Type_use {
    struct type_def* td;
    Vector dim;
    bool is_ref;
    bool is_mut;
    bool original_is_mut;
    bool is_array;
    bool is_slice;
    Type_context context;
    struct Ast_node* proto;        /* functions are duck-typed */
} Type_use;

#ifdef __cplusplus
extern "C" {
#endif

void Type_use_init(Type_use* tu);
void Type_use_create(Type_use** tu);
void Type_use_destroy(Type_use* tu);
Type_use* Type_use_copy(Type_use* tu);
void Type_use_reduce_dimension(Type_use* tu);

#ifdef __cplusplus
}
#endif

#endif