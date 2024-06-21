#include "zinc/unit_test.h"
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "test_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_struct_field_assign()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("struct Person\n"
                "  firstName: [100 const]u8\n"
                "  lastName: [100 const]u8\n"
                "  age: i64\n"
                "end\n"
                "let p: Person\n"
                "p.firstName = \"John\"\n"
                "p.firstName\n", &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    /* struct */
    Ast_node* st = Ast_node_get(cu.root, 0);
    assert_ptr(st, "ptr st");
    expect_int_equal(st->type, Ast_type_struct, "struct st");

    /* firstName */
    Ast_node* d0 = Ast_node_get(st, 0);
    assert_ptr(d0, "ptr d0");
    expect_int_equal(d0->type, Ast_type_declaration, "declaration d0");

    Ast_node* id0 = Ast_node_get(d0, 0);
    assert_ptr(id0, "ptr id0");
    expect_int_equal(id0->type, Ast_type_id, "id id0");
    expect_str(&id0->value, "firstName", "firstName id0");

    Ast_node* t0 = Ast_node_get(d0, 1);
    assert_ptr(t0, "ptr t0");
    expect_int_equal(t0->type, Ast_type_type, "type t0");

    struct type_def* td0 = t0->td;
    assert_ptr(td0, "ptr td0");
    expect_int_equal(td0->type, type_integer, "type td0");
    expect_str(&td0->name, "u8", "name td0");

    /* lastName */
    Ast_node* d1 = Ast_node_get(st, 1);
    assert_ptr(d1, "ptr d1");
    expect_int_equal(d1->type, Ast_type_declaration, "declaration d1");

    Ast_node* id1 = Ast_node_get(d1, 0);
    assert_ptr(id1, "ptr id1");
    expect_int_equal(id1->type, Ast_type_id, "id id1");
    expect_str(&id1->value, "lastName", "lastName id1");

    Ast_node* t1 = Ast_node_get(d1, 1);
    assert_ptr(t1, "ptr t0");
    expect_int_equal(t1->type, Ast_type_type, "type t1");

    struct type_def* td1 = t1->td;
    assert_ptr(td1, "ptr td1");
    expect_int_equal(td1->type, type_integer, "type td1");
    expect_str(&td1->name, "u8", "name td1");

    /* age */
    Ast_node* d2 = Ast_node_get(st, 2);
    assert_ptr(d2, "ptr d2");
    expect_int_equal(d2->type, Ast_type_declaration, "declaration d2");

    Ast_node* id2 = Ast_node_get(d2, 0);
    assert_ptr(id2, "ptr id2");
    expect_int_equal(id2->type, Ast_type_id, "id id2");
    expect_str(&id2->value, "age", "age id2");

    Ast_node* t2 = Ast_node_get(d2, 1);
    assert_ptr(t2, "ptr t2");
    expect_int_equal(t2->type, Ast_type_type, "type t2");

    struct type_def* td2 = t2->td;
    assert_ptr(td2, "ptr td2");
    expect_int_equal(td2->type, type_integer, "integer td2");
    expect_str(&td2->name, "i64", "i64 td2");

    /* let */
    Ast_node* let = Ast_node_get(cu.root, 1);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ast_type_let, "type let");

    Ast_node* let_lseq = Ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, Ast_type_let_lseq, "let_lseq let_lseq");

    Ast_node* let_type = Ast_node_get(let, 1);
    assert_ptr(let_type, "ptr let_type");
    expect_int_equal(let_type->type, Ast_type_type, "type let_type");

    struct type_def* td = let_type->td;
    assert_ptr(td, "ptr td");
    expect_int_equal(td->type, type_struct, "struct td");
    expect_str(&td->name, "Person", "Person td");

    Ast_node* p = Ast_node_get(let_lseq, 0);
    assert_ptr(p, "ptr p");
    expect_int_equal(p->type, Ast_type_id, "id p");
    expect_str(&p->value, "p", "p p");

    parse_teardown(&cu);
}

void test_parse_struct_let_literal()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("struct Person\n"
        "  first_name: [100 const]u8\n"
        "  last_name: [100 const]u8\n"
        "  age: i64\n"
        "end\n"
        "let p: Person = Person\n"
        "  first_name: \"John\"\n"
        "  last_name: \"Smith\"\n"
        "  age: 35\n"
        "end\n",
        &cu);
    expect_no_errors(&cu.el);
    expect_true(cu.valid, "valid");

    assert_ptr(cu.root, "ptr cu.root");
    expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* st = Ast_node_get(cu.root, 0);
    assert_ptr(st, "ptr st");
    expect_int_equal(st->type, Ast_type_struct, "type st");

    Ast_node* let = Ast_node_get(cu.root, 1);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ast_type_let, "type let");

    Ast_node* let_lseq = Ast_node_get(let, 0);
    assert_ptr(let_lseq, "ptr let_lseq");
    expect_int_equal(let_lseq->type, Ast_type_let_lseq, "type let_lseq");

    Ast_node* p = Ast_node_get(let_lseq, 0);
    assert_ptr(p, "ptr p");
    expect_int_equal(p->type, Ast_type_id, "type p");

    Ast_node* let_tu = Ast_node_get(let, 1);
    assert_ptr(let_tu, "ptr let_tu");
    expect_int_equal(let_tu->type, Ast_type_type, "type let_tu");

    Ast_node* let_rseq = Ast_node_get(let, 2);
    assert_ptr(let_rseq, "ptr let_rseq");
    expect_int_equal(let_rseq->type, Ast_type_let_rseq, "type let_rseq");

    Ast_node* literal = Ast_node_get(let_rseq, 0);
    assert_ptr(literal, "ptr literal");
    expect_int_equal(literal->type, Ast_type_struct_literal, "type literal");

    Ast_node* field0 = Ast_node_get(literal, 0);
    assert_ptr(field0, "ptr field0");
    expect_int_equal(field0->type, Ast_type_struct_literal_field, "type field0");

    Ast_node* id0 = Ast_node_get(field0, 0);
    assert_ptr(id0, "ptr id0");
    expect_int_equal(id0->type, Ast_type_id, "type id0");
    expect_str(&id0->value, "first_name", "value id0");

    Ast_node* expr0 = Ast_node_get(field0, 1);
    assert_ptr(expr0, "ptr expr0");
    expect_int_equal(expr0->type, Ast_type_string, "type expr0");
    expect_str(&expr0->value, "John", "value expr0");

    Ast_node* field1 = Ast_node_get(literal, 1);
    assert_ptr(field1, "ptr field1");
    expect_int_equal(field1->type, Ast_type_struct_literal_field, "type field1");

    Ast_node* id1 = Ast_node_get(field1, 0);
    assert_ptr(id1, "ptr id1");
    expect_int_equal(id1->type, Ast_type_id, "type id1");
    expect_str(&id1->value, "last_name", "value id1");

    Ast_node* expr1 = Ast_node_get(field1, 1);
    assert_ptr(expr1, "ptr expr1");
    expect_int_equal(expr1->type, Ast_type_string, "type expr1");
    expect_str(&expr1->value, "Smith", "value expr1");

    Ast_node* field2 = Ast_node_get(literal, 2);
    assert_ptr(field2, "ptr field2");
    expect_int_equal(field2->type, Ast_type_struct_literal_field, "type field2");

    Ast_node* id2 = Ast_node_get(field2, 0);
    assert_ptr(id2, "ptr id2");
    expect_int_equal(id2->type, Ast_type_id, "type id2");
    expect_str(&id2->value, "age", "value id2");

    Ast_node* expr2 = Ast_node_get(field2, 1);
    assert_ptr(expr2, "ptr expr2");
    expect_int_equal(expr2->type, Ast_type_number, "type expr2");
    expect_str(&expr2->value, "35", "value expr2");
}

void test_parse_struct_error_invalid_field()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("struct Person\n"
                "  first_name: [100 const]u8\n"
                "  last_name: [100 const]u8\n"
                "  age: i64\n"
                "end\n"
                "let p: Person = Person\n"
                "  first_name: \"John\"\n"
                "  middle_name: \"Freddy\"\n"
                "  last_name: \"Smith\"\n"
                "  age: 35\n"
                "end\n",
                &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "Not a valid field for Person: middle_name");
}

void test_parse_struct_error_field_missing()
{
    test_name(__func__);

    struct comp_unit cu;
    parse_setup("struct Person\n"
                "  first_name: [100 const]u8\n"
                "  last_name: [100 const]u8\n"
                "  age: i64\n"
                "end\n"
                "let p: Person = Person\n"
                "  first_name: \"John\"\n"
                "  age: 35\n"
                "end\n",
                &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "struct field missing: last_name");
}

void test_parse_struct_error_dot_invalid_field()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup(
            "struct Person\n"
            "  firstName: [100 const]u8\n"
            "  lastName: [100 const]u8\n"
            "  age: i64\n"
            "end\n"
            "let p: Person\n"
            "p.abc\n",
            &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "identifier not a field of struct: abc");

    parse_teardown(&cu);
}

void test_parse_struct_error_expected_identifier()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("struct end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "expected identifier");

    parse_teardown(&cu);
}

void test_parse_struct_error_expected_end()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("struct Person", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_struct_error_expected_end2()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("struct Person x: i64", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "expected end");

    parse_teardown(&cu);
}

void test_parse_struct_error_duplicate()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let Person: i64 = 1; struct Person end", &cu);
    expect_has_errors(&cu.el);
    expect_false(cu.valid, "valid");
    expect_source_error(&cu.el, "duplicate variable in scope: Person");

    parse_teardown(&cu);
}

void test_parse_struct()
{
    test_parse_struct_field_assign();
    test_parse_struct_let_literal();
    test_parse_struct_error_invalid_field();
    test_parse_struct_error_field_missing();
    test_parse_struct_error_dot_invalid_field();
    test_parse_struct_error_expected_identifier();
    test_parse_struct_error_expected_end();
    test_parse_struct_error_expected_end2();
    test_parse_struct_error_duplicate();
}