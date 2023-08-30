#include "zinc/unit_test.h"
#include "akela/lookahead_char.h"
#include "akela/source.h"
#include "akela/scan.h"
#include "test_scan_setup.h"
#include "akela/unit_test_compiler.h"
#include "zinc/error_unit_test.h"

void test_scan_number_negative_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("-", &sns, &lc, &el);

	valid = scan_get_token(&sns, &got_token, &t);
	expect_true(valid, "valid 0");
	expect_true(got_token, "got_token 0");
	expect_int_equal(t->type, token_minus, "minus 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_whole()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500", "500");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_fraction_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.", "500.");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_fraction()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123", "500.123");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

void test_scan_number_exponent_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500e", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "valid 0");
	assert_ptr(t, "ptr t 0");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_number, "number 0");
	expect_str(&t->value, "500", "500 0");

	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "valid 1");
	assert_ptr(t, "ptr t 1");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_id, "number 1");
	expect_str(&t->value, "e", "e 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_fraction_exponent_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123e", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "valid 0");
	assert_ptr(t, "ptr t 0");
	assert_true(got_token, "got token 0");
	expect_int_equal(t->type, token_number, "number 0");
	expect_str(&t->value, "500.123", "500.123 0");

	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "valid 1");
	assert_ptr(t, "ptr t 1");
	assert_true(got_token, "got token 1");
	expect_int_equal(t->type, token_id, "number 1");
	expect_str(&t->value, "e", "e 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_fraction_exponent()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123e2", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123e2", "500.123e2");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_fraction_exponent_sign_start_negative()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123e-", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	expect_has_errors(sns.el);
	expect_false(valid, "valid 0");
	expect_false(got_token, "got token 0");
	expect_source_error(sns.el, "invalid number");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_fraction_exponent_sign_start_positive()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123e+", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	expect_has_errors(sns.el);
	expect_false(valid, "valid 0");
	expect_false(got_token, "got token 0");
	expect_source_error(sns.el, "invalid number");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_fraction_exponent_negative()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123e-2", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123e-2", "500.123e-2");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_exponent_positive()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct scan_state sns;
	struct error_list el;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123e+2", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123e+2", "500.123e+2");
	expect_int_equal(t->loc.line, 1, "line");
	expect_int_equal(t->loc.col, 1, "col");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

/* dynamic-output-none */
void test_scan_number_exponent_add()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;
	int got_token;

	/* allocate sns{} */
	scan_setup("500.123e + 1", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123", "500.123");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "e", "e");
	expect_int_equal(t->loc.line, 1, "line");
	expect_int_equal(t->loc.col, 8, "col");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_plus, "plus");
	expect_int_equal(t->loc.line, 1, "line");
	expect_int_equal(t->loc.col, 10, "col");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &got_token, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t");
	assert_true(got_token, "got token");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");
	expect_int_equal(t->loc.line, 1, "line");
	expect_int_equal(t->loc.col, 12, "col");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy sns{} */
	scan_teardown(&sns);
}

void test_scan_number()
{
	test_scan_number_negative_start();
	test_scan_number_whole();
	test_scan_number_fraction_start();
	test_scan_number_fraction();
	test_scan_number_exponent_start();
	test_scan_number_fraction_exponent_start();
	test_scan_number_fraction_exponent();
	test_scan_number_fraction_exponent_sign_start_negative();
	test_scan_number_fraction_exponent_sign_start_positive();
	test_scan_number_fraction_exponent_negative();
	test_scan_number_exponent_positive();
	test_scan_number_exponent_add();
}
