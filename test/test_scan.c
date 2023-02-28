#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/memory.h"
#include "alba/unit_test_compiler.h"
#include "alba/token.h"
#include "alba/scan.h"
#include "alba/uconv.h"
#include "alba/input.h"
#include "alba/lookahead_char.h"
#include "alba/source.h"
#include "test_scan_setup.h"
#include "zinc/error_unit_test.h"

/* dynamic-output-none */
void test_scan_blank()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	struct token* t;
	int got_token;
	bool valid;

	/* allocate sns{} */
	scan_setup("", &sns, &lc, &el);

	/* allocatge sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token 0");
	expect_int_equal(t->type, token_none, "none");
	expect_str(&t->value, "", "(blank)");
	expect_false(got_token, "got token false");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_assign()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("a = 1", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "a", "a");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_equal, "equal");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_addition()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("speed + 1", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "speed", "speed");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_subtraction()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("100 - delta", &sns, &lc, &el);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_minus, "minus");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "delta", "delta");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_multiplication()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("100 * 20", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_mult, "mult");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "20", "20");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_divide()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("45 / 11", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "45", "45");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_divide, "divide");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "is number");
	expect_str(&t->value, "11", "11");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_stmts_expr()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("i + 1\nx * 4", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline");
	assert_true(got_token, "got token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");
	assert_true(got_token, "got token 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_mult, "mult");
	assert_true(got_token, "got token 5");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");
	assert_true(got_token, "got token 6");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 7");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_stmts_expr2()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("i + 1\nx * 4\n", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline");
	assert_true(got_token, "got token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");
	assert_true(got_token, "got token 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_mult, "mult");
	assert_true(got_token, "got token 5");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");
	assert_true(got_token, "got token 6");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline2");
	assert_true(got_token, "got token 7");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 8");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 9");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_stmts_assign()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("i + 1\nx = 4", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");
	assert_true(got_token, "got token 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");
	assert_true(got_token, "got token 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	assert_true(got_token, "got token 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline");
	assert_true(got_token, "got token 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");
	assert_true(got_token, "got token 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_equal, "equal");
	assert_true(got_token, "got token 5");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");
	assert_true(got_token, "got token 6");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 7");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_function()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("function foo () \n end", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_function, "function");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "foo", "foo");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_left_paren, "left paren");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_right_paren, "right paren");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_end, "end");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token 6");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_comma()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup(",", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_comma, "comma");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "got token");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_semicolon()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup(";", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_semicolon, "semicolon");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "got token");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_if() {
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("if elseif else", &sns, &lc, &el);
	
	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_if, "if");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_elseif, "elseif");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_else, "else");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(!got_token, "no token");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{}*/
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_compound_operators() {
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("== != <= >= && || ::", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_double_equal, "double equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_not_equal, "not equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_less_than_or_equal, "less than or equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_greater_than_or_equal, "greater than or equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_and, "and");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_or, "or");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 6");
	expect_int_equal(t->type, token_double_colon, "double colon");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_compound_operators2() {
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("= ! < > & | :", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_equal, "equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_not, "not");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_less_than, "less than");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_greater_than, "greater_than");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_ampersand, "ampersand");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_vertical_bar, "vertical_bar");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 6");
	expect_int_equal(t->type, token_colon, "colon");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_for_range()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("for i = 0:10 1 end", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_for, "for");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_id, "id 1");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_equal, "equal 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_number, "number 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_colon, "colon 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_number, "number 5");
	expect_str(&t->value, "10", "10 5");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 6");
	expect_int_equal(t->type, token_number, "number 6");
	expect_str(&t->value, "1", "1 6");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 7");
	expect_int_equal(t->type, token_end, "end 7");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_for_iteration()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("for x in list 1 end", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_for, "for");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_id, "id 1");
	expect_str(&t->value, "x", "x 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_in, "in 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 3");
	expect_int_equal(t->type, token_id, "id 3");
	expect_str(&t->value, "list", "list 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 4");
	expect_int_equal(t->type, token_number, "number 4");
	expect_str(&t->value, "1", "1 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 5");
	expect_int_equal(t->type, token_end, "end 5");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_error_unrecognized_character()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("$", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_false(valid, "scan_get_token");
	assert_has_errors(sns.el);
	assert_false(got_token, "got token");
	assert_null(t, "t");
	expect_error(&el, "Unrecognized character: $");

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_square_brackets()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("[]", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_left_square_bracket, "left-square-bracket 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_right_square_bracket, "right-square-bracket 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_string()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("\"hello\"", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_string, "string 0");
	expect_str(&t->value, "hello", "hello 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_string2()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("x = \"\\\\hello\n\r\"", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_id, "id 0");
	expect_str(&t->value, "x", "x 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_equal, "equal 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_true(got_token, "got token 2");
	expect_int_equal(t->type, token_string, "string 2");
	expect_str(&t->value, "\\hello\n\r", "hello 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_string_escape_error()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("\"\\x\"", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_false(valid, "scan_get_token");
	assert_has_errors(sns.el);
	expect_error(&el, "Unrecognized escape sequence: x");

	assert_false(got_token, "got_token");
	assert_null(t, "t");

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_line_col()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("10 + 20\n30 + 40", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 10");
	assert_true(got_token, "got token 10");
	expect_int_equal(t->type, token_number, "number 10");
	expect_str(&t->value, "10", "10");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t +");
	assert_true(got_token, "got token +");
	expect_int_equal(t->type, token_plus, "plus +");
	expect_int_equal(t->loc.line, 1, "line +");
	expect_int_equal(t->loc.col, 4, "col +");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 20");
	assert_true(got_token, "got token 20");
	expect_int_equal(t->type, token_number, "number 20");
	expect_int_equal(t->loc.line, 1, "line 20");
	expect_int_equal(t->loc.col, 6, "col 20");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t newline");
	assert_true(got_token, "got token newline");
	expect_int_equal(t->type, token_newline, "newline newline");
	expect_int_equal(t->loc.line, 1, "line newline");
	expect_int_equal(t->loc.col, 8, "col newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 30");
	assert_true(got_token, "got token 30");
	expect_int_equal(t->type, token_number, "newline 30");
	expect_int_equal(t->loc.line, 2, "line 30");
	expect_int_equal(t->loc.col, 1, "col 30");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t +");
	assert_true(got_token, "got token +");
	expect_int_equal(t->type, token_plus, "plus +");
	expect_int_equal(t->loc.line, 2, "line +");
	expect_int_equal(t->loc.col, 4, "col +");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 40");
	assert_true(got_token, "got token 40");
	expect_int_equal(t->type, token_number, "number 40");
	expect_int_equal(t->loc.line, 2, "line 40");
	expect_int_equal(t->loc.col, 6, "col 40");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

void test_scan_error_underscore_letter()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("_1", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_has_errors(sns.el);
	expect_false(valid, "scan_get_token valid");
	expect_null(t, "null t");
	expect_false(got_token, "got token");
	expect_error(&el, "Must have a letter following underscore at start of id");

	/* destroy sns{} */
	scan_teardown(&sns);
}

void test_scan_error_underscore_letter2()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("__", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_has_errors(sns.el);
	expect_false(valid, "scan_get_token valid");
	expect_null(t, "null t");
	expect_false(got_token, "got token");
	expect_error(&el, "Must have a letter following underscore at start of id");

	/* destroy sns{} */
	scan_teardown(&sns);
}

void test_scan_error_exponent_sign()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("100e-a", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_has_errors(sns.el);
	expect_false(valid, "scan_get_token valid");
	expect_null(t, "null t");
	expect_false(got_token, "got token");
	expect_error(&el, "invalid number");

	/* destroy sns{} */
	scan_teardown(&sns);
}

void test_scan_module()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	int got_token;

	scan_setup("module 1 end", &sns, &lc, &el);

	struct token* module;
	valid = scan_get_token(&sns, &got_token, &module);
	assert_no_errors(sns.el);
	assert_true(valid, "valid module");
	assert_true(got_token, "got_token module");
	assert_ptr(module, "ptr module");
	expect_int_equal(module->type, token_module, "module module");

	token_destroy(module);
	free(module);

	struct token* number;
	valid = scan_get_token(&sns, &got_token, &number);
	assert_no_errors(sns.el);
	assert_true(valid, "valid number");
	assert_true(got_token, "got_token number");
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, token_number, "number number");
	expect_str(&number->value, "1", "1 number");

	token_destroy(number);
	free(number);

	struct token* end;
	valid = scan_get_token(&sns, &got_token, &end);
	assert_no_errors(sns.el);
	assert_true(valid, "valid end");
	assert_true(got_token, "got_token end");
	assert_ptr(end, "ptr end");
	expect_int_equal(end->type, token_end, "end end");

	token_destroy(end);
	free(end);

	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan()
{
	test_scan_blank();
	test_scan_assign();
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
	test_scan_for_range();
	test_scan_for_iteration();
	test_scan_error_unrecognized_character();
	test_scan_square_brackets();
	test_scan_string();
	test_scan_string2();
	test_scan_string_escape_error();
	test_scan_line_col();
	test_scan_error_underscore_letter();
	test_scan_error_underscore_letter2();
	test_scan_error_exponent_sign();
	test_scan_module();
}
