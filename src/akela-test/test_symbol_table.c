#include "zinc/unit_test.h"
#include "zinc/memory.h"
#include "akela/symbol_table.h"
#include "akela/symbol.h"

void test_symbol_table_env()
{
	test_name(__func__);

	struct Ake_environment* env;

	malloc_safe((void**)&env, sizeof(struct Ake_environment));
	Ake_environment_init(env, NULL);

	struct buffer value;
	buffer_init(&value);

	buffer_copy_str(&value, "index");

	struct Ake_symbol* sym;
	malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
	Ake_symbol_init(sym);
	sym->type = Ake_symbol_type_variable;

	struct Ake_symbol* x;
	x = Ake_environment_get(env, &value);
	expect_null(x, "x null");

	Ake_environment_put(env, &value, sym);

	x = Ake_environment_get(env, &value);
	assert_ptr(x, "x ptr");

	assert_true(x == sym, "x == sym");

	Ake_environment_destroy(env);
	buffer_destroy(&value);
}

void test_symbol_table_global()
{
	test_name(__func__);

	struct Ake_symbol_table st;
	Ake_symbol_table_init(&st);

	expect_true(Ake_symbol_table_is_global(&st), "is global");

	Ake_symbol_table_destroy(&st);
}

void test_symbol_table()
{
	test_symbol_table_env();
	test_symbol_table_global();
}
