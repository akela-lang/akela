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

void Cent_environment_destroy(Cent_environment* environment)
{
    hash_table_destroy(&environment->symbols);
}

void Cent_environment_add_symbol(Cent_environment* environment, struct buffer* name, Cent_symbol* symbol)
{
    hash_table_add(&environment->symbols, name, symbol);
}

void cent_environment_add_symbol_str(Cent_environment* environment, char* name, Cent_symbol* symbol)
{
    hash_table_add_str(&environment->symbols, name, symbol);
}