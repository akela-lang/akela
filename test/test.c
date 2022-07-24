#include "zinc/unit_test.h"
#include "test_input.h"
#include "test_scan.h"
#include "test_parse_expr.h"
#include "test_parse_stmts.h"
#include "test_token.h"
#include "test_lookahead_translator.h"
#include "test_lookahead_char.h"
#include "test_dag.h"
#include "test_hash.h"
#include "test_symbol_table.h"

/* dynamic-output-none */
int main(void) {
	test_lookahead_char();
	test_lookahead_translator();
	test_input();
	test_token();
	test_hash();
	test_scan();
	test_dag();
	test_parse_expression();
	test_parse_statements();
	test_symbol_table();
	print_results();
	return 0;
}
