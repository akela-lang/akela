#include <zinc/test.h>

void AkeLlvmUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        return;
    }

    Zinc_test_perform(test);
}