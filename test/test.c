#include "assert.h"
#include "test_allocator.h"
#include "test_input.h"
#include "test_buffer.h"
#include "test_scan.h"
#include "test_parse_expr.h"
#include "test_parse_stmts.h"

int main(void) {
	test_allocator();
	test_input();
	test_buffer();
	test_scan();
	test_parse_expression();
	test_parse_statements();
	print_results();
	return 0;
}
