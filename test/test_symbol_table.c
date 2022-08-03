#include "zinc/unit_test.h"
#include "zinc/memory.h"
#include "alba/symbol_table.h"

void test_symbol_table_env()
{
	test_name(__func__);

	struct environment* env;

	/* allocate env */
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, NULL);

	struct buffer value;
	buffer_init(&value);

	/* allocate bf{} */
	buffer_copy_str(&value, "index");

	/* allocate sym */
	struct symbol* sym;
	malloc_safe((void**)&sym, sizeof(struct symbol));
	buffer_init(&sym->type);

	/* allocate sym{} */
	buffer_copy_str(&sym->type, "Int32");

	struct symbol* x;
	x = environment_get(env, &value);
	expect_null(x, "x null");

	/* allocate env{} */
	/* transfer sym -> env{} */
	environment_put(env, &value, sym);

	x = environment_get(env, &value);
	assert_ptr(x, "x ptr");

	assert_true(x == sym, "x == sym");

	/* destroy env env{} value{} */
	environment_destroy(env);
	buffer_destroy(&value);
}

void test_symbol_table()
{
	test_symbol_table_env();
}
