#include "zinc/unit_test.h"
#include "unit_lex.h"
#include "test_parse_expr.h"
#include "test_parse_stmts.h"
#include "test_token.h"
#include "unit_ast.h"
#include "test_symbol_table.h"
#include "test_parse_types.h"
#include "test_parse_factor.h"
#include "test_type.h"
#include "unit_lex_number.h"
#include "unit_comp_unit.h"
#include "unit_comp_table.h"
#include "test_lex_tools.h"
#include "test_parse_function.h"
#include "test_parse_struct.h"

void AkeUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        // test_token();
        Zinc_test_register(test, AkeUnit_lex);
        Zinc_test_register(test, AkeUnit_lex_number);
        // test_lex_tools();
        Zinc_test_register(test, AkeUnit_ast);
        // CentUnit_type();
        // test_parse_factor();
        // test_parse_expression();
        // test_parse_statements();
        // test_parse_function();
        // test_symbol_table();
        // test_parse_types();
        Zinc_test_register(test, AkeUnit_comp_unit);
        Zinc_test_register(test, AkeUnit_comp_table);
        // test_parse_struct();
        // Zinc_print_results();

        return;
    }

    Zinc_test_perform(test);
}
