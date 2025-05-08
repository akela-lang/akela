#include <zinc/test.h>
#include "zinc/unit_test.h"
#include "test_lex.h"
#include "unit_dom.h"
#include "test_parse.h"
#include "test_stringify.h"

void Json_unit(Zinc_test *test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Json_unit_dom);
        // test_lex();
        // test_parse();
        // test_stringify();

        return;
    }

    Zinc_test_perform(test);
}