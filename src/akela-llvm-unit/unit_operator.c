#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_operator_add2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32 = 4\n"
             "a + 61\n",
             &result);
    Zinc_expect_string(test, &result.value, "65", "65");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_operator_sub(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("15-2", &result);
    Zinc_expect_string(test, &result.value, "13", "13");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_operator_sub2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("const a: Int32 = 10\n"
             "a - 2\n",
             &result);
    Zinc_expect_string(test, &result.value, "8", "8");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_operator(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_operator_add2);
        Zinc_test_register(test, AkeLlvmUnit_operator_sub);
        Zinc_test_register(test, AkeLlvmUnit_operator_sub2);

        return;
    }

    Zinc_test_perform(test);
}