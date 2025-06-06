#include "unit_data.h"
#include "unit_parse.h"

void CoverUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, CoverUnit_data);
        Zinc_test_register(test, CoverUnit_parse);

        return;
    }

    Zinc_test_perform(test);
}
