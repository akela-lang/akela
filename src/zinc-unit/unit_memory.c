#include <stdlib.h>
#include "zinc/result.h"
#include "zinc/memory.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void Zinc_unit_memory_malloc(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	char* p = NULL;

	Zinc_malloc_safe((void**)&p, 10);

	for (int i = 0; i < 10; i++) {
		p[i] = 0;
	}

	// needs at least one test
	Zinc_expect_true(test, true, "true");

	free(p);
}


void Zinc_unit_memory(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, Zinc_unit_memory_malloc);

		return;
	}

	Zinc_test_perform(test);
}