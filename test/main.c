#include "../src/scan.h"
#include "../src/parse.h"

int test_count = 0;
int error_count = 0;

struct token_list tl;
struct string s;

void assert_int_equal(int a, int b, char* message)
{
	test_count++;
	if (a == b) return;
	printf("%d = %d error: %s\n", a,b, message);
	error_count++;
}

void assert_true(int value, char* message)
{
	test_count++;
	if (value) return;
	printf("%d = true error: %s\n", value, message);
	error_count++;
}

void assert_ptr(void* p, char* message)
{
	test_count++;
	if (p) return;
	printf("%p != NULL error: %s\n", p, message);
	error_count++;
}

void assert_null(void* p, char* message)
{
	test_count++;
	if (p == NULL) return;
	printf("%p == null error: %s\n", p, message);
	error_count++;
}

void test_name(char* name)
{
	printf("%s\n", name);
}

static void setup()
{
	string_init(&s);
	enum result_enum r = array2string("speed + 1", &s);
	assert_int_equal(r, ok_result, "array2string ok");
	token_list_init(&tl);
}

static void test_scan()
{
	test_name("scan");

	enum result_enum r;
	r = scan(&s, &tl);
	assert_int_equal(r, ok_result, "scan ok");

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
	printf("tests: %d\n", test_count);
	printf("errors: %d\n", error_count);
	return 0;
}
