#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "dag.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "source.h"
#include "scan.h"
#include "parse_types.h"

bool var(struct parse_state* ps, struct dag_node** root);
bool assignment(struct parse_state* ps, struct dag_node** root);
bool boolean(struct parse_state* ps, struct dag_node** root);
bool boolean_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point);
bool comparison(struct parse_state* ps, struct dag_node** root);
bool comparison_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point);
bool add(struct parse_state* ps, struct dag_node** root);
bool add_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point);
bool mult(struct parse_state* ps, struct dag_node** root);
bool mult_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point);
bool array_subscript(struct parse_state* ps, struct dag_node** root);
bool array_subscript_prime(struct parse_state* ps, struct dag_node* parent);

/* expr -> id = expr | boolean */
/* dynamic-output ps{} root root{} */
bool expr(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 2, &num);
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);


	/* let, assignment, or operator/term */
	if (t0 && t0->type == token_var) {
		valid = valid && var(ps, &n);
	} else if (t0 && t0->type == token_id && t1 && t1->type == token_equal) {
		valid = valid && assignment(ps, &n);
	} else {
		/* allocate n n{} */
		valid = valid && boolean(ps, &n);
	}

	/* transfer n n{} -> root */
	if (valid) {
		*root = n;
	}
	return valid;
}

bool var(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	struct token* vrt = NULL;
	valid = valid && match(ps, token_var, "expected var", &vrt);

	token_destroy(vrt);
	free(vrt);

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate ps{} id id{} */
	struct dag_node* a = NULL;
	valid = valid && declaration(ps, &a);
	if (!a) {
		set_source_error(ps->el, &loc, "expected declaration after var");
		valid = false;
		return valid;
	}

	int num;
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_equal) {
		/* allocate ps{} equal equal{} */
		struct token* equal = NULL;
		valid = valid && match(ps, token_equal, "expected equal", &equal);

		/* destroy equal equal{} */
		token_destroy(equal);
		free(equal);

		struct dag_node* b = NULL;
		valid = valid && expr(ps, &b);

		if (!b) {
			/* destroy a a{} */
			dag_destroy(a);
			return valid;
		}

		dag_create_node(&n);
		n->type = dag_type_var;

		dag_add_child(n, a);
		dag_add_child(n, b);
	} else {
		dag_create_node(&n);
		n->type = dag_type_var;
		dag_add_child(n, a);
	}

	if (valid) {
		*root = n;
	}

	return valid;
}

bool assignment(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = valid && match(ps, token_id, "expected identifier", &id);

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	valid = valid && match(ps, token_equal, "expected equal", &equal);

	/* allocate b b{} */
	struct dag_node* b = NULL;
	valid = valid && expr(ps, &b);

	if (!b) {
		valid = set_source_error(ps->sns->el, &loc, "expect expression on rhs of assignment operator");
	}

	if (valid) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_assign;

		/* allocate a */
		struct dag_node* a;
		dag_create_node(&a);
		a->type = dag_type_id;

		/* allocate a{} */
		#pragma warning (suppress:6011)
		buffer_copy(&id->value, &a->value);

		/* transfer a a{} -> n{} */
		dag_add_child(n, a);

		/* transfer b b{} -> n{} */
		dag_add_child(n, b);

		/* transfer n n{} -> root */
		*root = n;
	}

	if (valid) {
		struct symbol* sym = environment_get(ps->st->top, &id->value);
		if (!sym) {
			char* a;
			buffer2array(&id->value, &a);
			valid = set_source_error(ps->el, &loc, "identifier not declared: %s", a);
			free(a);
		}
	}

	return valid;
}

/* boolean -> comparison boolean' */
/* dynamic-output ps{} root root{} */
bool boolean(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct dag_node* n = NULL;
	struct location loc;

	valid = valid && get_parse_location(ps, &loc);

	/* allocate a a{} */
	valid = valid && comparison(ps, &a);

	/* allocate b{} */
	valid = valid && boolean_prime(ps, &b, &c);

	if (a && !b) {
		/* transfer a a{} -> root */
		n = a;
	} else if (a && b) {
		/* allocate b{} */
		dag_push(c, a);
		/* transfer b b{} -> root */
		n = b;
	} else if (!a && b) {
		dag_destroy(b);
		set_source_error(ps->el, &loc, "internal comparison non-terminal parse error");
		valid = false;
	}

	if (valid) {
		*root = n;
	}
	return valid;
}

/* boolean' -> && comparison boolean' | || comparison boolean' | e */
/* dynamic-output ps{} root root{} */
bool boolean_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	bool valid = true;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return valid;
	}

	/* operator */
	if (t0->type == token_and) {
		type = dag_type_and;
	} else if (t0->type == token_or) {
		type = dag_type_or;
	} else {
		/* e */
		return valid;
	}

	/* allocate n */
	dag_create_node(&n);
	n->type = type;

	/* allocate ps{} op op{} */
	struct token* op = NULL;
	valid = valid && match(ps, t0->type, "expecting + or -", &op);

	/* destroy op op{} */
	token_destroy(op);
	free(op);

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* comparison */
	/* allocate a a{} */
	valid = valid && comparison(ps, &a);

	/* boolean' */
	/* allocate b b{} */
	valid = valid && boolean_prime(ps, &b, &c);

	if (a && !b) {
		/* transfer a a{} -> n{} */
		dag_add_child(n, a);
		/* transfer n n{} -> root */
		*root = n;
		*insert_point = n;
		return valid;
	} else if (a && b) {
		/* transfer a a{} -> n{} */
		/* transfer n n{} -> b{} */
		/* transfer b b{} -> root */
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return valid;
	} else if (!a && !b) {
		/* destroy n */
		dag_destroy(n);
		set_source_error(ps->el, &loc, "no right hand op in logical expression");
		valid = false;
		return valid;
	} else if (!a && b) {
		/* destroy n b */
		dag_destroy(n);
		dag_destroy(b);
		set_source_error(ps->el, &loc, "no right hand op in logical expression");
		valid = false;
		return valid;
	} else if (!a && !b) {
		/* destroy n */
		dag_destroy(n);
		set_source_error(ps->el, &loc, "no right hand op in logical expression");
		valid = false;
		return valid;
	}

	return valid;
}

/* comparison -> add comparison' */
/* dynamic-output ps{} root root{} */
bool comparison(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;

	/* allocate ps{} */
	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate ps{} a a{} */
	valid = valid && add(ps, &a);

	/* allocate ps{} b b{} */
	valid = valid && comparison_prime(ps, &b, &c);

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
		set_source_error(ps->el, &loc, "internal error in comparison non-terminal");
		valid = false;
	}

	return valid;
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
bool comparison_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	bool valid = true;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return valid;
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
		return valid;
	}

	/* allocate ps{} op op{} */
	struct token* op = NULL;
	valid = valid && match(ps, t0->type, "expecting comparator", &op);

	/* destroy op op{} */
	token_destroy(op);
	free(op);

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate n */
	dag_create_node(&n);
	n->type = type;

	/* add */
	/* allocate ps{} a a{} */
	valid = valid && add(ps, &a);

	/* comparison' */
	/* allocate ps{} b b{} */
	valid = valid && comparison_prime(ps, &b, &c);

	if (a && !b) {
		/* transfer a -> n{} */
		/* transfer n -> root */
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return valid;
	} else if (a && b) {
		/* transfer a -> n{} */
		/* transfer n -> b b{} */
		/* transfer b b{} -> root root{} */
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return valid;
	} else if (!a && b) {
		/* destroy b n */
		dag_destroy(b);
		dag_destroy(n);
		set_source_error(ps->el, &loc, "Missing right-hand-side of comparison operator");
		valid = false;
	} else if (!a && !b) {
		/* destroy n */
		dag_destroy(n);
		set_source_error(ps->el, &loc, "Missing right-hand-side of comparison operator");
		valid = false;
	}

	return valid;
}

/* add -> mult add' */
/* dynamic-output ps{} root root{} */
bool add(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate a a{} */
	valid = valid && mult(ps, &a);

	/* allocate b b{} */
	valid = valid && add_prime(ps, &b, &c);

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
		set_source_error(ps->el, &loc, "expected term before operator");
		valid = false;
	}

	return valid;
}

/* add' -> + mult add' | - mult add' | e */
/* dynamic-output ps{} root root{} */
bool add_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	bool valid = true;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return valid;
	}

	/* operator */
	if (t0->type == token_plus) {
		type = dag_type_plus;
	} else if (t0->type == token_minus) {
		type = dag_type_minus;
	} else {
		/* e */
		return valid;
	}

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate n */
	dag_create_node(&n);
	n->type = type;

	/* allocate ps{} */
	struct token* op = NULL;
	valid = valid && match(ps, t0->type, "expecting + or -", &op);

	/* destroy op op{} */
	token_destroy(op);
	free(op);

	/* term */
	/* allocate a a{} */
	valid = valid && mult(ps, &a);

	if (!a) {
		/* destroy n n{} */
		dag_destroy(n);
		set_source_error(ps->el, &loc, "expected term after additive operator");
		valid = false;
		return valid;
	}

	/* add' */
	/* allocate b b{} */
	valid = valid && add_prime(ps, &b, &c);

	if (a && !b) {
		/* transfer a -> n{} */
		/* transfer n -> root */
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return valid;
	} else if (a && b) {
		/* transfer a -> n{} */
		/* transfer n -> b{} */
		/* transfer b -> root */
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return valid;
	} else {
		return valid;
	}
}

/* mult -> array_subscript mult_prime */
/* dynamic-output ps{} root root{} */
bool mult(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct location loc;

	valid = valid && get_parse_location(ps, &loc);

	/* allocate ps{} a a{} */
	valid = valid && array_subscript(ps, &a);

	/* allocate ps{} b b{} */
	valid = valid && mult_prime(ps, &b, &c);

	if (a && !b) {
		/* transfer a -> root */
		*root = a;
	} else if (a && b) {
		/* transfer a -> b{} */
		/* transfer b -> root */
		dag_push(c, a);
		*root = b;
	} else if (!a && b) {
		/* destroy b b{} */
		dag_destroy(b);
		set_source_error(ps->el, &loc, "expected term before operator");
		valid = false;
	}

	return valid;
}

/* term' -> * array_subscript mult' | / array_subscript mult' | e */
/* dynamic-output ps{} root root{} */
bool mult_prime(struct parse_state* ps, struct dag_node** root, struct dag_node** insert_point)
{
	bool valid = true;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;

	/* allocate ps{} */
	int num;
	valid = valid && get_lookahead(ps, 1, &num);
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		return valid;
	}

	/* operator */
	if (t0->type == token_mult) {
		type = dag_type_mult;
	} else if (t0->type == token_divide) {
		type = dag_type_divide;
	} else {
		return valid;
	}

	/* allocate ps{} t t{} */
	struct token* op = NULL;
	valid = valid && match(ps, t0->type, "expecting * or /", &op);

	token_destroy(op);
	free(op);

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* factor */
	/* allocate ps{} a a{} */
	valid = valid && array_subscript(ps, &a);

	if (!a) {
		/* allocate ps{} */
		set_source_error(ps->el, &loc, "Expecting term after operator");
		valid = false;
	}

	/* mult' */
	/* allocate ps{} b b{} */
	valid = valid && mult_prime(ps, &b, &c);

	/* allocate n */
	dag_create_node(&n);
	n->type = type;

	if (a && !b) {
		/* transfer a -> n{} */
		/* transfer n -> root */
		dag_add_child(n, a);
		*root = n;
		*insert_point = n;
		return valid;
	} else if (a && b) {
		/* transfer a -> n{} */
		/* transfer n -> b{} */
		/* transfer b -> root */
		dag_add_child(n, a);
		dag_push(c, n);
		*root = b;
		*insert_point = n;
		return valid;
	} else {
		return valid;
	}
}

/* array_subscript -> factor array_subscript' */
/* dynamic-output ps{} root root{} */
bool array_subscript(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	int num;
	struct dag_node* n = NULL;
	
	/* allocate ps{} n n{} */
	valid = valid && factor(ps, &n);

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		/* transfer n -> a */
		/* allocate n */
		/* transfer a -> n{} */
		struct dag_node* a = n;
		dag_create_node(&n);
		n->type = dag_type_array_subscript;
		dag_add_child(n, a);

		/* allocate ps{} lsb lsb{} */
		struct token* lsb = NULL;
		valid = valid && match(ps, token_left_square_bracket, "expecting array subscript operator", &lsb);

		/* destroy lsb lsb{} */
		token_destroy(lsb);
		free(lsb);

		/* allocate ps{} */
		struct location loc;
		valid = valid && get_parse_location(ps, &loc);

		/* allocate b b{} */
		struct dag_node* b = NULL;
		valid = valid && array_subscript(ps, &b);
		if (!b) {
			set_source_error(ps->el, &loc, "expected array subscript after subscript operator");
			valid = false;
		} else {
			/* transfer b -> n{} */
			dag_add_child(n, b);
		}

		/* allocate ps{} rsb rsb{} */
		struct token* rsb = NULL;
		valid = valid && match(ps, token_right_square_bracket, "expecting array subscript operator", &rsb);

		/* destroy rsb rsb{} */
		token_destroy(rsb);
		free(rsb);

		/* allocate n{} */
		valid = valid && array_subscript_prime(ps, n);
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}
	return valid;
}

/* array_subscript' -> [expr] array_subscript' | e */
/* dynamic-output ps{} parent{} */
bool array_subscript_prime(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (!t0 || t0->type != token_left_square_bracket) {
		/* e */
		return valid;
	}

	/* allocate ps{} lsb lsb{} */
	struct token* lsb = NULL;
	valid = valid && match(ps, token_left_square_bracket, "expected left square bracket", &lsb);

	/* destroy lsb lsb{} */
	token_destroy(lsb);
	free(lsb);

	/* allocate ps{} */
	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate a a{} */
	struct dag_node* a = NULL;
	valid = valid && expr(ps, &a);

	if (!a) {
		/* allocate ps{} */
		set_source_error(ps->el, &loc, "expected array subscript after subscript operator");
		valid = false;
	} else {
		/* transfer a -> parent */
		dag_add_child(parent, a);
	}

	/* allocate ps{} rsb rsb{} */
	struct token* rsb = NULL;
	valid = valid && match(ps, token_right_square_bracket, "expected right square_bracket", &rsb);

	/* destroy rsb rsb{} */
	token_destroy(rsb);
	free(rsb);

	/* allocate ps{} parent{} */
	struct dag_node* b = NULL;
	valid = valid && array_subscript_prime(ps, parent);

	return valid;
}
