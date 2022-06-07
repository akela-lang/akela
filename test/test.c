#include "assert.h"
#include "test_scan.h"
#include "test_parse.h"
#include "test_string.h"
#include "test_allocator.h"

int main(void) {
	test_allocator();
	test_string();
	test_scan();
	test_parse();
	print_results();
	return 0;
}
