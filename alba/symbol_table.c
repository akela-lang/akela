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
	sym->tk_type = token_none;
	sym->dec = NULL;
}

/* destroy sym sym{} */
void environment_destroy_symbol(struct symbol* sym)
{
	free(sym);
}

/* dynamic-destroy env env{} */
void environment_destroy(struct environment* env)
{
	hash_table_map(&env->ht, (void (*)(void*))environment_destroy_symbol);
	hash_table_destroy(&env->ht);
	free(env);
}

/* dynamic-temp bf{} */
void symbol_table_add_reserved(struct environment* env, char* name, enum token_enum type)
{
	struct buffer bf;

	/* initialize bf{} */
	buffer_init(&bf);
	buffer_copy_str(&bf, name);

	/* allocate sym */
	struct symbol* sym = NULL;
	malloc_safe(&sym, sizeof(struct symbol));
	symbol_init(sym);
	sym->tk_type = type;

	/* allocate wt{} */
	environment_put(env, &bf, sym);

	/* destroy bf{} */
	buffer_destroy(&bf);
}

/* dynamic-output wt{} */
void symbol_table_init_reserved(struct environment* env)
{
	/* allocate wt{} */
	symbol_table_add_reserved(env, "function", token_function);
	symbol_table_add_reserved(env, "end", token_end);
	symbol_table_add_reserved(env, "if", token_if);
	symbol_table_add_reserved(env, "elseif", token_elseif);
	symbol_table_add_reserved(env, "else", token_else);
	symbol_table_add_reserved(env, "while", token_while);
	symbol_table_add_reserved(env, "for", token_for);
	symbol_table_add_reserved(env, "in", token_in);
	symbol_table_add_reserved(env, "var", token_var);
	symbol_table_add_reserved(env, "true", token_boolean);
	symbol_table_add_reserved(env, "false", token_boolean);
}

void symbol_table_add_builtin_type(struct environment* env, char* name)
{
	struct buffer bf;

	/* initialize bf{} */
	buffer_init(&bf);
	buffer_copy_str(&bf, name);

	/* allocate sym */
	struct symbol* sym = NULL;
	malloc_safe(&sym, sizeof(struct symbol));
	symbol_init(sym);
	sym->tk_type = token_type_name;

	/* allocate wt{} */
	environment_put(env, &bf, sym);

	/* destroy bf{} */
	buffer_destroy(&bf);
}

void symbol_table_init_builtin_types(struct environment* env)
{
	symbol_table_add_builtin_type(env, "Int32");
	symbol_table_add_builtin_type(env, "Int64");
	symbol_table_add_builtin_type(env, "UInt32");
	symbol_table_add_builtin_type(env, "UInt62");
	symbol_table_add_builtin_type(env, "Float32");
	symbol_table_add_builtin_type(env, "Float64");
	symbol_table_add_builtin_type(env, "String");
	symbol_table_add_builtin_type(env, "Bool");
	symbol_table_add_builtin_type(env, "Vector");
	symbol_table_add_builtin_type(env, "Function");
}

void symbol_table_init(struct symbol_table* st)
{
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, NULL);
	symbol_table_init_reserved(env);
	symbol_table_init_builtin_types(env);
	st->top = env;
}

void symbol_table_destroy(struct symbol_table* st)
{
	environment_destroy(st->top);
}
