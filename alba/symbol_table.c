#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type_info.h"
#include "type_node.h"
#include <assert.h>

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
	if (sym->tk_type == token_none) {
		assert(sym->tk_type != token_none);
	}
	/* allocate ht{} */
	hash_table_add(&env->ht, value, sym);
}

/* dynamic-output-none */
struct symbol* environment_get(struct environment* env, struct buffer* value)
{
	for (struct environment* p = env; p; p = p->prev) {
		struct symbol* found = hash_table_get(&p->ht, value);
		if (found) {
			assert(found->tk_type != token_none);
			return found;
		}
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
	sym->tn = NULL;
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

void symbol_table_init_builtin_types(struct symbol_table* st, struct environment* env)
{
	char* name;
	struct type_info* ti = NULL;
	struct symbol* sym = NULL;
	
	name = "Int32";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_integer;
	buffer_copy_str(&ti->name, name);
	ti->is_signed = true;
	ti->bit_count = 32;
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "Int64";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_integer;
	buffer_copy_str(&ti->name, name);
	ti->is_signed = true;
	ti->bit_count = 64;
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "UInt32";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_integer;
	buffer_copy_str(&ti->name, name);
	ti->bit_count = 32;
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "UInt64";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_integer;
	buffer_copy_str(&ti->name, name);
	ti->bit_count = 64;
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "Float32";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_float;
	buffer_copy_str(&ti->name, name);
	ti->bit_count = 32;
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "Float64";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_float;
	buffer_copy_str(&ti->name, name);
	ti->bit_count = 64;
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "String";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_string;
	buffer_copy_str(&ti->name, name);
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "Bool";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_boolean;
	buffer_copy_str(&ti->name, name);
	symbol_table_add_reserved(env, name, token_id, ti);

	name = "Vector";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_array;
	buffer_copy_str(&ti->name, name);
	symbol_table_add_reserved(env, "Vector", token_id, ti);

	name = "Function";
	malloc_safe(&ti, sizeof(struct type_info));
	type_info_init(ti);
	ti->type = type_function;
	buffer_copy_str(&ti->name, name);
	symbol_table_add_reserved(env, name, token_id, ti);
}

void symbol_table_numeric_pool_init(struct symbol_table* st)
{
	char* numeric[] = {
		"Int32",
		"Int64",
		"UInt32",
		"UInt64",
		"Float32",
		"Float64",
		"",
	};

	struct type_info* pool = NULL;
	malloc_safe(&pool, sizeof(struct type_info));
	type_info_init(pool);
	struct buffer bf;
	buffer_init(&bf);

	st->numeric_pool = pool;
}

void symbol_table_init(struct symbol_table* st)
{
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, NULL);
	symbol_table_init_reserved(env);
	st->initial = env;
	st->top = env;
	st->numeric_pool = NULL;
	symbol_table_init_builtin_types(st, env);
	symbol_table_numeric_pool_init(st);
}

void symbol_table_destroy(struct symbol_table* st)
{
	struct environment* env = st->top;
	while (env) {
		struct environment* prev = env->prev;
		environment_destroy(env);
		env = prev;
	}
	type_info_destroy(st->numeric_pool);
}

bool symbol_table_is_global(struct symbol_table* st)
{
	return st->top && (st->top->prev == st->initial);
}

bool type_find(struct symbol_table* st, struct type_info* a, struct type_info* b, bool *promote, struct type_info** c)
{
	*promote = false;
	*c = NULL;

	if (buffer_compare(&a->name, &b->name)) {
		return true;
	}

	if ((a->type == type_integer || a->type == type_float) && (b->type == type_integer || b->type == type_float)) {
		enum type type = a->type;

		if (b->type == type_float) {
			type = b->type;
		}

		bool is_signed = a->is_signed;
		if (b->is_signed) {
			is_signed = b->is_signed;
		}

		int bit_count = a->bit_count;
		if (b->bit_count > bit_count) {
			bit_count = b->bit_count;
		}

		struct type_info* x = st->numeric_pool->head;
		do {
			if (x->type == type && x->is_signed == is_signed && x->bit_count == bit_count) {
				*promote = true;
				*c = x;
				return true;
			}
		} while (x);
	}

	return false;
}

bool type_find_whole(struct symbol_table* st, struct type_node* a, struct type_node* b)
{
	if (a && b) {
		if (a->ti != b->ti) {
			return false;
		}
		bool promote = false;
		struct type_info* ti = NULL;
		if (!type_find(st, a->ti, b->ti, &promote, &ti)) {
			return false;
		}
		if (promote) {
			a->ti = ti;
		}

		struct type_node* x = a->head;
		struct type_node* y = b->head;
		do {
			if (!type_find_whole(st, x, y)) {
				return false;
			}
			if (x) x = x->next;
			if (y) y = y->next;
		} while (x || y);
	} else if (!a && !b) {
		return true;
	} else {
		return false;
	}

	return true;
}
