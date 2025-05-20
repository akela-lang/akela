#include <zinc/test.h>

#include "zinc/unit_test.h"
#include "test_literal.h"
#include "test_variable.h"
#include "test_operator.h"
#include "test_stmts.h"
#include "unit_function.h"
#include "unit_array.h"
#include "test_struct.h"

void AkeLlvmUnit(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        // test_akela_llvm_literal();
        // test_akela_llvm_variable();
        // test_akela_llvm_operator();
        // test_akela_llvm_stmts();
        Zinc_test_register(test, AkeLlvmUnit_array);
        Zinc_test_register(test, AkeLlvmUnit_function);
        // test_akela_llvm_struct();

        return;
    }

    Zinc_test_perform(test);
}