#ifndef CENTIPEDE_ENVIRONMENT_H
#define CENTIPEDE_ENVIRONMENT_H

#include "zinc/hash_map_string.h"
#include "symbol.h"

typedef struct Cent_environment {
    Zinc_hash_map_string symbols;
    struct Cent_environment* prev;
} Cent_environment;

void Cent_environment_init(Cent_environment* environment);
void Cent_environment_create(Cent_environment** environment);
void Cent_environment_destroy(Cent_environment* env);
void Cent_environment_add_symbol(Cent_environment* environment, struct Zinc_string* name, Cent_symbol* symbol);
void Cent_environment_add_symbol_str(Cent_environment* environment, char* name, Cent_symbol* symbol);
Cent_symbol* Cent_environment_get(Cent_environment* top, struct Zinc_string* name);
Cent_symbol* Cent_environment_get_str(Cent_environment* top, char* name);
Cent_symbol* Cent_environment_get_local(Cent_environment* top, struct Zinc_string* name);
Cent_symbol* Cent_environment_get_local_str(Cent_environment* top, char* name);

#endif
