#include "zinc/unit_test.h"
#include "test_utf8.h"
#include "test_vector.h"

int main()
{
	test_utf8();
    test_vector();
	Zinc_print_results();
	return 0;
}
