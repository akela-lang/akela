#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_parse_expr.h"
#include "test_parse_stmts.h"
#include "test_token.h"
#include "test_ast.h"
#include "test_symbol_table.h"
#include "test_parse_types.h"
#include "test_parse_factor.h"
#include "test_type.h"
#include "test_lex_number.h"
#include "test_comp_unit.h"
#include "test_comp_table.h"
#include "test_code_gen.h"
#include "test_lex_tools.h"

/* dynamic-output-none */
int main(void) {
	test_token();
	test_lex();
	test_lex_number();
    test_lex_tools();
	test_ast();
	test_type();
	test_parse_factor();
	test_parse_expression();
	test_parse_statements();
	test_symbol_table();
	test_parse_types();
	test_comp_unit();
	test_comp_table();
    test_code_gen();
	print_results();
	return 0;
}
