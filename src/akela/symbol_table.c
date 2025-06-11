#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type_def.h"
#include <assert.h>
#include "ast.h"
#include "symbol.h"

bool Ake_type_use_can_cast_prototype(Ake_ast* a, Ake_ast* b);
bool Ake_type_use_match(Ake_type_use* a, Ake_type_use* b);

void Ake_begin_environment(struct Ake_symbol_table* st, Ake_ast* n)
{
    Ake_Environment* env = NULL;
    Ake_EnvironmentCreate(&env, st->top);
    st->top = env;
	if (n) {
		n->env = env;
	}
}

void Ake_end_environment(struct Ake_symbol_table* st)
{
    Ake_Environment* env = st->top;
    st->top = env->prev;
    env->prev = st->deactivated;
    st->deactivated = env;
}

void Ake_symbol_table_add_reserved_word(Ake_symbol_table* st, const char* name, Ake_token_enum tk_type)
{
    struct Zinc_string bf;

    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, name);

    struct Ake_symbol* sym = NULL;
    Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
    Ake_symbol_init(sym);
    sym->type = Ake_symbol_type_reserved_word;
    sym->tk_type = tk_type;

    Ake_EnvironmentAdd(st, &bf, sym);

    Zinc_string_destroy(&bf);
}

void Ake_symbol_table_add_type(Ake_symbol_table* st, const char* name, Ake_type_def* td)
{
	struct Zinc_string bf;

	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);

	struct Ake_symbol* sym = NULL;
	Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
	Ake_symbol_init(sym);
        sym->type = Ake_symbol_type_type;
	sym->td = td;

	Ake_EnvironmentAdd(st, &bf, sym);

	Zinc_string_destroy(&bf);
}

void Ake_symbol_table_init_reserved(Ake_symbol_table* st)
{
	Ake_symbol_table_add_reserved_word(st, "fn", Ake_token_fn);
	Ake_symbol_table_add_reserved_word(st, "end", Ake_token_end);
	Ake_symbol_table_add_reserved_word(st, "if", Ake_token_if);
	Ake_symbol_table_add_reserved_word(st, "elseif", Ake_token_elseif);
	Ake_symbol_table_add_reserved_word(st, "else", Ake_token_else);
	Ake_symbol_table_add_reserved_word(st, "while", Ake_token_while);
	Ake_symbol_table_add_reserved_word(st, "for", Ake_token_for);
	Ake_symbol_table_add_reserved_word(st, "in", Ake_token_in);
	Ake_symbol_table_add_reserved_word(st, "true", Ake_token_boolean);
	Ake_symbol_table_add_reserved_word(st, "false", Ake_token_boolean);
	Ake_symbol_table_add_reserved_word(st, "struct", Ake_token_struct);
	Ake_symbol_table_add_reserved_word(st, "return", Ake_token_return);
    Ake_symbol_table_add_reserved_word(st, "extern", Ake_token_extern);
    Ake_symbol_table_add_reserved_word(st, "mut", Ake_token_mut);
    Ake_symbol_table_add_reserved_word(st, "const", Ake_token_const);
	Ake_symbol_table_add_reserved_word(st, "var", Ake_token_var);
    Ake_symbol_table_add_reserved_word(st, "impl", Ake_token_impl);
    Ake_symbol_table_add_reserved_word(st, "self", Ake_token_self);
}

void Ake_symbol_table_init_builtin_types(Ake_symbol_table* st)
{
	const char* name;
	Ake_type_def* td = NULL;

	name = "Int8";
	Zinc_malloc_safe((void**)&td, sizeof(Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 8;
	Ake_symbol_table_add_type(st, name, td);

	name = "Int16";
	Zinc_malloc_safe((void**)&td, sizeof(Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 16;
	Ake_symbol_table_add_type(st, name, td);

	name = "Int32";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 32;
	Ake_symbol_table_add_type(st, name, td);

	name = "Int64";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->is_signed = true;
	td->bit_count = 64;
	Ake_symbol_table_add_type(st, name, td);

    name = "Nat8";
    Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
    Ake_type_def_init(td);
    td->type = Ake_type_integer;
    Zinc_string_add_str(&td->name, name);
    td->bit_count = 8;
    Ake_symbol_table_add_type(st, name, td);

	name = "Nat16";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 16;
	Ake_symbol_table_add_type(st, name, td);

    name = "Nat32";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 32;
	Ake_symbol_table_add_type(st, name, td);

	name = "Nat64";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_integer;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 64;
	Ake_symbol_table_add_type(st, name, td);

	name = "Real16";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_float;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 16;
	td->is_signed = false;
	Ake_symbol_table_add_type(st, name, td);

	name = "Real32";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_float;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 32;
	td->is_signed = false;
	Ake_symbol_table_add_type(st, name, td);

	name = "Real64";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_float;
	Zinc_string_add_str(&td->name, name);
	td->bit_count = 64;
	td->is_signed = false;
	Ake_symbol_table_add_type(st, name, td);

	name = "Bool";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_boolean;
	Zinc_string_add_str(&td->name, name);
	Ake_symbol_table_add_type(st, name, td);

    name = "Function";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_function;
	Zinc_string_add_str(&td->name, name);
	Ake_symbol_table_add_type(st, name, td);

	name = "Module";
	Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
	Ake_type_def_init(td);
	td->type = Ake_type_module;
	Zinc_string_add_str(&td->name, name);
	Ake_symbol_table_add_type(st, name, td);
}

void Ake_symbol_table_add_numeric(struct Ake_symbol_table* st, const char* name)
{
	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);
	struct Ake_symbol* sym = Ake_EnvironmentGet(st, &bf);
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
	st->deactivated = NULL;
	st->id_count = 0;
	st->count = 0;
	st->top = NULL;

	Ake_begin_environment(st, NULL);
	Ake_symbol_table_init_reserved(st);
	Ake_symbol_table_init_builtin_types(st);
	Ake_symbol_table_numeric_pool_init(st);
}

void Ake_symbol_table_create(Ake_symbol_table** st)
{
    Zinc_malloc_safe((void**)st, sizeof(Ake_symbol_table));
    Ake_symbol_table_init(*st);
}

void Ake_symbol_table_destroy(Ake_symbol_table* st)
{
	Ake_Environment* env = st->top;
	while (env) {
		Ake_Environment* prev = env->prev;
		Ake_EnvironmentDestroy(env);
		free(env);
		env = prev;
	}
    env = st->deactivated;
    while (env) {
        Ake_Environment* prev = env->prev;
        Ake_EnvironmentDestroy(env);
    	free(env);
        env = prev;
    }
    Ake_type_use_destroy(st->numeric_pool);
}

bool Ake_is_numeric(struct Ake_type_def* td)
{
	return td->type == Ake_type_integer || td->type == Ake_type_float;
}

bool Ake_type_find(Ake_symbol_table* st, Ake_type_def* a, Ake_type_def* b, bool *promote, Ake_type_def** c)
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
			Ake_type_def* x = tu->td;
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

void Ake_set_current_function(Ake_symbol_table* st, Ake_ast* fd)
{
	struct Ake_symbol* sym = NULL;
	Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
	Ake_symbol_init(sym);
	sym->type = Ake_symbol_type_info;
	sym->root_ptr = fd;

	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, "|current_function|");
	Ake_EnvironmentAdd(st, &bf, sym);
	Zinc_string_destroy(&bf);
}

Ake_ast* Ake_get_current_function(Ake_symbol_table* st)
{
	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, "|current_function|");
	struct Ake_symbol* sym = Ake_EnvironmentGet(st, &bf);
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

size_t Ake_symbol_table_get_seq_num(Ake_symbol_table* st)
{
	return st->count++;
}

Ake_Environment* Ake_get_current_env(Ake_ast* n)
{
	while (n) {
		if (n->env) {
			return n->env;
		}
		n = n->parent;
	}

	return NULL;
}