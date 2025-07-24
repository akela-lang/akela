#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_stmts_if_else(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("if true\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    Zinc_expect_string(test, &result.value, "1", "1 value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("if false\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    Zinc_expect_string(test, &result.value, "2", "2 value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_stmts_if_elseif_else(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("if true\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("if false\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("if false\n"
             "  1\n"
             "elseif false\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_stmts_if_expression(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = if true 1 else 2 end\n"
             "a\n",
             &result);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: Int32 = if false 1 else 2 end\n"
             "a\n",
             &result);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_stmts_printf(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = true;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("extern printf([100 const]Nat8, ...)\n"
             "const s: [100 const]Nat8 = \"test printf\n\"\n"
             "printf(s)\n",
             &result);
    Zinc_expect_string(test, &result.value, "", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_stmts(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_stmts_if_else);
        Zinc_test_register(test, AkeLlvmUnit_stmts_if_elseif_else);
        Zinc_test_register(test, AkeLlvmUnit_stmts_if_expression);
        Zinc_test_register(test, AkeLlvmUnit_stmts_printf);

        return;
    }

    Zinc_test_perform(test);
}