#include "zinc/memory.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "type.h"
#include <assert.h>
#include "ast.h"
#include "symbol.h"

bool Ake_type_use_can_cast_prototype(Ake_Ast* a, Ake_Ast* b);
bool Ake_type_use_match(Ake_type_use* a, Ake_type_use* b);

void Ake_begin_environment(struct Ake_symbol_table* st, Ake_Ast* n)
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

    struct Ake_Symbol* sym = NULL;
    Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_Symbol));
    Ake_SymbolInit(sym);
    sym->kind = AKE_SYMBOL_RESERVED_WORD;
    sym->tk_type = tk_type;

    Ake_EnvironmentAdd(st->top, &bf, sym, AKE_SEQ_DEFAULT);

    Zinc_string_destroy(&bf);
}

void Ake_symbol_table_add_type(Ake_symbol_table* st, const char* name, Ake_Type* td)
{
	Zinc_string bf;

	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);

	Ake_symbol* sym = NULL;
	Zinc_malloc_safe((void**)&sym, sizeof(Ake_symbol));
	Ake_SymbolInit(sym);
    sym->kind = AKE_SYMBOL_TYPE;
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
    Ake_symbol_table_add_reserved_word(st, "const", Ake_token_const);
	Ake_symbol_table_add_reserved_word(st, "var", Ake_token_var);
    Ake_symbol_table_add_reserved_word(st, "impl", Ake_token_impl);
    Ake_symbol_table_add_reserved_word(st, "self", Ake_token_self);
}

void Ake_symbol_table_init_builtin_types(Ake_symbol_table* st)
{
	const char* name;
	Ake_Type* type = NULL;
	Ake_type_def* old = NULL;

	name = "Int8";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_INTEGER;
	Zinc_string_add_str(&type->name, name);
	type->data.integer.bit_count = 8;
	Ake_symbol_table_add_type(st, name, type);

	name = "Int16";
	Ake_TypeCreate(&type);
	Ake_TypeSet(type, AKE_TYPE_INTEGER);
	Zinc_string_add_str(&type->name, name);
	type->data.integer.bit_count = 16;
	Ake_symbol_table_add_type(st, name, type);

	name = "Int32";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_INTEGER;
	Zinc_string_add_str(&type->name, name);
	type->data.integer.bit_count = 32;
	Ake_symbol_table_add_type(st, name, type);

	name = "Int64";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_INTEGER;
	Zinc_string_add_str(&type->name, name);
	type->data.integer.bit_count = 64;
	Ake_symbol_table_add_type(st, name, type);

    name = "Nat8";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_NATURAL;
    Zinc_string_add_str(&type->name, name);
    type->data.natural.bit_count = 8;
    Ake_symbol_table_add_type(st, name, type);

	name = "Nat16";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_NATURAL;
	Zinc_string_add_str(&type->name, name);
	type->data.natural.bit_count = 16;
	Ake_symbol_table_add_type(st, name, type);

    name = "Nat32";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_NATURAL;
	Zinc_string_add_str(&type->name, name);
	type->data.natural.bit_count = 32;
	Ake_symbol_table_add_type(st, name, type);

	name = "Nat64";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_NATURAL;
	Zinc_string_add_str(&type->name, name);
	type->data.natural.bit_count = 64;
	Ake_symbol_table_add_type(st, name, type);

	name = "Real16";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_REAL;
	Zinc_string_add_str(&type->name, name);
	type->data.real.bit_count = 16;
	Ake_symbol_table_add_type(st, name, type);

	name = "Real32";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_REAL;
	Zinc_string_add_str(&type->name, name);
	type->data.real.bit_count = 32;
	Ake_symbol_table_add_type(st, name, type);

	name = "Real64";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_REAL;
	Zinc_string_add_str(&type->name, name);
	type->data.real.bit_count = 64;
	Ake_symbol_table_add_type(st, name, type);

	name = "Bool";
	Ake_TypeCreate(&type);
	type->kind = AKE_TYPE_BOOLEAN;
	Zinc_string_add_str(&type->name, name);
	Ake_symbol_table_add_type(st, name, type);
}

void Ake_symbol_table_add_numeric(Ake_symbol_table* st, const char* name)
{
	Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, name);
	Ake_symbol* sym = Ake_EnvironmentGet(st->top, &bf, AKE_SEQ_ANY);
	assert(sym);
	assert(sym->kind == AKE_SYMBOL_TYPE);
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

bool Ake_is_numeric(Ake_Type* td)
{
	if (td->kind == AKE_TYPE_INTEGER) {
		return true;
	}
	if (td->kind == AKE_TYPE_NATURAL) {
		return true;
	}
	if (td->kind == AKE_TYPE_REAL) {
		return true;
	}

	return false;
}

void Ake_set_current_function(Ake_symbol_table* st, Ake_Ast* fd)
{
	struct Ake_Symbol* sym = NULL;
	Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_Symbol));
	Ake_SymbolInit(sym);
	sym->kind = AKE_SYMBOL_INFO;
	sym->root_ptr = fd;

	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, "|current_function|");
	Ake_EnvironmentAdd(st->top, &bf, sym, AKE_SEQ_DEFAULT);
	Zinc_string_destroy(&bf);
}

Ake_Ast* Ake_get_current_function(Ake_symbol_table* st)
{
	struct Zinc_string bf;
	Zinc_string_init(&bf);
	Zinc_string_add_str(&bf, "|current_function|");
	struct Ake_Symbol* sym = Ake_EnvironmentGet(st->top, &bf, AKE_SEQ_ANY);
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

Ake_Environment* Ake_get_current_env(Ake_Ast* n)
{
	while (n) {
		if (n->env) {
			return n->env;
		}
		n = n->parent;
	}

	return NULL;
}