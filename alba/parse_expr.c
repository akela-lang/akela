#include "zinc/result.h"
#include "token.h"
#include "dag.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "source.h"
#include "scan.h"

/* expr -> id = expr | boolean */
/* dynamic-output ps{} root root{} */
enum result expr(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;
	struct location loc;

	/* allocate ps{} */
	r = get_lookahead(ps, 2, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* id = expr */
	if (t0 && t0->type == token_id && t1 && t1->type == token_equal) {

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		r = match(ps, token_id, "expected word", &id);
		if (r == result_error) {
			return r;
		}

		/* allocate ps{} equal equal{} */
		struct token* equal = NULL;
		r = match(ps, token_equal, "expected equal", &equal);
		if (r == result_error) {
			/* destroy id id{} */
			token_destroy(id);
			free(id);
			return r;
		}

		/* destroy equal equal{} */
		token_destroy(equal);
		free(equal);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_assign;

		/* allocate a */
		struct dag_node* a;
		dag_create_node(&a);
		a->type = dag_type_id;

		/* allocate a{} */
		buffer_copy(&id->value, &a->value);

		/* destroy id id{} */
		token_destroy(id);
		free(id);

		/* transfer a a{} -> n{} */
		dag_add_child(n, a);

		/* allocate b b{} */
		struct dag_node* b;
		r = expr(ps, &b);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		if (b == NULL) {
			dag_destroy(n);
			return set_source_error(ps->sns->el, &loc, "expect expression on rhs of assignment operator");
		}

		/* transfer b b{} -> n{} */
		dag_add_child(n, b);

		/* transfer n n{} -> root */
		*root = n;
		return r;
	}

	/* allocate n n{} */
	r = boolean(ps, &n);
	if (r == result_error) {
		return r;
	}

	/* transfer n n{} -> root */
	*root = n;
	return r;
}

/* boolean -> comparison boolean' */
/* dynamic-output ps{} root root{} */
enum result boolean(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct location loc;

	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate a a{} */
	r = comparison(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* allocate b{} */
	r = boolean_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		/* transfer a a{} -> root */
		*root = a;
	} else if (a && b) {
		/* allocate b{} */
		dag_push(c, a);
		/* transfer b b{} -> root */
		*root = b;
	} else if (!a && b) {
		dag_destroy(b);
		return set_source_error(ps->el, &loc, "internal comparison non-terminal parse error");
	}

	return r;
}

/* boolean' -> && comparison boolean' | || comparison boolean' | e */
/* dynamic-output ps{} root root{} */
enum result boolean_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	/* allocate ps{} */
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

	/* allocate n */
	dag_create_node(&n);
	n->type = type;

	/* allocate ps{} op op{} */
	struct token* op = NULL;
	r = match(ps, t0->type, "expecting + or -", &op);
	if (r == result_error) {
		/* destroy n */
		dag_destroy(n);
		return r;
	}

	/* destroy op op{} */
	token_destroy(op);
	free(op);

	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		dag_destroy(n);
		return r;
	}

	/* comparison */
	/* allocate a a{} */
	r = comparison(ps, &a);
	if (r == result_error) {
		/* destroy n */
		dag_destroy(n);
		return r;
	}

	/* boolean' */
	/* allocate b b{} */
	r = boolean_prime(ps, &b, &c);
	if (r == result_error) {
		/* destroy n */
		dag_destroy(n);
		return r;
	}

	if (a && !b) {
		/* transfer a a{} -> n{} */
		dag_add_child(n, a);
		/* transfer n n{} -> root */
		*root = n;
		*insert_point = n;
		return r;
	} else if (a && b) {
		/* transfer a a{} -> n{} */
		/* transfer n n{} -> b{} */
		/* transfer b b{} -> root */
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return r;
	} else if (!a && !b) {
		/* destroy n */
		dag_destroy(n);
		return set_source_error(ps->el, &loc, "no right hand op in logical expression");
	} else if (!a && b) {
		/* destroy n b */
		dag_destroy(n);
		dag_destroy(b);
		return set_source_error(ps->el, &loc, "no right hand op in logical expression");
	} else if (!a && !b) {
		/* destroy n */
		dag_destroy(n);
		return set_source_error(ps->el, &loc, "no right hand op in logical expression");
	}

	return r;
}

/* comparison -> add comparison' */
/* dynamic-output ps{} root root{} */
enum result comparison(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;

	/* allocate ps{} */
	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate ps{} a a{} */
	r = add(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* allocate ps{} b b{} */
	r = comparison_prime(ps, &b, &c);
	if (r == result_error) {
		return r;
	}

	if (a && !b) {
		/* transfer a -> root */
		*root = a;
	} else if (a && b) {
		/* transfer a -> b{} */
		/* transfer b -> root */
		dag_push(c, a);
		*root = b;
	} else if (!a && b) {
		/* destroy b */
		dag_destroy(b);
		return set_source_error(ps->el, &loc, "internal error in comparison non-terminal");
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
/* dynamic-output ps{} root root{} */
enum result comparison_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	/* allocate ps{} */
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

	/* allocate ps{} op op{} */
	struct token* op = NULL;
	r = match(ps, t0->type, "expecting comparator", &op);
	if (r == result_error) {
		return r;
	}

	/* destroy op op{} */
	token_destroy(op);
	free(op);

	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate n */
	dag_create_node(&n);
	n->type = type;

	/* add */
	/* allocate ps{} a a{} */
	r = add(ps, &a);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* comparison' */
	/* allocate ps{} b b{} */
	r = comparison_prime(ps, &b, &c);
	if (r == result_error) {
		/* destroy n n{} a a{} */
		dag_destroy(n);
		dag_destroy(a);
		return r;
	}

	if (a && !b) {
		/* transfer a -> n{} */
		/* transfer n -> root */
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return r;
	} else if (a && b) {
		/* transfer a -> n{} */
		/* transfer n -> b b{} */
		/* transfer b b{} -> root root{} */
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return r;
	} else if (!a && b) {
		/* destroy b n */
		dag_destroy(b);
		dag_destroy(n);
		return set_source_error(ps->el, &loc, "Missing right-hand-side of comparison operator");
	} else if (!a && !b) {
		/* destroy n */
		dag_destroy(n);
		return set_source_error(ps->el, &loc, "Missing right-hand-side of comparison operator");
	}

	return r;
}

/* add -> mult add' */
/* dynamic-output ps{} root root{} */
enum result add(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	int num;

	/* allocate ps{} */
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);

	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate a a{} */
	r = mult(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* allocate b b{} */
	r = add_prime(ps, &b, &c);
	if (r == result_error) {
		dag_destroy(a);
		return r;
	}

	if (a && !b) {
		/* transfer a -> n */
		/* transfer n -> root */
		*root = a;
	} else if (a && b) {
		/* transfer a -> b{} */
		/* trasnfer b -> root */
		dag_push(c, a);
		*root = b;
	} else if (!a && b) {
		/* destroy b */
		dag_destroy(b);
		r = set_source_error(ps->el, &loc, "expected term before operator");
	}

	return r;
}

/* add' -> + mult add' | - mult add' | e */
/* dynamic-output ps{} root root{} */
enum result add_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	/* allocate ps{} */
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

	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate n */
	dag_create_node(&n);
	n->type = type;

	/* allocate ps{} */
	struct token* op = NULL;
	r = match(ps, t0->type, "expecting + or -", &op);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* destroy op op{} */
	token_destroy(op);
	free(op);

	/* term */
	/* allocate a a{} */
	r = mult(ps, &a);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	if (!a) {
		/* destroy n n{} */
		dag_destroy(n);
		r = set_source_error(ps->el, &loc, "expected term after additive operator");
		return r;
	}

	/* add' */
	/* allocate b b{} */
	r = add_prime(ps, &b, &c);
	if (r == result_error) {
		/* destroy n n{} a a{} */
		dag_destroy(n);
		dag_destroy(a);
		return r;
	}

	if (a && !b) {
		/* transfer a -> n{} */
		/* transfer n -> root */
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return r;
	} else if (a && b) {
		/* transfer a -> n{} */
		/* transfer n -> b{} */
		/* transfer b -> root */
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return r;
	} else {
		return r;
	}
}

/* mult -> array_subscript mult_prime */
/* dynamic-output ps{} root root{} */
enum result mult(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct location loc;

	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate ps{} a a{} */
	r = array_subscript(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* allocate ps{} b b{} */
	r = mult_prime(ps, &b, &c);
	if (r == result_error) {
		/* destroy a a{} */
		dag_destroy(a);
		return r;
	}

	if (a && !b) {
		/* transfer a -> root */
		*root = a;
	} else if (a && b) {
		/* transfer a -> b{} */
		/* transfer b -> root */
		dag_push(c, a);
		*root = b;
	} else if (!a && b) {
		dag_destroy(b);
		return set_source_error(ps->el, &loc, "expected term before operator");
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
		return r;
	}

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		dag_destroy(n);
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		struct dag_node* a = n;
		dag_create_node(&n);
		n->type = dag_type_array_subscript;
		dag_add_child(n, a);

		r = match(ps, token_left_square_bracket, "expecting array subscript operator", &t);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		struct location loc;
		r = get_parse_location(ps, &loc);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		struct dag_node* b = NULL;
		r = array_subscript(ps, &b);
		if (!b) {
			return set_source_error(ps->el, &loc, "expected array subscript after subscript operator");
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

	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	struct dag_node* a = NULL;
	r = array_subscript(ps, &a);
	if (r == result_error) {
		return r;
	}
	if (!a) {
		return set_source_error(ps->el, &loc, "expected array subscript after subscript operator");
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
