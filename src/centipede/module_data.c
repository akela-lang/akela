#include "module_data.h"
#include "zinc/memory.h"

void Cent_module_init(Cent_module* mod)
{
    hash_table_init(&mod->submodules, 16);
    mod->env = NULL;
    mod->pr = NULL;
    mod->value = NULL;
}

void Cent_module_create(Cent_module** mod)
{
    malloc_safe((void**)mod, sizeof(Cent_module));
    Cent_module_init(*mod);
}

void Cent_module_destroy(Cent_module* mod)
{
    hash_table_destroy(&mod->submodules);
}

void Cent_module_free(Cent_module* mod)
{
    Cent_module_destroy(mod);
    free(mod);
}

void Cent_module_add(Cent_module* mod, struct buffer* name, Cent_module* submodule)
{
    hash_table_add(&mod->submodules, name, submodule);
}