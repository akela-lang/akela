#include "assert.h"
#include "test_parse.h"
#include "alba/dag.h"
#include "alba/parse_tools.h"

void test_parse_blank()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "", &ts, &root);

	root = check_stmts(root, "stmts root");

	assert_null(root, "null root");

	parse_teardown(&al, &ts);
}

void test_parse_num()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "32", &ts, &root);

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* num = dag_get_child(root, 0);
	assert_ptr(num, "ptr num");
	expect_int_equal(num->type, dag_type_number, "number num");
	expect_str(&num->value, "32", "32 num");

	parse_teardown(&al, &ts);
}

void test_parse_id()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "app_name", &ts, &root);

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* id = dag_get_child(root, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "app_name", "app_name id");

	parse_teardown(&al, &ts);
}

void test_parse_id2()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "_a23", &ts, &root);

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* id = dag_get_child(root, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "_a23", "_a23 id");

	parse_teardown(&al, &ts);
}

void test_parse_id3()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "a2", &ts, &root);

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* id = dag_get_child(root, 0);
	assert_ptr(id, "ptr id");
	expect_int_equal(id->type, dag_type_id, "id id");
	expect_str(&id->value, "a2", "a2 id");

	parse_teardown(&al, &ts);
}

void test_parse_num_negative()
{
	test_name(__func__);

	struct allocator al;
	struct dag_node* root;
	struct token_state ts;

	parse_setup(&al, "-30", &ts, &root);

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sign = dag_get_child(root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, dag_type_sign, "sign sign");

	struct dag_node* left = dag_get_child(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_minus, "minus");

	struct dag_node* right = dag_get_child(sign, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sign = dag_get_child(root, 0);
	assert_ptr(sign, "ptr sign");
	expect_int_equal(sign->type, dag_type_sign, "sign sign");

	struct dag_node* left = dag_get_child(sign, 0);
	assert_ptr(left, "left");
	assert_int_equal(left->type, dag_type_plus, "plus");

	struct dag_node* right = dag_get_child(sign, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* add = dag_get_child(root, 0);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, dag_type_plus, "plus add");

	struct dag_node* left = dag_get_child(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(add, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* add = dag_get_child(root, 0);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, dag_type_plus, "plus add");

	struct dag_node* left = dag_get_child(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(add, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* add = dag_get_child(root, 0);
	assert_ptr(add, "ptr add");
	expect_int_equal(add->type, dag_type_plus, "plus add");

	struct dag_node* left = dag_get_child(add, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(add, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(add, 2), "only 2 children");

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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sub = dag_get_child(root, 0);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, dag_type_minus, "minus sub");

	struct dag_node* left = dag_get_child(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "100", "100");

	struct dag_node* right = dag_get_child(sub, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sub = dag_get_child(root, 0);
	assert_ptr(sub, "root");
	expect_int_equal(sub->type, dag_type_minus, "minus sub");

	struct dag_node* left = dag_get_child(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(sub, 2), "only 2 children");

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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* sub = dag_get_child(root, 0);
	assert_ptr(sub, "ptr sub");
	expect_int_equal(sub->type, dag_type_minus, "minus sub");

	struct dag_node* left = dag_get_child(sub, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(sub, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(sub, 2), "only 2 children");

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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 0);
	assert_ptr(mult, "root");
	expect_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(mult, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 0);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(mult, 2), "only 2 children");

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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 0);
	assert_ptr(mult, "ptr mult");
	expect_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_id, "id");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(mult, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_sign, "sign");

	assert_null(dag_get_child(mult, 2), "only 2 children");

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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* divide = dag_get_child(root, 0);
	assert_ptr(divide, "ptr divide");
	assert_int_equal(divide->type, dag_type_divide, "divide divide");

	struct dag_node* left = dag_get_child(divide, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "52", "52");

	struct dag_node* right = dag_get_child(divide, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* plus0 = dag_get_child(root, 0);
	assert_ptr(plus0, "ptr plus0");
	assert_int_equal(plus0->type, dag_type_plus, "plus plus0");

	struct dag_node* left = dag_get_child(plus0, 0);
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

	struct dag_node* right2 = dag_get_child(plus0, 1);
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

	assert_ptr(root, "ptr root");
	assert_int_equal(root->type, dag_type_stmts, "stmts root");

	struct dag_node* mult = dag_get_child(root, 0);
	assert_ptr(mult, "ptr mult");
	assert_int_equal(mult->type, dag_type_mult, "mult mult");

	struct dag_node* left = dag_get_child(mult, 0);
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

	struct dag_node* right2 = dag_get_child(mult, 1);
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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	root = check_stmts(root, "stmts root");

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

	parse_setup(&al, "count == 10\ncount <= 11\ncount >= 12", &ts, &root);

	struct dag_node* cond0 = dag_get_child(root, 0);
	assert_ptr(cond0, "ptr cond0");
	expect_int_equal(cond0->type, dag_type_equality, "equality cond0");

	struct dag_node* left0 = dag_get_child(cond0, 0);
	assert_ptr(left0, "ptr left0");
	expect_int_equal(left0->type, dag_type_id, "id left0");
	expect_str(&left0->value, "count", "count left0");

	struct dag_node* right0 = dag_get_child(cond0, 1);
	assert_ptr(right0, "ptr right0");
	expect_int_equal(right0->type, dag_type_number, "number right0");
	expect_str(&right0->value, "10", "10 right0");

	struct dag_node* cond1 = dag_get_child(root, 1);
	assert_ptr(cond1, "ptr cond1");
	expect_int_equal(cond1->type, dag_type_less_than_or_equal, "less than or equal cond1");

	struct dag_node* left1 = dag_get_child(cond1, 0);
	assert_ptr(left1, "ptr left1");
	expect_int_equal(left1->type, dag_type_id, "id left1");
	expect_str(&left1->value, "count", "count left1");

	struct dag_node* right1 = dag_get_child(cond1, 1);
	assert_ptr(right1, "ptr right1");
	expect_int_equal(right1->type, dag_type_number, "number right1");
	expect_str(&right1->value, "11", "11 right1");

	struct dag_node* cond2 = dag_get_child(root, 2);
	assert_ptr(cond2, "ptr cond2");
	expect_int_equal(cond2->type, dag_type_greater_than_or_equal, "greater than or equal cond2");

	struct dag_node* left2 = dag_get_child(cond2, 0);
	assert_ptr(left2, "ptr left2");
	expect_int_equal(left2->type, dag_type_id, "id left2");
	expect_str(&left2->value, "count", "count left2");

	struct dag_node* right2 = dag_get_child(cond2, 1);
	assert_ptr(right2, "ptr right2");
	expect_int_equal(right2->type, dag_type_number, "number right2");
	expect_str(&right2->value, "12", "12 right2");

	parse_teardown(&al, &ts);
}

void test_parse_expression()
{
	test_parse_blank();
	test_parse_num();
	test_parse_id();
	test_parse_id2();
	test_parse_id3();
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
