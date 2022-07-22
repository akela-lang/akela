#include "assert.h"
#include "test_input.h"
#include "test_scan.h"
#include "test_parse_expr.h"
#include "test_parse_stmts.h"
#include "test_token.h"
#include "test_uconv.h"
#include "test_lookahead_translator.h"
#include "test_lookahead_char.h"
#include "test_dag.h"

int main(void) {
	test_uconv();
	test_lookahead_char();
	test_lookahead_translator();
	test_input();
	test_token();
	test_scan();
	test_dag();
	test_parse_expression();
	test_parse_statements();
	print_results();
	return 0;
}
