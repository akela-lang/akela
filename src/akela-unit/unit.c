#include "unit_lex.h"
#include "unit_parse_expr.h"
#include "unit_parse_stmts.h"
#include "unit_ast.h"
#include "unit_symbol_table.h"
#include "unit_parse_types.h"
#include "unit_parse_factor.h"
#include "unit_type.h"
#include "unit_lex_number.h"
#include "unit_comp_unit.h"
#include "unit_comp_table.h"
#include "unit_lex_tools.h"
#include "unit_parse_function.h"
#include "unit_parse_struct.h"
#include "unit_environment.h"
#include "unit_type_list.h"
#include "unit_type_slots.h"

void AkeUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_lex);
        Zinc_test_register(test, AkeUnit_lex_number);
        Zinc_test_register(test, AkeUnit_lex_tools);
        Zinc_test_register(test, AkeUnit_ast);
        Zinc_test_register(test, AkeUnit_type);
        Zinc_test_register(test, AkeUnit_parse_factor);
        Zinc_test_register(test, AkeUnit_parse_expression);
        Zinc_test_register(test, AkeUnit_parse_statements);
        Zinc_test_register(test, AkeUnit_parse_function);
        Zinc_test_register(test, AkeUnit_symbol_table);
        Zinc_test_register(test, AkeUnit_parse_types);
        Zinc_test_register(test, AkeUnit_comp_unit);
        Zinc_test_register(test, AkeUnit_comp_table);
        Zinc_test_register(test, AkeUnit_parse_struct);
        Zinc_test_register(test, AkeUnit_Environment);
        Zinc_test_register(test, AkeUnit_TypeList);
        Zinc_test_register(test, AkeUnit_TypeSlots);

        return;
    }

    Zinc_test_perform(test);
}
