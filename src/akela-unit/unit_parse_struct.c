#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_struct_let_literal(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;
    AkeUnit_parse_setup("struct Person\n"
        "  first_name: [100 const]Nat8\n"
        "  last_name: [100 const]Nat8\n"
        "  age: Int32\n"
        "end\n"
        "const p: Person = Person\n"
        "  first_name: \"John\"\n"
        "  last_name: \"Smith\"\n"
        "  age: 35\n"
        "end\n",
        &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, cu.root->kind, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_Ast* st = Ake_ast_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, st, "ptr st")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, st->kind, Ake_ast_type_struct, "type st");

    Ake_Ast* let = Ake_ast_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, let, "ptr let")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let->kind, Ake_ast_type_const, "type let");

    Ake_Ast* p = Ake_ast_get(let, 0);
    if (!Zinc_expect_ptr(test, p, "ptr p")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, p->kind, Ake_ast_type_id, "type p");

    Ake_Ast* let_type = Ake_ast_get(let, 1);
    if (!Zinc_expect_ptr(test, let_type, "ptr let_tu")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let_type->kind, Ake_ast_type_type, "type let_tu");

    Ake_Ast* literal = Ake_ast_get(let, 2);
    if (!Zinc_expect_ptr(test, literal, "ptr literal")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, literal->kind, Ake_ast_type_struct_literal, "type literal");

    Ake_Ast* field0 = Ake_ast_get(literal, 0);
    if (!Zinc_expect_ptr(test, field0, "ptr field0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, field0->kind, Ake_ast_type_struct_literal_field, "type field0");

    Ake_Ast* id0 = Ake_ast_get(field0, 0);
    if (!Zinc_expect_ptr(test, id0, "ptr id0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id0->kind, Ake_ast_type_id, "type id0");
    Zinc_expect_string(test, &id0->value, "first_name", "value id0");

    Ake_Ast* expr0 = Ake_ast_get(field0, 1);
    if (!Zinc_expect_ptr(test, expr0, "ptr expr0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, expr0->kind, Ake_ast_type_string, "type expr0");
    Zinc_expect_string(test, &expr0->value, "John", "value expr0");

    Ake_Ast* field1 = Ake_ast_get(literal, 1);
    if (!Zinc_expect_ptr(test, field1, "ptr field1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, field1->kind, Ake_ast_type_struct_literal_field, "type field1");

    Ake_Ast* id1 = Ake_ast_get(field1, 0);
    if (!Zinc_expect_ptr(test, id1, "ptr id1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id1->kind, Ake_ast_type_id, "type id1");
    Zinc_expect_string(test, &id1->value, "last_name", "value id1");

    Ake_Ast* expr1 = Ake_ast_get(field1, 1);
    if (!Zinc_expect_ptr(test, expr1, "ptr expr1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, expr1->kind, Ake_ast_type_string, "type expr1");
    Zinc_expect_string(test, &expr1->value, "Smith", "value expr1");

    Ake_Ast* field2 = Ake_ast_get(literal, 2);
    if (!Zinc_expect_ptr(test, field2, "ptr field2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, field2->kind, Ake_ast_type_struct_literal_field, "type field2");

    Ake_Ast* id2 = Ake_ast_get(field2, 0);
    if (!Zinc_expect_ptr(test, id2, "ptr id2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id2->kind, Ake_ast_type_id, "type id2");
    Zinc_expect_string(test, &id2->value, "age", "value id2");

    Ake_Ast* expr2 = Ake_ast_get(field2, 1);
    if (!Zinc_expect_ptr(test, expr2, "ptr expr2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, expr2->kind, Ake_ast_type_number, "type expr2");
    Zinc_expect_string(test, &expr2->value, "35", "value expr2");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct_error_invalid_field(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;
    AkeUnit_parse_setup("struct Person\n"
                "  first_name: [100 const]Nat8\n"
                "  last_name: [100 const]Nat8\n"
                "  age: Int32\n"
                "end\n"
                "const p: Person = Person\n"
                "  first_name: \"John\"\n"
                "  middle_name: \"Freddy\"\n"
                "  last_name: \"Smith\"\n"
                "  age: 35\n"
                "end\n",
                &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "Not a valid field for Person: middle_name");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct_error_field_missing(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;
    AkeUnit_parse_setup("struct Person\n"
                "  first_name: [100 const]Nat8\n"
                "  last_name: [100 const]Nat8\n"
                "  age: Int32\n"
                "end\n"
                "const p: Person = Person\n"
                "  first_name: \"John\"\n"
                "  age: 35\n"
                "end\n",
                &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "struct field missing: last_name");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct_error_dot_invalid_field(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup(
            "struct Person\n"
            "  firstName: [100 const]Nat8\n"
            "  lastName: [100 const]Nat8\n"
            "  age: Int32\n"
            "end\n"
            "var p: Person\n"
            "p.abc\n",
            &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "identifier not a field of struct: abc");

    AkeUnit_parse_teardown(&cu);
}

void AkeUnit_parse_struct_error_expected_identifier(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;

    AkeUnit_parse_setup("struct end", &cu);
    Zinc_expect_has_errors(test, &cu.errors);
    Zinc_expect_false(test, cu.valid, "valid");
    Zinc_expect_source_error(test, &cu.errors, "expected identifier");

    AkeUnit_parse_teardown(&cu);
}

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

        Zinc_test_register(test, AkeUnit_parse_struct_let_literal);
        Zinc_test_register(test, AkeUnit_parse_struct_error_invalid_field);
        Zinc_test_register(test, AkeUnit_parse_struct_error_field_missing);
        Zinc_test_register(test, AkeUnit_parse_struct_error_dot_invalid_field);
        Zinc_test_register(test, AkeUnit_parse_struct_error_expected_identifier);
        Zinc_test_register(test, AkeUnit_parse_struct_error_expected_end);
        Zinc_test_register(test, AkeUnit_parse_struct_error_expected_end2);
        Zinc_test_register(test, AkeUnit_parse_struct_error_duplicate);
        Zinc_test_register(test, AkeUnit_parse_struct_impl);

        return;
    }

    Zinc_test_perform(test);
}