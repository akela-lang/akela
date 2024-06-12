#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_code_gen_array_const()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::[4 const]UInt8 = [1,2,3,4]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "2");

    CodeGenResultDestroy(&result);
}

void test_code_gen_const_string()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("\"hello\"\n", &result);
    expect_str(&result.value, "hello", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_const_string2()
{
    test_name(__func__);
    CodeGenResult result;
    CodeGenResultInit(&result);

    cg_setup("let a::[6 const]UInt8 = \"hello\"\n"
             "a\n",
             &result);
    expect_str(&result.value, "hello", "value");

    CodeGenResultDestroy(&result);
}

void test_code_gen_array_const2()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value 0");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value 1");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value 2");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value 3");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8 = [1,2,3,4,0]\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "0", "value 4");
    CodeGenResultDestroy(&result);
}

void test_code_gen_array_const3()
{
    test_name(__func__);
    CodeGenResult result;

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    CodeGenResultDestroy(&result);

    CodeGenResultInit(&result);
    cg_setup("let a::[5 const]UInt8\n"
             "a[0] = 1\n"
             "a[1] = 2\n"
             "a[2] = 3\n"
             "a[3] = 4\n"
             "a[4] = 0\n"
             "a[4]\n",
             &result);
    expect_str(&result.value, "0", "value");
    CodeGenResultDestroy(&result);
}

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

void test_akela_llvm_variable()
{
    test_code_gen_array_const();
    test_code_gen_const_string();
    test_code_gen_const_string2();
    test_code_gen_array_const2();
    test_code_gen_array_const3();
    test_code_gen_let_void1();
    test_code_gen_let_void2();
    test_code_gen_let_int();
    test_code_gen_assign();
    test_code_gen_assign_multiple();
}