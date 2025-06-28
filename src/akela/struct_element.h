#ifndef AKELA_STRUCT_ELEMENT_H
#define AKELA_STRUCT_ELEMENT_H

#include "ast.h"

typedef enum Ake_struct_element_type {
    Ake_struct_element_type_none,
    Ake_struct_element_type_field,
    Ake_struct_element_type_method,
} Ake_struct_element_type;

typedef struct Ake_struct_element {
    Ake_struct_element_type type;
    Ake_Ast* tu;
    Ake_Ast* func;
    size_t index;
} Ake_struct_element;

void Ake_struct_element_init(Ake_struct_element* se);
void Ake_struct_element_create(Ake_struct_element** se);
void Ake_struct_element_destroy(Ake_struct_element* se);

#endif