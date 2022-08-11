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
bool comparison(struct parse_state* ps, struct dag_node** root);
bool add(struct parse_state* ps, struct dag_node** root);
bool mult(struct parse_state* ps, struct dag_node** root);
bool array_subscript(struct parse_state* ps, struct dag_node** root);

/* expr -> id = expr | boolean */
/* dynamic-output ps{} root root{} */
bool expr(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = get_lookahead(ps, 2, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);


	/* let, assignment, or operator/term */
	if (t0 && t0->type == token_var) {
		valid = var(ps, &n) && valid;
	} else if (t0 && t0->type == token_id && t1 && t1->type == token_equal) {
		valid = assignment(ps, &n) && valid;
	} else {
		/* allocate n n{} */
		valid = boolean(ps, &n) && valid;
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
	valid = match(ps, token_var, "expected var", &vrt) && valid;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} id id{} */
	struct dag_node* a = NULL;
	valid = declaration(ps, &a) && valid;
	if (!a) {
		valid = set_source_error(ps->el, &loc, "expected declaration after var");
	}

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);

	struct token* equal = NULL;
	struct dag_node* b = NULL;
	if (t0 && t0->type == token_equal) {
		/* allocate ps{} equal equal{} */
		valid = match(ps, token_equal, "expected equal", &equal) && valid;

		valid = get_parse_location(ps, &loc) && valid;
		valid = expr(ps, &b) && valid;

		if (!b) {
			valid = set_source_error(ps->el, &loc, "expected expression after equal");
		}
	}

	if (valid) {
		dag_create_node(&n);
		n->type = dag_type_var;

		dag_add_child(n, a);
		if (b) {
			dag_add_child(n, b);
		}

		*root = n;
	} else {
		dag_destroy(a);
		dag_destroy(b);
	}

	token_destroy(vrt);
	free(vrt);
	token_destroy(equal);
	free(equal);

	return valid;
}

bool assignment(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expected identifier", &id) && valid;

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	valid = match(ps, token_equal, "expected equal", &equal) && valid;

	/* allocate b b{} */
	struct dag_node* b = NULL;
	valid = expr(ps, &b) && valid;

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
/* boolean' -> && comparison boolean' | || comparison boolean' | e */
/* dynamic-output ps{} root root{} */
bool boolean(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
	struct dag_node* left;

	/* allocate a a{} */
	struct dag_node* a = NULL;
	valid = comparison(ps, &a) && valid;
	if (!a) {
		return valid;
	}

	left = a;

	while (true) {
		/* allocate b{} */
		/* allocate ps{} */
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		/* operator */
		enum dag_type type = dag_type_none;
		if (t0 && t0->type == token_and) {
			type = dag_type_and;
		} else if (t0 && t0->type == token_or) {
			type = dag_type_or;
		} else {
			break;
		}

		/* allocate n */
		dag_create_node(&n);
		n->type = type;

		/* allocate ps{} op op{} */
		struct token* op = NULL;
		valid = match(ps, t0->type, "expecting + or -", &op) && valid;

		/* destroy op op{} */
		token_destroy(op);
		free(op);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* comparison */
		/* allocate a a{} */
		struct dag_node* b = NULL;
		valid = comparison(ps, &b) && valid;

		if (!b) {
			valid = set_source_error(ps->el, &loc, "expected term after + or -");
		}

		if (valid) {
			dag_add_child(n, left);
			dag_add_child(n, b);
			left = n;
		} else {
			dag_destroy(b);
			break;
		}
	}

	if (valid) {
		*root = left;
	} else {
		dag_destroy(left);
	}

	return valid;
}

/* comparison -> add comparison' */
/*
* comparison' -> == add comparison'
*			   | < add comparison'
*	           | <= add comparison'
*			   | > add comparison'
*	           | >= add comparison'
*	           | e
*/
/* dynamic-output ps{} root root{} */
bool comparison(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* left = NULL;
	int num;

	/* allocate ps{} a a{} */
	valid = add(ps, &a) && valid;

	if (!a) {
		return valid;
	}

	if (valid) {
		left = a;
	} else {
		dag_destroy(a);
	}

	while (true) {
		/* allocate ps{} */
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);
		enum dag_type type = dag_type_none;

		if (!t0) {
			break;
		}

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
			break;
		}

		/* allocate ps{} op op{} */
		struct token* op = NULL;
		valid = match(ps, t0->type, "expecting comparator", &op) && valid;

		/* destroy op op{} */
		token_destroy(op);
		free(op);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* add */
		/* allocate ps{} a a{} */
		struct dag_node* b = NULL;
		valid = add(ps, &b) && valid;

		if (!b) {
			valid = set_source_error(ps->el, &loc, "expected term after compare operator");
		}

		if (valid) {
			/* allocate n */
			dag_create_node(&n);
			n->type = type;

			dag_add_child(n, left);
			dag_add_child(n, b);
			left = n;
		} else {
			dag_destroy(b);
		}
	}

	if (valid) {
		*root = left;
	} else {
		dag_destroy(left);
	}

	return valid;
}

/* add -> mult add' */
/* add' -> + mult add' | - mult add' | e */
/* dynamic-output ps{} root root{} */
bool add(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* n = NULL;
	struct dag_node* left = NULL;
	int num;

	/* allocate a a{} */
	valid = mult(ps, &a) && valid;

	if (!a) {
		return valid;
	}

	if (valid) {
		left = a;
	} else {
		dag_destroy(a);
	}

	while (true) {
		/* allocate ps{} */
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		/* e */
		if (t0 == NULL) {
			break;
		}

		/* operator */
		enum dag_type type;
		if (t0->type == token_plus) {
			type = dag_type_plus;
		} else if (t0->type == token_minus) {
			type = dag_type_minus;
		} else {
			break;
		}

		/* allocate ps{} */
		struct token* op = NULL;
		valid = match(ps, t0->type, "expecting + or -", &op) && valid;

		/* destroy op op{} */
		token_destroy(op);
		free(op);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* allocate a a{} */
		valid = mult(ps, &b) && valid;

		if (!b) {
			/* destroy n n{} */
			dag_destroy(n);
			valid = set_source_error(ps->el, &loc, "expected term after additive operator");
			break;
		}

		if (valid) {
			dag_create_node(&n);
			n->type = type;

			dag_add_child(n, left);
			dag_add_child(n, b);
			left = n;
		} else {
			dag_destroy(b);
		}
	}

	if (valid) {
		*root = left;
	} else {
		dag_destroy(left);
	}

	return valid;
}

/* mult -> array_subscript mult_prime */
/* dynamic-output ps{} root root{} */
bool mult(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* left = NULL;
	struct dag_node* n = NULL;
	struct location loc;


	/* allocate ps{} a a{} */
	valid = array_subscript(ps, &a) && valid;

	if (!a) {
		return valid;
	}

	if (valid) {
		left = a;
	} else {
		dag_destroy(a);
	}

	while (true) {
		/* allocate ps{} */
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		/* e */
		if (t0 == NULL) {
			break;
		}

		/* operator */
		enum dag_type type;
		if (t0->type == token_mult) {
			type = dag_type_mult;
		} else if (t0->type == token_divide) {
			type = dag_type_divide;
		} else {
			break;
		}

		/* allocate ps{} t t{} */
		struct token* op = NULL;
		valid = match(ps, t0->type, "expecting * or /", &op) && valid;

		token_destroy(op);
		free(op);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* factor */
		/* allocate ps{} a a{} */
		valid = array_subscript(ps, &b) && valid;

		if (!b) {
			/* allocate ps{} */
			valid = set_source_error(ps->el, &loc, "expected term after operator");
		}

		if (valid) {
			/* allocate n */
			dag_create_node(&n);
			n->type = type;

			dag_add_child(n, left);
			dag_add_child(n, b);
			left = n;
		} else {
			dag_destroy(b);
		}
	}

	if (valid) {
		*root = left;
	} else {
		dag_destroy(left);
	}

	return valid;
}

/* array_subscript -> factor array_subscript' */
/* array_subscript' -> [expr] array_subscript' | e */
/* dynamic-output ps{} root root{} */
bool array_subscript(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	int num;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	
	/* allocate ps{} n n{} */
	valid = factor(ps, &a) && valid;


	while (true) {
		/* allocate ps{} */
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		if (!t0 || t0->type != token_left_square_bracket) {
			break;
		}

		/* allocate ps{} lsb lsb{} */
		struct token* lsb = NULL;
		valid = match(ps, token_left_square_bracket, "expecting array subscript operator", &lsb) && valid;

		/* destroy lsb lsb{} */
		token_destroy(lsb);
		free(lsb);

		/* allocate b b{} */
		struct dag_node* b = NULL;
		valid = expr(ps, &b) && valid;

		/* allocate ps{} rsb rsb{} */
		struct token* rsb = NULL;
		valid = match(ps, token_right_square_bracket, "expecting array subscript operator", &rsb) && valid;

		/* destroy rsb rsb{} */
		token_destroy(rsb);
		free(rsb);

		if (valid) {
			if (!n) {
				dag_create_node(&n);
				n->type = dag_type_array_subscript;
				dag_add_child(n, a);
			}
			dag_add_child(n, b);
		} else {
			dag_destroy(b);
		}
	}

	if (!n && a) {
		n = a;
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}
	return valid;
}
