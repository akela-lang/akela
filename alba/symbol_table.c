#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type_def.h"
#include "type_use.h"
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
	sym->td = NULL;
	sym->tu = NULL;
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
void symbol_table_add_reserved(struct environment* env, char* name, enum token_enum type, struct type_def* td)
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
	sym->td = td;

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
	struct type_def* td = NULL;
	struct symbol* sym = NULL;
	
	name = "Int32";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 32;
	symbol_table_add_reserved(env, name, token_id, td);

	name = "Int64";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 64;
	symbol_table_add_reserved(env, name, token_id, td);

	name = "UInt32";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->bit_count = 32;
	symbol_table_add_reserved(env, name, token_id, td);

	name = "UInt64";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->bit_count = 64;
	symbol_table_add_reserved(env, name, token_id, td);

	name = "Float32";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_float;
	buffer_copy_str(&td->name, name);
	td->bit_count = 32;
	symbol_table_add_reserved(env, name, token_id, td);

	name = "Float64";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_float;
	buffer_copy_str(&td->name, name);
	td->bit_count = 64;
	symbol_table_add_reserved(env, name, token_id, td);

	name = "String";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_string;
	buffer_copy_str(&td->name, name);
	symbol_table_add_reserved(env, name, token_id, td);

	name = "Bool";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_boolean;
	buffer_copy_str(&td->name, name);
	symbol_table_add_reserved(env, name, token_id, td);

	name = "Vector";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_array;
	td->is_generic = true;
	td->generic_count = 1;
	buffer_copy_str(&td->name, name);
	symbol_table_add_reserved(env, "Vector", token_id, td);

	name = "Function";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_function;
	td->is_generic = true;
	buffer_copy_str(&td->name, name);
	symbol_table_add_reserved(env, name, token_id, td);

	name = "Input";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_function_input;
	td->is_generic = true;
	buffer_copy_str(&td->name, name);
	symbol_table_add_reserved(env, name, token_id, td);

	name = "Output";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_function_output;
	td->is_generic = true;
	td->generic_count = 1;
	buffer_copy_str(&td->name, name);
	symbol_table_add_reserved(env, name, token_id, td);
}

void symbol_table_add_numeric(struct symbol_table* st, char* name)
{
	struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, name);
	struct symbol* sym = environment_get(st->top, &bf);
	assert(sym);
	assert(sym->td);
	type_def_add(st->numeric_pool, sym->td);
}

void symbol_table_numeric_pool_init(struct symbol_table* st)
{
	struct type_def* pool = NULL;
	malloc_safe(&pool, sizeof(struct type_def));
	type_def_init(pool);
	st->numeric_pool = pool;

	symbol_table_add_numeric(st, "Int32");
	symbol_table_add_numeric(st, "Int64");
	symbol_table_add_numeric(st, "UInt32");
	symbol_table_add_numeric(st, "UInt64");
	symbol_table_add_numeric(st, "Float32");
	symbol_table_add_numeric(st, "Float64");
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
	type_def_destroy(st->numeric_pool);
}

bool symbol_table_is_global(struct symbol_table* st)
{
	return st->top && (st->top->prev == st->initial);
}

bool is_numeric(struct type_def* td)
{
	return td->type == type_integer || td->type == type_float;
}

bool type_find(struct symbol_table* st, struct type_def* a, struct type_def* b, bool *promote, struct type_def** c)
{
	*promote = false;
	*c = NULL;

	if (a == b) {
		return true;
	}

	if (is_numeric(a) && is_numeric(b)) {
		enum type type = a->type;

		if (b->type == type_float) {
			type = b->type;
		}

		bool is_signed = false;
		if (type == type_integer) {
			is_signed = a->is_signed;
			if (b->is_signed) {
				is_signed = b->is_signed;
			}
		}

		int bit_count = a->bit_count;
		if (b->bit_count > bit_count) {
			bit_count = b->bit_count;
		}

		struct type_def* x = st->numeric_pool->head;
		assert(x);
		do {
			if (x->type == type && x->is_signed == is_signed && x->bit_count == bit_count) {
				*promote = true;
				*c = x;
				return true;
			}
			x = x->next;
		} while (x);
	}

	return false;
}

bool type_find_whole(struct symbol_table* st, struct type_use* a, struct type_use* b)
{
	if (a && b) {
		bool promote;
		struct type_def* td = NULL;
		if (!type_find(st, a->td, b->td, &promote, &td)) {
			return false;
		}
		if (promote) {
			a->td = td;
		}

		struct type_use* x = a->head;
		struct type_use* y = b->head;
		do {
			if (!type_find_whole(st, x, y)) {
				return false;
			}
			if (x) x = x->next;
			if (y) y = y->next;
		} while (x || y);
		
		return true;

	} else if (!a && !b) {
		return true;
	} else {
		return false;
	}
}

bool type_use_can_cast(struct symbol_table* st, struct type_use* a, struct type_use* b)
{
	if (a && b) {
		bool promote;
		struct type_def* td = NULL;
		if (!type_find(st, a->td, b->td, &promote, &td)) {
			return false;
		}

		struct type_use* x = a->head;
		struct type_use* y = b->head;
		do {
			if (!type_use_can_cast(st, x, y)) {
				return false;
			}
			if (x) x = x->next;
			if (y) y = y->next;
		} while (x || y);

		return true;

	} else if (!a && !b) {
		return true;
	} else {
		return false;
	}
}
