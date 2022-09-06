#include "zinc/unit_test.h"
#include "test_input.h"
#include "test_scan.h"
#include "test_parse_expr.h"
#include "test_parse_stmts.h"
#include "test_token.h"
#include "test_lookahead_translator.h"
#include "test_lookahead_char.h"
#include "test_ast.h"
#include "test_hash.h"
#include "test_symbol_table.h"
#include "test_parse_types.h"
#include "test_parse_factor.h"
#include "test_type.h"
#include "test_scan_number.h"
#include "test_comp_unit.h"
#include "test_comp_table.h"

/* dynamic-output-none */
int main(void) {
	test_lookahead_char();
	test_lookahead_translator();
	test_input();
	test_token();
	test_hash();
	test_scan();
	test_scan_number();
	test_ast();
	test_type();
	test_parse_factor();
	test_parse_expression();
	test_parse_statements();
	test_symbol_table();
	test_parse_types();
	test_comp_unit();
	test_comp_table();
	print_results();
	return 0;
}
