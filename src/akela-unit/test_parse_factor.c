#include <stdbool.h>
#include "zinc/unit_test.h"
#include "akela/parse_tools.h"
#include "akela/ast.h"
#include "test_parse.h"
#include "akela/type_def.h"
#include "akela/comp_unit.h"
#include "zinc/error_unit_test.h"

void test_parse_number_integer()
{
	Zinc_test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("32", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* number = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(number, "ptr number");
	Zinc_expect_int_equal(number->type, Ake_ast_type_number, "number num");
	Zinc_expect_string(&number->value, "32", "32 num");

	Ake_type_use* tu = number->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "type td");
	Zinc_expect_string(&td->name, "Int32", "str td");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_number_float()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("5.0e0", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* number = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(number, "ptr num");
	Zinc_expect_int_equal(number->type, Ake_ast_type_number, "number number");
	Zinc_expect_string(&number->value, "5.0e0", "5.0e0 number");

	Ake_type_use* tu = number->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_float, "integer td");
	Zinc_expect_string(&td->name, "Real64", "str td");

    parse_teardown(&cu);
}

void test_parse_string()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("\"hello\"", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "type cu.root");

	Ake_ast* string = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(string, "ptr string");
	Zinc_expect_int_equal(string->type, Ake_ast_type_string, "type string");
	Zinc_expect_string(&string->value, "hello", "value string");

	Ake_type_use* tu = string->tu;
	Zinc_assert_ptr(tu, "ptr tu");
    Zinc_expect_true(tu->is_array, "is_array tu");
    Zinc_expect_size_t_equal(tu->dim.count, 1, "dim.count tu");

    Ake_type_dimension* dim = (Ake_type_dimension*)ZINC_VECTOR_PTR(&tu->dim, 0);
    Zinc_assert_ptr(dim, "ptr dim");
    Zinc_expect_size_t_equal(dim->size, 6, "size dim");
    Zinc_expect_int_equal(dim->option, Ake_array_element_const, "option dim");

	Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "type td");
	Zinc_expect_string(&td->name, "Nat8", "name td");

    parse_teardown(&cu);
}

void test_parse_boolean_true()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("true", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* true_node = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(true_node, "ptr true_node");
	Zinc_expect_int_equal(true_node->type, Ake_ast_type_boolean, "boolean true_node");
	Zinc_expect_string(&true_node->value, "true", "true true_node");

	Ake_type_use* tu = true_node->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(&td->name, "Bool", "name td");

    parse_teardown(&cu);
}

void test_parse_boolean_false()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("false", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* false_node = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(false_node, "ptr false_node");
	Zinc_expect_int_equal(false_node->type, Ake_ast_type_boolean, "boolean false_node");
	Zinc_expect_string(&false_node->value, "false", "false false_node");

	Ake_type_use* tu = false_node->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(&td->name, "Bool", "name td");

    parse_teardown(&cu);
}

void test_parse_id()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("const x: Int64; x", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* let = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(let, "ptr let");
	Zinc_assert_int_equal(let->type, Ake_ast_type_const, "type let");

	Ake_ast* let_lseq = Ast_node_get(let, 0);
	Zinc_assert_ptr(let_lseq, "ptr let_lseq");
	Zinc_assert_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "let_lseq let_lseq");

	Ake_ast* id = Ast_node_get(let_lseq, 0);
	Zinc_assert_ptr(id, "ptr id");
	Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(&id->value, "x", "x");

	Ake_ast* type = Ast_node_get(let, 1);
	Zinc_assert_ptr(type, "ptr type");
	Zinc_expect_int_equal(type->type, Ake_ast_type_type, "type type");

	Ake_ast* id2 = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(id2, "ptr id2");
	Zinc_expect_int_equal(id2->type, Ake_ast_type_id, "id id2");
	Zinc_expect_string(&id2->value, "x", "x id2");

	Ake_type_use* tu = id2->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_string(&td->name, "Int64", "name td");

    parse_teardown(&cu);
}

void test_parse_id2()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("const _a23: Int64; _a23", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* id = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(id, "ptr id");
	Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(&id->value, "_a23", "_a23 id");

    parse_teardown(&cu);
}

void test_parse_id3()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("const a2: Int64; a2", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* id = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(id, "ptr id");
	Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(&id->value, "a2", "a2 id");

    parse_teardown(&cu);
}

void test_parse_id_greek()
{
    Zinc_test_name(__func__);


    struct Ake_comp_unit cu;

    parse_setup("const αβγ: Int64; αβγ", &cu);
    Zinc_assert_no_errors(&cu.errors);
    Zinc_expect_true(cu.valid, "parse_setup valid");

    Zinc_assert_ptr(cu.root, "ptr cu.root");
    Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* id = Ast_node_get(cu.root, 1);
    Zinc_assert_ptr(id, "ptr id");
    Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id");
    Zinc_expect_string(&id->value, "αβγ", "value");

    parse_teardown(&cu);
}

void test_parse_id_cyrillic()
{
    Zinc_test_name(__func__);
    Ake_comp_unit cu;

    parse_setup("const я: Int32; я", &cu);
    Zinc_expect_false(cu.valid, "parse_setup valid");
    Zinc_assert_has_errors(&cu.errors);
    Zinc_error* e = Zinc_expect_source_error(&cu.errors, "Unrecognized character: я");
    Zinc_assert_ptr(e, "ptr e");
    Zinc_expect_size_t_equal(e->loc.start_pos, 6, "start_pos");
    Zinc_expect_size_t_equal(e->loc.end_pos, 8, "size");
    Zinc_expect_size_t_equal(e->loc.line, 1, "line");
    Zinc_expect_size_t_equal(e->loc.col, 7, "col");

    parse_teardown(&cu);
}

void test_parse_sign_negative()
{
	Zinc_test_name(__func__);


	Ake_comp_unit cu;

    parse_setup("-30", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sign = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(sign, "ptr sign");
	Zinc_expect_int_equal(sign->type, Ake_ast_type_sign, "sign sign");

	Ake_type_use* tu = sign->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_string(&td->name, "Int32", "Int32 td");

	Ake_ast* left = Ast_node_get(sign, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_expect_int_equal(left->type, Ake_ast_type_minus, "minus");

	Ake_ast* right = Ast_node_get(sign, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_expect_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_string(&right->value, "30", "30");

    parse_teardown(&cu);
}

void test_parse_sign_positive()
{
	Zinc_test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("+30", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sign = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(sign, "ptr sign");
	Zinc_expect_int_equal(sign->type, Ake_ast_type_sign, "sign sign");

	Ake_type_use* tu = sign->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_integer, "integer td");
	Zinc_expect_string(&td->name, "Int32", "Int32 td");

	Ake_ast* left = Ast_node_get(sign, 0);
	Zinc_assert_ptr(left, "left");
	Zinc_assert_int_equal(left->type, Ake_ast_type_plus, "plus");

	Ake_ast* right = Ast_node_get(sign, 1);
	Zinc_assert_ptr(right, "right");
	Zinc_assert_int_equal(right->type, Ake_ast_type_number, "number");
	Zinc_expect_string(&right->value, "30", "30");

    parse_teardown(&cu);
}

void test_parse_sign_error_no_value()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("fn foo() end\n-foo()", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "negative operator was used on expression with no value");

    parse_teardown(&cu);
}

void test_parse_sign_expected_factor()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("-", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "expected parse_factor after sign");

    parse_teardown(&cu);
}

void test_parse_not_id()
{
	Zinc_test_name(__func__);

	Ake_comp_unit cu;

    parse_setup("const a: Bool; !a", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* not = Ast_node_get(cu.root, 1);
	Zinc_assert_ptr(not, "ptr not");
	Zinc_expect_int_equal(not->type, Ake_ast_type_not, "not not");

	Ake_type_use* tu = not->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(&td->name, "Bool", "name td");

	Ake_ast* id = Ast_node_get(not, 0);
	Zinc_assert_ptr(id, "ptr id");
	Zinc_expect_int_equal(id->type, Ake_ast_type_id, "id id");
	Zinc_expect_string(&id->value, "a", "a id");

    parse_teardown(&cu);
}

void test_parse_not_literal()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("!true", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* not = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(not, "ptr not");
	Zinc_expect_int_equal(not->type, Ake_ast_type_not, "not not");

	Ake_type_use* tu = not->tu;
	Zinc_assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	Zinc_assert_ptr(td, "ptr td");
	Zinc_expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	Zinc_expect_string(&td->name, "Bool", "name td");

	Ake_ast* lit_bool = Ast_node_get(not, 0);
	Zinc_assert_ptr(lit_bool, "ptr lit_bool");
	Zinc_expect_int_equal(lit_bool->type, Ake_ast_type_boolean, "boolean true");
	Zinc_expect_string(&lit_bool->value, "true", "true lit_bool");

    parse_teardown(&cu);
}

void test_parse_not_error()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; !foo()", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "! operator used on parse_factor with no value");

    parse_teardown(&cu);
}

void test_parse_array_literal_integer()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1,2,3]", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* a_tu = a->tu;
	Zinc_assert_ptr(a_tu, "ptr array_tu");
    Zinc_expect_true(a_tu->is_array, "is_array a_tu");
    Zinc_expect_size_t_equal(a_tu->dim.count, 1, "dim.count a_tu");
    Zinc_expect_size_t_equal(*(size_t*)ZINC_VECTOR_PTR(&a_tu->dim, 0), 3, "dim[0] a_tu");

	struct Ake_type_def* a_td = a_tu->td;
	Zinc_assert_ptr(a_td, "ptr array_td");
	Zinc_expect_int_equal(a_td->type, Ake_type_integer, "integer array_td");
	Zinc_expect_string(&a_td->name, "Int32", "Int32 array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	Zinc_assert_ptr(a0, "ptr a0");
	Zinc_expect_int_equal(a0->type, Ake_ast_type_number, "number a0");
	Zinc_expect_string(&a0->value, "1", "1 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	Zinc_assert_ptr(a1, "ptr a1");
	Zinc_expect_int_equal(a1->type, Ake_ast_type_number, "number a1");
	Zinc_expect_string(&a1->value, "2", "2 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	Zinc_assert_ptr(a2, "ptr a2");
	Zinc_expect_int_equal(a2->type, Ake_ast_type_number, "number a2");
	Zinc_expect_string(&a2->value, "3", "3 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_float()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1.0,2.5,3.2]", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* array_tu = a->tu;
	Zinc_assert_ptr(array_tu, "ptr array_tu");

	struct Ake_type_def* array_td = array_tu->td;
	Zinc_assert_ptr(array_td, "ptr array_td");
	Zinc_expect_int_equal(array_td->type, Ake_type_float, "float array_td");
	Zinc_expect_string(&array_td->name, "Real64", "name array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	Zinc_assert_ptr(a0, "ptr a0");
	Zinc_expect_int_equal(a0->type, Ake_ast_type_number, "number a0");
	Zinc_expect_string(&a0->value, "1.0", "1.0 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	Zinc_assert_ptr(a1, "ptr a1");
	Zinc_expect_int_equal(a1->type, Ake_ast_type_number, "number a1");
	Zinc_expect_string(&a1->value, "2.5", "2.5 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	Zinc_assert_ptr(a2, "ptr a2");
	Zinc_expect_int_equal(a2->type, Ake_ast_type_number, "number a2");
	Zinc_expect_string(&a2->value, "3.2", "3.2 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_numeric()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1, 2.5, 3]", &cu);
	Zinc_expect_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(a, "ptr a");
	Zinc_expect_int_equal(a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* array_tu = a->tu;
	Zinc_assert_ptr(array_tu, "ptr array_tu");

	struct Ake_type_def* array_td = array_tu->td;
	Zinc_assert_ptr(array_td, "ptr array_td");
	Zinc_expect_int_equal(array_td->type, Ake_type_float, "float array_td");
	Zinc_expect_string(&array_td->name, "Real64", "name array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	Zinc_assert_ptr(a0, "ptr a0");
	Zinc_expect_int_equal(a0->type, Ake_ast_type_number, "number a0");
	Zinc_expect_string(&a0->value, "1", "1 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	Zinc_assert_ptr(a1, "ptr a1");
	Zinc_expect_int_equal(a1->type, Ake_ast_type_number, "number a1");
	Zinc_expect_string(&a1->value, "2.5", "2.5 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	Zinc_assert_ptr(a2, "ptr a2");
	Zinc_expect_int_equal(a2->type, Ake_ast_type_number, "number a2");
	Zinc_expect_string(&a2->value, "3", "3 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_mixed_error()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1,true,3]", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "array elements not the same type");

    parse_teardown(&cu);
}

void test_parse_array_literal_empty_error()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[]", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "array literal has no elements");

    parse_teardown(&cu);
}

void test_parse_array_literal_error_right_square_bracket()
{
	Zinc_test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("[1,2", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "expected right square bracket");

    parse_teardown(&cu);
}

void test_parse_array_literal_error_expected_expr()
{
	Zinc_test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("[1,]", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "expected expr after comma");

    parse_teardown(&cu);
}

void test_parse_paren_num()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("(32)", &cu);
	Zinc_assert_no_errors(&cu.errors);
	Zinc_expect_true(cu.valid, "parse_setup valid");

	Zinc_assert_ptr(cu.root, "ptr cu.root");
	Zinc_expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* paren = Ast_node_get(cu.root, 0);
	Zinc_assert_ptr(paren, "ptr paren");
	Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* number = Ast_node_get(paren, 0);
	Zinc_expect_int_equal(number->type, Ake_ast_type_number, "number number");
	Zinc_expect_string(&number->value, "32", "32 number");

    parse_teardown(&cu);
}

void test_parse_paren_error_empty()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("()", &cu);
	Zinc_assert_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	struct Zinc_error* e = Zinc_expect_source_error(&cu.errors, "empty parenthesis");
    if (e) {
        Zinc_expect_size_t_equal(e->loc.line, 1, "line");
        Zinc_expect_size_t_equal(e->loc.col, 2, "col");
        Zinc_expect_size_t_equal(e->loc.start_pos, 1, "start_pos");
        Zinc_expect_size_t_equal(e->loc.end_pos, 2, "size");
    }

    parse_teardown(&cu);
}

void test_parse_paren_error_right_parenthesis()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("(1", &cu);
	Zinc_assert_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_paren_error_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; (foo())", &cu);
	Zinc_assert_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "parenthesis on expression that has no value");

    parse_teardown(&cu);
}

void test_parse_not_error_expected_factor()
{
	Zinc_test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("!", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "expected parse_factor after !");

    parse_teardown(&cu);
}

void test_parse_not_error_no_value()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; !foo()", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "! operator used on parse_factor with no value");

    parse_teardown(&cu);
}

void test_parse_not_error_not_boolean()
{
	Zinc_test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("!1", &cu);
	Zinc_expect_has_errors(&cu.errors);
	Zinc_expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.errors, "not operator used on non-boolean");

    parse_teardown(&cu);
}

void test_parse_factor_newline_let()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const\nx: Int32", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    parse_teardown(&cu);
}

void test_parse_factor_newline_const_assign()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("const\nx: Int32 =\n1", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    parse_teardown(&cu);
}

void test_parse_factor_newline_not()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("!\ntrue", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    Ake_ast* stmts = cu.root;
    Zinc_assert_ptr(stmts, "ptr parse_stmts");
    Zinc_expect_int_equal(stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* not = stmts->head;
    Zinc_assert_ptr(not, "ptr not");
    Zinc_expect_int_equal(not->type, not->type, "not not");

    Ake_ast* tr = not->head;
    Zinc_assert_ptr(tr, "ptr tr");
    Zinc_expect_int_equal(tr->type, Ake_ast_type_boolean, "boolean tr");
    Zinc_expect_string(&tr->value, "true", "true");

    parse_teardown(&cu);
}

void test_parse_factor_newline_sign()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("-\n1", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    Ake_ast* stmts = cu.root;
    Zinc_assert_ptr(stmts, "ptr parse_stmts");
    Zinc_expect_int_equal(stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* sign = stmts->head;
    Zinc_assert_ptr(sign, "ptr sign");
    Zinc_expect_int_equal(sign->type, sign->type, "sign sign");

    Ake_ast* op = sign->head;
    Zinc_assert_ptr(op, "ptr op");
    Zinc_expect_int_equal(op->type, Ake_ast_type_minus, "minus op");

    parse_teardown(&cu);
}

void test_parse_factor_newline_array_literal()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("[\n1,\n2,\n3\n]", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    Ake_ast* stmts = cu.root;
    Zinc_assert_ptr(stmts, "ptr parse_stmts");
    Zinc_expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_ast* al = Ast_node_get(stmts, 0);
    Zinc_assert_ptr(al, "ptr al");
    Zinc_expect_int_equal(al->type, Ake_ast_type_array_literal, "array_literal al");

    Ake_ast* one = Ast_node_get(al, 0);
    Zinc_assert_ptr(one, "ptr one");
    Zinc_expect_int_equal(one->type, Ake_ast_type_number, "number one");
    Zinc_expect_string(&one->value, "1", "1");

    Ake_ast* two = Ast_node_get(al, 1);
    Zinc_assert_ptr(two, "ptr two");
    Zinc_expect_int_equal(two->type, Ake_ast_type_number, "number two");
    Zinc_expect_string(&two->value, "2", "2");

    Ake_ast* three = Ast_node_get(al, 2);
    Zinc_assert_ptr(three, "ptr three");
    Zinc_expect_int_equal(three->type, Ake_ast_type_number, "number three");
    Zinc_expect_string(&three->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_factor_newline_array_parenthesis()
{
    Zinc_test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("(\n1+2)", &cu);
    Zinc_expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.errors);

    Ake_ast* stmts = cu.root;
    Zinc_assert_ptr(stmts, "ptr parse_stmts");
    Zinc_expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_ast* paren = Ast_node_get(stmts, 0);
    Zinc_assert_ptr(paren, "ptr paren");
    Zinc_expect_int_equal(paren->type, Ake_ast_type_parenthesis, " parenthesis paren");

    Ake_ast* plus = Ast_node_get(paren, 0);
    Zinc_assert_ptr(plus, "ptr plus");
    Zinc_expect_int_equal(plus->type, Ake_ast_type_plus, "plus plus");

    parse_teardown(&cu);
}

void test_parse_factor_array_element_const()
{
    Zinc_test_name(__func__);
    struct Ake_comp_unit cu;

    parse_setup("const mut a: [4 const]Int32 = [1, 2, 3, 4]\n"
                "a[0]\n",
                &cu);
    Zinc_expect_true(cu.valid, "valid");
    Zinc_expect_no_errors(&cu.errors);

    Ake_ast* let = Ast_node_get(cu.root, 0);
    Zinc_assert_ptr(let, "ptr let");
    Zinc_expect_int_equal(let->type, Ake_ast_type_const, "type let");

    Ake_ast* let_type = Ast_node_get(let, 1);
    Zinc_assert_ptr(let_type, "ptr type");
    Zinc_expect_int_equal(let_type->type, Ake_ast_type_type, "type type");
    Zinc_expect_true(let_type->tu->is_array, "is_array type");
    Zinc_expect_size_t_equal(let_type->tu->dim.count, 1, "dim.count type");

    struct Ake_type_dimension* let_type_dim = (Ake_type_dimension*)ZINC_VECTOR_PTR(&let_type->tu->dim, 0);
    Zinc_expect_size_t_equal(let_type_dim->size, 4, "size let_type_dim");
    Zinc_expect_int_equal(let_type_dim->option, Ake_array_element_const, "option let_type_dim");

    parse_teardown(&cu);
}

void test_parse_factor_array_element_const_error()
{
    Zinc_test_name(__func__);
    struct Ake_comp_unit cu;

    parse_setup("const mut a: [4 const]Int32 = [1, 2, 3, 4]\n"
                "a[0] = 10\n",
                &cu);
    Zinc_expect_false(cu.valid, "valid");
    Zinc_expect_has_errors(&cu.errors);
    Zinc_expect_source_error(&cu.errors, "immutable variable changed in assignment");
    parse_teardown(&cu);
}

void test_parse_factor()
{
	test_parse_number_integer();
	test_parse_number_float();
	test_parse_string();
	test_parse_boolean_true();
	test_parse_boolean_false();
	test_parse_id();
	test_parse_id2();
	test_parse_id3();
    test_parse_id_greek();
    test_parse_id_cyrillic();
	test_parse_sign_negative();
	test_parse_sign_positive();
	test_parse_sign_error_no_value();
	test_parse_sign_expected_factor();
	test_parse_not_id();
	test_parse_not_literal();
	test_parse_not_error();
	test_parse_array_literal_integer();
	test_parse_array_literal_float();
	test_parse_array_literal_numeric();
	test_parse_array_literal_mixed_error();
	test_parse_array_literal_empty_error();
	test_parse_array_literal_error_right_square_bracket();
	test_parse_array_literal_error_expected_expr();
	test_parse_paren_num();
	test_parse_paren_error_empty();
	test_parse_paren_error_right_parenthesis();
	test_parse_paren_error_no_value();
	test_parse_not_error_expected_factor();
	test_parse_not_error_no_value();
	test_parse_not_error_not_boolean();
    test_parse_factor_newline_let();
    test_parse_factor_newline_const_assign();
    test_parse_factor_newline_not();
    test_parse_factor_newline_sign();
    test_parse_factor_newline_array_literal();
    test_parse_factor_newline_array_parenthesis();
    test_parse_factor_array_element_const();
    test_parse_factor_array_element_const_error();
}
