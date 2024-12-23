#ifndef CENTIPEDE_SYMBOL_H
#define CENTIPEDE_SYMBOL_H

#include "token.h"

typedef enum Cent_symbol_type {
    Cent_symbol_type_none,
    Cent_symbol_type_reserved_word,
    Cent_symbol_type_type,
    Cent_symbol_type_value,
} Cent_symbol_type;

typedef struct Cent_symbol {
    Cent_symbol_type type;
    Cent_token_type reserved_token_type;
    struct buffer value;
} Cent_symbol;

void Cent_symbol_init(Cent_symbol *sym);
void Cent_symbol_create(Cent_symbol** sym);
void Cent_symbol_destroy(Cent_symbol *sym);

#endif