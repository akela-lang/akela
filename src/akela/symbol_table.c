#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type_def.h"
#include <assert.h>
#include "ast.h"
#include "symbol.h"

bool Ake_type_use_can_cast_prototype(Ake_ast* a, Ake_ast* b);
bool Ake_type_use_match(Ake_type_use* a, Ake_type_use* b);

void Ake_environment_begin(struct Ake_symbol_table* st)
{
    struct Ake_environment* env = NULL;
    Ake_environment_create(&env, st->top);
    st->top = env;
}

void Ake_environment_end(struct Ake_symbol_table* st)
{
    struct Ake_environment* env = st->top;
    st->top = env->prev;
    env->prev = st->deactivated;
    st->deactivated = env;
}

void Ake_symbol_table_add_reserved_word(struct Ake_environment* env, const char* name, enum Ake_token_enum tk_type)
{
    struct Zinc_string bf;

    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, name);

    struct Ake_symbol* sym = NULL;
    Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
    Ake_symbol_init(sym);
    sym->type = Ake_symbol_type_reserved_word;
    sym->tk_type = tk_type;

    Ake_environment_put(env, &bf, sym);

    Zinc_string_destroy(&bf);
}

void Ake_symbol_table_add_type(struct Ake_environment* env, const char* name, struct Ake_type_def* td)
{
	struct Zinc_string bf;

	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);

	struct Ake_symbol* sym = NULL;
	Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
	Ake_symbol_init(sym);
    sym->type = Ake_symbol_type_type;
	sym->td = td;

	Ake_environment_put(env, &bf, sym);

	Zinc_string_destroy(&bf);
}

void Ake_symbol_table_init_reserved(struct Ake_environment* env)
{
	Ake_symbol_table_add_reserved_word(env, "fn", Ake_token_fn);
	Ake_symbol_table_add_reserved_word(env, "end", Ake_token_end);
	Ake_symbol_table_add_reserved_word(env, "if", Ake_token_if);
	Ake_symbol_table_add_reserved_word(env, "elseif", Ake_token_elseif);
	Ake_symbol_table_add_reserved_word(env, "else", Ake_token_else);
	Ake_symbol_table_add_reserved_word(env, "while", Ake_token_while);
	Ake_symbol_table_add_reserved_word(env, "for", Ake_token_for);
	Ake_symbol_table_add_reserved_word(env, "in", Ake_token_in);
	Ake_symbol_table_add_reserved_word(env, "let", Ake_token_let);
	Ake_symbol_table_add_reserved_word(env, "true", Ake_token_boolean);
	Ake_symbol_table_add_reserved_word(env, "false", Ake_token_boolean);
	Ake_symbol_table_add_reserved_word(env, "module", Ake_token_module);
	Ake_symbol_table_add_reserved_word(env, "struct", Ake_token_struct);
	Ake_symbol_table_add_reserved_word(env, "return", Ake_token_return);
    Ake_symbol_table_add_reserved_word(env, "extern", Ake_token_extern);
    Ake_symbol_table_add_reserved_word(env, "mut", Ake_token_mut);
    Ake_symbol_table_add_reserved_word(env, "const", Ake_token_const);
    Ake_symbol_table_add_reserved_word(env, "impl", Ake_token_impl);
    Ake_symbol_table_add_reserved_word(env, "self", Ake_token_self);
}

void Ake_symbol_table_init_builtin_types(struct Ake_symbol_table* st, struct Ake_environment* env)
{
	const char* name;
	struct Ake_type_def* td = NULL;

	name = "Int32";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 32;
	Ake_symbol_table_add_type(env, name, td);

	name = "Int64";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 64;
	Ake_symbol_table_add_type(env, name, td);

    name = "Nat8";
    Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
    Ake_type_def_init(td);
    td->type = Ake_type_integer;
    Zinc_string_add_str(&td->name, name);
    td->bit_count = 8;
    Ake_symbol_table_add_type(env, name, td);

    name = "Nat32";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 32;
	Ake_symbol_table_add_type(env, name, td);

	name = "Nat64";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 64;
	Ake_symbol_table_add_type(env, name, td);

	name = "Real32";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_float;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 32;
	Ake_symbol_table_add_type(env, name, td);

	name = "Real64";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_float;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 64;
	Ake_symbol_table_add_type(env, name, td);

	name = "Bool";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_boolean;
	Zinc_string_add_str(&td->name, name);
	Ake_symbol_table_add_type(env, name, td);

    name = "Function";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_function;
	Zinc_string_add_str(&td->name, name);
	Ake_symbol_table_add_type(env, name, td);
    st->function_type_def = td;

	name = "Module";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_module;
	Zinc_string_add_str(&td->name, name);
	Ake_symbol_table_add_type(env, name, td);
}

void Ake_symbol_table_add_numeric(struct Ake_symbol_table* st, const char* name)
{
	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);
	struct Ake_symbol* sym = Ake_environment_get(st->top, &bf);
	assert(sym);
	assert(sym->td);
	Ake_type_use* tu = NULL;
    Ake_type_use_create(&tu);
	tu->td = sym->td;
    Ake_type_use_add(st->numeric_pool, tu);
    Zinc_string_destroy(&bf);
}

void Ake_symbol_table_numeric_pool_init(struct Ake_symbol_table* st)
{
	Ake_type_use* pool = NULL;
	Ake_type_use_create(&pool);
	st->numeric_pool = pool;

	Ake_symbol_table_add_numeric(st, "Int32");
	Ake_symbol_table_add_numeric(st, "Int64");
    Ake_symbol_table_add_numeric(st, "Nat8");
	Ake_symbol_table_add_numeric(st, "Nat32");
	Ake_symbol_table_add_numeric(st, "Nat64");
	Ake_symbol_table_add_numeric(st, "Real32");
	Ake_symbol_table_add_numeric(st, "Real64");
}

void Ake_symbol_table_init(struct Ake_symbol_table* st)
{
	struct Ake_environment* env = NULL;
	Zinc_malloc_safe((void**)&env, sizeof(struct Ake_environment));
	Ake_environment_init(env, NULL);
	st->top = env;
	st->initial = env;
	Ake_symbol_table_init_reserved(env);
	Ake_symbol_table_init_builtin_types(st, env);
	Ake_symbol_table_numeric_pool_init(st);

	env = NULL;
	Zinc_malloc_safe((void**)&env, sizeof(struct Ake_environment));
	Ake_environment_init(env, st->top);
	st->top = env;
	st->global = env;
    st->deactivated = NULL;
    st->id_count = 0;
}

void Ake_symbol_table_create(struct Ake_symbol_table** st)
{
    Zinc_malloc_safe((void**)st, sizeof(struct Ake_symbol_table));
    Ake_symbol_table_init(*st);
}

void Ake_symbol_table_destroy(struct Ake_symbol_table* st)
{
	struct Ake_environment* env = st->top;
	while (env) {
		struct Ake_environment* prev = env->prev;
		Ake_environment_destroy(env);
		env = prev;
	}
    env = st->deactivated;
    while (env) {
        struct Ake_environment* prev = env->prev;
        Ake_environment_destroy(env);
        env = prev;
    }
    Ake_type_use_destroy(st->numeric_pool);
}

bool Ake_symbol_table_is_global(struct Ake_symbol_table* st)
{
	return st->top == st->global;
}

bool Ake_is_numeric(struct Ake_type_def* td)
{
	return td->type == Ake_type_integer || td->type == Ake_type_float;
}

bool Ake_type_find(struct Ake_symbol_table* st, struct Ake_type_def* a, struct Ake_type_def* b, bool *promote, struct Ake_type_def** c)
{
	*promote = false;
	*c = NULL;

	if (a == b) {
		return true;
	}

	if (Ake_is_numeric(a) && Ake_is_numeric(b)) {
		enum Ake_type type = a->type;

		if (b->type == Ake_type_float) {
			type = b->type;
		}

		bool is_signed = false;
		if (type == Ake_type_integer) {
			is_signed = a->is_signed;
			if (b->is_signed) {
				is_signed = b->is_signed;
			}
		}

		int bit_count = a->bit_count;
		if (b->bit_count > bit_count) {
			bit_count = b->bit_count;
		}

		Ake_type_use* tu = st->numeric_pool->head;
		assert(tu);
		do {
			struct Ake_type_def* x = tu->td;
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
bool Ake_type_find_whole(struct Ake_symbol_table* st, Ake_type_use* a, Ake_type_use* b)
{
	if (a && b) {
		bool promote;
		struct Ake_type_def* td = NULL;
		if (!Ake_type_find(st, a->td, b->td, &promote, &td)) {
			return false;
		}
		if (promote) {
			a->td = td;
		}

        if (a->head && b->head) {
            Ake_type_use* x = a->head;
            Ake_type_use* y = b->head;
            do {
                if (!Ake_type_find_whole(st, x, y)) {
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

bool Ake_type_def_can_cast(struct Ake_type_def* a, struct Ake_type_def* b)
{
	if (a == b) {
		return true;
	}

	if (a && b && Ake_is_numeric(a) && Ake_is_numeric(b)) {
		return true;
	}

	return false;
}

bool Ake_type_def_match(struct Ake_type_def* a, struct Ake_type_def* b)
{
    if (a == b) {
        return true;
    }

    return false;
}

bool Ake_type_def_should_match(Ake_type_use* a, Ake_type_use* b)
{
    if (a->type == Ake_type_use_function_inputs || a->type == Ake_type_use_function_outputs) {
        return true;
    }

    if (b->type == Ake_type_use_function_inputs || b->type == Ake_type_use_function_outputs) {
        return true;
    }

    return false;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_type_use_can_cast(Ake_type_use* a, Ake_type_use* b)
{
    if (a && b) {
        if (!Ake_type_def_can_cast(a->td, b->td)) {
            return false;
        }

        if (a->head && b->head) {
            Ake_type_use* x = a->head;
            Ake_type_use* y = b->head;
            while (x || y) {
                if (Ake_type_def_should_match(a, b)) {
                    if (!Ake_type_use_match(x, y)) {
                        return false;
                    }
                } else {
                    if (!Ake_type_use_can_cast(x, y)) {
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

/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_type_use_match(Ake_type_use* a, Ake_type_use* b)
{
    if (a && b) {
        if (a->td != b->td) {
            return false;
        }

        Ake_type_use* x = a->head;
        Ake_type_use* y = b->head;
        while (x || y) {
            if (Ake_type_def_should_match(a, b)) {
                if (!Ake_type_use_match(x, y)) {
                    return false;
                }
            } else {
                if (!Ake_type_use_can_cast(x, y)) {
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
bool Ake_type_use_can_cast_prototype(Ake_ast* a, Ake_ast* b)
{
	if (a && b) {
        if (!Ake_type_use_match(a->tu, b->tu)) {
            return false;
        }

        Ake_ast *x = a->head;
        Ake_ast *y = b->head;
        do {
            if (!Ake_type_use_can_cast_prototype(x, y)) {
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

void Ake_transfer_global_symbols(struct Ake_symbol_table* src, struct Ake_symbol_table* dest)
{
	for (int i = 0; i < src->global->ht.size; i++) {
		struct Zinc_hash_map_string_entry* p = src->global->ht.buckets[i].head;
		while (p) {
			struct Ake_symbol* src_sym = (struct Ake_symbol*)p->item;
			struct Ake_symbol* dest_sym = Ake_symbol_copy(src_sym);
			Ake_environment_put(dest->global, &p->value, dest_sym);
			p = p->next;
		}
	}
}

void Ake_transfer_module_symbols(struct Ake_environment* src, struct Ake_environment* dest, struct Zinc_string* module_name)
{
	for (int i = 0; i < src->ht.size; i++) {
		struct Zinc_hash_map_string_entry* p = src->ht.buckets[i].head;
		while (p) {
			struct Ake_symbol* src_sym = (struct Ake_symbol*)p->item;
			struct Ake_symbol* dest_sym = Ake_symbol_copy(src_sym);

			/* value is module_name.sym_name */
			struct Zinc_string value;
			Zinc_string_init(&value);
			Zinc_string_copy(module_name, &value);
			Zinc_string_add_char(&value, '.');
			Zinc_string_copy(&p->value, &value);
			Ake_environment_put(dest, &value, dest_sym);
            Zinc_string_destroy(&value);
			p = p->next;
		}
	}
}

void Ake_set_current_function(struct Ake_environment* env, Ake_ast* fd)
{
	struct Ake_symbol* sym = NULL;
	Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
	Ake_symbol_init(sym);
	sym->type = Ake_symbol_type_info;
	sym->root_ptr = fd;

	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, "|current_function|");
	Ake_environment_put(env, &bf, sym);
	Zinc_string_destroy(&bf);
}

Ake_ast* Ake_get_current_function(struct Ake_environment* env)
{
	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, "|current_function|");
	struct Ake_symbol* sym = Ake_environment_get(env, &bf);
	Zinc_string_destroy(&bf);
	if (sym) {
		return sym->root_ptr;
	} else {
		return NULL;
	}
}

size_t Ake_symbol_table_generate_id(struct Ake_symbol_table* st)
{
    return st->id_count++;
}

void Ake_symbol_table_print(struct Ake_symbol_table* st)
{
    printf("\n");
    struct Ake_environment* p = st->top;
    while (p) {
        struct Zinc_hash_table* ht = &p->ht;
        for (int i = 0; i < ht->size; i++) {
            struct Zinc_hash_map_string_list* list = &ht->buckets[i];
            struct Zinc_hash_map_string_entry* entry = list->head;
            while (entry) {
                Zinc_string_finish(&entry->value);
                printf("%s ", entry->value.buf);
                struct Ake_symbol* sym = entry->item;
                if (sym->td) {
                    Zinc_string_finish(&sym->td->name);
                    printf("%s %d %d %d", sym->td->name.buf, sym->td->type, sym->td->bit_count, sym->td->is_signed);
                }
                printf("\n");
                entry = entry->next;
            }
        }
        p = p->prev;
    }

    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "Int64");
    struct Ake_symbol* sym = Ake_environment_get(st->top, &bf);
    assert(sym && sym->td);
    assert(sym->td->bit_count == 64);
    assert(sym->td->is_signed);
    Zinc_string_destroy(&bf);
}