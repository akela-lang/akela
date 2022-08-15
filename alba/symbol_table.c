#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>

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
	sym->ti = NULL;
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
void symbol_table_add_reserved(struct environment* env, char* name, enum token_enum type, struct type_info* ti)
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
	sym->ti = ti;

	/* allocate wt{} */
	environment_put(env, &bf, sym);

	/* destroy bf{} */
	buffer_destroy(&bf);

	return sym;
}

/* dynamic-output wt{} */
void symbol_table_init_reserved(struct environment* env)
{
	/* allocate wt{} */
	symbol_table_add_reserved(env, "function", token_function, NULL);
	symbol_table_add_reserved(env, "end", token_end, NULL);
	symbol_table_add_reserved(env, "if", token_if, NULL);
	symbol_table_add_reserved(env, "elseif", token_elseif, NULL);
	symbol_table_add_reserved(env, "else", token_else, NULL);
	symbol_table_add_reserved(env, "while", token_while, NULL);
	symbol_table_add_reserved(env, "for", token_for, NULL);
	symbol_table_add_reserved(env, "in", token_in, NULL);
	symbol_table_add_reserved(env, "var", token_var, NULL);
	symbol_table_add_reserved(env, "true", token_boolean, NULL);
	symbol_table_add_reserved(env, "false", token_boolean, NULL);
}

void symbol_table_init_builtin_types(struct environment* env)
{
	char* name;
	struct type_info* ti = NULL;
	struct symbol* sym = NULL;
	
	name = "Int32";
	malloc_safe(&ti, sizeof(struct type_info));
	symbol_table_add_reserved(env, name, token_type_name, ti);
	type_info_init(ti);
	buffer_copy_str(&ti->name, name);
	ti->is_integer = true;
	ti->is_signed = true;
	ti->bit_count = 32;

	name = "Int64";
	malloc_safe(&ti, sizeof(struct type_info));
	symbol_table_add_reserved(env, name, token_type_name, ti);
	type_info_init(ti);
	buffer_copy_str(&ti->name, name);
	ti->is_integer = true;
	ti->is_signed = true;
	ti->bit_count = 64;

	name = "UInt32";
	malloc_safe(&ti, sizeof(struct type_info));
	symbol_table_add_reserved(env, name, token_type_name, ti);
	type_info_init(ti);
	buffer_copy_str(&ti->name, name);
	ti->is_integer = true;
	ti->bit_count = 32;

	name = "UInt64";
	malloc_safe(&ti, sizeof(struct type_info));
	symbol_table_add_reserved(env, name, token_type_name, ti);
	type_info_init(ti);
	buffer_copy_str(&ti->name, name);
	ti->is_integer = true;
	ti->bit_count = 64;

	name = "Float32";
	malloc_safe(&ti, sizeof(struct type_info));
	symbol_table_add_reserved(env, name, token_type_name, ti);
	type_info_init(ti);
	buffer_copy_str(&ti->name, name);
	ti->is_float = true;
	ti->bit_count = 32;

	name = "Float64";
	malloc_safe(&ti, sizeof(struct type_info));
	symbol_table_add_reserved(env, name, token_type_name, ti);
	type_info_init(ti);
	buffer_copy_str(&ti->name, name);
	ti->is_float = true;
	ti->bit_count = 64;

	symbol_table_add_reserved(env, "String", token_type_name, NULL);
	symbol_table_add_reserved(env, "Bool", token_type_name, NULL);
	symbol_table_add_reserved(env, "Vector", token_array_type_name, NULL);
	symbol_table_add_reserved(env, "Function", token_type_name, NULL);
}

void symbol_table_init(struct symbol_table* st)
{
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, NULL);
	symbol_table_init_reserved(env);
	symbol_table_init_builtin_types(env);
	st->initial = env;
	st->top = env;
	st->ti_head = NULL;
}

void symbol_table_destroy(struct symbol_table* st)
{
	struct environment* env = st->top;
	while (env) {
		struct environment* prev = env->prev;
		environment_destroy(env);
		env = prev;
	}
	struct type_info* ti = st->ti_head;
	while (ti) {
		struct type_info* temp = ti;
		ti = ti->next;
		type_info_destroy(temp);
	}
}

bool symbol_table_is_global(struct symbol_table* st)
{
	return st->top && (st->top->prev == st->initial);
}

void type_info_init(struct type_info* ti)
{
	buffer_init(&ti->name);
	ti->is_integer = false;
	ti->is_float = false;
	ti->is_signed = false;
	ti->bit_count = 0;
	ti->next = NULL;
}

void type_info_destroy(struct type_info* ti)
{
	buffer_destroy(&ti->name);
}

struct type_info* symbol_table_add_type_info(struct symbol_table* st, struct type_info* ti)
{
	ti->next = st->ti_head;
	st->ti_head = ti;
	return ti;
}
