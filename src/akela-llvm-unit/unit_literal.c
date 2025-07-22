#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_literal_double(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    struct Zinc_string v_exp;
    Zinc_string_init(&v_exp);
    Zinc_string_add_format(&v_exp, "%lf", 1.5);
    Zinc_string_finish(&v_exp);

    AkeLlvmUnit_cg_setup("1.5", &result);
    Zinc_expect_string(test, &result.value, v_exp.buf, "1.5");

    Zinc_string_destroy(&v_exp);
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_literal_boolean_true(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("true",
             &result);
    Zinc_expect_string(test, &result.value, "true", "true");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_literal_boolean_false(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    AkeLlvmUnit_cg_setup("false",
             &result);
    Zinc_expect_string(test, &result.value, "false", "false");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_literal(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_literal_integer);
        Zinc_test_register(test, AkeLlvmUnit_literal_double);
        Zinc_test_register(test, AkeLlvmUnit_literal_boolean_true);
        Zinc_test_register(test, AkeLlvmUnit_literal_boolean_false);

        return;
    }

    Zinc_test_perform(test);
}