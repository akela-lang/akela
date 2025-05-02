#include "zinc/unit_test.h"
#include "test_result.h"
#include "test_memory.h"
#include "test_unit_test.h"
#include "test_utf8.h"
#include "test_os_unix.h"
#include "test_piece.h"
#include "test_list.h"
#include "test_vector.h"
#include "test_unicode.h"
#include "test_spec_error.h"
#include "test_priority_queue.h"
#include "test_test.h"

int main()
{
	test_result();
	test_memory();
	test_assert();
	test_utf8();
    test_os_unix();
    test_piece();
    test_list();
    test_vector();
	test_unicode();
	test_spec_error();
	test_priority_queue();
	test_test();
	Zinc_print_results();
	return 0;
}
