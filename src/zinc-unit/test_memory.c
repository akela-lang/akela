#include <stdlib.h>
#include "zinc/result.h"
#include "zinc/memory.h"
#include "zinc/unit_test.h"

void test_memory_malloc()
{
	Zinc_test_name(__func__);

	char* p = NULL;

	Zinc_malloc_safe((void**)&p, 10);

	for (int i = 0; i < 10; i++) {
		p[i] = 0;
	}

	free(p);
}


void test_memory()
{
	test_memory_malloc();
}
