#include "zinc/unit_test.h"
#include "akela/lookahead_char.h"
#include "akela/source.h"
#include "akela/lex.h"
#include "test_lex_setup.h"
#include "akela/unit_test_compiler.h"
#include "zinc/error_unit_test.h"

void test_lex_number_negative_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("-", &ls, &lc, &el);

	valid = lex(&ls, &t);
	expect_true(valid, "valid 0");
	expect_int_equal(t->type, token_minus, "minus 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_whole()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("500", &ls, &lc, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500", "500");
	expect_size_t_equal(t->loc.line, 1, "line 10");
	expect_size_t_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_fraction_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("500.", &ls, &lc, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.", "500.");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_fraction()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("500.123", &ls, &lc, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123", "500.123");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_number_exponent_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("500e", &ls, &lc, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "0 valid");
	assert_ptr(t, "0 ptr t");
	expect_int_equal(t->type, token_number, "0 number");
	expect_str(&t->value, "500e", "0 value");

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "1 valid");
	assert_ptr(t, "1 ptr t");
	expect_int_equal(t->type, token_eof, "1 eof");
	expect_str(&t->value, "", "1 value");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent_start()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("500.123e", &ls, &lc, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "0 valid");
	assert_ptr(t, "0 ptr t");
	expect_int_equal(t->type, token_number, "0 number");
	expect_str(&t->value, "500.123e", "0 value");

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "1 valid");
	assert_ptr(t, "1 ptr t");
	expect_int_equal(t->type, token_eof, "1 eof");
	expect_str(&t->value, "", "1 value");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("500.123e2", &ls, &lc, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123e2", "500.123e2");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent_sign_start_negative()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("500.123e-", &ls, &lc, &el);

	valid = lex(&ls, &t);
	expect_has_errors(ls.el);
	expect_false(valid, "0 valid");
	expect_source_error(ls.el, "expected number after exponent sign");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent_sign_start_positive()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("500.123e+", &ls, &lc, &el);

	valid = lex(&ls, &t);
	expect_has_errors(ls.el);
	expect_false(valid, "0 valid");
	expect_source_error(ls.el, "expected number after exponent sign");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_fraction_exponent_negative()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("500.123e-2", &ls, &lc, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123e-2", "500.123e-2");
	expect_int_equal(t->loc.line, 1, "line 10");
	expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_exponent_positive()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct lex_state ls;
	struct error_list el;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("500.123e+2", &ls, &lc, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "500.123e+2", "500.123e+2");
	expect_int_equal(t->loc.line, 1, "line");
	expect_int_equal(t->loc.col, 1, "col");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_number_exponent_add()
{
	test_name(__func__);

	struct lookahead_char lc;
	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("500.123e + 1", &ls, &lc, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "0 valid");
	assert_ptr(t, "0 ptr t");
	expect_int_equal(t->type, token_number, "0 number");
	expect_str(&t->value, "500.123e", "0 value");
	expect_int_equal(t->loc.line, 1, "0 line 10");
	expect_int_equal(t->loc.col, 1, "0 col 10");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "1 valid");
	assert_ptr(t, "1 ptr t");
	expect_int_equal(t->type, token_plus, "1 plus");
	expect_size_t_equal(t->loc.line, 1, "1 line");
	expect_size_t_equal(t->loc.col, 10, "1 col");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "2 valid");
	assert_ptr(t, "2 ptr t");
	expect_int_equal(t->type, token_number, "2 number");
	expect_str(&t->value, "1", "2 value");
	expect_size_t_equal(t->loc.line, 1, "2 line");
	expect_size_t_equal(t->loc.col, 12, "2 col");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number()
{
	test_lex_number_negative_start();
	test_lex_number_whole();
	test_lex_number_fraction_start();
	test_lex_number_fraction();
	test_lex_number_exponent_start();
	test_lex_number_fraction_exponent_start();
	test_lex_number_fraction_exponent();
	test_lex_number_fraction_exponent_sign_start_negative();
	test_lex_number_fraction_exponent_sign_start_positive();
	test_lex_number_fraction_exponent_negative();
	test_lex_number_exponent_positive();
	test_lex_number_exponent_add();
}
