#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_last()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("let a: i64 = 1\n"
             "let b: i64 = 2\n"
             "a + b\n",
             &result);
    Zinc_expect_str(&result.value, "3", "value");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_if()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;
    Ake_code_gen_result_init(&result);

    cg_setup("if true\n"
             "  1\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "", "value");

    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_if_else()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("if true\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "1", "1 value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("if false\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "2", "2 value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_if_elseif_else()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("if true\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif false\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    Zinc_expect_str(&result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_code_gen_if_expression()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("let a: i64 = if true 1 else 2 end\n"
             "a\n",
             &result);
    Zinc_expect_str(&result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    cg_setup("let a: i64 = if false 1 else 2 end\n"
             "a\n",
             &result);
    Zinc_expect_str(&result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);
}

void akela_llvm_stmts_printf()
{
    Zinc_test_name(__func__);
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    cg_setup("extern printf([100 const]u8, ...)\n"
             "let s: [100 const]u8 = \"test printf\n\"\n"
             "printf(s)\n",
             &result);
    Zinc_expect_str(&result.value, "", "value");
    Ake_code_gen_result_destroy(&result);
}

void test_akela_llvm_stmts()
{
    test_code_gen_last();
    test_code_gen_if();
    test_code_gen_if_else();
    test_code_gen_if_elseif_else();
    test_code_gen_if_expression();
    akela_llvm_stmts_printf();
}