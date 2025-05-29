#include <zinc/test.h>
#include "unit_lex.h"
#include "unit_dom.h"
#include "unit_parse.h"
#include "unit_stringify.h"

void Json_unit(Zinc_test *test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Json_unit_dom);
        Zinc_test_register(test, Zinc_unit_lex);
        Zinc_test_register(test, Json_unit_parse);
        Zinc_test_register(test, Json_unit_stringify);

        return;
    }

    Zinc_test_perform(test);
}