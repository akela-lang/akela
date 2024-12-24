#ifndef CENTIPEDE_SYMBOL_H
#define CENTIPEDE_SYMBOL_H

#include "value.h"
#include "element.h"
#include "enumerate.h"

typedef enum Cent_symbol_type {
    Cent_symbol_type_none,
    Cent_symbol_type_value,
    Cent_symbol_type_element,
    Cent_symbol_type_enumerate,
} Cent_symbol_type;

typedef struct Cent_symbol {
    Cent_symbol_type type;
    Cent_value* value;
    Cent_element* element;
    Cent_enumerate* enumerate;
} Cent_symbol;

void Cent_symbol_init(Cent_symbol *sym);
void Cent_symbol_create(Cent_symbol** sym);
void Cent_symbol_destroy(Cent_symbol *sym);

#endif