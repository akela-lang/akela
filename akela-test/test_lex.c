#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/memory.h"
#include "akela/token.h"
#include "akela/lex.h"
#include "test_lex_setup.h"
#include "zinc/error_unit_test.h"

void test_lex_blank()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	struct token* t;
	bool valid;

	lex_setup("", &ls, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex 0");
	expect_int_equal(t->type, token_eof, "none");
	expect_str(&t->value, "", "(blank)");
    expect_size_t_equal(t->loc.line, 1, "line 1");
    expect_size_t_equal(t->loc.col, 1, "col 1");
    expect_size_t_equal(t->loc.size, 3, "size 0");
    expect_size_t_equal(t->loc.byte_pos, 0, "byte pos 0");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

void test_lex_assign()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("a = 1", &ls, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "a", "a");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_equal, "equal");

	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");
    expect_size_t_equal(t->loc.line, 1, "line 1");
    expect_size_t_equal(t->loc.col, 6, "col 6");
    expect_size_t_equal(t->loc.size, 3, "size 3");
    expect_size_t_equal(t->loc.byte_pos, 5, "byte pos 5");

    /* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_addition()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("speed + 1", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "speed", "speed");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
    assert_int_equal(t->type, token_eof, "eof");
	assert_true(valid, "lex valid");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_subtraction()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("100 - delta", &ls, &el);

	/* allocate ls t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_minus, "minus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "delta", "delta");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_multiplication()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("100 * 20", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_mult, "mult");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "20", "20");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_divide()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("45 / 11", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "45", "45");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_divide, "divide");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "is number");
	expect_str(&t->value, "11", "11");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_stmts_expr()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("i + 1\nx * 4", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_mult, "mult");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_stmts_expr2()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("i + 1\nx * 4\n", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_mult, "mult");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_newline, "newline2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_stmts_assign()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("i + 1\nx = 4", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_equal, "equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_function()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("fn foo () \n end", &ls, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_fn, "fn");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "foo", "foo");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_left_paren, "left paren");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_right_paren, "right paren");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_newline, "newline");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_end, "end");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_comma()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup(",", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_comma, "comma");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_semicolon()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup(";", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_semicolon, "semicolon");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_if() {
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("if elseif else", &ls, &el);
	
	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_if, "if");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_elseif, "elseif");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_else, "else");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{}*/
	lex_teardown(&ls);
}

void test_lex_compound_operators() {
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("== != <= >= && || ->", &ls, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_double_equal, "double equal");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_not_equal, "not equal");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_less_than_or_equal, "less than or equal");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_greater_than_or_equal, "greater than or equal");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_and, "and");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_or, "or");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_arrow, "arrow");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_compound_operators2() {
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("= ! < > & | :", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_equal, "equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_not, "not");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_less_than, "less than");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_greater_than, "greater_than");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_ampersand, "ampersand");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_vertical_bar, "vertical_bar");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_colon, "colon");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_for_range()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("for i = 0:10 1 end", &ls, &el);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_for, "for");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id 1");
	expect_str(&t->value, "i", "i");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_equal, "equal 2");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number 3");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_colon, "colon 4");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number 5");
	expect_str(&t->value, "10", "10 5");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number 6");
	expect_str(&t->value, "1", "1 6");

	token_destroy(t);
	free(t);

	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_end, "end 7");

	token_destroy(t);
	free(t);

	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_for_iteration()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("for x in list 1 end", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_for, "for");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id 1");
	expect_str(&t->value, "x", "x 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_in, "in 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id 3");
	expect_str(&t->value, "list", "list 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_number, "number 4");
	expect_str(&t->value, "1", "1 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_end, "end 5");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_error_unrecognized_character()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("$", &ls, &el);

	valid = lex(&ls, &t);
	assert_false(valid, "lex");
	assert_has_errors(ls.el);
	assert_null(t, "t");
	expect_source_error(&el, "Unrecognized character: $");

	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_square_brackets()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("[]", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_left_square_bracket, "left-square-bracket 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_right_square_bracket, "right-square-bracket 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_string()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("\"hello\"", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_string, "string 0");
	expect_str(&t->value, "hello", "hello 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_string2()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("x = \"\\\\hello\n\r\"", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_id, "id 0");
	expect_str(&t->value, "x", "x 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_equal, "equal 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	expect_int_equal(t->type, token_string, "string 2");
	expect_str(&t->value, "\\hello\n\r", "hello 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_string_escape_error()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("\"\\x\"", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_false(valid, "lex");
	assert_has_errors(ls.el);
	expect_source_error(&el, "Unrecognized escape sequence: x");

	assert_null(t, "t");

	/* destroy ls{} */
	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex_line_col()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("10 + 20\n30 + 40", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t 10");
	expect_int_equal(t->type, token_number, "number 10");
	expect_str(&t->value, "10", "10");
	expect_size_t_equal(t->loc.line, 1, "line 10");
	expect_size_t_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t +");
	expect_size_t_equal(t->type, token_plus, "plus +");
	expect_size_t_equal(t->loc.line, 1, "line +");
	expect_size_t_equal(t->loc.col, 4, "col +");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t 20");
	expect_size_t_equal(t->type, token_number, "number 20");
	expect_size_t_equal(t->loc.line, 1, "line 20");
	expect_size_t_equal(t->loc.col, 6, "col 20");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t newline");
	expect_size_t_equal(t->type, token_newline, "newline newline");
	expect_size_t_equal(t->loc.line, 1, "line newline");
	expect_size_t_equal(t->loc.col, 8, "col newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t 30");
	expect_size_t_equal(t->type, token_number, "newline 30");
	expect_size_t_equal(t->loc.line, 2, "line 30");
	expect_size_t_equal(t->loc.col, 1, "col 30");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t +");
	expect_size_t_equal(t->type, token_plus, "plus +");
	expect_size_t_equal(t->loc.line, 2, "line +");
	expect_size_t_equal(t->loc.col, 4, "col +");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_no_errors(ls.el);
	assert_true(valid, "lex valid");
	assert_ptr(t, "ptr t 40");
	expect_int_equal(t->type, token_number, "number 40");
	expect_size_t_equal(t->loc.line, 2, "line 40");
	expect_size_t_equal(t->loc.col, 6, "col 40");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_error_underscore_letter()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("_1", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_has_errors(ls.el);
	expect_false(valid, "lex valid");
	expect_null(t, "null t");
	expect_source_error(&el, "Must have a letter following underscore at start of id");

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_error_underscore_letter2()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	/* allocate ls{} */
	lex_setup("__", &ls, &el);

	/* allocate ls{} t t{} */
	valid = lex(&ls, &t);
	assert_has_errors(ls.el);
	expect_false(valid, "lex valid");
	expect_null(t, "null t");
	expect_source_error(&el, "Must have a letter following underscore at start of id");

	/* destroy ls{} */
	lex_teardown(&ls);
}

void test_lex_error_exponent_sign()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;
	struct token* t;

	lex_setup("100e-a", &ls, &el);

	valid = lex(&ls, &t);
	assert_has_errors(ls.el);
	expect_false(valid, "lex valid");
	expect_null(t, "null t");
	expect_source_error(&el, "expected number after exponent sign");

	lex_teardown(&ls);
}

void test_lex_module()
{
	test_name(__func__);

	struct error_list el;
	struct lex_state ls;
	bool valid;

	lex_setup("module 1 end", &ls, &el);

	struct token* module;
	valid = lex(&ls, &module);
	assert_no_errors(ls.el);
	assert_true(valid, "valid module");
	assert_ptr(module, "ptr module");
	expect_int_equal(module->type, token_module, "module module");

	token_destroy(module);
	free(module);

	struct token* number;
	valid = lex(&ls, &number);
	assert_no_errors(ls.el);
	assert_true(valid, "valid number");
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, token_number, "number number");
	expect_str(&number->value, "1", "1 number");

	token_destroy(number);
	free(number);

	struct token* end;
	valid = lex(&ls, &end);
	assert_no_errors(ls.el);
	assert_true(valid, "valid end");
	assert_ptr(end, "ptr end");
	expect_int_equal(end->type, token_end, "end end");

	token_destroy(end);
	free(end);

	lex_teardown(&ls);
}

/* dynamic-output-none */
void test_lex()
{
	test_lex_blank();
	test_lex_assign();
	test_lex_addition();
	test_lex_subtraction();
	test_lex_multiplication();
	test_lex_divide();
	test_lex_stmts_expr();
	test_lex_stmts_expr2();
	test_lex_stmts_assign();
	test_lex_function();
	test_lex_comma();
	test_lex_semicolon();
	test_lex_if();
	test_lex_compound_operators();
	test_lex_compound_operators2();
	test_lex_for_range();
	test_lex_for_iteration();
	test_lex_error_unrecognized_character();
	test_lex_square_brackets();
	test_lex_string();
	test_lex_string2();
	test_lex_string_escape_error();
	test_lex_line_col();
	test_lex_error_underscore_letter();
	test_lex_error_underscore_letter2();
	test_lex_error_exponent_sign();
	test_lex_module();
}
