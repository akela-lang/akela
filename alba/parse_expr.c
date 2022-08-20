#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_tools.h"
#include "parse_factor.h"
#include "source.h"
#include "scan.h"
#include "parse_types.h"
#include "parse_expr.h"
#include "type_use.h"
#include <assert.h>

bool var(struct parse_state* ps, struct ast_node** root);
bool assignment(struct parse_state* ps, struct ast_node** root);
bool boolean(struct parse_state* ps, struct ast_node** root);
bool comparison(struct parse_state* ps, struct ast_node** root);
bool add(struct parse_state* ps, struct ast_node** root);
bool mult(struct parse_state* ps, struct ast_node** root);
bool array_subscript(struct parse_state* ps, struct ast_node** root);

/* expr -> id = expr | boolean */
/* dynamic-output ps{} root root{} */
bool expr(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
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

bool var(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	struct token* vrt = NULL;
	valid = match(ps, token_var, "expected var", &vrt) && valid;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} id id{} */
	struct ast_node* a = NULL;
	valid = declaration(ps, &a) && valid;
	if (!a) {
		valid = set_source_error(ps->el, &loc, "expected declaration after var");
	}

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);

	struct token* equal = NULL;
	struct ast_node* b = NULL;
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
		ast_node_create(&n);
		n->type = ast_type_var;

		ast_node_add(n, a);
		if (b) {
			ast_node_add(n, b);
		}

		*root = n;
	} else {
		ast_node_destroy(a);
		ast_node_destroy(b);
	}

	token_destroy(vrt);
	free(vrt);
	token_destroy(equal);
	free(equal);

	return valid;
}

bool assignment(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expected identifier", &id) && valid;

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	valid = match(ps, token_equal, "expected equal", &equal) && valid;

	/* allocate b b{} */
	struct ast_node* b = NULL;
	valid = expr(ps, &b) && valid;

	if (!b) {
		valid = set_source_error(ps->sns->el, &loc, "expect expression on rhs of assignment operator");
	}

	if (valid) {
		/* allocate n */
		ast_node_create(&n);
		n->type = ast_type_assign;

		/* allocate a */
		struct ast_node* a;
		ast_node_create(&a);
		a->type = ast_type_id;

		/* allocate a{} */
		#pragma warning (suppress:6011)
		buffer_copy(&id->value, &a->value);

		/* transfer a a{} -> n{} */
		ast_node_add(n, a);

		/* transfer b b{} -> n{} */
		ast_node_add(n, b);

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
bool boolean(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* left;

	/* allocate a a{} */
	struct ast_node* a = NULL;
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
		enum ast_type type = ast_type_none;
		if (t0 && t0->type == token_and) {
			type = ast_type_and;
		} else if (t0 && t0->type == token_or) {
			type = ast_type_or;
		} else {
			break;
		}

		/* allocate n */
		ast_node_create(&n);
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
		struct ast_node* b = NULL;
		valid = comparison(ps, &b) && valid;

		if (!b) {
			valid = set_source_error(ps->el, &loc, "expected term after + or -");
		}

		if (valid) {
			ast_node_add(n, left);
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
			break;
		}

		if (valid) {
			if (!left->tu) {
				struct location loc;
				valid = set_source_error(ps->el, &loc, "left-side operand of boolean operator has no type");
			} else if (!b->tu) {
				valid = set_source_error(ps->el, &loc, "operand of boolean operator has no type");
			} else if (left->tu->td->type != type_boolean) {
				get_token_location(op, &loc);
				valid = set_source_error(ps->el, &loc, "left-side expression of boolean operator is not boolean");
			} else if (b->tu->td->type != type_boolean) {
				valid = set_source_error(ps->el, &loc, "expression of boolean operator is not boolean");
			} else {
				n->tu = type_use_copy(left->tu);
			}

			left = n;
		}

	}

	if (valid) {
		*root = left;
	} else {
		ast_node_destroy(left);
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
bool comparison(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;
	struct ast_node* left = NULL;
	int num;

	/* allocate ps{} a a{} */
	valid = add(ps, &a) && valid;

	if (!a) {
		return valid;
	}

	if (valid) {
		left = a;
	} else {
		ast_node_destroy(a);
	}

	while (true) {
		/* allocate ps{} */
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);
		enum ast_type type = ast_type_none;

		if (!t0) {
			break;
		}

		if (t0->type == token_double_equal) {
			type = ast_type_equality;
		} else if (t0->type == token_not_equal) {
			type = ast_type_not_equal;
		} else if (t0->type == token_less_than) {
			type = ast_type_less_than;
		} else if (t0->type == token_less_than_or_equal) {
			type = ast_type_less_than_or_equal;
		} else if (t0->type == token_greater_than) {
			type = ast_type_greater_than;
		} else if (t0->type == token_greater_than_or_equal) {
			type = ast_type_greater_than_or_equal;
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
		struct ast_node* b = NULL;
		valid = add(ps, &b) && valid;

		if (!b) {
			valid = set_source_error(ps->el, &loc, "expected term after compare operator");
		}

		if (valid) {
			/* allocate n */
			ast_node_create(&n);
			n->type = type;

			ast_node_add(n, left);
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
		}

		if (valid) {
			assert(a);
			assert(b);
			if (!left->tu) {
				struct location loc;
				get_token_location(op, &loc);
				valid = set_source_error(ps->el, &loc, "operand has no value");
			} else if (!b->tu) {
				valid = set_source_error(ps->el, &loc, "operand has no value");
			} else {
				if (!is_identity_comparison(type) && !is_numeric(left->tu->td)) {
					struct location loc;
					get_token_location(op, &loc);
					valid = set_source_error(ps->el, &loc, "comparison operand is not numeric");
				} else if (!is_identity_comparison(type) && !is_numeric(b->tu->td)) {
					valid = set_source_error(ps->el, &loc, "comparison operand is not numeric");
				} else {
					struct type_use* tu = type_use_copy(left->tu);
					type_find_whole(ps->st, tu, b->tu);
					n->tu = tu;
				}
			}
			left = n;
		}
	}

	if (valid) {
		*root = left;
	} else {
		ast_node_destroy(left);
	}

	return valid;
}

/* add -> mult add' */
/* add' -> + mult add' | - mult add' | e */
/* dynamic-output ps{} root root{} */
bool add(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;
	struct ast_node* n = NULL;
	struct ast_node* left = NULL;
	char* op_name;
	int num;

	struct location loc_a;
	valid = get_parse_location(ps, &loc_a) && valid;

	/* allocate a a{} */
	valid = mult(ps, &a) && valid;

	if (!a) {
		return valid;
	}

	if (valid) {
		left = a;
	} else {
		ast_node_destroy(a);
	}

	struct location loc_b;
	bool first = true;
	while (true) {
		/* allocate ps{} */
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		/* e */
		if (t0 == NULL) {
			break;
		}

		/* operator */
		enum ast_type type;
		if (t0->type == token_plus) {
			type = ast_type_plus;
			op_name = "addition";
		} else if (t0->type == token_minus) {
			type = ast_type_minus;
			op_name = "subtraction";
		} else {
			break;
		}

		/* allocate ps{} */
		struct token* op = NULL;
		valid = match(ps, t0->type, "expecting + or -", &op) && valid;

		struct location loc_op;
		get_token_location(op, &loc_op);

		/* destroy op op{} */
		token_destroy(op);
		free(op);

		valid = get_parse_location(ps, &loc_b) && valid;

		/* allocate a a{} */
		valid = mult(ps, &b) && valid;

		if (!b) {
			/* destroy n n{} */
			ast_node_destroy(n);
			valid = set_source_error(ps->el, &loc_b, "expected term after additive operator");
			break;
		}

		if (valid) {
			ast_node_create(&n);
			n->type = type;

			ast_node_add(n, left);
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
		}

		if (valid) {
			struct type_use* tu_a = a->tu;
			struct type_use* tu_b = b->tu;

			if (!tu_a) {
				valid = set_source_error(ps->el, &loc_a, "%s operand has no value", op_name);
			} else if (!is_numeric(tu_a->td)) {
				valid = set_source_error(ps->el, &loc_a, "%s on non-numeric operand");
			}

			if (!tu_b) {
				valid = set_source_error(ps->el, &loc_b, "%s operand has no value", op_name);
			} else if (!is_numeric(tu_b->td)) {
				valid = set_source_error(ps->el, &loc_b, "%s on non-numeric operand");
			}

			if (valid) {
				struct type_use* tu = type_use_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					struct location loc;
					get_token_location(op, &loc);
					valid = set_source_error(ps->el, &loc, "invalid types for %s", op_name);
				} else {
					n->tu = tu;
				}
			}

			left = n;
		}
	}

	if (valid) {
		*root = left;
	} else {
		ast_node_destroy(left);
	}

	return valid;
}

/* mult -> array_subscript mult_prime */
/* dynamic-output ps{} root root{} */
bool mult(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;
	struct ast_node* left = NULL;
	struct ast_node* n = NULL;
	char* op_name;

	struct location loc_a;
	valid = get_parse_location(ps, &loc_a);

	/* allocate ps{} a a{} */
	valid = array_subscript(ps, &a) && valid;

	if (!a) {
		return valid;
	}

	if (valid) {
		left = a;
	} else {
		ast_node_destroy(a);
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
		enum ast_type type;
		if (t0->type == token_mult) {
			type = ast_type_mult;
			op_name = "multiplication";
		} else if (t0->type == token_divide) {
			type = ast_type_divide;
			op_name = "division";
		} else {
			break;
		}

		/* allocate ps{} t t{} */
		struct token* op = NULL;
		valid = match(ps, t0->type, "expecting * or /", &op) && valid;

		struct location loc_op;
		get_token_location(op, &loc_op);

		token_destroy(op);
		free(op);

		struct location loc_b;
		valid = get_parse_location(ps, &loc_b) && valid;

		/* factor */
		/* allocate ps{} a a{} */
		valid = array_subscript(ps, &b) && valid;

		if (!b) {
			/* allocate ps{} */
			valid = set_source_error(ps->el, &loc_b, "expected term after operator");
		}

		if (valid) {
			/* allocate n */
			ast_node_create(&n);
			n->type = type;

			ast_node_add(n, left);
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
		}

		if (valid) {
			assert(a);
			assert(b);
			struct type_use* tu_a = a->tu;
			struct type_use* tu_b = b->tu;

			if (!tu_a) {
				valid = set_source_error(ps->el, &loc_a, "%s operand has no value", op_name);
			} else if (!is_numeric(tu_a->td)) {
				valid = set_source_error(ps->el, &loc_a, "%s on non-numeric operand");
			}

			if (!tu_b) {
				valid = set_source_error(ps->el, &loc_b, "%s operand has no value", op_name);
			} else if (!is_numeric(tu_b->td)) {
				valid = set_source_error(ps->el, &loc_b, "%s on non-numeric operand");
			}

			if (valid) {
				struct type_use* tu = type_use_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					struct location loc;
					get_token_location(op, &loc);
					valid = set_source_error(ps->el, &loc, "invalid types for %s", op_name);
				} else {
					n->tu = tu;
				}
			}

			left = n;
		}
	}

	if (valid) {
		*root = left;
	} else {
		ast_node_destroy(left);
	}

	return valid;
}

/* array_subscript -> factor array_subscript' */
/* array_subscript' -> [expr] array_subscript' | e */
/* dynamic-output ps{} root root{} */
bool array_subscript(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	int num;
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;
	
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
		struct ast_node* b = NULL;
		valid = expr(ps, &b) && valid;

		/* allocate ps{} rsb rsb{} */
		struct token* rsb = NULL;
		valid = match(ps, token_right_square_bracket, "expecting array subscript operator", &rsb) && valid;

		/* destroy rsb rsb{} */
		token_destroy(rsb);
		free(rsb);

		if (valid) {
			if (!n) {
				ast_node_create(&n);
				n->type = ast_type_array_subscript;
				ast_node_add(n, a);
			}
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
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
