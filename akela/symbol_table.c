#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type_def.h"
#include <assert.h>
#include "ast.h"
#include "symbol.h"

bool type_use_can_cast_prototype(Ast_node* a, Ast_node* b);
bool type_use_match(Type_use* a, Type_use* b);

void environment_begin(struct symbol_table* st)
{
    struct environment* env = NULL;
    environment_create(&env, st->top);
    st->top = env;
}

void environment_end(struct symbol_table* st)
{
    struct environment* env = st->top;
    st->top = env->prev;
    env->prev = st->deactivated;
    st->deactivated = env;
}

void symbol_table_add_reserved_word(struct environment* env, const char* name, enum token_enum tk_type)
{
    struct buffer bf;

    buffer_init(&bf);
    buffer_copy_str(&bf, name);

    struct symbol* sym = NULL;
    malloc_safe((void**)&sym, sizeof(struct symbol));
    symbol_init(sym);
    sym->type = Symbol_type_reserved_word;
    sym->tk_type = tk_type;

    environment_put(env, &bf, sym);

    buffer_destroy(&bf);
}

void symbol_table_add_type(struct environment* env, const char* name, struct type_def* td)
{
	struct buffer bf;

	buffer_init(&bf);
	buffer_copy_str(&bf, name);

	struct symbol* sym = NULL;
	malloc_safe((void**)&sym, sizeof(struct symbol));
	symbol_init(sym);
    sym->type = Symbol_type_type;
	sym->td = td;

	environment_put(env, &bf, sym);

	buffer_destroy(&bf);
}

void symbol_table_init_reserved(struct environment* env)
{
	symbol_table_add_reserved_word(env, "fn", token_fn);
	symbol_table_add_reserved_word(env, "end", token_end);
	symbol_table_add_reserved_word(env, "if", token_if);
	symbol_table_add_reserved_word(env, "elseif", token_elseif);
	symbol_table_add_reserved_word(env, "else", token_else);
	symbol_table_add_reserved_word(env, "while", token_while);
	symbol_table_add_reserved_word(env, "for", token_for);
	symbol_table_add_reserved_word(env, "in", token_in);
	symbol_table_add_reserved_word(env, "let", token_let);
	symbol_table_add_reserved_word(env, "true", token_boolean);
	symbol_table_add_reserved_word(env, "false", token_boolean);
	symbol_table_add_reserved_word(env, "module", token_module);
	symbol_table_add_reserved_word(env, "struct", token_struct);
	symbol_table_add_reserved_word(env, "return", token_return);
    symbol_table_add_reserved_word(env, "extern", token_extern);
    symbol_table_add_reserved_word(env, "mut", token_mut);
    symbol_table_add_reserved_word(env, "const", token_const);
    symbol_table_add_reserved_word(env, "impl", token_impl);
    symbol_table_add_reserved_word(env, "self", token_self);
}

void symbol_table_init_builtin_types(struct symbol_table* st, struct environment* env)
{
	const char* name;
	struct type_def* td = NULL;

	name = "i32";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 32;
	symbol_table_add_type(env, name, td);

	name = "i64";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 64;
	symbol_table_add_type(env, name, td);

    name = "u8";
    malloc_safe((void**)&td, sizeof(struct type_def));
    type_def_init(td);
    td->type = type_integer;
    buffer_copy_str(&td->name, name);
    td->bit_count = 8;
    symbol_table_add_type(env, name, td);

    name = "u32";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->bit_count = 32;
	symbol_table_add_type(env, name, td);

	name = "u64";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_integer;
	buffer_copy_str(&td->name, name);
	td->bit_count = 64;
	symbol_table_add_type(env, name, td);

	name = "f32";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_float;
	buffer_copy_str(&td->name, name);
	td->bit_count = 32;
	symbol_table_add_type(env, name, td);

	name = "f64";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_float;
	buffer_copy_str(&td->name, name);
	td->bit_count = 64;
	symbol_table_add_type(env, name, td);

	name = "bool";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_boolean;
	buffer_copy_str(&td->name, name);
	symbol_table_add_type(env, name, td);

    name = "Function";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_function;
	buffer_copy_str(&td->name, name);
	symbol_table_add_type(env, name, td);
    st->function_type_def = td;

	name = "Module";
	malloc_safe((void**)&td, sizeof(struct type_def));
	type_def_init(td);
	td->type = type_module;
	buffer_copy_str(&td->name, name);
	symbol_table_add_type(env, name, td);
}

void symbol_table_add_numeric(struct symbol_table* st, const char* name)
{
	struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, name);
	struct symbol* sym = environment_get(st->top, &bf);
	assert(sym);
	assert(sym->td);
	Type_use* tu = NULL;
    Type_use_create(&tu);
	tu->td = sym->td;
    Type_use_add(st->numeric_pool, tu);
    buffer_destroy(&bf);
}

void symbol_table_numeric_pool_init(struct symbol_table* st)
{
	Type_use* pool = NULL;
	Type_use_create(&pool);
	st->numeric_pool = pool;

	symbol_table_add_numeric(st, "i32");
	symbol_table_add_numeric(st, "i64");
    symbol_table_add_numeric(st, "u8");
	symbol_table_add_numeric(st, "u32");
	symbol_table_add_numeric(st, "u64");
	symbol_table_add_numeric(st, "f32");
	symbol_table_add_numeric(st, "f64");
}

void symbol_table_init(struct symbol_table* st)
{
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, NULL);
	st->top = env;
	st->initial = env;
	symbol_table_init_reserved(env);
	symbol_table_init_builtin_types(st, env);
	symbol_table_numeric_pool_init(st);

	env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, st->top);
	st->top = env;
	st->global = env;
    st->deactivated = NULL;
    st->id_count = 0;
}

void symbol_table_create(struct symbol_table** st)
{
    malloc_safe((void**)st, sizeof(struct symbol_table));
    symbol_table_init(*st);
}

void symbol_table_destroy(struct symbol_table* st)
{
	struct environment* env = st->top;
	while (env) {
		struct environment* prev = env->prev;
		environment_destroy(env);
		env = prev;
	}
    env = st->deactivated;
    while (env) {
        struct environment* prev = env->prev;
        environment_destroy(env);
        env = prev;
    }
    Type_use_destroy(st->numeric_pool);
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

		Type_use* tu = st->numeric_pool->head;
		assert(tu);
		do {
			struct type_def* x = tu->td;
			assert(x);
			if (x->type == type && x->is_signed == is_signed && x->bit_count == bit_count) {
				*promote = true;
				*c = x;
				return true;
			}
            tu = tu->next;
		} while (tu);
	}

	return false;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool type_find_whole(struct symbol_table* st, Type_use* a, Type_use* b)
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

        if (a->head && b->head) {
            Type_use* x = a->head;
            Type_use* y = b->head;
            do {
                if (!type_find_whole(st, x, y)) {
                    return false;
                }
                if (x) x = x->next;
                if (y) y = y->next;
            } while (x || y);
        } else if (a->head) {
            return false;
        } else if (b->head) {
            return false;
        }

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

	if (a && b && is_numeric(a) && is_numeric(b)) {
		return true;
	}

	return false;
}

bool type_def_match(struct type_def* a, struct type_def* b)
{
    if (a == b) {
        return true;
    }

    return false;
}

bool type_def_should_match(Type_use* a, Type_use* b)
{
    if (a->type == Type_use_function_inputs || a->type == Type_use_function_outputs) {
        return true;
    }

    if (b->type == Type_use_function_inputs || b->type == Type_use_function_outputs) {
        return true;
    }

    return false;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool type_use_can_cast(Type_use* a, Type_use* b)
{
    if (a && b) {
        if (!type_def_can_cast(a->td, b->td)) {
            return false;
        }

        if (a->head && b->head) {
            Type_use* x = a->head;
            Type_use* y = b->head;
            while (x || y) {
                if (type_def_should_match(a, b)) {
                    if (!type_use_match(x, y)) {
                        return false;
                    }
                } else {
                    if (!type_use_can_cast(x, y)) {
                        return false;
                    }
                }
                if (x) {
                    x = x->next;
                }
                if (y) {
                    y = y->next;
                }
            }
        }
    } else if (a) {
        return false;
    } else if (b) {
        return false;
    }

    return true;
}

bool type_use_match(Type_use* a, Type_use* b)
{
    if (a && b) {
        if (a->td != b->td) {
            return false;
        }

        Type_use* x = a->head;
        Type_use* y = b->head;
        while (x || y) {
            if (type_def_should_match(a, b)) {
                if (!type_use_match(x, y)) {
                    return false;
                }
            } else {
                if (!type_use_can_cast(x, y)) {
                    return false;
                }
            }

            if (x) {
                x = x->next;
            }
            if (y) {
                y = y->next;
            }
        }
    } else if (a) {
        return false;
    } else if (b) {
        return false;
    }

    return true;
}

/**
 * Check if the types are be cast
 * @param a type use
 * @param b type use
 * @param in_prototype be more strict if in a prototype
 * @return
 */
/* NOLINTNEXTLINE(misc-no-recursion) */
bool type_use_can_cast_prototype(Ast_node* a, Ast_node* b)
{
	if (a && b) {
        if (!type_use_match(a->tu, b->tu)) {
            return false;
        }

        Ast_node *x = a->head;
        Ast_node *y = b->head;
        do {
            if (!type_use_can_cast_prototype(x, y)) {
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
			struct symbol* src_sym = (struct symbol*)p->item;
			struct symbol* dest_sym = symbol_copy(src_sym);
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
			struct symbol* src_sym = (struct symbol*)p->item;
			struct symbol* dest_sym = symbol_copy(src_sym);

			/* value is module_name.sym_name */
			struct buffer value;
			buffer_init(&value);
			buffer_copy(module_name, &value);
			buffer_add_char(&value, '.');
			buffer_copy(&p->value, &value);
			environment_put(dest, &value, dest_sym);
            buffer_destroy(&value);
			p = p->next;
		}
	}
}

void set_current_function(struct environment* env, Ast_node* fd)
{
	struct symbol* sym = NULL;
	malloc_safe((void**)&sym, sizeof(struct symbol));
	symbol_init(sym);
	sym->type = Symbol_type_info;
	sym->root_ptr = fd;

	struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, "|current_function|");
	environment_put(env, &bf, sym);
	buffer_destroy(&bf);
}

Ast_node* get_current_function(struct environment* env)
{
	struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, "|current_function|");
	struct symbol* sym = environment_get(env, &bf);
	buffer_destroy(&bf);
	if (sym) {
		return sym->root_ptr;
	} else {
		return NULL;
	}
}

size_t symbol_table_generate_id(struct symbol_table* st)
{
    return st->id_count++;
}

void symbol_table_print(struct symbol_table* st)
{
    printf("\n");
    struct environment* p = st->top;
    while (p) {
        struct hash_table* ht = &p->ht;
        for (int i = 0; i < ht->size; i++) {
            struct hash_list* list = &ht->buckets[i];
            struct hash_entry* entry = list->head;
            while (entry) {
                buffer_finish(&entry->value);
                printf("%s ", entry->value.buf);
                struct symbol* sym = entry->item;
                if (sym->td) {
                    buffer_finish(&sym->td->name);
                    printf("%s %d %d %d", sym->td->name.buf, sym->td->type, sym->td->bit_count, sym->td->is_signed);
                }
                printf("\n");
                entry = entry->next;
            }
        }
        p = p->prev;
    }

    struct buffer bf;
    buffer_init(&bf);
    buffer_copy_str(&bf, "Int64");
    struct symbol* sym = environment_get(st->top, &bf);
    assert(sym && sym->td);
    assert(sym->td->bit_count == 64);
    assert(sym->td->is_signed);
    buffer_destroy(&bf);
}