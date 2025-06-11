#include "zinc/memory.h"
#include "akela/symbol_table.h"
#include "akela/symbol.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_symbol_table(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		return;
	}

	Zinc_test_perform(test);
}
