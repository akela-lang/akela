#include "zinc/memory.h"
#include "akela/symbol_table.h"
#include "akela/symbol.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_symbol_table_env(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_environment* env;

	Zinc_malloc_safe((void**)&env, sizeof(struct Ake_environment));
	Ake_environment_init(env, NULL);

	struct Zinc_string value;
	Zinc_string_init(&value);

	Zinc_string_add_str(&value, "index");

	struct Ake_symbol* sym;
	Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
	Ake_symbol_init(sym);
	sym->type = Ake_symbol_type_variable;

	struct Ake_symbol* x;
	x = Ake_environment_get(env, &value);
	Zinc_expect_null(test, x, "x null");

	Ake_environment_put(env, &value, sym);

	x = Ake_environment_get(env, &value);
	if (!Zinc_expect_ptr(test, x, "x ptr")) {
		return Zinc_assert();
	}

	if (!Zinc_expect_true(test, x == sym, "x == sym")) {
		return Zinc_assert();
	}

	Ake_environment_destroy(env);
	Zinc_string_destroy(&value);
}

void AkeUnit_symbol_table(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_symbol_table_env);

		return;
	}

	Zinc_test_perform(test);
}
