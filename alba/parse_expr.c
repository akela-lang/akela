#include "zinc/result.h"
#include "token.h"
#include "dag.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "source.h"
#include "scan.h"

/*
* expr -> not_operator
*/
enum result expr(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;
	struct token* t;

	r = get_lookahead(ps, 2, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	/* id = expr */
	if (t0 && t0->type == token_id && t1 && t1->type == token_equal) {
		r = match(ps, token_id, "expected word", &t);
		if (r == result_error) {
			goto function_error;
		}
		r = match(ps, token_equal, "expected equal", &t);
		if (r == result_error) {
			goto function_error;
		}

		dag_create_node(&n);
		n->type = dag_type_assign;

		struct dag_node* a;
		dag_create_node(&a);
		a->type = dag_type_id;
		buffer_copy(&t0->value, &a->value);
		dag_add_child(n, a);

		struct dag_node* b;
		r = expr(ps, &b);
		if (r == result_error) {
			goto function_error;
		}
		if (b == NULL) {
			r = set_source_error(ps->sns->el, t0, ps->sns->lc, "expect expression on rhs of assignment operator");
			goto function_error;
		}
		dag_add_child(n, b);
		goto function_success;
	}

	r = boolean(ps, &n);
	if (r == result_error) {
		goto function_error;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* boolean -> comparison boolean'
*/

enum result boolean(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct dag_node* n = NULL;

	r = comparison(ps, &a);
	if (r == result_error) {
		return r;
	}

	r = boolean_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		n = a;
		*root = n;
	} else if (a && b) {
		dag_push(c, a);
		*root = b;
	}

	return r;
}

/*
* boolean' -> && comparison boolean'
*			| || comparison boolean'
*			| e
*/
enum result boolean_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;
	struct token* t;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return r;
	}

	/* operator */
	if (t0->type == token_and) {
		type = dag_type_and;
	} else if (t0->type == token_or) {
		type = dag_type_or;
	} else {
		/* e */
		return r;
	}

	dag_create_node(&n);

	n->type = type;
	r = match(ps, t0->type, "expecting + or -", &t);
	if (r == result_error) {
		return r;
	}

	/* comparison */
	r = comparison(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* boolean' */
	r = boolean_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return r;
	} else if (a && b) {
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return r;
	} else {
		return r;
	}
}

/*
* comparison -> add comparison'
*/
enum result comparison(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct dag_node* n = NULL;

	r = add(ps, &a);
	if (r == result_error) {
		return r;
	}

	r = comparison_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		n = a;
		*root = n;
	} else if (a && b) {
		dag_push(c, a);
		*root = b;
	}

	return r;
}

/*
* comparison' -> == add comparison'
*			   | < add comparison'
*	           | <= add comparison'
*			   | > add comparison'
*	           | >= add comparison'
*	           | e
*/
enum result comparison_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;
	struct token* t;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return r;
	}

	/* operator */
	if (t0->type == token_double_equal) {
		type = dag_type_equality;
	} else if (t0->type == token_not_equal) {
		type = dag_type_not_equal;
	} else if (t0->type == token_less_than) {
		type = dag_type_less_than;
	} else if (t0->type == token_less_than_or_equal) {
		type = dag_type_less_than_or_equal;
	} else if (t0->type == token_greater_than) {
		type = dag_type_greater_than;
	} else if (t0->type == token_greater_than_or_equal) {
		type = dag_type_greater_than_or_equal;
	} else {
		/* e */
		return r;
	}

	dag_create_node(&n);

	n->type = type;
	r = match(ps, t0->type, "expecting + or -", &t);
	if (r == result_error) {
		return r;
	}

	/* add */
	r = add(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* comparison' */
	r = comparison_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return r;
	} else if (a && b) {
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return r;
	} else {
		return r;
	}
}

/*
* add -> mult add'
*/
enum result add(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct dag_node* n = NULL;
	int num;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);

	r = mult(ps, &a);
	if (r == result_error) {
		return r;
	}

	r = add_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		n = a;
		*root = n;
	} else if (a && b) {
		dag_push(c, a);
		*root = b;
	} else if (!a && b) {
		r = set_source_error(ps->el, t0, ps->sns->lc, "expected term before operator");
	}

	return r;
}

/*
* add' -> + mult add'
*	    | - mult add'
*	    | e
*/
enum result add_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;
	struct token* t;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return r;
	}

	/* operator */
	if (t0->type == token_plus) {
		type = dag_type_plus;
	} else if (t0->type == token_minus) {
		type = dag_type_minus;
	} else {
		/* e */
		return r;
	}

	dag_create_node(&n);

	n->type = type;
	r = match(ps, t0->type, "expecting + or -", &t);
	if (r == result_error) {
		return r;
	}

	/* term */
	r = mult(ps, &a);
	if (r == result_error) {
		return r;
	}

	if (!a) {
		r = set_source_error(ps->el, t0, ps->sns->lc, "expected term after additive operator");
		return r;
	}

	/* add' */
	r = add_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return r;
	} else if (a && b) {
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return r;
	} else {
		return r;
	}
}

/*
* mult -> array_subscript mult_prime
*/
enum result mult(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;

	r = array_subscript(ps, &a);
	if (r == result_error) {
		return r;
	}

	int num;
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t = get_token(&ps->lookahead, 0);

	r = mult_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		n = a;
		*root = n;
	} else if (a && b) {
		dag_push(c, a);
		*root = b;
	} else if (!a && b) {
		r = set_source_error(ps->el, t, ps->sns->lc, "expected term before operator");
	}

	return r;
}

/*
* term' -> * array_subscript mult'
*	     | / array_subscript mult'
*	     | e
*/
enum result mult_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	struct token* t;

	int num;
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return r;
	}

	/* operator */
	if (t0->type == token_mult) {
		type = dag_type_mult;
	} else if (t0->type == token_divide) {
		type = dag_type_divide;
	} else {
		return r;
	}

	dag_create_node(&n);

	n->type = type;
	r = match(ps, t0->type, "expecting * or /", &t);
	if (r == result_error) {
		return r;
	}

	/* factor */
	r = array_subscript(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* mult' */
	r = mult_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return r;
	} else if (a && b) {
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return r;
	} else {
		return r;
	}
}

/*
* array_subscript -> factor array_subscript'
* array_subscript' -> [array_subscript] array_subscript' | e
*/
enum result array_subscript(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	int num;
	struct dag_node* n = NULL;
	struct token* t;
	
	r = factor(ps, &n);
	if (r == result_error) {
		goto function_error;
	}

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		struct dag_node* a = n;
		dag_create_node(&n);
		n->type = dag_type_array_subscript;
		dag_add_child(n, a);

		r = match(ps, token_left_square_bracket, "expecting array subscript operator", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* b = NULL;
		r = array_subscript(ps, &b);
		if (!b) {
			r = set_source_error(ps->el, t0, ps->sns->lc, "expected array subscript after subscript operator");
			goto function_error;
		}
		dag_add_child(n, b);

		r = match(ps, token_right_square_bracket, "expecting array subscript operator", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = array_subscript_prime(ps, n);
		if (r == result_error) {
			goto function_error;
		}
	}

function_ok:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* array_subscript' -> [array_subscript] array_subscript' | e
*/
enum result array_subscript_prime(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;
	struct token* t;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (!t0 || t0->type != token_left_square_bracket) {
		/* e */
		return r;
	}

	r = match(ps, token_left_square_bracket, "expected left square bracket", &t);
	if (r == result_error) {
		return r;
	}

	struct dag_node* a = NULL;
	r = array_subscript(ps, &a);
	if (r == result_error) {
		return r;
	}
	if (!a) {
		return set_source_error(ps->el, t0, ps->sns->lc, "expected array subscript after subscript operator");
	}
	dag_add_child(parent, a);

	r = match(ps, token_right_square_bracket, "expected right square_bracket", &t);
	if (r == result_error) {
		return r;
	}

	struct dag_node* b = NULL;
	r = array_subscript_prime(ps, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}
