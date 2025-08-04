#include "akela/code_gen.h"
#include "unit_cg_tools.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeLlvmUnit_struct_array8(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup(""
             "struct Foo\n"
             "  x: Nat8\n"
             "end\n"
             "var foo: Foo\n"
             "foo.x = 5\n"
             "foo.x\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "5", "value");

    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_array9(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Foo\n"
             "  x: [4]Nat8\n"
             "end\n"
             "const foo: Foo = Foo\n"
             "  x: [1,2,3,4]\n"
             "end\n"
             "foo.x[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_array10(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Int32 = [1,2,3,4]\n"
            "struct Foo\n"
             "  x: [4]Nat8\n"
             "end\n"
             "const foo: Foo = Foo\n"
             "  x: a\n"
             "end\n"
             "foo.x[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_array11(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Foo\n"
            "  x: [4]Int32\n"
            "end\n"
            "var foo: Foo\n"
            "foo.x[0] = 1\n"
            "foo.x[1] = 2\n"
            "foo.x[2] = 3\n"
            "foo.x[3] = 4\n"
            "foo.x[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Nat8 = [1,2,3,4]\n"
             "struct Foo\n"
             "  x: [4]Nat8\n"
             "end\n"
             "var foo: Foo\n"
             "foo.x = a\n"
             "foo.x[1]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Nat8 = [1,2,3,4]\n"
             "struct Foo\n"
             "  x: [4]Nat8\n"
             "end\n"
             "var foo: Foo\n"
             "foo.x = a\n"
             "foo.x[2]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "3", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("const a: [4]Nat8 = [1,2,3,4]\n"
             "struct Foo\n"
             "  x: [4]Nat8\n"
             "end\n"
             "var foo: Foo\n"
             "foo.x = a\n"
             "foo.x[3]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_array12(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Person\n"
             "  first_name: [100 const]Nat8\n"
             "  last_name: [100 const]Nat8\n"
             "  age: Nat32\n"
             "end\n"
             "const p: Person = Person\n"
             "  first_name: \"John\"\n"
             "  last_name: \"Smith\"\n"
             "  age: 35\n"
             "end\n"
             "p.first_name[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "74", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Person\n"
             "  first_name: [100 const]Nat8\n"
             "  last_name: [100 const]Nat8\n"
             "  age: Nat32\n"
             "end\n"
             "const p: Person = Person\n"
             "  first_name: \"John\"\n"
             "  last_name: \"Smith\"\n"
             "  age: 35\n"
             "end\n"
             "p.last_name[0]\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "83", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Person\n"
             "  first_name: [100 const]Nat8\n"
             "  last_name: [100 const]Nat8\n"
             "  age: Nat32\n"
             "end\n"
             "const p: Person = Person\n"
             "  first_name: \"John\"\n"
             "  last_name: \"Smith\"\n"
             "  age: 35\n"
             "end\n"
             "p.age\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "35", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_struct(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Point\n"
             "  x: Real64\n"
             "  y: Real64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "const line: Line = Line\n"
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
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1.500000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Point\n"
             "  x: Real64\n"
             "  y: Real64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "const line: Line = Line\n"
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
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "2.500000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Point\n"
             "  x: Real64\n"
             "  y: Real64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "const line: Line = Line\n"
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
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "3.000000", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Point\n"
             "  x: Real64\n"
             "  y: Real64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "const line: Line = Line\n"
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
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "4.000000", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_struct2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Point\n"
             "  x: Real64\n"
             "  y: Real64\n"
             "end\n"
             "var p: Point = Point\n"
             "  x: 1.5\n"
             "  y: 2.5\n"
             "end\n"
             "p.x = 1.6\n"
             "p.x\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1.600000", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_struct3(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Point\n"
             "  x: Real64\n"
             "  y: Real64\n"
             "end\n"
             "struct Line\n"
             "  p0: Point\n"
             "  p1: Point\n"
             "end\n"
             "var line: Line = Line\n"
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
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "1.600000", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }
    Ake_code_gen_result result;

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Foo\n"
             "  compute: fn(Int32)->Int32\n"
             "end\n"
             "const foo: Foo = Foo\n"
             "  compute: fn (x: Int32)->Int32\n"
             "    x + 1\n"
             "  end\n"
             "end\n"
             "foo.compute(77)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "78", "value");
    Ake_code_gen_result_destroy(&result);

    Ake_code_gen_result_init(&result);
    AkeLlvmUnit_cg_setup("struct Foo\n"
             "  compute: fn(Int32)->Int32\n"
             "end\n"
             "const foo: Foo = Foo\n"
             "  compute: fn (x: Int32)->Int32\n"
             "    x + 2\n"
             "  end\n"
             "end\n"
             "foo.compute(77)\n",
             &result);
    Zinc_expect_no_errors(test, &result.cu->errors);
    Zinc_expect_string(test, &result.value, "79", "value");
    Ake_code_gen_result_destroy(&result);
}

void AkeLlvmUnit_struct(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeLlvmUnit_struct_array8);
        Zinc_test_register(test, AkeLlvmUnit_struct_array9);
        Zinc_test_register(test, AkeLlvmUnit_struct_array10);
        Zinc_test_register(test, AkeLlvmUnit_struct_array11);
        Zinc_test_register(test, AkeLlvmUnit_struct_array12);
        Zinc_test_register(test, AkeLlvmUnit_struct_struct);
        Zinc_test_register(test, AkeLlvmUnit_struct_struct2);
        Zinc_test_register(test, AkeLlvmUnit_struct_struct3);
        Zinc_test_register(test, AkeLlvmUnit_struct_function);

        return;
    }

    Zinc_test_perform(test);
}