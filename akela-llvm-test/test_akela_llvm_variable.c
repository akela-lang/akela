#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"



void test_code_gen_let_void1()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64", &result);
    expect_str(&result.value, "", "blank");

    CodeGenResultDestroy(&result);
}

void test_code_gen_let_void2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 1", &result);
    expect_str(&result.value, "", "blank");

    CodeGenResultDestroy(&result);
}

void test_code_gen_let_int()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::Int64 = 31\n"
             "a"
            ,&result);
    expect_str(&result.value, "31", "31");

    CodeGenResultDestroy(&result);
}

void test_code_gen_assign()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64\n"
             "a = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_multiple()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n"
             "a",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n"
             "b",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n"
             "c",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a,b,c::Int64\n"
             "a = b = c = 44\n",
             &result);
    expect_str(&result.value, "44", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_copy_from_variable()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 50\n"
             "let b::Int64 = a\n"
             "a + b\n",
             &result);
    expect_str(&result.value, "100", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_int64()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 2\n"
             "a\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_int32()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int32 = 2\n"
             "a\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_int32_int64()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 1\n"
             "let b::Int32 = a\n"
             "b\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_scalar_id()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::Int64 = 11\n"
             "let b::Int64\n"
             "b = a\n"
             "b\n",
             &result);
    expect_str(&result.value, "11", "value");
    CodeGenResultDestroy(&result);
}

void test_code_gen_assign_eseq()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "a",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "b",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "c",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a, b, c, d::Int64\n"
             "a, b, c, d = 1, 2, 3, 4\n"
             "d",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);

}

void test_akela_llvm_variable()
{
    test_code_gen_let_void1();
    test_code_gen_let_void2();
    test_code_gen_let_int();
    test_code_gen_assign();
    test_code_gen_assign_multiple();
    test_code_gen_copy_from_variable();
    test_code_gen_int64();
    test_code_gen_int32();
    test_code_gen_int32_int64();
    test_code_gen_assign_scalar_id();
    test_code_gen_assign_eseq();
}