#include <zinc/test.h>

#include "zinc/unit_test.h"
#include "unit_literal.h"
#include "test_variable.h"
#include "unit_operator.h"
#include "unit_stmts.h"
#include "unit_function.h"
#include "unit_array.h"
#include "test_struct.h"

void AkeLlvmUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_literal);
        // test_akela_llvm_variable();
        Zinc_test_register(test, AkeLlvmUnit_operator);
        Zinc_test_register(test, AkeLlvmUnit_stmts);
        Zinc_test_register(test, AkeLlvmUnit_array);
        Zinc_test_register(test, AkeLlvmUnit_function);
        // test_akela_llvm_struct();

        return;
    }

    Zinc_test_perform(test);
}