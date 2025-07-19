#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_struct_error_expected_end(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("struct Person", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected end");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct_error_expected_end2(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("struct Person x: Int32", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected end");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct_error_duplicate(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("const Person: Int32 = 1; struct Person end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "duplicate variable in scope: Person");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct_impl(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup(
            "extern pow(Real64, Real64)->Real64\n"
            "extern sqrt(Real64)->Real64\n"
            "struct Point\n"
            "  x: Real64\n"
            "  y: Real64\n"
            "end\n"
            "struct Line\n"
            "  p0: Point\n"
            "  p1: Point\n"
            " end\n"
            "impl Line\n"
            "  fn length(self)->Real64\n"
            "    sqrt(pow(self.p1.x - self.p0.x, 2) + pow(self.p1.y - self.p0.y, 2))\n"
            "  end\n"
            "end\n",
            &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;

        Zinc_test_register(test, AkeUnit_parse_struct_error_expected_end);
        Zinc_test_register(test, AkeUnit_parse_struct_error_expected_end2);
        Zinc_test_register(test, AkeUnit_parse_struct_error_duplicate);
        Zinc_test_register(test, AkeUnit_parse_struct_impl);

        return;
    }

    Zinc_test_perform(test);
}