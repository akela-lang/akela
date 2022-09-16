#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type_def.h"
#include <assert.h>
#include "ast.h"
#include "hash.h"

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
	sym->td = NULL;
	sym->tu = NULL;
	sym->constructor = NULL;
	sym->root = NULL;
}

/* destroy sym sym{} */
void environment_destroy_symbol(struct symbol* sym)
{
	ast_node_destroy(sym->tu);
	type_def_destroy(sym->td);
	if (sym->constructor) {
		environment_destroy_symbol(sym->constructor);
	}
	ast_node_destroy(sym->root);
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
	symbol_table_add_reserved(env, "module", token_module, NULL);
	symbol_table_add_reserved(env, "struct", token_struct, NULL);
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

	name = "Module";
	malloc_safe(&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_module;
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
	struct ast_node* n = NULL;
	ast_node_create(&n);
	n->type = ast_type_type_pool;
	n->td = sym->td;
	ast_node_add(st->numeric_pool, n);
}

void symbol_table_numeric_pool_init(struct symbol_table* st)
{
	struct ast_node* pool = NULL;
	malloc_safe(&pool, sizeof(struct ast_node));
	ast_node_init(pool);
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
	st->top = env;
	st->initial = env;
	symbol_table_init_reserved(env);
	symbol_table_init_builtin_types(st, env);
	symbol_table_numeric_pool_init(st);

	env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, st->top);
	st->top = env;
	st->global = env;
}

void symbol_table_destroy(struct symbol_table* st)
{
	struct environment* env = st->top;
	while (env) {
		struct environment* prev = env->prev;
		environment_destroy(env);
		env = prev;
	}
	ast_node_destroy(st->numeric_pool);
}

bool symbol_table_is_global(struct symbol_table* st)
{
	return st->top == st->global;
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

		struct ast_node* n = st->numeric_pool->head;
		assert(n);
		do {
			struct type_def* x = n->td;
			assert(x);
			if (x->type == type && x->is_signed == is_signed && x->bit_count == bit_count) {
				*promote = true;
				*c = x;
				return true;
			}
			n = n->next;
		} while (n);
	}

	return false;
}

bool type_find_whole(struct symbol_table* st, struct ast_node* a, struct ast_node* b)
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

		struct ast_node* x = a->head;
		struct ast_node* y = b->head;
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

bool type_def_can_cast(struct type_def* a, struct type_def* b)
{
	if (a == b) {
		return true;
	}

	if (is_numeric(a) && is_numeric(b)) {
		return true;
	}

	return false;
}

bool type_use_can_cast(struct ast_node* a, struct ast_node* b)
{
	if (a && b) {
		struct type_def* td = NULL;
		if (!type_def_can_cast(a->td, b->td)) {
			return false;
		}

		struct ast_node* x = a->head;
		struct ast_node* y = b->head;
		do {
			if (!type_use_can_cast(x, y)) {
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

void transfer_global_symbols(struct symbol_table* src, struct symbol_table* dest)
{
	for (int i = 0; i < src->global->ht.size; i++) {
		struct hash_entry* p = src->global->ht.buckets[i].head;
		while (p) {
			struct symbol* src_sym = p->item;
			struct symbol* dest_sym = NULL;
			malloc_safe(&dest_sym, sizeof(struct symbol));
			symbol_init(dest_sym);
			dest_sym->tk_type = src_sym->tk_type;
			dest_sym->td = type_def_copy(src_sym->td);
			dest_sym->tu = ast_node_copy(src_sym->tu);
			dest_sym->root = ast_node_copy(src_sym->root);
			environment_put(dest->global, &p->value, dest_sym);
			p = p->next;
		}
	}
}

void transfer_module_symbols(struct environment* src, struct environment* dest, struct buffer* module_name)
{
	for (int i = 0; i < src->ht.size; i++) {
		struct hash_entry* p = src->ht.buckets[i].head;
		while (p) {
			struct symbol* src_sym = p->item;
			struct symbol* dest_sym = NULL;
			malloc_safe(&dest_sym, sizeof(struct symbol));
			symbol_init(dest_sym);
			dest_sym->tk_type = src_sym->tk_type;
			dest_sym->td = type_def_copy(src_sym->td);
			dest_sym->tu = ast_node_copy(src_sym->tu);
			dest_sym->root = ast_node_copy(src_sym->root);

			/* value is module_name.sym_name */
			struct buffer value;
			buffer_init(&value);
			buffer_copy(module_name, &value);
			buffer_add_char(&value, '.');
			buffer_copy(&p->value, &value);
			environment_put(dest, &value, dest_sym);

			p = p->next;
		}
	}
}
