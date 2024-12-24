#ifndef CENTIPEDE_ENVIRONMENT_H
#define CENTIPEDE_ENVIRONMENT_H

#include "zinc/hash.h"
#include "symbol.h"

typedef struct Cent_environment {
    struct hash_table symbols;
    struct Cent_environment* prev;
} Cent_environment;

void Cent_environment_init(Cent_environment* environment);
void Cent_environment_create(Cent_environment** environment);
void Cent_environment_destroy(Cent_environment* environment);
void Cent_environment_add_symbol(Cent_environment* environment, struct buffer* name, Cent_symbol* symbol);
void cent_environment_add_symbol_str(Cent_environment* environment, char* name, Cent_symbol* symbol);

#endif