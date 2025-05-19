#include <stdbool.h>
#include "zinc/memory.h"
#include "akela/token.h"
#include "akela/lex.h"
#include "unit_lex_setup.h"
#include "zinc/test.h"
#include "zinc/expect.h"

void AkeUnit_lex_blank(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;
		return;
	}

	struct Zinc_error_list el;
	struct Ake_lex_state ls;
	struct Ake_token* t;
	bool valid;

	AkeUnit_lex_setup("", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex 0");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "none");
	Zinc_test_expect_string(test, &t->value, "", "(blank)");
    Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "line 1");
    Zinc_test_expect_size_t_equal(test, t->loc.col, 1, "col 1");
    Zinc_test_expect_size_t_equal(test, t->loc.start_pos, 0, "start_pos 0");
    Zinc_test_expect_size_t_equal(test, t->loc.end_pos, 1, "end_pos 0");

	Ake_token_destroy(t);
	free(t);

	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_assign(Zinc_test* test)
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

	AkeUnit_lex_setup("a = 1", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id");
	Zinc_test_expect_string(test, &t->value, "a", "a");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_equal, "equal");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "1", "1");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");
    Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "line 1");
    Zinc_test_expect_size_t_equal(test, t->loc.col, 6, "col 6");
    Zinc_test_expect_size_t_equal(test, t->loc.start_pos, 5, "byte pos 5");
    Zinc_test_expect_size_t_equal(test, t->loc.end_pos, 6, "end_pos 3");

	Ake_token_destroy(t);
	free(t);

	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_addition(Zinc_test* test)
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

	AkeUnit_lex_setup("speed + 1", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id");
	Zinc_test_expect_string(test, &t->value, "speed", "speed");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_plus, "plus");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "1", "1");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
    Zinc_test_assert_int_equal(test, t->type, Ake_token_eof, "eof");
	Zinc_test_assert_true(test, valid, "lex valid");

	Ake_token_destroy(t);
	free(t);

	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_subtraction(Zinc_test* test)
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
	AkeUnit_lex_setup("100 - delta", &ls, &el);

	/* allocate ls t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "100", "100");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_minus, "minus");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id");
	Zinc_test_expect_string(test, &t->value, "delta", "delta");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_multiplication(Zinc_test* test)
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
	AkeUnit_lex_setup("100 * 20", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "100", "100");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_mult, "mult");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "20", "20");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_divide(Zinc_test* test)
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
	AkeUnit_lex_setup("45 / 11", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "45", "45");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_divide, "divide");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "is number");
	Zinc_test_expect_string(test, &t->value, "11", "11");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_stmts_expr(Zinc_test* test)
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
	AkeUnit_lex_setup("i + 1\nx * 4", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id");
	Zinc_test_expect_string(test, &t->value, "i", "i");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_plus, "plus");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "1", "1");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_newline, "newline");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id2");
	Zinc_test_expect_string(test, &t->value, "x", "x");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_mult, "mult");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number2");
	Zinc_test_expect_string(test, &t->value, "4", "4");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_stmts_expr2(Zinc_test* test)
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
	AkeUnit_lex_setup("i + 1\nx * 4\n", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id");
	Zinc_test_expect_string(test, &t->value, "i", "i");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_plus, "plus");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "1", "1");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_newline, "newline");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id2");
	Zinc_test_expect_string(test, &t->value, "x", "x");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_mult, "mult");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number2");
	Zinc_test_expect_string(test, &t->value, "4", "4");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_newline, "newline2");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_stmts_assign(Zinc_test* test)
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
	AkeUnit_lex_setup("i + 1\nx = 4", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id");
	Zinc_test_expect_string(test, &t->value, "i", "i");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_plus, "plus");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number");
	Zinc_test_expect_string(test, &t->value, "1", "1");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_newline, "newline");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id2");
	Zinc_test_expect_string(test, &t->value, "x", "x");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_equal, "equal");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number2");
	Zinc_test_expect_string(test, &t->value, "4", "4");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_function(Zinc_test* test)
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

	AkeUnit_lex_setup("fn foo () \n end", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_fn, "fn");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id");
	Zinc_test_expect_string(test, &t->value, "foo", "foo");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_left_paren, "left paren");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_right_paren, "right paren");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_newline, "newline");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_end, "end");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	Ake_token_destroy(t);
	free(t);

	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_comma(Zinc_test* test)
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
	AkeUnit_lex_setup(",", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_comma, "comma");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_semicolon(Zinc_test* test)
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
	AkeUnit_lex_setup(";", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_semicolon, "semicolon");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_if(Zinc_test* test)
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
	AkeUnit_lex_setup("if elseif else", &ls, &el);
	
	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_if, "if");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_elseif, "elseif");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_else, "else");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
    Zinc_test_assert_ptr(test, t, "ptr t");
    Zinc_test_expect_int_equal(test, t->type, Ake_token_eof, "eof");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{}*/
	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_compound_operators(Zinc_test* test)
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

	AkeUnit_lex_setup("== != <= >= && || ->", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_double_equal, "double equal");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_not_equal, "not equal");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_less_than_or_equal, "less than or equal");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_greater_than_or_equal, "greater than or equal");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_and, "and");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_or, "or");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_arrow, "arrow");

	Ake_token_destroy(t);
	free(t);

	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_compound_operators2(Zinc_test* test)
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
	AkeUnit_lex_setup("= ! < > & | :", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_equal, "equal");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_not, "not");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_less_than, "less than");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_greater_than, "greater_than");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_ampersand, "ampersand");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_vertical_bar, "vertical_bar");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_colon, "colon");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_for_range(Zinc_test* test)
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

	AkeUnit_lex_setup("for i = 0:10 1 end", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_for, "for");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id 1");
	Zinc_test_expect_string(test, &t->value, "i", "i");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_equal, "equal 2");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number 3");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_colon, "colon 4");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number 5");
	Zinc_test_expect_string(test, &t->value, "10", "10 5");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number 6");
	Zinc_test_expect_string(test, &t->value, "1", "1 6");

	Ake_token_destroy(t);
	free(t);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_end, "end 7");

	Ake_token_destroy(t);
	free(t);

	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_for_iteration(Zinc_test* test)
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
	AkeUnit_lex_setup("for x in list 1 end", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_for, "for");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id 1");
	Zinc_test_expect_string(test, &t->value, "x", "x 1");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_in, "in 2");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id 3");
	Zinc_test_expect_string(test, &t->value, "list", "list 3");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number 4");
	Zinc_test_expect_string(test, &t->value, "1", "1 4");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_end, "end 5");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_error_unrecognized_character(Zinc_test* test)
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

	AkeUnit_lex_setup("$", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_false(test, valid, "lex");
	Zinc_test_assert_has_errors(test, ls.el);
	Zinc_test_assert_null(test, t, "t");
	Zinc_test_expect_source_error(test, &el, "Unrecognized character: $");

	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_square_brackets(Zinc_test* test)
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
	AkeUnit_lex_setup("[]", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_left_square_bracket, "left-square-bracket 0");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_right_square_bracket, "right-square-bracket 1");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_string(Zinc_test* test)
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
	AkeUnit_lex_setup("\"hello\"", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_string, "string 0");
	Zinc_test_expect_string(test, &t->value, "hello", "hello 0");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_string2(Zinc_test* test)
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
	AkeUnit_lex_setup("x = \"\\\\hello\n\r\"", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_id, "id 0");
	Zinc_test_expect_string(test, &t->value, "x", "x 0");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_equal, "equal 1");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_string, "string 2");
	Zinc_test_expect_string(test, &t->value, "\\hello\n\r", "hello 2");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_string_escape_error(Zinc_test* test)
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
	AkeUnit_lex_setup("\"\\x\"", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_false(test, valid, "lex");
	Zinc_test_assert_has_errors(test, ls.el);
	Zinc_test_expect_source_error(test, &el, "Unrecognized escape sequence: x");

	Zinc_test_assert_null(test, t, "t");

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

/* dynamic-output-none */
void AkeUnit_lex_line_col(Zinc_test* test)
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
	AkeUnit_lex_setup("10 + 20\n30 + 40", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t 10");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number 10");
	Zinc_test_expect_string(test, &t->value, "10", "10");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "line 10");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 1, "col 10");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t +");
	Zinc_test_expect_size_t_equal(test, t->type, Ake_token_plus, "plus +");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "line +");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 4, "col +");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t 20");
	Zinc_test_expect_size_t_equal(test, t->type, Ake_token_number, "number 20");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "line 20");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 6, "col 20");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t newline");
	Zinc_test_expect_size_t_equal(test, t->type, Ake_token_newline, "newline newline");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 1, "line newline");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 8, "col newline");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t 30");
	Zinc_test_expect_size_t_equal(test, t->type, Ake_token_number, "newline 30");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 2, "line 30");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 1, "col 30");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t +");
	Zinc_test_expect_size_t_equal(test, t->type, Ake_token_plus, "plus +");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 2, "line +");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 4, "col +");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "lex valid");
	Zinc_test_assert_ptr(test, t, "ptr t 40");
	Zinc_test_expect_int_equal(test, t->type, Ake_token_number, "number 40");
	Zinc_test_expect_size_t_equal(test, t->loc.line, 2, "line 40");
	Zinc_test_expect_size_t_equal(test, t->loc.col, 6, "col 40");

	/* destroy t t{} */
	Ake_token_destroy(t);
	free(t);

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_error_underscore_letter(Zinc_test* test)
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
	AkeUnit_lex_setup("_1", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_has_errors(test, ls.el);
	Zinc_test_expect_false(test, valid, "lex valid");
	Zinc_test_expect_null(test, t, "null t");
	Zinc_test_expect_source_error(test, &el, "Must have a letter following underscore at start of id");

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_error_underscore_letter2(Zinc_test* test)
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
	AkeUnit_lex_setup("__", &ls, &el);

	/* allocate ls{} t t{} */
	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_has_errors(test, ls.el);
	Zinc_test_expect_false(test, valid, "lex valid");
	Zinc_test_expect_null(test, t, "null t");
	Zinc_test_expect_source_error(test, &el, "Must have a letter following underscore at start of id");

	/* destroy ls{} */
	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_error_exponent_sign(Zinc_test* test)
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

	AkeUnit_lex_setup("100e-a", &ls, &el);

	valid = Ake_lex(&ls, &t);
	Zinc_test_assert_has_errors(test, ls.el);
	Zinc_test_expect_false(test, valid, "lex valid");
	Zinc_test_expect_null(test, t, "null t");
	Zinc_test_expect_source_error(test, &el, "expected number after exponent sign");

	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_module(Zinc_test* test)
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

	AkeUnit_lex_setup("module 1 end", &ls, &el);

	struct Ake_token* module;
	valid = Ake_lex(&ls, &module);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "valid module");
	Zinc_test_assert_ptr(test, module, "ptr module");
	Zinc_test_expect_int_equal(test, module->type, Ake_token_module, "module module");

	Ake_token_destroy(module);
	free(module);

	struct Ake_token* number;
	valid = Ake_lex(&ls, &number);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "valid number");
	Zinc_test_assert_ptr(test, number, "ptr number");
	Zinc_test_expect_int_equal(test, number->type, Ake_token_number, "number number");
	Zinc_test_expect_string(test, &number->value, "1", "1 number");

	Ake_token_destroy(number);
	free(number);

	struct Ake_token* end;
	valid = Ake_lex(&ls, &end);
	Zinc_test_assert_no_errors(test, ls.el);
	Zinc_test_assert_true(test, valid, "valid end");
	Zinc_test_assert_ptr(test, end, "ptr end");
	Zinc_test_expect_int_equal(test, end->type, Ake_token_end, "end end");

	Ake_token_destroy(end);
	free(end);

	AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex_comment(Zinc_test* test)
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

    AkeUnit_lex_setup("150 # this is 150\n"
              "275 # this is 275\n",
              &ls, &el);

    struct Ake_token* number0;
    valid = Ake_lex(&ls, &number0);
    Zinc_test_assert_no_errors(test, ls.el);
    Zinc_test_assert_true(test, valid, "valid number0");
    Zinc_test_assert_ptr(test, number0, "ptr number0");
    Zinc_test_expect_int_equal(test, number0->type, Ake_token_number, "type number0");
    Zinc_test_expect_string(test, &number0->value, "150", "value number0");

    Ake_token_destroy(number0);
    free(number0);

    struct Ake_token* newline0;
    valid = Ake_lex(&ls, &newline0);
    Zinc_test_assert_no_errors(test, ls.el);
    Zinc_test_assert_true(test, valid, "valid newline0");
    Zinc_test_assert_ptr(test, newline0, "ptr newline0");
    Zinc_test_expect_int_equal(test, newline0->type, Ake_token_newline, "type newline0");

    Ake_token_destroy(newline0);
    free(newline0);

    struct Ake_token* number1;
    valid = Ake_lex(&ls, &number1);
    Zinc_test_assert_no_errors(test, ls.el);
    Zinc_test_assert_true(test, valid, "valid number1");
    Zinc_test_assert_ptr(test, number1, "ptr number1");
    Zinc_test_expect_int_equal(test, number1->type, Ake_token_number, "type number1");
    Zinc_test_expect_string(test, &number1->value, "275", "value number1");

    Ake_token_destroy(number1);
    free(number1);

    struct Ake_token* newline1;
    valid = Ake_lex(&ls, &newline1);
    Zinc_test_assert_no_errors(test, ls.el);
    Zinc_test_assert_true(test, valid, "valid newline1");
    Zinc_test_assert_ptr(test, newline1, "ptr newline1");
    Zinc_test_expect_int_equal(test, newline1->type, Ake_token_newline, "type newline1");

    Ake_token_destroy(newline1);
    free(newline1);

    struct Ake_token* eof;
    valid = Ake_lex(&ls, &eof);
    Zinc_test_assert_no_errors(test, ls.el);
    Zinc_test_assert_true(test, valid, "valid eof");
    Zinc_test_assert_ptr(test, eof, "ptr eof");
    Zinc_test_expect_int_equal(test, eof->type, Ake_token_eof, "type eof");

    Ake_token_destroy(eof);
    free(eof);

    AkeUnit_lex_teardown(&ls);
}

void AkeUnit_lex(Zinc_test* test)
{
	if (test->dry_run) {
		Zinc_string_add_str(&test->name, __func__);
		test->mute = false;
		test->solo = false;

		Zinc_test_register(test, AkeUnit_lex_blank);
		Zinc_test_register(test, AkeUnit_lex_assign);
		Zinc_test_register(test, AkeUnit_lex_addition);
		Zinc_test_register(test, AkeUnit_lex_subtraction);
		Zinc_test_register(test, AkeUnit_lex_multiplication);
		Zinc_test_register(test, AkeUnit_lex_divide);
		Zinc_test_register(test, AkeUnit_lex_stmts_expr);
		Zinc_test_register(test, AkeUnit_lex_stmts_expr2);
		Zinc_test_register(test, AkeUnit_lex_stmts_assign);
		Zinc_test_register(test, AkeUnit_lex_function);
		Zinc_test_register(test, AkeUnit_lex_comma);
		Zinc_test_register(test, AkeUnit_lex_semicolon);
		Zinc_test_register(test, AkeUnit_lex_if);
		Zinc_test_register(test, AkeUnit_lex_compound_operators);
		Zinc_test_register(test, AkeUnit_lex_compound_operators2);
		Zinc_test_register(test, AkeUnit_lex_for_range);
		Zinc_test_register(test, AkeUnit_lex_for_iteration);
		Zinc_test_register(test, AkeUnit_lex_error_unrecognized_character);
		Zinc_test_register(test, AkeUnit_lex_square_brackets);
		Zinc_test_register(test, AkeUnit_lex_string);
		Zinc_test_register(test, AkeUnit_lex_string2);
		Zinc_test_register(test, AkeUnit_lex_string_escape_error);
		Zinc_test_register(test, AkeUnit_lex_line_col);
		Zinc_test_register(test, AkeUnit_lex_error_underscore_letter);
		Zinc_test_register(test, AkeUnit_lex_error_underscore_letter2);
		Zinc_test_register(test, AkeUnit_lex_error_exponent_sign);
		Zinc_test_register(test, AkeUnit_lex_module);
		Zinc_test_register(test, AkeUnit_lex_comment);

		return;
	}

	Zinc_test_perform(test);
}
