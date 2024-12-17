#include "zinc/unit_test.h"
#include "zinc/error_unit_test.h"
#include "akela/code_gen.h"
#include "test_code_gen_tools.h"

void test_akela_llvm_struct_assign_elements()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "let a: Point\n"
             "a.x = 1.0\n"
             "a.y = 2.0\n"
             "a.x\n",
             &result);
    expect_str(&result.value, "1.000000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "let a: Point\n"
             "a.x = 1.0\n"
             "a.y = 2.0\n"
             "a.y\n",
             &result);
    expect_str(&result.value, "2.000000", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_literal()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "let a: Point = Point\n"
             "  x: 1.0\n"
             "  y: 2.0\n"
             "end\n"
             "a.x\n",
             &result);
    expect_str(&result.value, "1.000000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "let a: Point = Point\n"
             "  x: 1.0\n"
             "  y: 2.0\n"
             "end\n"
             "a.y\n",
             &result);
    expect_str(&result.value, "2.000000", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [6 const]u8 = \"hello\"\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "104", "value");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array3()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [6 const]u8 = \"John\"\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "74", "value");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array4()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [100 const]u8 = \"John\"\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "74", "value");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array5()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [100 const]u8 = \"John\"\n"
             "a[0]\n",
             &result);
    expect_str(&result.value, "74", "value");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array6()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [100 const]u8 = \"John\"\n"
             "let b: [100 const]u8 = a\n"
             "b[0]\n",
             &result);
    expect_str(&result.value, "74", "value");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array7()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4 const]u8 = [1,2,3,4]\n"
             "let b: [4 const]u8 = a\n"
             "b[0]\n",
             &result);
    expect_str(&result.value, "1", "value");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array8()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup(""
             "struct Foo\n"
             "  x: u8\n"
             "end\n"
             "let foo: Foo\n"
             "foo.x = 5\n"
             "foo.x\n",
             &result);
    expect_str(&result.value, "5", "value");

    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array9()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Foo\n"
             "  x: [4]u8\n"
             "end\n"
             "let foo: Foo = Foo\n"
             "  x: [1,2,3,4]\n"
             "end\n"
             "foo.x[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array10()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("let a: [4]i32 = [1,2,3,4]\n"
            "struct Foo\n"
             "  x: [4]u8\n"
             "end\n"
             "let foo: Foo = Foo\n"
             "  x: a\n"
             "end\n"
             "foo.x[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array11()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Foo\n"
            "  x: [4]i32\n"
            "end\n"
            "let foo: Foo\n"
            "foo.x[0] = 1\n"
            "foo.x[1] = 2\n"
            "foo.x[2] = 3\n"
            "foo.x[3] = 4\n"
            "foo.x[0]\n",
             &result);
    expect_str(&result.value, "1", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]u8 = [1,2,3,4]\n"
             "struct Foo\n"
             "  x: [4]u8\n"
             "end\n"
             "let foo: Foo\n"
             "foo.x = a\n"
             "foo.x[1]\n",
             &result);
    expect_str(&result.value, "2", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]u8 = [1,2,3,4]\n"
             "struct Foo\n"
             "  x: [4]u8\n"
             "end\n"
             "let foo: Foo\n"
             "foo.x = a\n"
             "foo.x[2]\n",
             &result);
    expect_str(&result.value, "3", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("let a: [4]u8 = [1,2,3,4]\n"
             "struct Foo\n"
             "  x: [4]u8\n"
             "end\n"
             "let foo: Foo\n"
             "foo.x = a\n"
             "foo.x[3]\n",
             &result);
    expect_str(&result.value, "4", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_array12()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Person\n"
             "  first_name: [100 const]u8\n"
             "  last_name: [100 const]u8\n"
             "  age: u32\n"
             "end\n"
             "let p: Person = Person\n"
             "  first_name: \"John\"\n"
             "  last_name: \"Smith\"\n"
             "  age: 35\n"
             "end\n"
             "p.first_name[0]\n",
             &result);
    expect_str(&result.value, "74", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Person\n"
             "  first_name: [100 const]u8\n"
             "  last_name: [100 const]u8\n"
             "  age: u32\n"
             "end\n"
             "let p: Person = Person\n"
             "  first_name: \"John\"\n"
             "  last_name: \"Smith\"\n"
             "  age: 35\n"
             "end\n"
             "p.last_name[0]\n",
             &result);
    expect_str(&result.value, "83", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Person\n"
             "  first_name: [100 const]u8\n"
             "  last_name: [100 const]u8\n"
             "  age: u32\n"
             "end\n"
             "let p: Person = Person\n"
             "  first_name: \"John\"\n"
             "  last_name: \"Smith\"\n"
             "  age: 35\n"
             "end\n"
             "p.age\n",
             &result);
    expect_str(&result.value, "35", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_struct()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "let line: Line = Line\n"
             "  p0: Point\n"
             "    x: 1.5\n"
             "    y: 2.5\n"
             "  end\n"
             "  p1: Point\n"
             "    x: 3.0\n"
             "    y: 4.0\n"
             "  end\n"
             "end\n"
             "line.p0.x\n",
             &result);
    expect_str(&result.value, "1.500000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "let line: Line = Line\n"
             "  p0: Point\n"
             "    x: 1.5\n"
             "    y: 2.5\n"
             "  end\n"
             "  p1: Point\n"
             "    x: 3.0\n"
             "    y: 4.0\n"
             "  end\n"
             "end\n"
             "line.p0.y\n",
             &result);
    expect_str(&result.value, "2.500000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "let line: Line = Line\n"
             "  p0: Point\n"
             "    x: 1.5\n"
             "    y: 2.5\n"
             "  end\n"
             "  p1: Point\n"
             "    x: 3.0\n"
             "    y: 4.0\n"
             "  end\n"
             "end\n"
             "line.p1.x\n",
             &result);
    expect_str(&result.value, "3.000000", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "let line: Line = Line\n"
             "  p0: Point\n"
             "    x: 1.5\n"
             "    y: 2.5\n"
             "  end\n"
             "  p1: Point\n"
             "    x: 3.0\n"
             "    y: 4.0\n"
             "  end\n"
             "end\n"
             "line.p1.y\n",
             &result);
    expect_str(&result.value, "4.000000", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "let mut p: Point = Point\n"
             "  x: 1.5\n"
             "  y: 2.5\n"
             "end\n"
             "p.x = 1.6\n"
             "p.x\n",
             &result);
    expect_str(&result.value, "1.600000", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_struct2()
{
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Point\n"
             "  x: f64\n"
             "  y: f64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "let mut line: Line = Line\n"
             "  p0: Point\n"
             "    x: 1.5\n"
             "    y: 2.5\n"
             "  end\n"
             "  p1: Point\n"
             "    x: 3.0\n"
             "    y: 4.0\n"
             "  end\n"
             "end\n"
             "line.p0.x = 1.6\n"
             "line.p0.x\n",
             &result);
    expect_str(&result.value, "1.600000", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct_function() {
    test_name(__func__);
    Code_gen_result result;

    Code_gen_result_init(&result);
    cg_setup("struct Foo\n"
             "  compute: fn(i64)->i64\n"
             "end\n"
             "let foo: Foo = Foo\n"
             "  compute: fn (x: i64)->i64\n"
             "    x + 1\n"
             "  end\n"
             "end\n"
             "foo.compute(77)\n",
             &result);
    expect_str(&result.value, "78", "value");
    Code_gen_result_destroy(&result);

    Code_gen_result_init(&result);
    cg_setup("struct Foo\n"
             "  compute: fn(i64)->i64\n"
             "end\n"
             "let foo: Foo = Foo\n"
             "  compute: fn (x: i64)->i64\n"
             "    x + 2\n"
             "  end\n"
             "end\n"
             "foo.compute(77)\n",
             &result);
    expect_str(&result.value, "79", "value");
    Code_gen_result_destroy(&result);
}

void test_akela_llvm_struct()
{
    test_akela_llvm_struct_assign_elements();
    test_akela_llvm_struct_literal();
    test_akela_llvm_struct_array2();
    test_akela_llvm_struct_array3();
    test_akela_llvm_struct_array4();
    test_akela_llvm_struct_array5();
    test_akela_llvm_struct_array6();
    test_akela_llvm_struct_array7();
    test_akela_llvm_struct_array8();
    test_akela_llvm_struct_array9();
    test_akela_llvm_struct_array10();
    test_akela_llvm_struct_array11();
    test_akela_llvm_struct_array12();
    test_akela_llvm_struct_struct();
    test_akela_llvm_struct2();
    test_akela_llvm_struct_struct2();
    test_akela_llvm_struct_function();
}