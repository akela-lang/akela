#include "akela/parse_tools.h"
#include "unit_parse.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_call_error_expected_expression(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn foo(x: Int32) end; foo(1,)", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_source_error(test, &cu.errors, "expected expression after comma");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_anonymous_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("fn\n(\na: Int32,\nb: Int32,\nc: Int32\n)->\nInt32\na+b+c\nend", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_factor_newline_anonymous_function_const(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup(
            "const foo: fn (a: Int32, b: Int32, c: Int32)->Int32 = fn\n"
            "(\n"
            "a: Int32,\n"
            "b: Int32,\n"
            "c: Int32\n"
            ")->\n"
            "Int32\n"
            "a+b+c\n"
            "end", &cu);
    Zinc_expect_true(test, cu.valid, "AkeUnit_parse_setup valid");
    Zinc_expect_no_errors(test, &cu.errors);

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_input_type_non_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn(Int32) = fn(x: Bool) end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_output_type_non_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn()->Int32 = fn()->Bool true end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_input_type_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn(Int64) = fn(x: Int32) end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function_error_output_type_numeric(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const a: fn()->Int64 = fn()->Int32 1 end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "parse valid");
    Zinc_expect_source_error(test, &cu.errors, "values in assignment are not compatible");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_function(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_parse_call_error_expected_expression);
        Zinc_test_register(test, AkeUnit_parse_factor_newline_anonymous_function);
        Zinc_test_register(test, AkeUnit_parse_factor_newline_anonymous_function_const);
        Zinc_test_register(test, AkeUnit_parse_function_error_input_type_non_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_output_type_non_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_input_type_numeric);
        Zinc_test_register(test, AkeUnit_parse_function_error_output_type_numeric);

        return;
    }

    Zinc_test_perform(test);
}