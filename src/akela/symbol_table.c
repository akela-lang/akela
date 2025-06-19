#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type.h"
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
    st->top = st->top->prev;
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

    Ake_EnvironmentAdd(st->top, &bf, sym, AKE_SEQ_DEFAULT);

    Zinc_string_destroy(&bf);
}

void Ake_symbol_table_add_type(Ake_symbol_table* st, const char* name, Ake_TypeDef* td)
{
	Zinc_string bf;

	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);

	Ake_symbol* sym = NULL;
	Zinc_malloc_safe((void**)&sym, sizeof(Ake_symbol));
	Ake_symbol_init(sym);
    sym->type = Ake_symbol_type_type;
	sym->td = td;

	Ake_EnvironmentAdd(st->top, &bf, sym, AKE_SEQ_DEFAULT);

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
	Ake_TypeDef* td = NULL;
	Ake_type_def* old = NULL;

	name = "Int8";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_INTEGER;
	Zinc_string_add_str(&td->name, name);
	td->data.integer.bit_count = 8;
	Ake_symbol_table_add_type(st, name, td);

	name = "Int16";
	Ake_TypeDefCreate(&td);
	Ake_TypeDefSet(td, AKE_TYPE_DEF_INTEGER);
	Zinc_string_add_str(&td->name, name);
	td->data.integer.bit_count = 16;
	Ake_symbol_table_add_type(st, name, td);

	name = "Int32";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_INTEGER;
	Zinc_string_add_str(&td->name, name);
	td->data.integer.bit_count = 32;
	Ake_symbol_table_add_type(st, name, td);

	name = "Int64";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_INTEGER;
	Zinc_string_add_str(&td->name, name);
	td->data.integer.bit_count = 64;
	Ake_symbol_table_add_type(st, name, td);

    name = "Nat8";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_NATURAL;
    Zinc_string_add_str(&td->name, name);
    td->data.natural.bit_count = 8;
    Ake_symbol_table_add_type(st, name, td);

	name = "Nat16";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_NATURAL;
	Zinc_string_add_str(&td->name, name);
	td->data.natural.bit_count = 16;
	Ake_symbol_table_add_type(st, name, td);

    name = "Nat32";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_NATURAL;
	Zinc_string_add_str(&td->name, name);
	td->data.natural.bit_count = 32;
	Ake_symbol_table_add_type(st, name, td);

	name = "Nat64";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_NATURAL;
	Zinc_string_add_str(&td->name, name);
	td->data.natural.bit_count = 64;
	Ake_symbol_table_add_type(st, name, td);

	name = "Real16";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_REAL;
	Zinc_string_add_str(&td->name, name);
	td->data.real.bit_count = 16;
	Ake_symbol_table_add_type(st, name, td);

	name = "Real32";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_REAL;
	Zinc_string_add_str(&td->name, name);
	td->data.real.bit_count = 32;
	Ake_symbol_table_add_type(st, name, td);

	name = "Real64";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_REAL;
	Zinc_string_add_str(&td->name, name);
	td->data.real.bit_count = 64;
	Ake_symbol_table_add_type(st, name, td);

	name = "Bool";
	Ake_TypeDefCreate(&td);
	td->kind = AKE_TYPE_DEF_BOOLEAN;
	Zinc_string_add_str(&td->name, name);
	Ake_symbol_table_add_type(st, name, td);
}

void Ake_symbol_table_add_numeric(Ake_symbol_table* st, const char* name)
{
	Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);
	Ake_symbol* sym = Ake_EnvironmentGet(st->top, &bf, AKE_SEQ_ANY);
	assert(sym);
	assert(sym->type == Ake_symbol_type_type);
	assert(sym->td);
    Zinc_list_add_item(&st->numeric_pool, sym->td);
    Zinc_string_destroy(&bf);
}

void Ake_symbol_table_numeric_pool_init(Ake_symbol_table* st)
{
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
	Zinc_list_init(&st->numeric_pool);
	st->id_count = 0;
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

    Zinc_list_destroy(&st->numeric_pool, NULL);
}

bool Ake_is_numeric(Ake_TypeDef* td)
{
	if (td->kind == AKE_TYPE_DEF_INTEGER) {
		return true;
	}
	if (td->kind == AKE_TYPE_DEF_NATURAL) {
		return true;
	}
	if (td->kind == AKE_TYPE_DEF_REAL) {
		return true;
	}

	return false;
}

bool Ake_type_find(Ake_symbol_table* st, Ake_TypeDef* a, Ake_TypeDef* b, bool *promote, Ake_TypeDef** c)
{
	*promote = false;
	*c = NULL;

	if (a == b) {
		return true;
	}

	if (Ake_is_numeric(a) && Ake_is_numeric(b)) {
		*c = a;
		*promote = true;
		return true;
	}

	return false;
}


/**
* Check if types are compatible
* Updates td's in 'a' if it needs to promote type
* Only numeric types can be promoted
* return true if compatible
/* NOLINTNEXTLINE(misc-no-recursion) */
bool Ake_type_find_whole(Ake_symbol_table* st, Ake_type_use* a, Ake_type_use* b)
{
	if (a && b) {
		bool promote;
		Ake_TypeDef* td = NULL;
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
        } else if (a->head || b->head) {
            return false;
        }

		return true;
	}

	if (!a && !b) {
		return true;
	}

	return false;
}

bool Ake_type_def_can_cast(Ake_TypeDef* a, Ake_TypeDef* b)
{
	if (a == b) {
		return true;
	}

	if (a && b && Ake_is_numeric(a) && Ake_is_numeric(b)) {
		return true;
	}

	return false;
}

bool Ake_type_def_match(Ake_type_def* a, Ake_type_def* b)
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
	Ake_EnvironmentAdd(st->top, &bf, sym, AKE_SEQ_DEFAULT);
	Zinc_string_destroy(&bf);
}

Ake_ast* Ake_get_current_function(Ake_symbol_table* st)
{
	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, "|current_function|");
	struct Ake_symbol* sym = Ake_EnvironmentGet(st->top, &bf, AKE_SEQ_ANY);
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