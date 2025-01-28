#ifndef WORM_SYMBOL_H
#define WORM_SYMBOL_H

#include "zinc/String_slice.h"
#include <stdbool.h>

typedef enum Worm_symbol_type {
    Worm_symbol_type_none,
    Worm_symbol_type_variable,
    //Worm_symbol_type_element,
    //Worm_symbol_type_enumerate,
    //Worm_symbol_type_object_value,
    Worm_symbol_type_file_name,
    //Worm_symbol_type_module,
} Worm_symbol_type;

typedef struct Worm_symbol {
    Worm_symbol_type type;
    union {
        struct {
            void* n;
            //Cent_value* value;
        } variable;
        //Worm_element_type* element;
        //Worm_enum_type* enumerate;
        //Worm_value* object_value;
        Zinc_string_slice file_name;
        //Worm_module* module;
    } data;
    bool is_copy;
} Worm_symbol;

void Worm_symbol_init(Worm_symbol *sym);
void Worm_symbol_create(Worm_symbol** sym);
void Worm_symbol_set_type(Worm_symbol *sym, Worm_symbol_type type);
void Worm_symbol_destroy(Worm_symbol *sym);
void Worm_symbol_free(Worm_symbol *sym);
//bool Worm_symbol_should_copy(Worm_symbol *sym);
void Worm_symbol_copy_shallow(Worm_symbol *src, Worm_symbol *dest);
Worm_symbol* Worm_symbol_clone_shallow(Worm_symbol *sym);

#endif
