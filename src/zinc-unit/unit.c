#include "test_buffer.h"

void Zinc_unit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_call(test, Zinc_unit_buffer);

        return;
    }

    Zinc_test_perform(test);
}