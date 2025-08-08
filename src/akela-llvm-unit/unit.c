#include <zinc/test.h>
#include "unit_function.h"

void AkeLlvmUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_function);

        return;
    }

    Zinc_test_perform(test);
}