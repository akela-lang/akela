#include "akela/lex.h"
#include "test_lex_setup.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_lex_number_negative_start(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("-", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_expect_true(test, valid, "valid 0");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_minus, "minus 0");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_number_whole(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "500", "500");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "line 10");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_number_fraction_start(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "500.", "500.");
	Zinc_test_expect_int_equal(test, t->loc.line, 1, "line 10");
	Zinc_test_expect_int_equal(test, t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_number_fraction(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.123", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "500.123", "500.123");
	Zinc_test_expect_int_equal(test, t->loc.line, 1, "line 10");
	Zinc_test_expect_int_equal(test, t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void AkeUnit_lex_number_exponent_start(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500e", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "0 valid");
	Zinc_test_assert_ptr(test, t, "0 ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "0 number");
	Zinc_test_expect_string(test, &t->value, "500e", "0 value");

    Ake_token_destroy(t);
    free(t);

    valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "1 valid");
	Zinc_test_assert_ptr(test, t, "1 ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "1 eof");
	Zinc_test_expect_string(test, &t->value, "", "1 value");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void AkeUnit_lex_number_fraction_exponent_start(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "0 valid");
	Zinc_test_assert_ptr(test, t, "0 ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "0 number");
	Zinc_test_expect_string(test, &t->value, "500.123e", "0 value");

    Ake_token_destroy(t);
    free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "1 valid");
	Zinc_test_assert_ptr(test, t, "1 ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "1 eof");
	Zinc_test_expect_string(test, &t->value, "", "1 value");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void AkeUnit_lex_number_fraction_exponent(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e2", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "500.123e2", "500.123e2");
	Zinc_test_expect_int_equal(test, t->loc.line, 1, "line 10");
	Zinc_test_expect_int_equal(test, t->loc.col, 1, "col 10");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void AkeUnit_lex_number_fraction_exponent_sign_start_negative(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e-", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_expect_has_errors(test, ls.el);
	Zinc_test_expect_false(test, valid, "0 valid");
	Zinc_test_expect_source_error(test, ls.el, "expected number after exponent sign");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void AkeUnit_lex_number_fraction_exponent_sign_start_positive(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e+", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_expect_has_errors(test, ls.el);
	Zinc_test_expect_false(test, valid, "0 valid");
	Zinc_test_expect_source_error(test, ls.el, "expected number after exponent sign");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_number_fraction_exponent_negative(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.123e-2", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "500.123e-2", "500.123e-2");
	Zinc_test_expect_int_equal(test, t->loc.line, 1, "line 10");
	Zinc_test_expect_int_equal(test, t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_number_exponent_positive(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Ake_lex_state ls;
	struct Zinc_error_list el;
	bool valid;
	struct Ake_token* t;

	/* allocate ls{} */
	lex_setup("500.123e+2", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "500.123e+2", "500.123e+2");
	Zinc_test_expect_int_equal(test, t->loc.line, 1, "line");
	Zinc_test_expect_int_equal(test, t->loc.col, 1, "col");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void AkeUnit_lex_number_exponent_add(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	bool valid;
	struct Ake_token* t;

	lex_setup("500.123e + 1", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "0 valid");
	Zinc_test_assert_ptr(test, t, "0 ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "0 number");
	Zinc_test_expect_string(test, &t->value, "500.123e", "0 value");
	Zinc_test_expect_int_equal(test, t->loc.line, 1, "0 line 10");
	Zinc_test_expect_int_equal(test, t->loc.col, 1, "0 col 10");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "1 valid");
	Zinc_test_assert_ptr(test, t, "1 ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_plus, "1 plus");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "1 line");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 10, "1 col");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "2 valid");
	Zinc_test_assert_ptr(test, t, "2 ptr t");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "2 number");
	Zinc_test_expect_string(test, &t->value, "1", "2 value");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "2 line");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 12, "2 col");

	Ake_token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void AkeUnit_lex_number(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_lex_number_negative_start);
		Zinc_test_register(test, AkeUnit_lex_number_whole);
		Zinc_test_register(test, AkeUnit_lex_number_fraction_start);
		Zinc_test_register(test, AkeUnit_lex_number_fraction);
		Zinc_test_register(test, AkeUnit_lex_number_exponent_start);
		Zinc_test_register(test, AkeUnit_lex_number_fraction_exponent_start);
		Zinc_test_register(test, AkeUnit_lex_number_fraction_exponent);
		Zinc_test_register(test, AkeUnit_lex_number_fraction_exponent_sign_start_negative);
		Zinc_test_register(test, AkeUnit_lex_number_fraction_exponent_sign_start_positive);
		Zinc_test_register(test, AkeUnit_lex_number_fraction_exponent_negative);
		Zinc_test_register(test, AkeUnit_lex_number_exponent_positive);
		Zinc_test_register(test, AkeUnit_lex_number_exponent_add);

		return;
	}

	Zinc_test_perform(test);
}
