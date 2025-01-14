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
	test_name(__func__);
	
	struct Ake_comp_unit cu;

    parse_setup("32", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* number = Ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, Ake_ast_type_number, "number num");
	expect_str(&number->value, "32", "32 num");

	Ake_type_use* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_number_float()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("5.0e0", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* number = Ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr num");
	expect_int_equal(number->type, Ake_ast_type_number, "number number");
	expect_str(&number->value, "5.0e0", "5.0e0 number");

	Ake_type_use* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_float, "integer td");
	expect_str(&td->name, "f64", "f64 td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_string()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("\"hello\"", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "type cu.root");

	Ake_ast* string = Ast_node_get(cu.root, 0);
	assert_ptr(string, "ptr string");
	expect_int_equal(string->type, Ake_ast_type_string, "type string");
	expect_str(&string->value, "hello", "value string");

	Ake_type_use* tu = string->tu;
	assert_ptr(tu, "ptr tu");
    expect_true(tu->is_array, "is_array tu");
    expect_size_t_equal(tu->dim.count, 1, "dim.count tu");

    Ake_type_dimension* dim = (Ake_type_dimension*)VECTOR_PTR(&tu->dim, 0);
    assert_ptr(dim, "ptr dim");
    expect_size_t_equal(dim->size, 6, "size dim");
    expect_int_equal(dim->option, Ake_array_element_const, "option dim");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_integer, "type td");
	expect_str(&td->name, "u8", "name td");

    parse_teardown(&cu);
}

void test_parse_boolean_true()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("true", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* true_node = Ast_node_get(cu.root, 0);
	assert_ptr(true_node, "ptr true_node");
	expect_int_equal(true_node->type, Ake_ast_type_boolean, "boolean true_node");
	expect_str(&true_node->value, "true", "true true_node");

	Ake_type_use* tu = true_node->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

    parse_teardown(&cu);
}

void test_parse_boolean_false()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("false", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* false_node = Ast_node_get(cu.root, 0);
	assert_ptr(false_node, "ptr false_node");
	expect_int_equal(false_node->type, Ake_ast_type_boolean, "boolean false_node");
	expect_str(&false_node->value, "false", "false false_node");

	Ake_type_use* tu = false_node->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

    parse_teardown(&cu);
}

void test_parse_id()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let x: i64; x", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* let = Ast_node_get(cu.root, 0);
	assert_ptr(let, "ptr let");
	assert_int_equal(let->type, Ake_ast_type_let, "type let");

	Ake_ast* let_lseq = Ast_node_get(let, 0);
	assert_ptr(let_lseq, "ptr let_lseq");
	assert_int_equal(let_lseq->type, Ake_ast_type_let_lseq, "let_lseq let_lseq");

	Ake_ast* id = Ast_node_get(let_lseq, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ake_ast_type_id, "id id");
	expect_str(&id->value, "x", "x");

	Ake_ast* type = Ast_node_get(let, 1);
	assert_ptr(type, "ptr type");
	expect_int_equal(type->type, Ake_ast_type_type, "type type");

	Ake_ast* id2 = Ast_node_get(cu.root, 1);
	assert_ptr(id2, "ptr id2");
	expect_int_equal(id2->type, Ake_ast_type_id, "id id2");
	expect_str(&id2->value, "x", "x id2");

	Ake_type_use* tu = id2->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

    parse_teardown(&cu);
}

void test_parse_id2()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("let _a23: i64; _a23", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* id = Ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ake_ast_type_id, "id id");
	expect_str(&id->value, "_a23", "_a23 id");

    parse_teardown(&cu);
}

void test_parse_id3()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a2: i64; a2", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* id = Ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ake_ast_type_id, "id id");
	expect_str(&id->value, "a2", "a2 id");

    parse_teardown(&cu);
}

void test_parse_id_greek()
{
    test_name(__func__);


    struct Ake_comp_unit cu;

    parse_setup("let αβγ: i64; αβγ", &cu);
    Zinc_assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

    Ake_ast* id = Ast_node_get(cu.root, 1);
    assert_ptr(id, "ptr id");
    expect_int_equal(id->type, Ake_ast_type_id, "id");
    expect_str(&id->value, "αβγ", "value");

    parse_teardown(&cu);
}

void test_parse_id_cyrillic()
{
    test_name(__func__);
    struct Ake_comp_unit cu;

    parse_setup("let я: i64; я", &cu);
    expect_false(cu.valid, "parse_setup valid");
    Zinc_assert_has_errors(&cu.el);
    struct Zinc_error* e = Zinc_expect_source_error(&cu.el, "Unrecognized character: я");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.start_pos, 4, "start_pos");
    expect_size_t_equal(e->loc.end_pos, 6, "size");
    expect_size_t_equal(e->loc.line, 1, "line");
    expect_size_t_equal(e->loc.col, 5, "col");

    parse_teardown(&cu);
}

void test_parse_sign_negative()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("-30", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sign = Ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, Ake_ast_type_sign, "sign sign");

	Ake_type_use* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ake_ast* left = Ast_node_get(sign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ake_ast_type_minus, "minus");

	Ake_ast* right = Ast_node_get(sign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ake_ast_type_number, "number");
	expect_str(&right->value, "30", "30");

    parse_teardown(&cu);
}

void test_parse_sign_positive()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("+30", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* sign = Ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, Ake_ast_type_sign, "sign sign");

	Ake_type_use* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ake_ast* left = Ast_node_get(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, Ake_ast_type_plus, "plus");

	Ake_ast* right = Ast_node_get(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, Ake_ast_type_number, "number");
	expect_str(&right->value, "30", "30");

    parse_teardown(&cu);
}

void test_parse_sign_error_no_value()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("fn foo() end\n-foo()", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "negative operator was used on expression with no value");

    parse_teardown(&cu);
}

void test_parse_sign_expected_factor()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("-", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "expected parse_factor after sign");

    parse_teardown(&cu);
}

void test_parse_not_id()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("let a: bool; !a", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* not = Ast_node_get(cu.root, 1);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, Ake_ast_type_not, "not not");

	Ake_type_use* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

	Ake_ast* id = Ast_node_get(not, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ake_ast_type_id, "id id");
	expect_str(&id->value, "a", "a id");

    parse_teardown(&cu);
}

void test_parse_not_literal()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("!true", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* not = Ast_node_get(cu.root, 0);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, Ake_ast_type_not, "not not");

	Ake_type_use* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct Ake_type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, Ake_type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

	Ake_ast* lit_bool = Ast_node_get(not, 0);
	assert_ptr(lit_bool, "ptr lit_bool");
	expect_int_equal(lit_bool->type, Ake_ast_type_boolean, "boolean true");
	expect_str(&lit_bool->value, "true", "true lit_bool");

    parse_teardown(&cu);
}

void test_parse_not_error()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; !foo()", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "! operator used on parse_factor with no value");

    parse_teardown(&cu);
}

void test_parse_array_literal_integer()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1,2,3]", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* a_tu = a->tu;
	assert_ptr(a_tu, "ptr array_tu");
    expect_true(a_tu->is_array, "is_array a_tu");
    expect_size_t_equal(a_tu->dim.count, 1, "dim.count a_tu");
    expect_size_t_equal(*(size_t*)VECTOR_PTR(&a_tu->dim, 0), 3, "dim[0] a_tu");

	struct Ake_type_def* a_td = a_tu->td;
	assert_ptr(a_td, "ptr array_td");
	expect_int_equal(a_td->type, Ake_type_integer, "integer array_td");
	expect_str(&a_td->name, "i64", "i64 array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, Ake_ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, Ake_ast_type_number, "number a1");
	expect_str(&a1->value, "2", "2 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, Ake_ast_type_number, "number a2");
	expect_str(&a2->value, "3", "3 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_float()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1.0,2.5,3.2]", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct Ake_type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, Ake_type_float, "float array_td");
	expect_str(&array_td->name, "f64", "f64 array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, Ake_ast_type_number, "number a0");
	expect_str(&a0->value, "1.0", "1.0 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, Ake_ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, Ake_ast_type_number, "number a2");
	expect_str(&a2->value, "3.2", "3.2 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_numeric()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1, 2.5, 3]", &cu);
	Zinc_expect_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* a = Ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ake_ast_type_array_literal, "array-literal a");

	Ake_type_use* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct Ake_type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, Ake_type_float, "float array_td");
	expect_str(&array_td->name, "f64", "f64 array_td");

	Ake_ast* a0 = Ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, Ake_ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	Ake_ast* a1 = Ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, Ake_ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	Ake_ast* a2 = Ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, Ake_ast_type_number, "number a2");
	expect_str(&a2->value, "3", "3 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_mixed_error()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[1,true,3]", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "array elements not the same type");

    parse_teardown(&cu);
}

void test_parse_array_literal_empty_error()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("[]", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "array literal has no elements");

    parse_teardown(&cu);
}

void test_parse_array_literal_error_right_square_bracket()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("[1,2", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "expected right square bracket");

    parse_teardown(&cu);
}

void test_parse_array_literal_error_expected_expr()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("[1,]", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "expected expr after comma");

    parse_teardown(&cu);
}

void test_parse_paren_num()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("(32)", &cu);
	Zinc_assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ake_ast_type_stmts, "parse_stmts cu.root");

	Ake_ast* paren = Ast_node_get(cu.root, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ake_ast_type_parenthesis, "parenthesis paren");

	Ake_ast* number = Ast_node_get(paren, 0);
	expect_int_equal(number->type, Ake_ast_type_number, "number number");
	expect_str(&number->value, "32", "32 number");

    parse_teardown(&cu);
}

void test_parse_paren_error_empty()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("()", &cu);
	Zinc_assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	struct Zinc_error* e = Zinc_expect_source_error(&cu.el, "empty parenthesis");
    if (e) {
        expect_size_t_equal(e->loc.line, 1, "line");
        expect_size_t_equal(e->loc.col, 2, "col");
        expect_size_t_equal(e->loc.start_pos, 1, "start_pos");
        expect_size_t_equal(e->loc.end_pos, 2, "size");
    }

    parse_teardown(&cu);
}

void test_parse_paren_error_right_parenthesis()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("(1", &cu);
	Zinc_assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_paren_error_no_value()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; (foo())", &cu);
	Zinc_assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "parenthesis on expression that has no value");

    parse_teardown(&cu);
}

void test_parse_not_error_expected_factor()
{
	test_name(__func__);


	struct Ake_comp_unit cu;

    parse_setup("!", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "expected parse_factor after !");

    parse_teardown(&cu);
}

void test_parse_not_error_no_value()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("fn foo() end; !foo()", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "! operator used on parse_factor with no value");

    parse_teardown(&cu);
}

void test_parse_not_error_not_boolean()
{
	test_name(__func__);

	struct Ake_comp_unit cu;

    parse_setup("!1", &cu);
	Zinc_expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	Zinc_expect_source_error(&cu.el, "not operator used on non-boolean");

    parse_teardown(&cu);
}

void test_parse_factor_newline_let()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let\nx: i64", &cu);
    expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.el);

    parse_teardown(&cu);
}

void test_parse_factor_newline_let_assign()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("let\nx: i64 =\n1", &cu);
    expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.el);

    parse_teardown(&cu);
}

void test_parse_factor_newline_not()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("!\ntrue", &cu);
    expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.el);

    Ake_ast* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* not = stmts->head;
    assert_ptr(not, "ptr not");
    expect_int_equal(not->type, not->type, "not not");

    Ake_ast* tr = not->head;
    assert_ptr(tr, "ptr tr");
    expect_int_equal(tr->type, Ake_ast_type_boolean, "boolean tr");
    expect_str(&tr->value, "true", "true");

    parse_teardown(&cu);
}

void test_parse_factor_newline_sign()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("-\n1", &cu);
    expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.el);

    Ake_ast* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ake_ast_type_stmts, "stmts parse_stmts");

    Ake_ast* sign = stmts->head;
    assert_ptr(sign, "ptr sign");
    expect_int_equal(sign->type, sign->type, "sign sign");

    Ake_ast* op = sign->head;
    assert_ptr(op, "ptr op");
    expect_int_equal(op->type, Ake_ast_type_minus, "minus op");

    parse_teardown(&cu);
}

void test_parse_factor_newline_array_literal()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("[\n1,\n2,\n3\n]", &cu);
    expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.el);

    Ake_ast* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_ast* al = Ast_node_get(stmts, 0);
    assert_ptr(al, "ptr al");
    expect_int_equal(al->type, Ake_ast_type_array_literal, "array_literal al");

    Ake_ast* one = Ast_node_get(al, 0);
    assert_ptr(one, "ptr one");
    expect_int_equal(one->type, Ake_ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    Ake_ast* two = Ast_node_get(al, 1);
    assert_ptr(two, "ptr two");
    expect_int_equal(two->type, Ake_ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    Ake_ast* three = Ast_node_get(al, 2);
    assert_ptr(three, "ptr three");
    expect_int_equal(three->type, Ake_ast_type_number, "number three");
    expect_str(&three->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_factor_newline_array_parenthesis()
{
    test_name(__func__);

    struct Ake_comp_unit cu;

    parse_setup("(\n1+2)", &cu);
    expect_true(cu.valid, "parse_setup valid");
    Zinc_expect_no_errors(&cu.el);

    Ake_ast* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ake_ast_type_stmts, "parse_stmts stmts");

    Ake_ast* paren = Ast_node_get(stmts, 0);
    assert_ptr(paren, "ptr paren");
    expect_int_equal(paren->type, Ake_ast_type_parenthesis, " parenthesis paren");

    Ake_ast* plus = Ast_node_get(paren, 0);
    assert_ptr(plus, "ptr plus");
    expect_int_equal(plus->type, Ake_ast_type_plus, "plus plus");

    parse_teardown(&cu);
}

void test_parse_factor_array_element_const()
{
    test_name(__func__);
    struct Ake_comp_unit cu;

    parse_setup("let mut a: [4 const]i64 = [1, 2, 3, 4]\n"
                "a[0]\n",
                &cu);
    expect_true(cu.valid, "valid");
    Zinc_expect_no_errors(&cu.el);

    Ake_ast* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ake_ast_type_let, "type let");

    Ake_ast* let_type = Ast_node_get(let, 1);
    assert_ptr(let_type, "ptr type");
    expect_int_equal(let_type->type, Ake_ast_type_type, "type type");
    expect_true(let_type->tu->is_array, "is_array type");
    expect_size_t_equal(let_type->tu->dim.count, 1, "dim.count type");

    struct Ake_type_dimension* let_type_dim = (Ake_type_dimension*)VECTOR_PTR(&let_type->tu->dim, 0);
    expect_size_t_equal(let_type_dim->size, 4, "size let_type_dim");
    expect_int_equal(let_type_dim->option, Ake_array_element_const, "option let_type_dim");

    parse_teardown(&cu);
}

void test_parse_factor_array_element_const_error()
{
    test_name(__func__);
    struct Ake_comp_unit cu;

    parse_setup("let mut a: [4 const]i64 = [1, 2, 3, 4]\n"
                "a[0] = 10\n",
                &cu);
    expect_false(cu.valid, "valid");
    Zinc_expect_has_errors(&cu.el);
    Zinc_expect_source_error(&cu.el, "immutable variable changed in assignment");
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
    test_parse_factor_newline_let_assign();
    test_parse_factor_newline_not();
    test_parse_factor_newline_sign();
    test_parse_factor_newline_array_literal();
    test_parse_factor_newline_array_parenthesis();
    test_parse_factor_array_element_const();
    test_parse_factor_array_element_const_error();
}
