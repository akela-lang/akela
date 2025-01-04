#include "zinc/hash.h"
#include "environment.h"
#include "zinc/memory.h"
#include <assert.h>
#include "type_def.h"

void environment_init(struct environment* env, struct environment* p)
{
    hash_table_init(&env->ht, ENVIRONMENT_HASH_TABLE_SIZE);
    env->prev = p;
}

void environment_create(struct environment** env, struct environment* p)
{
    malloc_safe((void**)env, sizeof(struct environment));
    environment_init(*env, p);
}

void environment_put(struct environment* env, struct buffer* value, struct symbol* sym)
{
    assert(sym->type != Symbol_type_none);
    hash_table_add(&env->ht, value, sym);
}

struct symbol* environment_get(struct environment* env, struct buffer* value)
{
    for (struct environment* p = env; p; p = p->prev) {
        struct symbol* found = (struct symbol*)hash_table_get(&p->ht, value);
        if (found) {
            return found;
        }
    }

    return NULL;
}

struct symbol* environment_get_local(struct environment* env, struct buffer* value)
{
    return (struct symbol*)hash_table_get(&env->ht, value);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void environment_destroy_symbol(struct symbol* sym)
{
    Type_use_destroy(sym->tu);
    type_def_destroy(sym->td);
    if (sym->constructor) {
        environment_destroy_symbol(sym->constructor);
    }
    Ake_ast_destroy(sym->root);
    free(sym);
}

void environment_destroy(struct environment* env)
{
    hash_table_map(&env->ht, (void (*)(void*))environment_destroy_symbol);
    hash_table_destroy(&env->ht);
    free(env);
}