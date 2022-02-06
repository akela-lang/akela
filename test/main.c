#include "../src/scan.h"
#include "../src/parse.h"
#include "../src/dag.h"
#include "testing.h"

struct token_list tl;
struct string s;
struct dag_node* root;

void setup(char* line)
{
	string_init(&s);
	enum result r = array2string(line, &s);
	assert_ok(r, "array2string");
	token_list_init(&tl);
	root = NULL;
}

void setup_scan(char* line)
{
	setup(line);
	enum result r;
	r = scan(&s, &tl);
	assert_ok(r, "scan");
}

void setup_parse(char* line)
{
	setup(line);
	enum result r;
	r = scan(&s, &tl);
	assert_ok(r, "scan");
	r = parse(&tl, &root);
	assert_ok(r, "parse");
}

void teardown()
{
	string_reset(&s);
	token_list_reset(&tl);
	dag_destroy(root);
}

void test_scan_addition()
{
	test_name(__func__);

	setup_scan("speed + 1");

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_word, "is word");
	expect_str(&t0->value, "speed", "speed");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	expect_int_equal(t1->type, token_plus, "is plus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	expect_int_equal(t2->type, token_number, "is number");
	expect_str(&t2->value, "1", "1");

	struct token* t3 = get_token(tl.head, 3);
	expect_null(t3, "no 3rd argument");

	teardown();
}

void test_scan_subtraction()
{
	test_name(__func__);

	setup_scan("100 - delta");

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_number, "is number");
	expect_str(&t0->value, "100", "100");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	expect_int_equal(t1->type, token_minus, "is minus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	expect_int_equal(t2->type, token_word, "is word");
	expect_str(&t2->value, "delta", "delta");

	struct token* t3 = get_token(tl.head, 3);
	expect_null(t3, "no 3rd argument");

	teardown();
}

void test_scan_multiplication()
{
	test_name(__func__);

	setup_scan("100 * 20");

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	expect_int_equal(t0->type, token_number, "is number");
	expect_str(&t0->value, "100", "100");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	expect_int_equal(t1->type, token_mult, "is mult");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	expect_int_equal(t2->type, token_number, "is number");
	expect_str(&t2->value, "20", "20");

	struct token* t3 = get_token(tl.head, 3);
	expect_null(t3, "no 3rd argument");

	teardown();
}

void test_parse_addition()
{
	test_name(__func__);

	setup_parse("speed + 1");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");
	expect_str(&left->value, "speed", "speed");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "1", "1");

	assert_null(dag_get_child(root, 2), "only 2 children");

	teardown();
}

void test_parse_subtraction()
{
	test_name(__func__);

	setup_parse("100 - delta");

	assert_ptr(root, "root");
	expect_int_equal(root->type, dag_type_minus, "minus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "100", "100");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_word, "word");
	expect_str(&right->value, "delta", "delta");

	assert_null(dag_get_child(root, 2), "only 2 children");

	teardown();
}

void test_parse_multiplication()
{
	test_name(__func__);

	setup_parse("5 * 2");

	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_mult, "mult");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_number, "number");
	expect_str(&left->value, "5", "5");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
	expect_str(&right->value, "2", "2");

	teardown();
}

int main(void) {
	test_scan_addition();
	test_scan_subtraction();
	test_scan_multiplication();

	test_parse_addition();
	test_parse_subtraction();
	test_parse_multiplication();

	print_results();
	return 0;
}
