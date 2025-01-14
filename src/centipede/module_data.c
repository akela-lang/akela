#include "module_data.h"
#include "zinc/memory.h"

void Cent_module_init(Cent_module* mod)
{
    Zinc_hash_map_string_init(&mod->submodules, 16);
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
    Zinc_hash_map_str_map(&mod->submodules, (Zinc_hash_map_string_func)Cent_module_free);
    Zinc_hash_map_string_destroy(&mod->submodules);
}

void Cent_module_free(Cent_module* mod)
{
    Cent_module_destroy(mod);
    free(mod);
}

void Cent_module_add(Cent_module* mod, struct Zinc_string* name, Cent_module* submodule)
{
    Zinc_hash_map_string_add(&mod->submodules, name, submodule);
}

Cent_module* Cent_module_get(Cent_module* mod, struct Zinc_string* name)
{
    return Zinc_hash_map_string_get(&mod->submodules, name);
}