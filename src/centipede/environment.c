#include "environment.h"

#include "symbol.h"
#include "zinc/memory.h"

void Cent_environment_init(Cent_environment* environment)
{
    Zinc_hash_map_string_init(&environment->symbols, 32);
    environment->prev = NULL;
}

void Cent_environment_create(Cent_environment** environment)
{
    malloc_safe((void**)environment, sizeof(Cent_environment));
    Cent_environment_init(*environment);
}

void Cent_environment_destroy(Cent_environment* env)
{
    Zinc_hash_map_str_map(&env->symbols, (Zinc_hash_map_string_func)Cent_symbol_free);
    Zinc_hash_map_string_destroy(&env->symbols);
}

void Cent_environment_free(Cent_environment* env)
{
    Zinc_hash_map_str_map(&env->symbols, (Zinc_hash_map_string_func)Cent_symbol_free);
    Zinc_hash_map_string_destroy(&env->symbols);
    free(env);
}

void Cent_environment_add_symbol(Cent_environment* environment, struct Zinc_string* name, Cent_symbol* symbol)
{
    Zinc_hash_map_string_add(&environment->symbols, name, symbol);
}

void Cent_environment_add_symbol_str(Cent_environment* environment, char* name, Cent_symbol* symbol)
{
    Zinc_hash_map_string_add_str(&environment->symbols, name, symbol);
}

Cent_symbol* Cent_environment_get(Cent_environment* top, struct Zinc_string* name)
{
    Cent_environment* env = top;
    while (env) {
        Cent_symbol* sym = Zinc_hash_map_string_get(&env->symbols, name);
        if (sym) {
            return sym;
        }
        env = env->prev;
    }

    return NULL;
}

Cent_symbol* Cent_environment_get_str(Cent_environment* top, char* name)
{
    Cent_environment* env = top;
    while (env) {
        Cent_symbol* sym = Zinc_hash_map_string_get_str(&env->symbols, name);
        if (sym) {
            return sym;
        }
        env = env->prev;
    }

    return NULL;
}

Cent_symbol* Cent_environment_get_local(Cent_environment* top, struct Zinc_string* name)
{
    return Zinc_hash_map_string_get(&top->symbols, name);
}

Cent_symbol* Cent_environment_get_local_str(Cent_environment* top, char* name)
{
    return Zinc_hash_map_string_get_str(&top->symbols, name);
}