#ifndef CENTIPEDE_SYMBOL_H
#define CENTIPEDE_SYMBOL_H

#include <zinc/string_slice.h>

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
        Zinc_string_slice file_name;
        Cent_module* module;
    } data;
    bool is_copy;
} Cent_symbol;

void Cent_symbol_init(Cent_symbol *sym);
void Cent_symbol_create(Cent_symbol** sym);
void Cent_symbol_set_type(Cent_symbol *sym, Cent_symbol_type type);
void Cent_symbol_destroy(Cent_symbol *sym);
void Cent_symbol_free(Cent_symbol *sym);
bool Cent_symbol_should_copy(Cent_symbol *sym);
void Cent_symbol_copy_shallow(Cent_symbol *src, Cent_symbol *dest);
Cent_symbol* Cent_symbol_clone_shallow(Cent_symbol *sym);

#endif