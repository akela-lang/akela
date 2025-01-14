#include "zinc/hash_map_string.h"
#include "environment.h"
#include "zinc/memory.h"
#include <assert.h>
#include "type_def.h"

void Ake_environment_init(struct Ake_environment* env, struct Ake_environment* p)
{
    Zinc_hash_map_string_init(&env->ht, ENVIRONMENT_HASH_TABLE_SIZE);
    env->prev = p;
}

void Ake_environment_create(struct Ake_environment** env, struct Ake_environment* p)
{
    malloc_safe((void**)env, sizeof(struct Ake_environment));
    Ake_environment_init(*env, p);
}

void Ake_environment_put(struct Ake_environment* env, Zinc_string* value, struct Ake_symbol* sym)
{
    assert(sym->type != Ake_symbol_type_none);
    Zinc_hash_map_string_add(&env->ht, value, sym);
}

struct Ake_symbol* Ake_environment_get(struct Ake_environment* env, Zinc_string* value)
{
    for (struct Ake_environment* p = env; p; p = p->prev) {
        struct Ake_symbol* found = (struct Ake_symbol*)Zinc_hash_map_string_get(&p->ht, value);
        if (found) {
            return found;
        }
    }

    return NULL;
}

struct Ake_symbol* Ake_environment_get_local(struct Ake_environment* env, Zinc_string* value)
{
    return (struct Ake_symbol*)Zinc_hash_map_string_get(&env->ht, value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_environment_destroy_symbol(struct Ake_symbol* sym)
{
    Ake_type_use_destroy(sym->tu);
    Ake_type_def_destroy(sym->td);
    if (sym->constructor) {
        Ake_environment_destroy_symbol(sym->constructor);
    }
    Ake_ast_destroy(sym->root);
    free(sym);
}

void Ake_environment_destroy(struct Ake_environment* env)
{
    Zinc_hash_map_str_map(&env->ht, (void (*)(void*))Ake_environment_destroy_symbol);
    Zinc_hash_map_string_destroy(&env->ht);
    free(env);
}