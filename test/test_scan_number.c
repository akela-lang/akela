#include "zinc/unit_test.h"
#include "akela/lookahead_char.h"
#include "akela/source.h"
#include "akela/lex.h"
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

	/* allocate sns{} */
	scan_setup("-", &sns, &lc, &el);

	valid = lex(&sns, &t);
	expect_true(valid, "valid 0");
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

	/* allocate sns{} */
	scan_setup("500", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500", "500");
	expect_size_t_equal(t->loc.line, 1, "line 10");
	expect_size_t_equal(t->loc.col, 1, "col 10");

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

	/* allocate sns{} */
	scan_setup("500.", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
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

	/* allocate sns{} */
	scan_setup("500.123", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
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

	scan_setup("500e", &sns, &lc, &el);

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "valid 0");
	assert_ptr(t, "ptr t 0");
	expect_int_equal(t->type, token_number, "number 0");
	expect_str(&t->value, "500", "500 0");

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "valid 1");
	assert_ptr(t, "ptr t 1");
	expect_int_equal(t->type, token_id, "number 1");
	expect_str(&t->value, "e", "e 1");

	token_destroy(t);
	free(t);

	scan_teardown(&sns);
}

void test_scan_number_fraction_exponent_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;

	scan_setup("500.123e", &sns, &lc, &el);

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "0 valid");
	assert_ptr(t, "0 ptr t");
	expect_int_equal(t->type, token_number, "0 number");
	expect_str(&t->value, "500.123e", "0 value");

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "1 valid");
	assert_ptr(t, "1 ptr t");
	expect_int_equal(t->type, token_eof, "1 eof");
	expect_str(&t->value, "", "1 value");

	token_destroy(t);
	free(t);

	scan_teardown(&sns);
}

void test_scan_number_fraction_exponent()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;

	scan_setup("500.123e2", &sns, &lc, &el);

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123e2", "500.123e2");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	token_destroy(t);
	free(t);

	scan_teardown(&sns);
}

void test_scan_number_fraction_exponent_sign_start_negative()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;

	scan_setup("500.123e-", &sns, &lc, &el);

	valid = lex(&sns, &t);
	expect_has_errors(sns.el);
	expect_false(valid, "0 valid");
	expect_source_error(sns.el, "expected number after exponent sign");

	token_destroy(t);
	free(t);

	scan_teardown(&sns);
}

void test_scan_number_fraction_exponent_sign_start_positive()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;

	scan_setup("500.123e+", &sns, &lc, &el);

	valid = lex(&sns, &t);
	expect_has_errors(sns.el);
	expect_false(valid, "0 valid");
	expect_source_error(sns.el, "expected number after exponent sign");

	token_destroy(t);
	free(t);

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

	/* allocate sns{} */
	scan_setup("500.123e-2", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
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

	/* allocate sns{} */
	scan_setup("500.123e+2", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
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

void test_scan_number_exponent_add()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct scan_state sns;
	bool valid;
	struct token* t;

	scan_setup("500.123e + 1", &sns, &lc, &el);

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "0 valid");
	assert_ptr(t, "0 ptr t");
	expect_int_equal(t->type, token_number, "0 number");
	expect_str(&t->value, "500.123e", "0 value");
	expect_int_equal(t->loc.line, 1, "0 line 10");
	expect_int_equal(t->loc.col, 1, "0 col 10");

	token_destroy(t);
	free(t);

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "1 valid");
	assert_ptr(t, "1 ptr t");
	expect_int_equal(t->type, token_plus, "1 plus");
	expect_size_t_equal(t->loc.line, 1, "1 line");
	expect_size_t_equal(t->loc.col, 10, "1 col");

	token_destroy(t);
	free(t);

	valid = lex(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "2 valid");
	assert_ptr(t, "2 ptr t");
	expect_int_equal(t->type, token_number, "2 number");
	expect_str(&t->value, "1", "2 value");
	expect_size_t_equal(t->loc.line, 1, "2 line");
	expect_size_t_equal(t->loc.col, 12, "2 col");

	token_destroy(t);
	free(t);

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
