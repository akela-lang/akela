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
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* number = ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, ast_type_number, "number num");
	expect_str(&number->value, "32", "32 num");

	struct ast_node* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

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
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* number = ast_node_get(cu.root, 0);
	assert_ptr(number, "ptr num");
	expect_int_equal(number->type, ast_type_number, "number number");
	expect_str(&number->value, "5.0e0", "5.0e0 number");

	struct ast_node* tu = number->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_float, "integer td");
	expect_str(&td->name, "Float64", "Float64 td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_string()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("\"hello\"", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* string = ast_node_get(cu.root, 0);
	assert_ptr(string, "ptr string");
	expect_int_equal(string->type, ast_type_string, "string string");
	expect_str(&string->value, "hello", "hello string");

	struct ast_node* tu = string->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_string, "string td");
	expect_str(&td->name, "String", "String td");

	/* destroy ps{} cu.root cu.root{} */

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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* true_node = ast_node_get(cu.root, 0);
	assert_ptr(true_node, "ptr true_node");
	expect_int_equal(true_node->type, ast_type_boolean, "boolean true_node");
	expect_str(&true_node->value, "true", "true true_node");

	struct ast_node* tu = true_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* false_node = ast_node_get(cu.root, 0);
	assert_ptr(false_node, "ptr false_node");
	expect_int_equal(false_node->type, ast_type_boolean, "boolean false_node");
	expect_str(&false_node->value, "false", "false false_node");

	struct ast_node* tu = false_node->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_id()
{
	test_name(__func__);

	
	struct comp_unit cu;
	bool valid;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("let x::Int64; x", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* var = ast_node_get(cu.root, 0);
	assert_ptr(var, "ptr var");
	assert_int_equal(var->type, ast_type_let, "var var");

	struct ast_node* var_lseq = ast_node_get(var, 0);
	assert_ptr(var_lseq, "ptr var_lseq");
	assert_int_equal(var_lseq->type, ast_type_let_lseq, "var_lseq var_lseq");

	struct ast_node* id = ast_node_get(var_lseq, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "x", "x");

	struct ast_node* type = ast_node_get(var, 1);
	assert_ptr(type, "ptr type");
	expect_int_equal(type->type, ast_type_type, "type type");

	struct ast_node* id2 = ast_node_get(cu.root, 1);
	assert_ptr(id2, "ptr id2");
	expect_int_equal(id2->type, ast_type_id, "id id2");
	expect_str(&id2->value, "x", "x id2");

	struct ast_node* tu = id2->tu;
	assert_ptr(tu, "ptr tu");
	
	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

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
    parse_setup("let _a23::Int64; _a23", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* id = ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "_a23", "_a23 id");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_id3()
{
	test_name(__func__);

	
	struct comp_unit cu;

    parse_setup("let a2::Int64; a2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* id = ast_node_get(cu.root, 1);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
	expect_str(&id->value, "a2", "a2 id");

    parse_teardown(&cu);
}

void test_parse_id_greek()
{
    test_name(__func__);


    struct comp_unit cu;

    parse_setup("let αβγ::Int64; αβγ", &cu);
    assert_no_errors(&cu.el);
    expect_true(cu.valid, "parse_setup valid");

    assert_ptr(cu.root, "ptr cu.root");
    assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

    struct ast_node* id = ast_node_get(cu.root, 1);
    assert_ptr(id, "ptr id");
    expect_int_equal(id->type, ast_type_id, "id");
    expect_str(&id->value, "αβγ", "value");

    parse_teardown(&cu);
}

void test_parse_id_cyrillic()
{
    test_name(__func__);


    struct comp_unit cu;

    parse_setup("let я::Int64; я", &cu);
    assert_has_errors(&cu.el);
    struct error* e = expect_source_error(&cu.el, "Unrecognized character: я");
    assert_ptr(e, "ptr e");
    expect_size_t_equal(e->loc.byte_pos, 4, "byte_pos");
    expect_size_t_equal(e->loc.line, 1, "line");
    expect_size_t_equal(e->loc.col, 5, "col");
    expect_size_t_equal(e->loc.size, 1, "size");
    expect_false(cu.valid, "parse_setup valid");

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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* sign = ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, ast_type_sign, "sign sign");

	struct ast_node* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	struct ast_node* left = ast_node_get(sign, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, ast_type_minus, "minus");

	struct ast_node* right = ast_node_get(sign, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, ast_type_number, "number");
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
	assert_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* sign = ast_node_get(cu.root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, ast_type_sign, "sign sign");

	struct ast_node* tu = sign->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	struct ast_node* left = ast_node_get(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, ast_type_plus, "plus");

	struct ast_node* right = ast_node_get(sign, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, ast_type_number, "number");
	expect_str(&right->value, "30", "30");

    parse_teardown(&cu);
}

void test_parse_sign_error_no_value()
{
	test_name(__func__);

	
	struct comp_unit cu;

    parse_setup("function foo() end\n-foo()", &cu);
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

    parse_setup("let a::Bool; !a", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* not = ast_node_get(cu.root, 1);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, ast_type_not, "not not");

	struct ast_node* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	struct ast_node* id = ast_node_get(not, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, ast_type_id, "id id");
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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* not = ast_node_get(cu.root, 0);
	assert_ptr(not, "ptr not");
	expect_int_equal(not->type, ast_type_not, "not not");

	struct ast_node* tu = not->tu;
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_boolean, "boolean td");
	expect_str(&td->name, "Bool", "Bool td");

	struct ast_node* lit_bool = ast_node_get(not, 0);
	assert_ptr(lit_bool, "ptr lit_bool");
	expect_int_equal(lit_bool->type, ast_type_boolean, "boolean true");
	expect_str(&lit_bool->value, "true", "true lit_bool");

    parse_teardown(&cu);
}

void test_parse_not_error()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("function foo() end; !foo()", &cu);
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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* a = ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct ast_node* a_tu = a->tu;
	assert_ptr(a_tu, "ptr array_tu");
    expect_true(a_tu->to.is_array, "is_array a_tu");
    expect_size_t_equal(a_tu->to.dim.count, 1, "dim.count a_tu");
    expect_size_t_equal(*(size_t*)VECTOR_PTR(&a_tu->to.dim, 0), 3, "dim[0] a_tu");

	struct type_def* a_td = a_tu->td;
	assert_ptr(a_td, "ptr array_td");
	expect_int_equal(a_td->type, type_integer, "integer array_td");
	expect_str(&a_td->name, "Int64", "Int64 array_td");

	struct ast_node* a0 = ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	struct ast_node* a1 = ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, ast_type_number, "number a1");
	expect_str(&a1->value, "2", "2 a1");

	struct ast_node* a2 = ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, ast_type_number, "number a2");
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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* a = ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct ast_node* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, type_float, "float array_td");
	expect_str(&array_td->name, "Float64", "Float64 array_td");

	struct ast_node* a0 = ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_number, "number a0");
	expect_str(&a0->value, "1.0", "1.0 a0");

	struct ast_node* a1 = ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	struct ast_node* a2 = ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, ast_type_number, "number a2");
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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* a = ast_node_get(cu.root, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_array_literal, "array-literal a");

	struct ast_node* array_tu = a->tu;
	assert_ptr(array_tu, "ptr array_tu");

	struct type_def* array_td = array_tu->td;
	assert_ptr(array_td, "ptr array_td");
	expect_int_equal(array_td->type, type_float, "float array_td");
	expect_str(&array_td->name, "Float64", "Float64 array_td");

	struct ast_node* a0 = ast_node_get(a, 0);
	assert_ptr(a0, "ptr a0");
	expect_int_equal(a0->type, ast_type_number, "number a0");
	expect_str(&a0->value, "1", "1 a0");

	struct ast_node* a1 = ast_node_get(a, 1);
	assert_ptr(a1, "ptr a1");
	expect_int_equal(a1->type, ast_type_number, "number a1");
	expect_str(&a1->value, "2.5", "2.5 a1");

	struct ast_node* a2 = ast_node_get(a, 2);
	assert_ptr(a2, "ptr a2");
	expect_int_equal(a2->type, ast_type_number, "number a2");
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

void test_parse_anonymous_function()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a::Function{Input{Int32, Int32, Int32}}; a = function(x::Int32,y::Int32,z::Int32) 1 end", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* assign = ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, ast_type_assign, "assign assign");

	struct ast_node* a = ast_node_get(assign, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct ast_node* f = ast_node_get(assign, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_anonymous_function, "type f");

    struct ast_node* proto = ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    struct ast_node* func_id = ast_node_get(proto, 0);
    assert_ptr(func_id, "ptr func_id");
    expect_int_equal(func_id->type, ast_type_id, "type func_id");
    expect_str(&func_id->value, "__anonymous_function_0", "value func_id");

	struct ast_node* dseq = ast_node_get(proto, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dec_x = ast_node_get(dseq, 0);
	assert_ptr(dec_x, "ptr dec_x");
	expect_int_equal(dec_x->type, ast_type_declaration, "declaration dec_x");

	struct ast_node* x = ast_node_get(dec_x, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x x");

	struct ast_node* type_x = ast_node_get(dec_x, 1);
	assert_ptr(type_x, "ptr type_x");
	expect_int_equal(type_x->type, ast_type_type, "type type_x");

	struct ast_node* dec_y = ast_node_get(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, ast_type_declaration, "declaration dec_y");

	struct ast_node* y = ast_node_get(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct ast_node* type_y = ast_node_get(dec_y, 1);
	assert_ptr(type_y, "ptr type_y");
	expect_int_equal(type_y->type, ast_type_type, "type type_y");

	struct ast_node* dec_z = ast_node_get(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, ast_type_declaration, "declaration dec_z");

	struct ast_node* z = ast_node_get(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, ast_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct ast_node* type_z = ast_node_get(dec_z, 1);
	assert_ptr(type_z, "ptr type_z");
	expect_int_equal(type_z->type, ast_type_type, "type type_z");

	struct ast_node* dret = ast_node_get(proto, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

    struct ast_node* type = ast_node_get(proto, 3);
    assert_ptr(type, "ptr type");
    expect_int_equal(type->type, ast_type_type, "type type");

	struct ast_node* stmts = ast_node_get(f, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts parse_stmts");

	struct ast_node* one = ast_node_get(stmts, 0);
	assert_ptr(one, "ptr one");
	expect_int_equal(one->type, ast_type_number, "number one");
	expect_str(&one->value, "1", "1 one");

    parse_teardown(&cu);
}

void test_parse_anonymous_function2()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup(
            "let a::Function{Input{Int32, Int32, Int32}, Output{Int32}}; a = function(x::Int32,y::Int32,z::Int32)::Int32 1 end",
            &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* assign = ast_node_get(cu.root, 1);
	assert_ptr(assign, "ptr assign");
	expect_int_equal(assign->type, ast_type_assign, "assign assign");

	struct ast_node* a = ast_node_get(assign, 0);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_id, "id a");
	expect_str(&a->value, "a", "a a");

	struct ast_node* f = ast_node_get(assign, 1);
	assert_ptr(f, "ptr f");
	expect_int_equal(f->type, ast_type_anonymous_function, "type f");

    struct ast_node* proto = ast_node_get(f, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    struct ast_node* func_id = ast_node_get(proto, 0);
    assert_ptr(func_id, "ptr func_id");
    expect_int_equal(func_id->type, ast_type_id, "type func_id");
    expect_str(&func_id->value, "__anonymous_function_0", "value func_id");

    struct ast_node* dseq = ast_node_get(proto, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dec_x = ast_node_get(dseq, 0);
	assert_ptr(dec_x, "ptr dec_x");
	expect_int_equal(dec_x->type, ast_type_declaration, "declaration dec_x");

	struct ast_node* x = ast_node_get(dec_x, 0);
	assert_ptr(x, "ptr x");
	expect_int_equal(x->type, ast_type_id, "id x");
	expect_str(&x->value, "x", "x x");

	struct ast_node* type_x = ast_node_get(dec_x, 1);
	assert_ptr(type_x, "ptr type_x");
	expect_int_equal(type_x->type, ast_type_type, "type_type type_x");

	struct ast_node* dec_y = ast_node_get(dseq, 1);
	assert_ptr(dec_y, "ptr dec_y");
	expect_int_equal(dec_y->type, ast_type_declaration, "declaration dec_y");

	struct ast_node* y = ast_node_get(dec_y, 0);
	assert_ptr(y, "ptr y");
	expect_int_equal(y->type, ast_type_id, "id y");
	expect_str(&y->value, "y", "y y");

	struct ast_node* type_y = ast_node_get(dec_y, 1);
	assert_ptr(type_y, "ptr name_y");
	expect_int_equal(type_y->type, ast_type_type, "type type_y");

	struct ast_node* dec_z = ast_node_get(dseq, 2);
	assert_ptr(dec_z, "ptr dec_z");
	expect_int_equal(dec_z->type, ast_type_declaration, "declaration dec_z");

	struct ast_node* z = ast_node_get(dec_z, 0);
	assert_ptr(z, "ptr z");
	expect_int_equal(z->type, ast_type_id, "id z");
	expect_str(&z->value, "z", "z z");

	struct ast_node* type_z = ast_node_get(dec_z, 1);
	assert_ptr(type_z, "ptr type_z");
	expect_int_equal(type_z->type, ast_type_type, "type type_z");

	struct ast_node* dret = ast_node_get(proto, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* dret_type = ast_node_get(dret, 0);
	assert_ptr(dret_type, "ptr dret_type");
	expect_int_equal(dret_type->type, ast_type_type, "id dret_type");

    struct ast_node* type = ast_node_get(proto, 3);
    assert_ptr(type, "ptr type");
    expect_int_equal(type->type, ast_type_type, "type type");

	struct ast_node* stmts = ast_node_get(f, 1);
	assert_ptr(stmts, "ptr parse_stmts");
	expect_int_equal(stmts->type, ast_type_stmts, "stmts parse_stmts");

	struct ast_node* one = ast_node_get(stmts, 0);
	assert_ptr(one, "ptr one");
	expect_int_equal(one->type, ast_type_number, "number one");
	expect_str(&one->value, "1", "1 one");

    parse_teardown(&cu);
}

void test_parse_anonymous_function3()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a::Function; a = function(x::Int64) let x::Int64 = 1 end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_source_error(&cu.el, "duplicate declaration in same scope: x");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_assignment_error()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("let a::Function = function(x::Int64) end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_source_error(&cu.el, "values in assignment not compatible");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_return_error()
{
	test_name(__func__);

	
	struct comp_unit cu;

    parse_setup("let f::Function{Output{Int64}} = function()::Int64 true end", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_source_error(&cu.el, "returned type does not match function return type");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_expected_right_paren()
{
	test_name(__func__);

	
	struct comp_unit cu;

    parse_setup("function(", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_source_error(&cu.el, "expected right parenthesis");

    parse_teardown(&cu);
}

void test_parse_anonymous_function_expected_end()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("function()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse valid");
	expect_source_error(&cu.el, "expected end");

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
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* paren = ast_node_get(cu.root, 0);
	assert_ptr(paren, "ptr paren");
	expect_int_equal(paren->type, ast_type_parenthesis, "parenthesis paren");

	struct ast_node* number = ast_node_get(paren, 0);
	expect_int_equal(number->type, ast_type_number, "number number");
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

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("(1", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected right parenthesis");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_paren_error_no_value()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo() end; (foo())", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "parenthesis on expression that has no value");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("function foo() 1 end; foo()", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

    struct ast_node* proto = ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

	struct ast_node* f_id = ast_node_get(proto, 0);
	assert_ptr(f_id, "ptr f_id");
	expect_int_equal(f_id->type, ast_type_id, "id f_id");
	expect_str(&f_id->value, "foo", "foo");

	struct ast_node* dseq = ast_node_get(proto, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dret dseq");

	struct ast_node* dret = ast_node_get(proto, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* a = ast_node_get(cu.root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_call, "call");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct ast_node* c = ast_node_get(a, 1);
	assert_ptr(c, "ptr c");
	expect_int_equal(c->type, ast_type_cseq, "cseq");

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_call_return_type()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo()::Int64 1 end; foo() + 2", &cu);
	assert_no_errors(&cu.el);
	expect_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* f = ast_node_get(cu.root, 0);
	assert_ptr(f, "ptr f");

	struct ast_node* add = ast_node_get(cu.root, 1);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, ast_type_plus, "plus add");

	struct ast_node* add_tu = add->tu;
	assert_ptr(add_tu, "patr add_tu");

	struct type_def* add_td = add_tu->td;
	assert_ptr(add_td, "ptr add_td");
	expect_int_equal(add_td->type, type_integer, "integer add_td");
	expect_str(&add_td->name, "Int64", "Int64 add_td");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

/* dynamic-output-none */
void test_parse_call_return_type_error()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo()::Bool true end; foo() + 2", &cu);
	assert_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "addition on non-numeric operand");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call2()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup("function foo(arg1::Int64) arg1 end; foo(2)", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

    struct ast_node* proto = ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

	struct ast_node* fname = ast_node_get(proto, 0);
	assert_ptr(fname, "ptr fname");
	expect_int_equal(fname->type, ast_type_id, "id fname");
	expect_str(&fname->value, "foo", "foo fname");

	struct ast_node* dseq = ast_node_get(proto, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dparam0 = ast_node_get(dseq, 0);
	assert_ptr(dparam0, "ptr dparam0");
	expect_int_equal(dparam0->type, ast_type_declaration, "declaration dparam0");

	struct ast_node* param0_name = ast_node_get(dparam0, 0);
	assert_ptr(param0_name, "ptr param0_name");
	expect_int_equal(param0_name->type, ast_type_id, "id param0_name");
	expect_str(&param0_name->value, "arg1", "arg1");

	struct ast_node* tu = ast_node_get(dparam0, 1);
	assert_ptr(tu, "ptr tu");

	struct type_def* td = tu->td;
	assert_ptr(td, "ptr td");
	expect_int_equal(td->type, type_integer, "integer td");
	expect_str(&td->name, "Int64", "Int64 td");

	struct ast_node* dret = ast_node_get(proto, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* f_stmts = ast_node_get(fd, 1);
	assert_ptr(f_stmts, "ptr f_stmts");
	expect_int_equal(f_stmts->type, ast_type_stmts, "parse_stmts f_stmts");

	struct ast_node* a = ast_node_get(cu.root, 1);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_call, "call");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct ast_node* cseq = ast_node_get(a, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, ast_type_cseq, "cseq");

	struct ast_node* cseq_a = ast_node_get(cseq, 0);
	assert_ptr(cseq_a, "ptr cseq_a");
	expect_int_equal(cseq_a->type, ast_type_number, "cseq_a");
	expect_str(&cseq_a->value, "2", "2 cseq_a");

    parse_teardown(&cu);
}

void test_parse_call3()
{
	test_name(__func__);

	struct comp_unit cu;

    parse_setup("function foo(arg1::Int64, arg2::Int64)::Int64 1 end; let x::Int64; let y::Int64; foo(x,y)", &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

    struct ast_node* proto = ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

    struct ast_node* fname = ast_node_get(proto, 0);
	assert_ptr(fname, "ptr fname");
	expect_int_equal(fname->type, ast_type_id, "id fname");
	expect_str(&fname->value, "foo", "foo fname");

	struct ast_node* fd_seq = ast_node_get(proto, 1);
	assert_ptr(fd_seq, "ptr fdseq");
	expect_int_equal(fd_seq->type, ast_type_dseq, "dseq fd_seq");

	struct ast_node* fd_param0 = ast_node_get(fd_seq, 0);
	assert_ptr(fd_param0, "ptr fd_param0");
	expect_int_equal(fd_param0->type, ast_type_declaration, "declaration fd_param0");

	struct ast_node* param0_name = ast_node_get(fd_param0, 0);
	assert_ptr(param0_name, "ptr param0_name");
	expect_int_equal(param0_name->type, ast_type_id, "id param0_name");
	expect_str(&param0_name->value, "arg1", "arg1");

	struct ast_node* param0_type = ast_node_get(fd_param0, 1);
	assert_ptr(param0_type, "ptr param0_id");
	expect_int_equal(param0_type->type, ast_type_type, "type param0_id");

	struct ast_node* dret = ast_node_get(proto, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* a = ast_node_get(cu.root, 3);
	assert_ptr(a, "ptr a");
	expect_int_equal(a->type, ast_type_call, "call");

	struct ast_node* b = ast_node_get(a, 0);
	assert_ptr(b, "ptr b");
	expect_int_equal(b->type, ast_type_id, "id");
	expect_str(&b->value, "foo", "foo");

	struct ast_node* cseq = ast_node_get(a, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, ast_type_cseq, "cseq");

	struct ast_node* cseq_a = ast_node_get(cseq, 0);
	assert_ptr(cseq_a, "ptr cseq_a");
	expect_int_equal(cseq_a->type, ast_type_id, "cseq_a");
	expect_str(&cseq_a->value, "x", "x cseq_a");

	struct ast_node* cseq_b = ast_node_get(cseq, 1);
	assert_ptr(cseq_b, "ptr cseq_b");
	expect_int_equal(cseq_b->type, ast_type_id, "cseq_b");
	expect_str(&cseq_b->value, "y", "y cseq_b");

	struct ast_node* cseq_c = ast_node_get(cseq, 2);
	assert_null(cseq_c, "null cseq_c");

	struct ast_node* c = ast_node_get(a, 2);
	assert_null(c, "null c");

    parse_teardown(&cu);
}

void test_parse_call4()
{
	test_name(__func__);
	
	struct comp_unit cu;

    parse_setup(
            "function foo(arg0::Int64, arg1::Int64, arg2::Int64)::Int64 100 end; let x::Int64; let y::Int64; foo(x, y, 1)",
            &cu);
	assert_no_errors(&cu.el);
	assert_true(cu.valid, "parse_setup valid");

	assert_ptr(cu.root, "ptr cu.root");
	expect_int_equal(cu.root->type, ast_type_stmts, "parse_stmts cu.root");

	struct ast_node* fd = ast_node_get(cu.root, 0);
	assert_ptr(fd, "ptr fd");
	expect_int_equal(fd->type, ast_type_function, "function fd");

    struct ast_node* proto = ast_node_get(fd, 0);
    assert_ptr(proto, "ptr proto");
    expect_int_equal(proto->type, ast_type_prototype, "type proto");

	struct ast_node* fd_id = ast_node_get(proto, 0);
	assert_ptr(fd_id, "ptr fd");
	expect_int_equal(fd_id->type, ast_type_id, "id fd_id");
	expect_str(&fd_id->value, "foo", "foo fd_id");

    struct ast_node* dseq = ast_node_get(proto, 1);
	assert_ptr(dseq, "ptr dseq");
	expect_int_equal(dseq->type, ast_type_dseq, "dseq dseq");

	struct ast_node* dseq_param0 = ast_node_get(dseq, 0);
	assert_ptr(dseq_param0, "ptr desq_param0");
	expect_int_equal(dseq_param0->type, ast_type_declaration, "declaration dseq_param0");

	struct ast_node* dseq_param0_id = ast_node_get(dseq_param0, 0);
	assert_ptr(dseq_param0_id, "ptr dseq_param0_id");
	expect_int_equal(dseq_param0_id->type, ast_type_id, "type dseq_param0_id");
	expect_str(&dseq_param0_id->value, "arg0", "arg0 dseq_param0_id");

	struct ast_node* dseq_param0_type = ast_node_get(dseq_param0, 1);
	assert_ptr(dseq_param0_type, "ptr dseq_param0_type");
	expect_int_equal(dseq_param0_type->type, ast_type_type, "type dseq_param0_type");

	struct ast_node* dseq_param1 = ast_node_get(dseq, 1);
	assert_ptr(dseq_param1, "ptr desq_param1");
	expect_int_equal(dseq_param1->type, ast_type_declaration, "declaration dseq_param1");

	struct ast_node* dseq_param1_id = ast_node_get(dseq_param1, 0);
	assert_ptr(dseq_param1_id, "ptr dseq_param1_id");
	expect_int_equal(dseq_param1_id->type, ast_type_id, "type dseq_param1_id");
	expect_str(&dseq_param1_id->value, "arg1", "arg1 dseq_param1_id");

	struct ast_node* dseq_param1_type_id = ast_node_get(dseq_param1, 1);
	assert_ptr(dseq_param1_type_id, "ptr dseq_param1_type_id");
	expect_int_equal(dseq_param1_type_id->type, ast_type_type, "type dseq_param1_type_id");

	struct ast_node* dseq_param2 = ast_node_get(dseq, 2);
	assert_ptr(dseq_param2, "ptr desq_param2");
	expect_int_equal(dseq_param2->type, ast_type_declaration, "declaration dseq_param2");

	struct ast_node* dseq_param2_id = ast_node_get(dseq_param2, 0);
	assert_ptr(dseq_param2_id, "ptr dseq_param2_id");
	expect_int_equal(dseq_param2_id->type, ast_type_id, "type dseq_param2_id");
	expect_str(&dseq_param2_id->value, "arg2", "arg2 dseq_param2_id");

	struct ast_node* dseq_param2_type_id = ast_node_get(dseq_param2, 1);
	assert_ptr(dseq_param2_type_id, "ptr dseq_param2_type_id");
	expect_int_equal(dseq_param2_type_id->type, ast_type_type, "type dseq_param2_type_id");

	struct ast_node* dret = ast_node_get(proto, 2);
	assert_ptr(dret, "ptr dret");
	expect_int_equal(dret->type, ast_type_dret, "dret dret");

	struct ast_node* dret_type_id = ast_node_get(dret, 0);
	assert_ptr(dret_type_id, "ptr dret_type_id");
	expect_int_equal(dret_type_id->type, ast_type_type, "type dret_type_id");

	struct ast_node* call = ast_node_get(cu.root, 3);
	assert_ptr(call, "ptr call");
	expect_int_equal(call->type, ast_type_call, "call call");

	struct ast_node* call_id = ast_node_get(call, 0);
	assert_ptr(call_id, "ptr call_id");
	expect_int_equal(call_id->type, ast_type_id, "id call_id");
	expect_str(&call_id->value, "foo", "foo call_id");

	struct ast_node* cseq = ast_node_get(call, 1);
	assert_ptr(cseq, "ptr cseq");
	expect_int_equal(cseq->type, ast_type_cseq, "cseq");

	struct ast_node* cseq_param0 = ast_node_get(cseq, 0);
	assert_ptr(cseq_param0, "ptr cseq_param0");
	expect_int_equal(cseq_param0->type, ast_type_id, "id cseq_param0");
	expect_str(&cseq_param0->value, "x", "x cseq_param0");

	struct ast_node* cseq_param1 = ast_node_get(cseq, 1);
	assert_ptr(cseq_param1, "ptr cseq_param1");
	expect_int_equal(cseq_param1->type, ast_type_id, "id cseq_param1");
	expect_str(&cseq_param1->value, "y", "y cseq_param1");

	struct ast_node* cseq_param2 = ast_node_get(cseq, 2);
	assert_ptr(cseq_param2, "ptr cseq_param2");
	expect_int_equal(cseq_param2->type, ast_type_number, "cseq_param2");
	expect_str(&cseq_param2->value, "1", "1 cseq_param2");

    parse_teardown(&cu);
}

void test_parse_call_missing_arguments()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo(a::Int64, b::Int64) end; foo(1)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "not enough arguments in function call");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call_too_many_arguments()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo(a::Int64) end; foo(1, 2)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "too many arguments in function call");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call_type_error()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo(a::Int64) end; foo(true)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "parameter and aguments types do not match");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call_anonymous_function_type_error()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("let foo::Function{Input{Int64}} = function (a::Int64) end; foo(true)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "parameter and aguments types do not match");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call_error_right_paren()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo() end; foo(", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected right parenthesis");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call_error_function_not_declared()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "variable not declared: foo");

	/* destroy ps{} cu.root cu.root{} */

    parse_teardown(&cu);
}

void test_parse_call_error_not_function()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("let foo::Int64; foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "not a function type");

	/* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_call_error_not_enough_arguments()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo(x::Int64) end; foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "not enough arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_error_too_many_arguments()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo() end; foo(1)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "too many arguments in function call");

    parse_teardown(&cu);
}

void test_parse_call_error_expected_expression()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo(x::Int64) end; foo(1,)", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected expression after comma");

	/* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_not_error_expected_factor()
{
	test_name(__func__);

	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("!", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "expected parse_factor after !");

	/* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_not_error_no_value()
{
	test_name(__func__);

	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("function foo() end; !foo()", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "! operator used on parse_factor with no value");

	/* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_not_error_not_boolean()
{
	test_name(__func__);
	
	struct comp_unit cu;

	/* allocate ps{} cu.root cu.root{} */
    parse_setup("!1", &cu);
	expect_has_errors(&cu.el);
	expect_false(cu.valid, "parse_setup valid");
	expect_source_error(&cu.el, "not operator used on non-boolean");

	/* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_var()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("let\nx::Int64", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_var_assign()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("let\nx::Int64 =\n1", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_anonymous_function()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("function\n(\na::Int64,\nb::Int64,\nc::Int64\n)::\nInt64\na+b+c\nend", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_anonymous_function_var()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup(
            "let foo::Function{Input{Int64,Int64,Int64},Output{Int64}} = function\n"
            "(\n"
            "a::Int64,\n"
            "b::Int64,\n"
            "c::Int64\n"
            ")::\n"
            "Int64\n"
            "a+b+c\n"
            "end", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_not()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("!\ntrue", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    struct ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, ast_type_stmts, "stmts parse_stmts");

    struct ast_node* not = stmts->head;
    assert_ptr(not, "ptr not");
    expect_int_equal(not->type, not->type, "not not");

    struct ast_node* tr = not->head;
    assert_ptr(tr, "ptr tr");
    expect_int_equal(tr->type, ast_type_boolean, "boolean tr");
    expect_str(&tr->value, "true", "true");

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_sign()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("-\n1", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    struct ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, ast_type_stmts, "stmts parse_stmts");

    struct ast_node* sign = stmts->head;
    assert_ptr(sign, "ptr sign");
    expect_int_equal(sign->type, sign->type, "sign sign");

    struct ast_node* op = sign->head;
    assert_ptr(op, "ptr op");
    expect_int_equal(op->type, ast_type_minus, "minus op");

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_array_literal()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("[\n1,\n2,\n3\n]", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    struct ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts stmts");

    struct ast_node* al = ast_node_get(stmts, 0);
    assert_ptr(al, "ptr al");
    expect_int_equal(al->type, ast_type_array_literal, "array_literal al");

    struct ast_node* one = ast_node_get(al, 0);
    assert_ptr(one, "ptr one");
    expect_int_equal(one->type, ast_type_number, "number one");
    expect_str(&one->value, "1", "1");

    struct ast_node* two = ast_node_get(al, 1);
    assert_ptr(two, "ptr two");
    expect_int_equal(two->type, ast_type_number, "number two");
    expect_str(&two->value, "2", "2");

    struct ast_node* three = ast_node_get(al, 2);
    assert_ptr(three, "ptr three");
    expect_int_equal(three->type, ast_type_number, "number three");
    expect_str(&three->value, "3", "3");

    /* destroy ps{} cu.root cu.root{} */
    parse_teardown(&cu);
}

void test_parse_factor_newline_array_parenthesis()
{
    test_name(__func__);

    struct comp_unit cu;

    /* allocate ps{} cu.root cu.root{} */
    parse_setup("(\n1+2)", &cu);
    expect_true(cu.valid, "parse_setup valid");
    expect_no_errors(&cu.el);

    struct ast_node* stmts = cu.root;
    assert_ptr(stmts, "ptr parse_stmts");
    expect_int_equal(stmts->type, ast_type_stmts, "parse_stmts stmts");

    struct ast_node* paren = ast_node_get(stmts, 0);
    assert_ptr(paren, "ptr paren");
    expect_int_equal(paren->type, ast_type_parenthesis, " parenthesis paren");

    struct ast_node* plus = ast_node_get(paren, 0);
    assert_ptr(plus, "ptr plus");
    expect_int_equal(plus->type, ast_type_plus, "plus plus");

    /* destroy ps{} cu.root cu.root{} */
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
	test_parse_anonymous_function();
	test_parse_anonymous_function2();
	test_parse_anonymous_function3();
	test_parse_anonymous_function_return_error();
	test_parse_anonymous_function_assignment_error();
	test_parse_anonymous_function_expected_right_paren();
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
	test_parse_call();
	test_parse_call_return_type();
	test_parse_call_return_type_error();
	test_parse_call2();
	test_parse_call3();
	test_parse_call4();
	test_parse_call_missing_arguments();
	test_parse_call_too_many_arguments();
	test_parse_call_type_error();
	test_parse_call_anonymous_function_type_error();
	test_parse_anonymous_function_expected_end();
	test_parse_call_error_right_paren();
	test_parse_call_error_function_not_declared();
	test_parse_call_error_not_function();
	test_parse_call_error_not_enough_arguments();
	test_parse_call_error_too_many_arguments();
	test_parse_call_error_expected_expression();
	test_parse_not_error_expected_factor();
	test_parse_not_error_no_value();
	test_parse_not_error_not_boolean();
    test_parse_factor_newline_var();
    test_parse_factor_newline_var_assign();
    test_parse_factor_newline_anonymous_function();
    test_parse_factor_newline_anonymous_function_var();
    test_parse_factor_newline_not();
    test_parse_factor_newline_sign();
    test_parse_factor_newline_array_literal();
    test_parse_factor_newline_array_parenthesis();
}
