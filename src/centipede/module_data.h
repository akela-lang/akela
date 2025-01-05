#ifndef CENTIPEDE_MODULE_DATA_H
#define CENTIPEDE_MODULE_DATA_H

#include "zinc/hash.h"

typedef struct Cent_module {
    struct hash_table submodules;
    void* env;
    void* pr;
    void* value;
} Cent_module;

void Cent_module_init(Cent_module* mod);
void Cent_module_create(Cent_module** mod);
void Cent_module_destroy(Cent_module* mod);
void Cent_module_free(Cent_module* mod);
void Cent_module_add(Cent_module* mod, struct buffer* name, Cent_module* submodule);

#endif