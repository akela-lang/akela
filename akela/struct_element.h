#ifndef AKELA_STRUCT_ELEMENT_H
#define AKELA_STRUCT_ELEMENT_H

#include "ast.h"

typedef enum Struct_element_type {
    Struct_element_type_none,
    Struct_element_type_field,
    Struct_element_type_method,
} Struct_element_type;

typedef struct Struct_element {
    Struct_element_type type;
    Ake_ast* tu;
    Ake_ast* func;
    size_t index;
} Struct_element;

void Struct_element_init(Struct_element* se);
void Struct_element_create(Struct_element** se);
void Struct_element_destroy(Struct_element* se);

#endif