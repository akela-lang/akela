#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "unit_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_parse_struct_field_assign(Zinc_test* test)
{
    if (test->dry_run) {
        Zinc_string_add_str(&test->name, __func__);
        test->mute = false;
        test->solo = false;
        return;
    }

    struct Ake_comp_unit cu;
    AkeUnit_parse_setup("struct Person\n"
                "  firstName: [100 const]Nat8\n"
                "  lastName: [100 const]Nat8\n"
                "  age: Int32\n"
                "end\n"
                "var p: Person\n"
                "p.firstName = \"John\"\n"
                "p.firstName\n", &cu);
    Zinc_expect_no_errors(test, &cu.errors);
    Zinc_expect_true(test, cu.valid, "valid");

    if (!Zinc_expect_ptr(test, cu.root, "ptr cu.root")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    /* struct */
    Ake_ast* st = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, st, "ptr st")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, st->type, Ake_ast_type_struct, "struct st");

    /* firstName */
    Ake_ast* d0 = Ast_node_get(st, 0);
    if (!Zinc_expect_ptr(test, d0, "ptr d0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, d0->type, Ake_ast_type_declaration, "declaration d0");

    Ake_ast* id0 = Ast_node_get(d0, 0);
    if (!Zinc_expect_ptr(test, id0, "ptr id0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id0->type, Ake_ast_type_id, "id id0");
    Zinc_expect_string(test, &id0->value, "firstName", "firstName id0");

    Ake_ast* type_node0 = Ast_node_get(d0, 1);
    if (!Zinc_expect_ptr(test, type_node0, "ptr type_node0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, type_node0->type, Ake_ast_type_type, "type type_node0");

    Ake_type_use* tu0 = type_node0->tu;
    if (!Zinc_expect_ptr(test, tu0, "ptr tu0")) {
        return Zinc_assert();
    }

    struct Ake_type_def* td0 = tu0->td;
    if (!Zinc_expect_ptr(test, td0, "ptr td0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, td0->type, Ake_type_integer, "type td0");
    Zinc_expect_string(test, &td0->name, "Nat8", "name td0");

    /* lastName */
    Ake_ast* d1 = Ast_node_get(st, 1);
    if (!Zinc_expect_ptr(test, d1, "ptr d1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, d1->type, Ake_ast_type_declaration, "declaration d1");

    Ake_ast* id1 = Ast_node_get(d1, 0);
    if (!Zinc_expect_ptr(test, id1, "ptr id1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id1->type, Ake_ast_type_id, "id id1");
    Zinc_expect_string(test, &id1->value, "lastName", "lastName id1");

    Ake_ast* type_node1 = Ast_node_get(d1, 1);
    if (!Zinc_expect_ptr(test, type_node1, "ptr type_node1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, type_node1->type, Ake_ast_type_type, "type type_node1");

    Ake_type_use* tu1 = type_node1->tu;
    if (!Zinc_expect_ptr(test, tu1, "ptr tu1")) {
        return Zinc_assert();
    }

    struct Ake_type_def* td1 = tu1->td;
    if (!Zinc_expect_ptr(test, td1, "ptr td1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, td1->type, Ake_type_integer, "type td1");
    Zinc_expect_string(test, &td1->name, "Nat8", "name td1");

    /* age */
    Ake_ast* d2 = Ast_node_get(st, 2);
    if (!Zinc_expect_ptr(test, d2, "ptr d2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, d2->type, Ake_ast_type_declaration, "declaration d2");

    Ake_ast* id2 = Ast_node_get(d2, 0);
    if (!Zinc_expect_ptr(test, id2, "ptr id2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id2->type, Ake_ast_type_id, "id id2");
    Zinc_expect_string(test, &id2->value, "age", "age id2");

    Ake_ast* type_node2 = Ast_node_get(d2, 1);
    if (!Zinc_expect_ptr(test, type_node2, "ptr type_node2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, type_node2->type, Ake_ast_type_type, "type type_node2");

    Ake_type_use* tu2 = type_node2->tu;
    if (!Zinc_expect_ptr(test, tu2, "ptr tu2")) {
        return Zinc_assert();
    }

    struct Ake_type_def* td2 = tu2->td;
    if (!Zinc_expect_ptr(test, td2, "ptr td2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, td2->type, Ake_type_integer, "integer td2");
    Zinc_expect_string(test, &td2->name, "Int32", "Int32 td2");

    /* let */
    Ake_ast* var = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, var, "ptr let")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, var->type, Ake_ast_type_var, "type let");

    Ake_ast* let_lseq = Ast_node_get(var, 0);
    if (!Zinc_expect_ptr(test, let_lseq, "ptr let_lseq")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let_lseq->type, Ake_ast_type_let_lseq, "let_lseq let_lseq");

    Ake_ast* let_type = Ast_node_get(var, 1);
    if (!Zinc_expect_ptr(test, let_type, "ptr let_type")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let_type->type, Ake_ast_type_type, "type let_type");

    Ake_type_use* tu = let_type->tu;
    if (!Zinc_expect_ptr(test, tu, "ptr tu")) {
        return Zinc_assert();
    }

    struct Ake_type_def* td = tu->td;
    if (!Zinc_expect_ptr(test, td, "ptr td")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, td->type, Ake_type_struct, "struct td");
    Zinc_expect_string(test, &td->name, "Person", "Person td");

    Ake_ast* p = Ast_node_get(let_lseq, 0);
    if (!Zinc_expect_ptr(test, p, "ptr p")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, p->type, Ake_ast_type_id, "id p");
    Zinc_expect_string(test, &p->value, "p", "p p");

    AkeUnit_parse_teardown(&cu);
}

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
    Zinc_expect_int_equal(test, cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* st = Ast_node_get(cu.root, 0);
    if (!Zinc_expect_ptr(test, st, "ptr st")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, st->type, Ake_ast_type_struct, "type st");

    Ake_ast* let = Ast_node_get(cu.root, 1);
    if (!Zinc_expect_ptr(test, let, "ptr let")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let->type, Ake_ast_type_const, "type let");

    Ake_ast* let_lseq = Ast_node_get(let, 0);
    if (!Zinc_expect_ptr(test, let_lseq, "ptr let_lseq")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let_lseq->type, Ake_ast_type_let_lseq, "type let_lseq");

    Ake_ast* p = Ast_node_get(let_lseq, 0);
    if (!Zinc_expect_ptr(test, p, "ptr p")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, p->type, Ake_ast_type_id, "type p");

    Ake_ast* let_tu = Ast_node_get(let, 1);
    if (!Zinc_expect_ptr(test, let_tu, "ptr let_tu")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let_tu->type, Ake_ast_type_type, "type let_tu");

    Ake_ast* let_rseq = Ast_node_get(let, 2);
    if (!Zinc_expect_ptr(test, let_rseq, "ptr let_rseq")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, let_rseq->type, Ake_ast_type_let_rseq, "type let_rseq");

    Ake_ast* literal = Ast_node_get(let_rseq, 0);
    if (!Zinc_expect_ptr(test, literal, "ptr literal")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, literal->type, Ake_ast_type_struct_literal, "type literal");

    Ake_ast* field0 = Ast_node_get(literal, 0);
    if (!Zinc_expect_ptr(test, field0, "ptr field0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, field0->type, Ake_ast_type_struct_literal_field, "type field0");

    Ake_ast* id0 = Ast_node_get(field0, 0);
    if (!Zinc_expect_ptr(test, id0, "ptr id0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id0->type, Ake_ast_type_id, "type id0");
    Zinc_expect_string(test, &id0->value, "first_name", "value id0");

    Ake_ast* expr0 = Ast_node_get(field0, 1);
    if (!Zinc_expect_ptr(test, expr0, "ptr expr0")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, expr0->type, Ake_ast_type_string, "type expr0");
    Zinc_expect_string(test, &expr0->value, "John", "value expr0");

    Ake_ast* field1 = Ast_node_get(literal, 1);
    if (!Zinc_expect_ptr(test, field1, "ptr field1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, field1->type, Ake_ast_type_struct_literal_field, "type field1");

    Ake_ast* id1 = Ast_node_get(field1, 0);
    if (!Zinc_expect_ptr(test, id1, "ptr id1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id1->type, Ake_ast_type_id, "type id1");
    Zinc_expect_string(test, &id1->value, "last_name", "value id1");

    Ake_ast* expr1 = Ast_node_get(field1, 1);
    if (!Zinc_expect_ptr(test, expr1, "ptr expr1")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, expr1->type, Ake_ast_type_string, "type expr1");
    Zinc_expect_string(test, &expr1->value, "Smith", "value expr1");

    Ake_ast* field2 = Ast_node_get(literal, 2);
    if (!Zinc_expect_ptr(test, field2, "ptr field2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, field2->type, Ake_ast_type_struct_literal_field, "type field2");

    Ake_ast* id2 = Ast_node_get(field2, 0);
    if (!Zinc_expect_ptr(test, id2, "ptr id2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, id2->type, Ake_ast_type_id, "type id2");
    Zinc_expect_string(test, &id2->value, "age", "value id2");

    Ake_ast* expr2 = Ast_node_get(field2, 1);
    if (!Zinc_expect_ptr(test, expr2, "ptr expr2")) {
        return Zinc_assert();
    }
    Zinc_expect_int_equal(test, expr2->type, Ake_ast_type_number, "type expr2");
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

        Zinc_test_register(test, AkeUnit_parse_struct_field_assign);
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