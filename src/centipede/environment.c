#include "environment.h"

#include "symbol.h"
#include "zinc/memory.h"

void Cent_environment_init(Cent_environment* environment)
{
    hash_table_init(&environment->symbols, 32);
    environment->prev = NULL;
}

void Cent_environment_create(Cent_environment** environment)
{
    malloc_safe((void**)environment, sizeof(Cent_environment));
    Cent_environment_init(*environment);
}

void Cent_environment_destroy(Cent_environment* env)
{
    hash_table_map(&env->symbols, (hash_table_func)Cent_symbol_free);
    hash_table_destroy(&env->symbols);
}

void Cent_environment_free(Cent_environment* env)
{
    hash_table_map(&env->symbols, (hash_table_func)Cent_symbol_free);
    hash_table_destroy(&env->symbols);
    free(env);
}

void Cent_environment_add_symbol(Cent_environment* environment, struct Zinc_string* name, Cent_symbol* symbol)
{
    hash_table_add(&environment->symbols, name, symbol);
}

void Cent_environment_add_symbol_str(Cent_environment* environment, char* name, Cent_symbol* symbol)
{
    hash_table_add_str(&environment->symbols, name, symbol);
}

Cent_symbol* Cent_environment_get(Cent_environment* top, struct Zinc_string* name)
{
    Cent_environment* env = top;
    while (env) {
        Cent_symbol* sym = hash_table_get(&env->symbols, name);
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
        Cent_symbol* sym = hash_table_get_str(&env->symbols, name);
        if (sym) {
            return sym;
        }
        env = env->prev;
    }

    return NULL;
}

Cent_symbol* Cent_environment_get_local(Cent_environment* top, struct Zinc_string* name)
{
    return hash_table_get(&top->symbols, name);
}

Cent_symbol* Cent_environment_get_local_str(Cent_environment* top, char* name)
{
    return hash_table_get_str(&top->symbols, name);
}