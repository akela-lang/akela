#include "assert.h"
#include "test_parse.h"
#include "alba/dag.h"
#include "alba/parse_tools.h"

void test_parse_num()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "32", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_number, "number");
	expect_str(&root->value, "32", "32");

	parse_teardown(&al, &ts);
}

void test_parse_id()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "lot", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_id, "id");
	expect_str(&root->value, "lot", "lot");

	parse_teardown(&al, &ts);
}

void test_parse_num_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "-30", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_sign, "sign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_minus, "minus");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	parse_teardown(&al, &ts);
}

void test_parse_num_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "+30", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_sign, "sign");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_plus, "plus");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	assert_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "30", "30");

	parse_teardown(&al, &ts);
}

void test_parse_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + 1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_add_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + +1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	parse_teardown(&al, &ts);
}

void test_parse_add_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + -1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	parse_teardown(&al, &ts);
}

void test_parse_sub()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "100 - delta", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "100", "100");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_id, "id");
	expect_str(&right->value, "delta", "delta");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_sub_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed - +1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	parse_teardown(&al, &ts);
}

void test_parse_sub_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed - -1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	parse_teardown(&al, &ts);
}


void test_parse_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "5 * 2", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	parse_teardown(&al, &ts);
}

void test_parse_mult_positive()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed * +1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_plus, "plus");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	parse_teardown(&al, &ts);
}

void test_parse_mult_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed * -1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(root, 2), "only 2 children");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_minus, "minus2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_str(&right2->value, "1", "1");


	parse_teardown(&al, &ts);
}


void test_parse_divide()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "52 / 2", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_divide, "divide");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "52", "52");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	parse_teardown(&al, &ts);
}

void test_parse_add_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 + 2 + 3", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus root");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, dag_type_plus, "plus left");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, dag_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, dag_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, dag_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

	parse_teardown(&al, &ts);
}

void test_parse_mult_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 * 2 * 3", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_mult, "mult root");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "ptr left");
	expect_int_equal(left->type, dag_type_mult, "mult left");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, dag_type_number, "number left2");
	expect_str(&left2->value, "1", "1 left2");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "ptr right");
	expect_int_equal(right->type, dag_type_number, "number right");
	expect_str(&right->value, "2", "2 right");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, dag_type_number, "number right2");
	expect_str(&right2->value, "3", "3 right2");

	parse_teardown(&al, &ts);
}

void test_parse_add_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "5 + 3 * 2", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_mult, "mult");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number 2");
	expect_str(&left2->value, "3", "3");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "2", "2");

	parse_teardown(&al, &ts);
}

void test_parse_mult_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "4 * 3 + 2", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_mult, "mult");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number 4");
	expect_str(&left2->value, "4", "4");

	struct dag_node* right2 = dag_get_child(left, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	parse_teardown(&al, &ts);
}

void test_parse_paren_num()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(32)", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_number, "number");
	expect_str(&root->value, "32", "32");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(speed + 1)", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(speed) + 1", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add3()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "speed + (1)", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add_add()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 + (2 + 3)", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "1", "1");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_plus, "plus 2");

	assert_null(dag_get_child(root, 2), "null");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "right");
	expect_int_equal(left2->type, dag_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	parse_teardown(&al, &ts);
}

void test_parse_paren_add_add2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(1 + 2) + 3", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_plus, "plus 2");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	parse_teardown(&al, &ts);
}

void test_parse_paren_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(5 * 2)", &ts, &root);

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	parse_teardown(&al, &ts);
}

void test_parse_paren_mult_mult()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "1 * (2 * 3)", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "1", "1");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_mult, "mult 2");

	struct dag_node* left2 = dag_get_child(right, 0);
	assert_ptr(left2, "left 2");
	expect_int_equal(left2->type, dag_type_number, "number 2");
	expect_str(&left2->value, "2", "2");

	struct dag_node* right2 = dag_get_child(right, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	parse_teardown(&al, &ts);
}

void test_parse_paren_mult_mult2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "(1 * 2) * 3", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_mult, "mult 2");

	struct dag_node* left2 = dag_get_child(left, 0);
	assert_ptr(left2, "left2");
	expect_int_equal(left2->type, dag_type_number, "number");
	expect_str(&left2->value, "1", "1");

	struct dag_node* right = dag_get_child(left, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number 2");
	expect_str(&right->value, "2", "2");

	struct dag_node* right2 = dag_get_child(root, 1);
	assert_ptr(right2, "right2");
	expect_int_equal(right2->type, dag_type_number, "number 3");
	expect_str(&right2->value, "3", "3");

	parse_teardown(&al, &ts);
}

void test_parse_comparison()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "if count == 10\nx + 1\nelseif count <= 11\nx + 2\nelseif count >= 12\nx + 3\nend", &ts, &root);

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_if, "if root");

	struct dag_node* cb0 = dag_get_child(root, 0);
	assert_ptr(cb0, "ptr cb0");
	expect_int_equal(cb0->type, dag_type_conditional_branch, "conditional branch cb0");

	struct dag_node* cond0 = dag_get_child(cb0, 0);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, dag_type_equality, "equality cond0");

	struct dag_node* count0 = dag_get_child(cond0, 0);
	assert_ptr(count0, "ptr count0");
	expect_int_equal(count0->type, dag_type_id, "id count0");
	expect_str(&count0->value, "count", "count count0");

	struct dag_node* num0 = dag_get_child(cond0, 1);
	assert_ptr(num0, "ptr num0");
	expect_int_equal(num0->type, dag_type_number, "number num0");
	expect_str(&num0->value, "10", "10 num0");

	struct dag_node* cb1 = dag_get_child(root, 1);
	assert_ptr(cb1, "ptr cb1");
	expect_int_equal(cb1->type, dag_type_conditional_branch, "conditional branch cb1");

	struct dag_node* cond1 = dag_get_child(cb1, 0);
	assert_ptr(cond1, "ptr cond1");
	expect_int_equal(cond1->type, dag_type_less_than_or_equal, "less than or equal cond1");

	struct dag_node* count1 = dag_get_child(cond1, 0);
	assert_ptr(count1, "ptr count1");
	expect_int_equal(count1->type, dag_type_id, "id count1");
	expect_str(&count1->value, "count", "count count1");

	struct dag_node* num1 = dag_get_child(cond1, 1);
	assert_ptr(num1, "ptr num1");
	expect_int_equal(num1->type, dag_type_number, "number num1");
	expect_str(&num1->value, "11", "11 num1");

	struct dag_node* cb2 = dag_get_child(root, 2);
	assert_ptr(cb2, "ptr cb2");
	expect_int_equal(cb2->type, dag_type_conditional_branch, "conditional branch cb2");

	struct dag_node* cond2 = dag_get_child(cb2, 0);
	assert_ptr(cond1, "ptr cond2");
	expect_int_equal(cond2->type, dag_type_greater_than_or_equal, "greater than or equal cond2");

	struct dag_node* count2 = dag_get_child(cond2, 0);
	assert_ptr(count2, "ptr count2");
	expect_int_equal(count2->type, dag_type_id, "id count2");
	expect_str(&count2->value, "count", "count count2");

	struct dag_node* num2 = dag_get_child(cond2, 1);
	assert_ptr(num2, "ptr num2");
	expect_int_equal(num2->type, dag_type_number, "number num2");
	expect_str(&num2->value, "12", "12 num2");

	parse_teardown(&al, &ts);
}

void test_parse_expression()
{
	test_parse_num();
	test_parse_id();
	test_parse_num_negative();
	test_parse_num_positive();
	test_parse_add();
	test_parse_add_positive();
	test_parse_add_negative();
	test_parse_sub();
	test_parse_sub_positive();
	test_parse_sub_negative();
	test_parse_mult();
	test_parse_mult_positive();
	test_parse_mult_negative();
	test_parse_divide();
	test_parse_add_add();
	test_parse_mult_mult();
	test_parse_add_mult();
	test_parse_mult_add();
	test_parse_paren_num();
	test_parse_paren_add();
	test_parse_paren_add2();
	test_parse_paren_add3();
	test_parse_paren_add_add();
	test_parse_paren_add_add2();
	test_parse_paren_mult();
	test_parse_paren_mult_mult();
	test_parse_paren_mult_mult2();
	test_parse_comparison();
}
