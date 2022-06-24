#include "assert.h"
#include "alba/token.h"
#include "alba/scan.h"
#include "alba/allocator.h"
#include "alba/uconv.h"
#include "alba/input.h"

enum result scan_setup(struct allocator* al, char* line, struct input_state* is)
{
	enum result r;

	allocator_init(al);

	struct buffer* bf;
	r = allocator_malloc(al, &bf, sizeof(struct buffer));
	assert_ok(r, "allocator malloc string");
	buffer_init(bf);
	r = array2buffer(al, line, bf);
	assert_ok(r, "ok");

	struct string_data* sd;
	r = allocator_malloc(al, &sd, sizeof(struct string_data));
	assert_ok(r, "allocator_malloc string_data");
	string_data_init(bf, sd);

	UConverter* conv;
	r = conv_open(&conv);
	assert_ok(r, "conv_open");
	input_state_init(string_getchar, sd, conv, is);
	return r;
}

void scan_teardown(struct allocator* al, struct input_state* is)
{
	conv_close(is->conv);
	allocator_destroy(al);
}

void test_scan_assign()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "a = 1", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token 0");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "a", "a");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan get_token 1");
	expect_int_equal(t->type, token_equal, "equal");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token2");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!got_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_num()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "11", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token");
	expect_int_equal(t->type, token_number, "number");
	assert_true(got_token, "got token");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token");
	assert_true(!got_token, "no token");

	scan_teardown(&al, &is);
}

void test_scan_addition()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "speed + 1", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "speed", "speed");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!got_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_subtraction()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "100 - delta", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t->type, token_minus, "minus");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "delta", "delta");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!got_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_multiplication()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "100 * 20", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t->type, token_mult, "mult");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "20", "20");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!got_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_divide()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "45 / 11", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "45", "45");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t->type, token_divide, "divide");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t->type, token_number, "is number");
	expect_str(&t->value, "11", "11");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token3");
	assert_true(!got_token, "no token 3");

	scan_teardown(&al, &is);
}

void test_scan_stmts_expr()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "i + 1\nx * 4", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 3");
	expect_int_equal(t->type, token_newline, "newline");
	assert_true(got_token, "got token 3");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 4");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");
	assert_true(got_token, "got token 4");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 5");
	expect_int_equal(t->type, token_mult, "mult");
	assert_true(got_token, "got token 5");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 6");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");
	assert_true(got_token, "got token 6");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token7");
	assert_true(!got_token, "no token 7");

	scan_teardown(&al, &is);
}

void test_scan_stmts_expr2()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "i + 1\nx * 4\n", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 3");
	expect_int_equal(t->type, token_newline, "newline");
	assert_true(got_token, "got token 3");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 4");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");
	assert_true(got_token, "got token 4");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 5");
	expect_int_equal(t->type, token_mult, "mult");
	assert_true(got_token, "got token 5");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 6");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");
	assert_true(got_token, "got token 6");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 7");
	expect_int_equal(t->type, token_newline, "newline2");
	assert_true(got_token, "got token 7");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 8");
	assert_true(!got_token, "no token 8");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token9");
	assert_true(!got_token, "no token 9");

	scan_teardown(&al, &is);
}

void test_scan_stmts_assign()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "i + 1\nx = 4", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");
	assert_true(got_token, "got token 0");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 3");
	expect_int_equal(t->type, token_newline, "newline");
	assert_true(got_token, "got token 3");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 4");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");
	assert_true(got_token, "got token 4");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 5");
	expect_int_equal(t->type, token_equal, "equal");
	assert_true(got_token, "got token 5");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 6");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");
	assert_true(got_token, "got token 6");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "scan_get_token7");
	assert_true(!got_token, "no token 7");

	scan_teardown(&al, &is);
}

void test_scan_function()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "function foo () \n end", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_function, "function");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "foo", "foo");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_left_paren, "left paren");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 3");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_right_paren, "right paren");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 4");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_newline, "newline");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 5");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_end, "end");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 6");
	assert_true(!got_token, "no token 6");

	scan_teardown(&al, &is);
}

void test_scan_comma()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, ",", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_comma, "comma");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(!got_token, "got token");

	scan_teardown(&al, &is);
}

void test_scan_semicolon()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, ";", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_semicolon, "semicolon");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(!got_token, "got token");

	scan_teardown(&al, &is);
}

void test_scan_if() {
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "if elseif else", &is);
	
	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_if, "if");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_elseif, "elseif");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_else, "else");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token");
	assert_true(!got_token, "no token");

	scan_teardown(&al, &is);
}

void test_scan_compound_operators() {
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "== != <= >= && ||", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_double_equal, "double equal");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_not_equal, "not equal");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_less_than_or_equal, "less than or equal");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 3");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_greater_than_or_equal, "greater than or equal");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 4");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_and, "and");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 5");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_or, "or");

	scan_teardown(&al, &is);
}

void test_scan_compound_operators2() {
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "= ! < > & |", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_equal, "equal");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_not, "not");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_less_than, "less than");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 3");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_greater_than, "greater_than");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 4");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_ampersand, "ampersand");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 5");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_vertical_bar, "vertical_bar");

	scan_teardown(&al, &is);
}

void test_scan_for()
{
	test_name(__func__);

	struct allocator al;
	struct input_state is;
	enum result r;
	struct token* t;
	int got_token;

	scan_setup(&al, "for i = 0; i < 10; i = i + 1; foo() end", &is);

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 0");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_for, "for");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 1");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_id, "id 1");
	expect_str(&t->value, "i", "i");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 2");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_equal, "equal 2");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 3");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_number, "number 3");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 4");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_semicolon, "semicolon 4");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 5");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_id, "id 5");
	expect_str(&t->value, "i", "i 5");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 6");
	assert_true(got_token, "got token 6");
	expect_int_equal(t->type, token_less_than, "less than 6");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 7");
	assert_true(got_token, "got token 7");
	expect_int_equal(t->type, token_number, "number 7");
	expect_str(&t->value, "10", "10 7");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 8");
	assert_true(got_token, "got token 8");
	expect_int_equal(t->type, token_semicolon, "semicolon 8");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 9");
	assert_true(got_token, "got token 9");
	expect_int_equal(t->type, token_id, "id 9");
	expect_str(&t->value, "i", "i 9");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 10");
	assert_true(got_token, "got token 10");
	expect_int_equal(t->type, token_equal, "id 10");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 11");
	assert_true(got_token, "got token 11");
	expect_int_equal(t->type, token_id, "id 11");
	expect_str(&t->value, "i", "i 11");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 12");
	assert_true(got_token, "got token 12");
	expect_int_equal(t->type, token_plus, "plus 12");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 13");
	assert_true(got_token, "got token 13");
	expect_int_equal(t->type, token_number, "number 13");
	expect_str(&t->value, "1", "1 13");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 14");
	assert_true(got_token, "got token 14");
	expect_int_equal(t->type, token_semicolon, "semicolon 14");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 15");
	assert_true(got_token, "got token 15");
	expect_int_equal(t->type, token_id, "number 15");
	expect_str(&t->value, "foo", "foo 15");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 16");
	assert_true(got_token, "got token 16");
	expect_int_equal(t->type, token_left_paren, "left paren 16");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 17");
	assert_true(got_token, "got token 17");
	expect_int_equal(t->type, token_right_paren, "right paren 17");

	r = scan_get_token(&al, &is, &got_token, &t);
	assert_ok(r, "get token 18");
	assert_true(got_token, "got token 18");
	expect_int_equal(t->type, token_end, "end 18");

	scan_teardown(&al, &is);
}

void test_scan()
{
	test_scan_assign();
	test_scan_num();
	test_scan_addition();
	test_scan_subtraction();
	test_scan_multiplication();
	test_scan_divide();
	test_scan_stmts_expr();
	test_scan_stmts_expr2();
	test_scan_stmts_assign();
	test_scan_function();
	test_scan_comma();
	test_scan_semicolon();
	test_scan_if();
	test_scan_compound_operators();
	test_scan_compound_operators2();
	test_scan_for();
}
