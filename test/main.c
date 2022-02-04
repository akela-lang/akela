#include "../src/scan.h"
#include "../src/parse.h"
#include "testing.h"

struct token_list tl;
struct string s;

void setup()
{
	string_init(&s);
	enum result_enum r = array2string("speed + 1", &s);
	assert_ok(r, "array2string");
	token_list_init(&tl);
}

void teardown()
{
	string_reset(&s);
	token_list_reset(&tl);
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

int main(void) {
	setup();
	test_scan();
	teardown();
	print_results();
	return 0;
}
