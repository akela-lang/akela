#include "unit_string.h"
#include "test_string_list.h"
#include "test_string_slice.h"

void Zinc_unit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, Zinc_unit_string);
        Zinc_test_register(test, Zinc_unit_string_list);
        Zinc_test_register(test, Zinc_unit_string_slice);

        return;
    }

    Zinc_test_perform(test);
}