#include <zinc/test.h>
#include "unit_function.h"
#include "unit_array.h"
#include "unit_struct.h"

void AkeLlvmUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_array);
        Zinc_test_register(test, AkeLlvmUnit_function);
        Zinc_test_register(test, AkeLlvmUnit_struct);

        return;
    }

    Zinc_test_perform(test);
}