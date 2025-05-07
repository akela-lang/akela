#include "zinc/test.h"
#include "unit_lex.h"
#include "unit_parse.h"

void Lava_unit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Lava_unit_lex);
        Zinc_test_register(test, Lava_unit_parse);

        return;
    }

    Zinc_test_perform(test);
}