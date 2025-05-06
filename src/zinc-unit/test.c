#include "zinc/unit_test.h"
#include "test_unit_test.h"
#include "test_utf8.h"
#include "test_vector.h"
#include "test_unicode.h"

int main()
{
	test_assert();
	test_utf8();
    test_vector();
	test_unicode();
	Zinc_print_results();
	return 0;
}
