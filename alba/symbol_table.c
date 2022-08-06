#include "zinc/memory.h"
#include "symbol_table.h"

/* dynamic-output env{} */
/* transfer p -> env */
void environment_init(struct environment* env, struct environment* p)
{
	/* allocate ht{} */
	hash_table_init(&env->ht, ENVIRONMENT_HASH_TABLE_SIZE);
	env->prev = p;
}

/* dynamic-output env{} */
/* transfer sym -> env{} */
void environment_put(struct environment* env, struct buffer* value, struct symbol* sym)
{
	/* allocate ht{} */
	hash_table_add(&env->ht, value, sym);
}

/* dynamic-output-none */
struct symbol* environment_get(struct environment* env, struct buffer* value)
{
	for (struct environment* p = env; p; p = p->prev) {
		struct symbol* found = hash_table_get(&p->ht, value);
		if (found) return found;
	}

	return NULL;
}

/* dynamic-output-none */
struct symbol* environment_get_local(struct environment* env, struct buffer* value)
{
	return hash_table_get(&env->ht, value);
}

/* dynamic-output-none */
void symbol_init(struct symbol* sym)
{
	buffer_init(&sym->type);
	sym->dec = NULL;
}

/* destroy sym sym{} */
void environment_destroy_symbol(struct symbol* sym)
{
	buffer_destroy(&sym->type);
	free(sym);
}

/* dynamic-destroy env env{} */
void environment_destroy(struct environment* env)
{
	hash_table_map(&env->ht, (void (*)(void*))environment_destroy_symbol);
	hash_table_destroy(&env->ht);
	free(env);
}
