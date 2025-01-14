#ifndef CENTIPEDE_MODULE_DATA_H
#define CENTIPEDE_MODULE_DATA_H

#include "zinc/hash_map_string.h"

typedef struct Cent_module {
    struct Zinc_hash_table submodules;
    void* env;
    void* pr;
    void* value;
} Cent_module;

void Cent_module_init(Cent_module* mod);
void Cent_module_create(Cent_module** mod);
void Cent_module_destroy(Cent_module* mod);
void Cent_module_free(Cent_module* mod);
void Cent_module_add(Cent_module* mod, struct Zinc_string* name, Cent_module* submodule);
Cent_module* Cent_module_get(Cent_module* mod, struct Zinc_string* name);

#endif