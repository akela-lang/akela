#include "assert.h"
#include "test_allocator.h"
#include "test_input.h"
#include "test_buffer.h"
#include "test_scan.h"
#include "test_parse.h"

int main(void) {
	test_allocator();
	test_input();
	test_buffer();
	test_scan();
	test_parse();
	print_results();
	return 0;
}
