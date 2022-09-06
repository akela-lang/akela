#include "zinc/unit_test.h"
#include "alba/comp_unit.h"
#include "zinc/memory.h"
#include "alba/input.h"

void test_comp_unit_compile()
{
	test_name(__func__);

	struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, "10");
	struct string_data sd;
	string_data_init(&bf, &sd);

	struct symbol_table st;
	symbol_table_init(&st);

	struct comp_unit* cu = NULL;
	malloc_safe(&cu, sizeof(struct comp_unit));
	comp_unit_init(cu);

	bool valid = comp_unit_compile(cu, string_getchar, &sd, &st);
	expect_true(valid, "valid");

	comp_unit_destroy(cu);
	free(cu);
	buffer_destroy(&bf);
}

void test_comp_unit()
{
	test_comp_unit_compile();
}
