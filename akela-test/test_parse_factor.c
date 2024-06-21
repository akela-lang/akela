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
	
	struct comp_unit cu;

    parse_setup("32", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* number = Ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, Ast_type_number, "number num");
	expect_str(&number->value, "32", "32 num");

	Ast_node* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_number_float()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("5.0e0", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* number = Ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr num");
	expect_int_equal(number->type, Ast_type_number, "number number");
	expect_str(&number->value, "5.0e0", "5.0e0 number");

	Ast_node* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_float, "integer td");
	expect_str(&td->name, "f64", "f64 td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_string()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("\"hello\"", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "type cu.root");

	Ast_node* string = Ast_node_get(cu.root, 0);
	assert_ptr(string, "ptr string");
	expect_int_equal(string->type, Ast_type_string, "type string");
	expect_str(&string->value, "hello", "value string");

	Ast_node* tu = string->tu;
	assert_ptr(tu, "ptr tu");
    expect_true(tu->to.is_array, "is_array tu");
    expect_size_t_equal(tu->to.dim.count, 1, "dim.count tu");

    Type_dimension* dim = (Type_dimension*)VECTOR_PTR(&tu->to.dim, 0);
    assert_ptr(dim, "ptr dim");
    expect_size_t_equal(dim->size, 6, "size dim");
    expect_int_equal(dim->option, Array_element_const, "option dim");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "type td");
	expect_str(&td->name, "u8", "name td");

    parse_teardown(&cu);
}

void test_parse_boolean_true()
{
	test_name(__func__);


	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("true", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* true_node = Ast_node_get(cu.root, 0);
	assert_ptr(true_node, "ptr true_node");
	expect_int_equal(true_node->type, Ast_type_boolean, "boolean true_node");
	expect_str(&true_node->value, "true", "true true_node");

	Ast_node* tu = true_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_boolean_false()
{
	test_name(__func__);


	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("false", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* false_node = Ast_node_get(cu.root, 0);
	assert_ptr(false_node, "ptr false_node");
	expect_int_equal(false_node->type, Ast_type_boolean, "boolean false_node");
	expect_str(&false_node->value, "false", "false false_node");

	Ast_node* tu = false_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_id()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("let x: i64; x", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* let = Ast_node_get(cu.root, 0);
	assert_ptr(let, "ptr let");
	assert_int_equal(let->type, Ast_type_let, "type let");

	Ast_node* let_lseq = Ast_node_get(let, 0);
	assert_ptr(let_lseq, "ptr let_lseq");
	assert_int_equal(let_lseq->type, Ast_type_let_lseq, "let_lseq let_lseq");

	Ast_node* id = Ast_node_get(let_lseq, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ast_type_id, "id id");
	expect_str(&id->value, "x", "x");

	Ast_node* type = Ast_node_get(let, 1);
	assert_ptr(type, "ptr type");
	expect_int_equal(type->type, Ast_type_type, "type type");

	Ast_node* id2 = Ast_node_get(cu.root, 1);
	assert_ptr(id2, "ptr id2");
	expect_int_equal(id2->type, Ast_type_id, "id id2");
	expect_str(&id2->value, "x", "x id2");

	Ast_node* tu = id2->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_id2()
{
	test_name(__func__);


	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("let _a23: i64; _a23", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* id = Ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ast_type_id, "id id");
	expect_str(&id->value, "_a23", "_a23 id");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_id3()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("let a2: i64; a2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* id = Ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ast_type_id, "id id");
	expect_str(&id->value, "a2", "a2 id");

    parse_teardown(&cu);
}

void test_parse_id_greek()
{
    test_name(__func__);


    struct comp_unit cu;

    parse_setup("let αβγ: i64; αβγ", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

    Ast_node* id = Ast_node_get(cu.root, 1);
    assert_ptr(id, "ptr id");
    expect_int_equal(id->type, Ast_type_id, "id");
    expect_str(&id->value, "αβγ", "value");

    parse_teardown(&cu);
}

void test_parse_id_cyrillic()
{
    test_name(__func__);
    struct comp_unit cu;

    parse_setup("let я: i64; я", &cu);
    expect_false(cu.valid, "parse_setup valid");
    assert_has_errors(&cu.el);
    struct error* e = expect_source_error(&cu.el, "Unrecognized character: я");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.byte_pos, 4, "byte_pos");
    expect_size_t_equal(e->loc.line, 1, "line");
    expect_size_t_equal(e->loc.col, 5, "col");
    expect_size_t_equal(e->loc.size, 1, "size");

    parse_teardown(&cu);
}

void test_parse_sign_negative()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("-30", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* sign = Ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, Ast_type_sign, "sign sign");

	Ast_node* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* left = Ast_node_get(sign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, Ast_type_minus, "minus");

	Ast_node* right = Ast_node_get(sign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "30", "30");

    parse_teardown(&cu);
}

void test_parse_sign_positive()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("+30", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* sign = Ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, Ast_type_sign, "sign sign");

	Ast_node* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "i64", "i64 td");

	Ast_node* left = Ast_node_get(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, Ast_type_plus, "plus");

	Ast_node* right = Ast_node_get(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, Ast_type_number, "number");
	expect_str(&right->value, "30", "30");

    parse_teardown(&cu);
}

void test_parse_sign_error_no_value()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("fn foo() end\n-foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "negative operator was used on expression with no value");

    parse_teardown(&cu);
}

void test_parse_sign_expected_factor()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("-", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected parse_factor after sign");

    parse_teardown(&cu);
}

void test_parse_not_id()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a: bool; !a", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* not = Ast_node_get(cu.root, 1);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, Ast_type_not, "not not");

	Ast_node* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

	Ast_node* id = Ast_node_get(not, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, Ast_type_id, "id id");
	expect_str(&id->value, "a", "a id");

    parse_teardown(&cu);
}

void test_parse_not_literal()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("!true", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* not = Ast_node_get(cu.root, 0);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, Ast_type_not, "not not");

	Ast_node* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "bool", "bool td");

	Ast_node* lit_bool = Ast_node_get(not, 0);
	assert_ptr(lit_bool, "ptr lit_bool");
	expect_int_equal(lit_bool->type, Ast_type_boolean, "boolean true");
	expect_str(&lit_bool->value, "true", "true lit_bool");

    parse_teardown(&cu);
}

void test_parse_not_error()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; !foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "! operator used on parse_factor with no value");

    parse_teardown(&cu);
}

void test_parse_array_literal_integer()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("[1,2,3]", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* a = Ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_array_literal, "array-literal a");

	Ast_node* a_tu = a->tu;
	assert_ptr(a_tu, "ptr array_tu");
    expect_true(a_tu->to.is_array, "is_array a_tu");
    expect_size_t_equal(a_tu->to.dim.count, 1, "dim.count a_tu");
    expect_size_t_equal(*(size_t*)VECTOR_PTR(&a_tu->to.dim, 0), 3, "dim[0] a_tu");

	struct type_def* a_td = a_tu->td;
	assert_ptr(a_td, "ptr array_td");
	expect_int_equal(a_td->type, type_integer, "integer array_td");
	expect_str(&a_td->name, "i64", "i64 array_td");

	Ast_node* a0 = Ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, Ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	Ast_node* a1 = Ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, Ast_type_number, "number a1");
	expect_str(&a1->value, "2", "2 a1");

	Ast_node* a2 = Ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, Ast_type_number, "number a2");
	expect_str(&a2->value, "3", "3 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_float()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("[1.0,2.5,3.2]", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* a = Ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_array_literal, "array-literal a");

	Ast_node* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, type_float, "float array_td");
	expect_str(&array_td->name, "f64", "f64 array_td");

	Ast_node* a0 = Ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, Ast_type_number, "number a0");
	expect_str(&a0->value, "1.0", "1.0 a0");

	Ast_node* a1 = Ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, Ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	Ast_node* a2 = Ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, Ast_type_number, "number a2");
	expect_str(&a2->value, "3.2", "3.2 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_numeric()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("[1, 2.5, 3]", &cu);
	expect_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* a = Ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, Ast_type_array_literal, "array-literal a");

	Ast_node* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, type_float, "float array_td");
	expect_str(&array_td->name, "f64", "f64 array_td");

	Ast_node* a0 = Ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, Ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	Ast_node* a1 = Ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, Ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	Ast_node* a2 = Ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, Ast_type_number, "number a2");
	expect_str(&a2->value, "3", "3 a2");

    parse_teardown(&cu);
}

void test_parse_array_literal_mixed_error()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("[1,true,3]", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "array elements not the same type");

    parse_teardown(&cu);
}

void test_parse_array_literal_empty_error()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("[]", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "array literal has no elements");

    parse_teardown(&cu);
}

void test_parse_array_literal_error_right_square_bracket()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("[1,2", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected right square bracket");

    parse_teardown(&cu);
}

void test_parse_array_literal_error_expected_expr()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("[1,]", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected expr after comma");

    parse_teardown(&cu);
}

void test_parse_paren_num()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("(32)", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, Ast_type_stmts, "parse_stmts cu.root");

	Ast_node* paren = Ast_node_get(cu.root, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, Ast_type_parenthesis, "parenthesis paren");

	Ast_node* number = Ast_node_get(paren, 0);
	expect_int_equal(number->type, Ast_type_number, "number number");
	expect_str(&number->value, "32", "32 number");

    parse_teardown(&cu);
}

void test_parse_paren_error_empty()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("()", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	struct error* e = expect_source_error(&cu.el, "empty parenthesis");
    if (e) {
        expect_size_t_equal(e->loc.line, 1, "line");
        expect_size_t_equal(e->loc.col, 2, "col");
        expect_size_t_equal(e->loc.size, 1, "size");
        expect_size_t_equal(e->loc.byte_pos, 1, "byte_pos");
    }

    parse_teardown(&cu);
}

void test_parse_paren_error_right_parenthesis()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("(1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_paren_error_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; (foo())", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "parenthesis on expression that has no value");

    parse_teardown(&cu);
}

void test_parse_not_error_expected_factor()
{
	test_name(__func__);


	struct comp_unit cu;

    parse_setup("!", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected parse_factor after !");

    parse_teardown(&cu);
}

void test_parse_not_error_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("fn foo() end; !foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "! operator used on parse_factor with no value");

    parse_teardown(&cu);
}

void test_parse_not_error_not_boolean()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("!1", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "not operator used on non-boolean");

    parse_teardown(&cu);
}

void test_parse_factor_newline_let()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let\nx: i64", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    parse_teardown(&cu);
}

void test_parse_factor_newline_let_assign()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("let\nx: i64 =\n1", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    parse_teardown(&cu);
}

void test_parse_factor_newline_not()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("!\ntrue", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    Ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ast_type_stmts, "stmts parse_stmts");

    Ast_node* not = stmts->head;
    assert_ptr(not, "ptr not");
    expect_int_equal(not->type, not->type, "not not");

    Ast_node* tr = not->head;
    assert_ptr(tr, "ptr tr");
    expect_int_equal(tr->type, Ast_type_boolean, "boolean tr");
    expect_str(&tr->value, "true", "true");

    parse_teardown(&cu);
}

void test_parse_factor_newline_sign()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("-\n1", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    Ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ast_type_stmts, "stmts parse_stmts");

    Ast_node* sign = stmts->head;
    assert_ptr(sign, "ptr sign");
    expect_int_equal(sign->type, sign->type, "sign sign");

    Ast_node* op = sign->head;
    assert_ptr(op, "ptr op");
    expect_int_equal(op->type, Ast_type_minus, "minus op");

    parse_teardown(&cu);
}

void test_parse_factor_newline_array_literal()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("[\n1,\n2,\n3\n]", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    Ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ast_type_stmts, "parse_stmts stmts");

    Ast_node* al = Ast_node_get(stmts, 0);
    assert_ptr(al, "ptr al");
    expect_int_equal(al->type, Ast_type_array_literal, "array_literal al");

    Ast_node* one = Ast_node_get(al, 0);
    assert_ptr(one, "ptr one");
    expect_int_equal(one->type, Ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    Ast_node* two = Ast_node_get(al, 1);
    assert_ptr(two, "ptr two");
    expect_int_equal(two->type, Ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    Ast_node* three = Ast_node_get(al, 2);
    assert_ptr(three, "ptr three");
    expect_int_equal(three->type, Ast_type_number, "number three");
    expect_str(&three->value, "3", "3");

    parse_teardown(&cu);
}

void test_parse_factor_newline_array_parenthesis()
{
    test_name(__func__);

    struct comp_unit cu;

    parse_setup("(\n1+2)", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    Ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, Ast_type_stmts, "parse_stmts stmts");

    Ast_node* paren = Ast_node_get(stmts, 0);
    assert_ptr(paren, "ptr paren");
    expect_int_equal(paren->type, Ast_type_parenthesis, " parenthesis paren");

    Ast_node* plus = Ast_node_get(paren, 0);
    assert_ptr(plus, "ptr plus");
    expect_int_equal(plus->type, Ast_type_plus, "plus plus");

    parse_teardown(&cu);
}

void test_parse_factor_array_element_const()
{
    test_name(__func__);
    struct comp_unit cu;

    parse_setup("let mut a: [4 const]i64 = [1, 2, 3, 4]\n"
                "a[0]\n",
                &cu);
    expect_true(cu.valid, "valid");
    expect_no_errors(&cu.el);

    Ast_node* let = Ast_node_get(cu.root, 0);
    assert_ptr(let, "ptr let");
    expect_int_equal(let->type, Ast_type_let, "type let");

    Ast_node* let_type = Ast_node_get(let, 1);
    assert_ptr(let_type, "ptr type");
    expect_int_equal(let_type->type, Ast_type_type, "type type");
    expect_true(let_type->to.is_array, "is_array type");
    expect_size_t_equal(let_type->to.dim.count, 1, "dim.count type");

    struct Type_dimension* let_type_dim = (Type_dimension*)VECTOR_PTR(&let_type->to.dim, 0);
    expect_size_t_equal(let_type_dim->size, 4, "size let_type_dim");
    expect_int_equal(let_type_dim->option, Array_element_const, "option let_type_dim");

    parse_teardown(&cu);
}

void test_parse_factor_array_element_const_error()
{
    test_name(__func__);
    struct comp_unit cu;

    parse_setup("let mut a: [4 const]i64 = [1, 2, 3, 4]\n"
                "a[0] = 10\n",
                &cu);
    expect_false(cu.valid, "valid");
    expect_has_errors(&cu.el);
    struct error* e = expect_source_error(&cu.el, "immutable variable changed in assignment");
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
