#include "zinc/test.h"
#include "unit_lex.h"

void Lava_unit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Lava_unit_lex);

        return;
    }

    Zinc_test_perform(test);
}