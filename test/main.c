#include "../src/scan.h"
#include "../src/parse.h"
#include "../src/dag.h"
#include "testing.h"

struct token_list tl;
struct string s;
struct dag_node* root;

void setup(char* line)
{
	test_name("setup: %s", line);
	string_init(&s);
	enum result_enum r = array2string(line, &s);
	assert_ok(r, "array2string");
	token_list_init(&tl);
	root = NULL;
}

void teardown()
{
	test_name("teardown");
	string_reset(&s);
	token_list_reset(&tl);
	dag_destroy(root);
}

void test_scan()
{
	test_name("scan");

	enum result_enum r;
	r = scan(&s, &tl);
	assert_ok(r, "scan");

	struct token* t0 = get_token(tl.head, 0);
	assert_ptr(t0, "get token");
	assert_int_equal(t0->type, token_word, "is word");

	struct token* t1 = get_token(tl.head, 1);
	assert_ptr(t1, "get token");
	assert_int_equal(t1->type, token_plus, "is plus");

	struct token* t2 = get_token(tl.head, 2);
	assert_ptr(t2, "get token");
	assert_int_equal(t2->type, token_number, "is number");

	struct token* t3 = get_token(tl.head, 3);
	assert_null(t3, "no 3rd argument");
}

void test_parse()
{
	enum result_enum r = parse(&tl, &root);
	assert_ok(r, "parse");
	
	assert_ptr(root, "root");
	assert_int_equal(root->type, dag_type_plus, "plus");

	struct dag_node* left = dag_get_child(root, 0);
	assert_ptr(left, "left");
	expect_int_equal(left->type, dag_type_word, "word");

	struct dag_node* right = dag_get_child(root, 1);
	assert_ptr(right, "right");
	expect_int_equal(right->type, dag_type_number, "number");
}

int main(void) {
	setup("speed + 1");
	test_scan();
	test_parse();
	teardown();
	print_results();
	return 0;
}
