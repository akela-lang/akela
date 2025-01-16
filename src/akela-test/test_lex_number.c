#include "zinc/unit_test.h"
#include "akela/lex.h"
#include "test_lex_setup.h"
#include "zinc/error_unit_test.h"

void test_lex_number_negative_start()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("-", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_expect_true(valid, "valid 0");
	Zinc_expect_int_equal(t->type, Ake_token_minus, "minus 0");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_whole()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "lex valid");
	Zinc_assert_ptr(t, "ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "number");
	Zinc_expect_str(&t->value, "500", "500");
	Zinc_expect_size_t_equal(t->loc.line, 1, "line 10");
	Zinc_expect_size_t_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_fraction_start()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "lex valid");
	Zinc_assert_ptr(t, "ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "number");
	Zinc_expect_str(&t->value, "500.", "500.");
	Zinc_expect_int_equal(t->loc.line, 1, "line 10");
	Zinc_expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_fraction()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.123", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "lex valid");
	Zinc_assert_ptr(t, "ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "number");
	Zinc_expect_str(&t->value, "500.123", "500.123");
	Zinc_expect_int_equal(t->loc.line, 1, "line 10");
	Zinc_expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_number_exponent_start()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500e", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "0 valid");
	Zinc_assert_ptr(t, "0 ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "0 number");
	Zinc_expect_str(&t->value, "500e", "0 value");

    Ake_token_destroy(t);
    free(t);

    valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "1 valid");
	Zinc_assert_ptr(t, "1 ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_eof, "1 eof");
	Zinc_expect_str(&t->value, "", "1 value");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent_start()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "0 valid");
	Zinc_assert_ptr(t, "0 ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "0 number");
	Zinc_expect_str(&t->value, "500.123e", "0 value");

    Ake_token_destroy(t);
    free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "1 valid");
	Zinc_assert_ptr(t, "1 ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_eof, "1 eof");
	Zinc_expect_str(&t->value, "", "1 value");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e2", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "lex valid");
	Zinc_assert_ptr(t, "ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "number");
	Zinc_expect_str(&t->value, "500.123e2", "500.123e2");
	Zinc_expect_int_equal(t->loc.line, 1, "line 10");
	Zinc_expect_int_equal(t->loc.col, 1, "col 10");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent_sign_start_negative()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e-", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_expect_has_errors(ls.el);
	Zinc_expect_false(valid, "0 valid");
	Zinc_expect_source_error(ls.el, "expected number after exponent sign");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_number_fraction_exponent_sign_start_positive()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e+", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_expect_has_errors(ls.el);
	Zinc_expect_false(valid, "0 valid");
	Zinc_expect_source_error(ls.el, "expected number after exponent sign");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_fraction_exponent_negative()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.123e-2", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "lex valid");
	Zinc_assert_ptr(t, "ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "number");
	Zinc_expect_str(&t->value, "500.123e-2", "500.123e-2");
	Zinc_expect_int_equal(t->loc.line, 1, "line 10");
	Zinc_expect_int_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_number_exponent_positive()
{
	Zinc_test_name(__func__);

	struct Ake_lex_state ls;
	struct Zinc_error_list el;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.123e+2", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "lex valid");
	Zinc_assert_ptr(t, "ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "number");
	Zinc_expect_str(&t->value, "500.123e+2", "500.123e+2");
	Zinc_expect_int_equal(t->loc.line, 1, "line");
	Zinc_expect_int_equal(t->loc.col, 1, "col");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_number_exponent_add()
{
	Zinc_test_name(__func__);

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e + 1", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "0 valid");
	Zinc_assert_ptr(t, "0 ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "0 number");
	Zinc_expect_str(&t->value, "500.123e", "0 value");
	Zinc_expect_int_equal(t->loc.line, 1, "0 line 10");
	Zinc_expect_int_equal(t->loc.col, 1, "0 col 10");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "1 valid");
	Zinc_assert_ptr(t, "1 ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_plus, "1 plus");
	Zinc_expect_size_t_equal(t->loc.line, 1, "1 line");
	Zinc_expect_size_t_equal(t->loc.col, 10, "1 col");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_assert_no_errors(ls.el);
	Zinc_assert_true(valid, "2 valid");
	Zinc_assert_ptr(t, "2 ptr t");
	Zinc_expect_int_equal(t->type, Ake_token_number, "2 number");
	Zinc_expect_str(&t->value, "1", "2 value");
	Zinc_expect_size_t_equal(t->loc.line, 1, "2 line");
	Zinc_expect_size_t_equal(t->loc.col, 12, "2 col");

	Ake_token_destroy(t);
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
