#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_last()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 1\n"
             "let b::Int64 = 2\n"
             "a + b\n",
             &result);
    expect_str(&result.value, "3", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_if()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("if true\n"
             "  1\n"
             "end\n",
             &result);
    expect_str(&result.value, "", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_if_else()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("if true\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    expect_str(&result.value, "1", "1 value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "else\n"
             "  2\n"
             "end\n",
             &result);
    expect_str(&result.value, "2", "2 value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_if_elseif_else()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("if true\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif true\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("if false\n"
             "  1\n"
             "elseif false\n"
             "  2\n"
             "else\n"
             "  3\n"
             "end\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);
}

void test_akela_llvm_stmts()
{
    test_code_gen_last();
    test_code_gen_if();
    test_code_gen_if_else();
    test_code_gen_if_elseif_else();
}