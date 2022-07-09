#include "assert.h"
#include "test_allocator.h"
#include "test_input.h"
#include "test_buffer.h"
#include "test_scan.h"
#include "test_parse_expr.h"
#include "test_parse_stmts.h"
#include "test_token.h"
#include "test_uconv.h"

int main(void) {
	test_allocator();
	test_uconv();
	test_input();
	test_buffer();
	test_token();
	test_scan();
	test_parse_expression();
	test_parse_statements();
	print_results();
	return 0;
}
