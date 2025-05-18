#include "zinc/unit_test.h"
#include "unit_lex.h"
#include "test_value.h"
#include "unit_ast.h"
#include "unit_parse.h"
#include "unit_build.h"
#include "unit_check_value_types.h"
#include "test_type.h"
#include "zinc/test.h"

void CentUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        CentUnit_lex(test);
        // test_value();
        Zinc_test_register(test, CentUnit_ast);
        CoverUnit_parse(test);
        CentUnit_build(test);
        CentUnit_check_value_types(test);
        // test_type();

        return;
    }

    Zinc_test_perform(test);
}
