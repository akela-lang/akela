#include <stdbool.h>
#include "zinc/unit_test.h"
#include "zinc/memory.h"
#include "akela/unit_test_compiler.h"
#include "akela/token.h"
#include "akela/scan.h"
#include "akela/uconv.h"
#include "akela/input.h"
#include "akela/lookahead_char.h"
#include "akela/source.h"
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
	bool valid;

	/* allocate sns{} */
	scan_setup("", &sns, &lc, &el);

	/* allocatge sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token 0");
	expect_int_equal(t->type, token_eof, "none");
	expect_str(&t->value, "", "(blank)");
    expect_size_t_equal(t->loc.line, 1, "line 1");
    expect_size_t_equal(t->loc.col, 1, "col 1");
    expect_size_t_equal(t->loc.size, 0, "size 0");
    expect_size_t_equal(t->loc.byte_pos, 0, "byte pos 0");

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

	/* allocate sns{} */
	scan_setup("a = 1", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "a", "a");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_equal, "equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");
    expect_size_t_equal(t->loc.line, 1, "line 1");
    expect_size_t_equal(t->loc.col, 6, "col 6");
    expect_size_t_equal(t->loc.size, 0, "size 0");
    expect_size_t_equal(t->loc.byte_pos, 5, "byte pos 5");

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

	/* allocate sns{} */
	scan_setup("speed + 1", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "speed", "speed");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
    assert_int_equal(t->type, token_eof, "eof");
	assert_true(valid, "scan_get_token valid");

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

	/* allocate sns{} */
	scan_setup("100 - delta", &sns, &lc, &el);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_minus, "minus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "delta", "delta");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("100 * 20", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "100", "100");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_mult, "mult");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "20", "20");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("45 / 11", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "45", "45");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_divide, "divide");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "is number");
	expect_str(&t->value, "11", "11");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("i + 1\nx * 4", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_mult, "mult");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("i + 1\nx * 4\n", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_mult, "mult");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("i + 1\nx = 4", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_plus, "plus");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number");
	expect_str(&t->value, "1", "1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id2");
	expect_str(&t->value, "x", "x");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_equal, "equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number2");
	expect_str(&t->value, "4", "4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("function foo () \n end", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_function, "function");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id");
	expect_str(&t->value, "foo", "foo");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_left_paren, "left paren");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_right_paren, "right paren");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_newline, "newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_end, "end");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup(",", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_comma, "comma");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup(";", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_semicolon, "semicolon");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("if elseif else", &sns, &lc, &el);
	
	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_if, "if");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_elseif, "elseif");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_else, "else");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
    assert_ptr(t, "ptr t");
    expect_int_equal(t->type, token_eof, "eof");

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

	/* allocate sns{} */
	scan_setup("== != <= >= && || ::", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_double_equal, "double equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_not_equal, "not equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_less_than_or_equal, "less than or equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_greater_than_or_equal, "greater than or equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_and, "and");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_or, "or");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
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

	/* allocate sns{} */
	scan_setup("= ! < > & | :", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_equal, "equal");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_not, "not");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_less_than, "less than");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_greater_than, "greater_than");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_ampersand, "ampersand");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_vertical_bar, "vertical_bar");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
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

	/* allocate sns{} */
	scan_setup("for i = 0:10 1 end", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_for, "for");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id 1");
	expect_str(&t->value, "i", "i");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_equal, "equal 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_colon, "colon 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number 5");
	expect_str(&t->value, "10", "10 5");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number 6");
	expect_str(&t->value, "1", "1 6");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
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

	/* allocate sns{} */
	scan_setup("for x in list 1 end", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_for, "for");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id 1");
	expect_str(&t->value, "x", "x 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_in, "in 2");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id 3");
	expect_str(&t->value, "list", "list 3");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_number, "number 4");
	expect_str(&t->value, "1", "1 4");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
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

	/* allocate sns{} */
	scan_setup("$", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_false(valid, "scan_get_token");
	assert_has_errors(sns.el);
	assert_null(t, "t");
	expect_source_error(&el, "Unrecognized character: $");

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

	/* allocate sns{} */
	scan_setup("[]", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_left_square_bracket, "left-square-bracket 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
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

	/* allocate sns{} */
	scan_setup("\"hello\"", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
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

	/* allocate sns{} */
	scan_setup("x = \"\\\\hello\n\r\"", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_id, "id 0");
	expect_str(&t->value, "x", "x 0");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	expect_int_equal(t->type, token_equal, "equal 1");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
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

	/* allocate sns{} */
	scan_setup("\"\\x\"", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_false(valid, "scan_get_token");
	assert_has_errors(sns.el);
	expect_source_error(&el, "Unrecognized escape sequence: x");

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

	/* allocate sns{} */
	scan_setup("10 + 20\n30 + 40", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 10");
	expect_int_equal(t->type, token_number, "number 10");
	expect_str(&t->value, "10", "10");
	expect_size_t_equal(t->loc.line, 1, "line 10");
	expect_size_t_equal(t->loc.col, 1, "col 10");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t +");
	expect_size_t_equal(t->type, token_plus, "plus +");
	expect_size_t_equal(t->loc.line, 1, "line +");
	expect_size_t_equal(t->loc.col, 4, "col +");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 20");
	expect_size_t_equal(t->type, token_number, "number 20");
	expect_size_t_equal(t->loc.line, 1, "line 20");
	expect_size_t_equal(t->loc.col, 6, "col 20");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t newline");
	expect_size_t_equal(t->type, token_newline, "newline newline");
	expect_size_t_equal(t->loc.line, 1, "line newline");
	expect_size_t_equal(t->loc.col, 8, "col newline");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 30");
	expect_size_t_equal(t->type, token_number, "newline 30");
	expect_size_t_equal(t->loc.line, 2, "line 30");
	expect_size_t_equal(t->loc.col, 1, "col 30");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t +");
	expect_size_t_equal(t->type, token_plus, "plus +");
	expect_size_t_equal(t->loc.line, 2, "line +");
	expect_size_t_equal(t->loc.col, 4, "col +");

	/* destroy t t{} */
	token_destroy(t);
	free(t);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_no_errors(sns.el);
	assert_true(valid, "scan_get_token valid");
	assert_ptr(t, "ptr t 40");
	expect_int_equal(t->type, token_number, "number 40");
	expect_size_t_equal(t->loc.line, 2, "line 40");
	expect_size_t_equal(t->loc.col, 6, "col 40");

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

	/* allocate sns{} */
	scan_setup("_1", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_has_errors(sns.el);
	expect_false(valid, "scan_get_token valid");
	expect_null(t, "null t");
	expect_source_error(&el, "Must have a letter following underscore at start of id");

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

	/* allocate sns{} */
	scan_setup("__", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_has_errors(sns.el);
	expect_false(valid, "scan_get_token valid");
	expect_null(t, "null t");
	expect_source_error(&el, "Must have a letter following underscore at start of id");

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

	/* allocate sns{} */
	scan_setup("100e-a", &sns, &lc, &el);

	/* allocate sns{} t t{} */
	valid = scan_get_token(&sns, &t);
	assert_has_errors(sns.el);
	expect_false(valid, "scan_get_token valid");
	expect_null(t, "null t");
	expect_source_error(&el, "invalid number");

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

	scan_setup("module 1 end", &sns, &lc, &el);

	struct token* module;
	valid = scan_get_token(&sns, &module);
	assert_no_errors(sns.el);
	assert_true(valid, "valid module");
	assert_ptr(module, "ptr module");
	expect_int_equal(module->type, token_module, "module module");

	token_destroy(module);
	free(module);

	struct token* number;
	valid = scan_get_token(&sns, &number);
	assert_no_errors(sns.el);
	assert_true(valid, "valid number");
	assert_ptr(number, "ptr number");
	expect_int_equal(number->type, token_number, "number number");
	expect_str(&number->value, "1", "1 number");

	token_destroy(number);
	free(number);

	struct token* end;
	valid = scan_get_token(&sns, &end);
	assert_no_errors(sns.el);
	assert_true(valid, "valid end");
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
