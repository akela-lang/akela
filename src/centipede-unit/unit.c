#include "zinc/unit_test.h"
#include "test_lex.h"
#include "test_value.h"
#include "unit_ast.h"
#include "test_parse.h"
#include "unit_build.h"
#include "test_check_value_types.h"
#include "test_type.h"
#include "zinc/test.h"

void CentUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        // Zinc_unit_lex();
        // test_value();
        Zinc_test_register(test, CentUnit_ast);
        // CoverUnit_parse();
        CentUnit_build(test);
        // test_check_value_types();
        // test_type();

        return;
    }

    Zinc_test_perform(test);
}
