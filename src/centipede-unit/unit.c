#include "zinc/unit_test.h"
#include "unit_lex.h"
#include "unit_value.h"
#include "unit_ast.h"
#include "unit_parse.h"
#include "unit_build.h"
#include "unit_check_value_types.h"
#include "unit_type.h"
#include "zinc/test.h"

void CentUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CentUnit_lex);
        Zinc_test_register(test, CentUnit_value);
        Zinc_test_register(test, CentUnit_ast);
        Zinc_test_register(test, CoverUnit_parse);
        Zinc_test_register(test, CentUnit_build);
        Zinc_test_register(test, CentUnit_check_value_types);
        Zinc_test_register(test, CentUnit_type);

        return;
    }

    Zinc_test_perform(test);
}
