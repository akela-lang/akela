#ifndef CENTIPEDE_SYMBOL_H
#define CENTIPEDE_SYMBOL_H

#include <zinc/String_slice.h>

#include "element.h"
#include "enumerate.h"
#include "module_data.h"

typedef enum Cent_symbol_type {
    Cent_symbol_type_none,
    Cent_symbol_type_variable,
    Cent_symbol_type_element,
    Cent_symbol_type_enumerate,
    Cent_symbol_type_object_value,
    Cent_symbol_type_file_name,
    Cent_symbol_type_module,
} Cent_symbol_type;

typedef struct Cent_symbol {
    Cent_symbol_type type;
    union {
        struct {
            void* n;
            Cent_value* value;
        } variable;
        Cent_element_type* element;
        Cent_enum_type* enumerate;
        Cent_value* object_value;
        String_slice file_name;
        Cent_module* module;
    } data;
    bool is_copy;
} Cent_symbol;

void Cent_symbol_init(Cent_symbol *sym);
void Cent_symbol_create(Cent_symbol** sym);
void Cent_symbol_set_type(Cent_symbol *sym, Cent_symbol_type type);
void Cent_symbol_destroy(Cent_symbol *sym);
void Cent_symbol_free(Cent_symbol *sym);

#endif