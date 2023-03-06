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
#include "type_def.h"
#include <assert.h>

bool assignment(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool boolean(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool comparison(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool add(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool mult(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool power(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool subscript(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool function_call(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool cseq(struct parse_state* ps, struct ast_node* tu, struct ast_node** root, struct location* loc);
bool dot_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);

/* expr -> id = expr | boolean */
/* dynamic-output ps{} root root{} */
bool expr(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	valid = assignment(ps, &n, loc) && valid;

	valid = location_default(ps, loc) && valid;

	/* transfer n n{} -> root */
	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

/* assignment -> boolean = assignment | boolean */
bool assignment(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	struct ast_node* last_p = NULL;
	struct ast_node* last_a = NULL;
	struct location loc_last_a;
	location_init(&loc_last_a);

	struct ast_node* a = NULL;
	struct location loc_a;

	while (true) {
		bool done = false;

		a = NULL;
		valid = boolean(ps, &a, &loc_a) && valid;
		location_update(loc, &loc_a);

		if (!a) {
			if (last_p) {
				valid = set_source_error(ps->el, &loc_a, "expected an assignment term");
				/* test case: test_parse_assign_error_term */
				break;
			} else {
				break;
			}
		}

		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);
		struct ast_node* p = NULL;
		if (t0 && t0->type == token_equal) {
			struct token* equal = NULL;
			valid = match(ps, token_equal, "expecting assign operator", &equal) && valid;
			location_update_token(loc, equal);
			/* test case: no test case needed */

			token_destroy(equal);
			free(equal);

			if (valid) {
				assert(a);

				ast_node_create(&p);
				p->type = ast_type_assign;
				ast_node_add(p, a);

				if (last_p) {
					ast_node_add(last_p, p);
				} else {
					n = p;
				}
			}
		} else {
			done = true;
			if (last_p) {
				ast_node_add(last_p, a);
			} else {
				n = a;
			}
		}

		if (valid) {
			assert(a);
			if (last_p && last_a) {
				struct ast_node* last_a_tu = last_a->tu;
				if (last_a->type == ast_type_var) {
					struct ast_node* dec = ast_node_get(last_a, 0);
					last_a_tu = ast_node_get(dec, 1);
				}

				struct ast_node* a_tu = a->tu;

				if (!last_a_tu) {
					valid = set_source_error(ps->el, &loc_last_a, "cannot assign with operand that has no value");
					/* test case: test_parse_assign_error_no_value_left */
				}
				if (!a_tu) {
					valid = set_source_error(ps->el, &loc_a, "cannot assign with operand that has no value");
					/* test case: test_parse_assign_error_no_value_right */
				}
				if (valid) {
					if (!type_use_can_cast(last_a_tu, a_tu)) {
						valid = set_source_error(ps->el, &loc_a, "values in assignment not compatible");
						/* test case: test_parse_assign_error_not_compatible */
					}
				}
				if (valid) {
					if (last_a->type != ast_type_id && last_a->type != ast_type_array_subscript && last_a->type != ast_type_var) {
						valid = set_source_error(ps->el, &loc_last_a, "not a valid lvalue");
						/* test case: test_parse_assign_error_lvalue*/
					}
				}
			}

			if (valid) {
				if (p && a->tu) {
					p->tu = ast_node_copy(a->tu);
				}
			}

			last_p = p;
			last_a = a;
			loc_last_a = loc_a;
		}

		if (done) {
			break;
		}
	}

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

/* boolean -> comparison boolean' */
/* boolean' -> && comparison boolean' | || comparison boolean' | e */
/* dynamic-output ps{} root root{} */
bool boolean(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* left;

	location_init(loc);

	/* allocate a a{} */
	struct ast_node* a = NULL;
	struct location loc_a;
	valid = comparison(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	if (!a) {
		valid = location_default(ps, loc) && valid;
		return valid;
	}

	left = a;
	struct location loc_left = loc_a;
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
		valid = match(ps, t0->type, "expecting && or ||", &op) && valid;
		location_update_token(loc, op);
		/* test case: no test cases needed */

        valid = consume_newline(ps) && valid;

		/* comparison */
		/* allocate a a{} */
		struct ast_node* b = NULL;
		struct location loc_b;
		valid = comparison(ps, &b, &loc_b) && valid;
		location_update(loc, &loc_b);

		if (!b) {
			valid = set_source_error(ps->el, &loc_b, "expected term after && or ||");
			/* test case: test_parse_boolean_error_expected_term */
		}

		if (valid) {
			ast_node_add(n, left);
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
			break;
		}

		if (valid) {
			assert(b);
			if (!left->tu) {
				valid = set_source_error(ps->el, &loc_left, "left-side operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_left_no_value */
			} else if (!b->tu) {
				valid = set_source_error(ps->el, &loc_b, "operand of boolean operator has no type");
				/* test case: test_parse_boolean_error_right_no_value */
			} else if (left->tu->td->type != type_boolean) {
				valid = set_source_error(ps->el, &loc_left, "left-side expression of boolean operator is not boolean");
			} else if (b->tu->td->type != type_boolean) {
				valid = set_source_error(ps->el, &loc_b, "expression of boolean operator is not boolean");
			} else {
				n->tu = ast_node_copy(left->tu);
			}

			left = n;
		}

		#pragma warning(suppress:6001)
		loc_left = op->loc;

		/* destroy op op{} */
		token_destroy(op);
		free(op);
	}

	if (valid) {
		*root = left;
	} else {
		ast_node_destroy(left);
	}

	valid = location_default(ps, loc) && valid;

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
bool comparison(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;
	struct ast_node* left = NULL;
	int num;

	location_init(loc);

	/* allocate ps{} a a{} */
	struct location loc_a;
	valid = add(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	if (!a) {
		valid = location_default(ps, loc) && valid;
		return valid;
	}

	if (valid) {
		left = a;
	} else {
		ast_node_destroy(a);
	}

	struct location loc_left = loc_a;

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
		location_update_token(loc, op);
		/* test case: no test case needed */

        valid = consume_newline(ps) && valid;

		/* add */
		/* allocate ps{} a a{} */
		struct ast_node* b = NULL;
		struct location loc_b;
		valid = add(ps, &b, &loc_b) && valid;
		location_update(loc, &loc_b);

		if (!b) {
			valid = set_source_error(ps->el, &loc_b, "expected term after comparison operator");
			/* case case: test_parse_comparison_error_no_term */
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
			assert(op);
			if (!left->tu) {
				struct location loc;
				get_token_location(op, &loc);
				valid = set_source_error(ps->el, &loc_left, "operand has no value");
				/* test case: test_parse_comparison_error_left_not_numeric */
			} else if (!b->tu) {
				valid = set_source_error(ps->el, &loc_b, "operand has no value");
				/* test case: test_parse_comparison_error_right_no_value */
			} else {
				if (!is_identity_comparison(type) && !is_numeric(left->tu->td)) {
					valid = set_source_error(ps->el, &loc_left, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_left_not_numeric */
				} else if (!is_identity_comparison(type) && !is_numeric(b->tu->td)) {
					valid = set_source_error(ps->el, &loc_b, "comparison operand is not numeric");
					/* test case: test_parse_comparison_error_right_not_numeric */
				} else {
					struct ast_node* tu = ast_node_copy(left->tu);
					type_find_whole(ps->st, tu, b->tu);
					n->tu = tu;
				}
			}
			left = n;
			#pragma warning(suppress:6001)
			loc_left = op->loc;
		}

		/* destroy op op{} */
		token_destroy(op);
		free(op);
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
bool add(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;
	struct ast_node* n = NULL;
	struct ast_node* left = NULL;
	char* op_name;
	int num;

	location_init(loc);

	/* allocate a a{} */
	struct location loc_a;
	valid = mult(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	if (!a) {
		valid = location_default(ps, loc) && valid;
		return valid;
	}

	left = n = a;

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
		location_update_token(loc, op);
		/* test case: no test case needed */

        valid = consume_newline(ps) && valid;

		/* allocate a a{} */
		struct location loc_b;
		valid = mult(ps, &b, &loc_b) && valid;
		location_update(loc, &loc_b);

		if (!b) {
			valid = set_source_error(ps->el, &loc_b, "expected term after additive operator");
			/* test case: test_parse_add_error_expected_term */
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
			struct ast_node* tu_a = a->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_a) {
				valid = set_source_error(ps->el, &loc_a, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_left_no_value */
			} else if (!is_numeric(tu_a->td)) {
				valid = set_source_error(ps->el, &loc_a, "%s on non-numeric operand", op_name);
			}

			if (!tu_b) {
				valid = set_source_error(ps->el, &loc_b, "%s operand has no value", op_name);
				/* test case: test_parse_add_error_right_no_value */
			} else if (!is_numeric(tu_b->td)) {
				valid = set_source_error(ps->el, &loc_b, "%s on non-numeric operand", op_name);
				/* test case: test_parse_add_error_right_not_numeric */
			}

			if (valid) {
				struct ast_node* tu = ast_node_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					valid = set_source_error(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
				} else {
					n->tu = tu;
				}
			}

			left = n;
		}

		/* destroy op op{} */
		token_destroy(op);
		free(op);
	}

	valid = location_default(ps, loc) && valid;

	if (valid) {
		*root = left;
	} else {
		ast_node_destroy(left);
	}

	return valid;
}

/* mult -> array_subscript mult_prime */
/* dynamic-output ps{} root root{} */
bool mult(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;
	struct ast_node* left = NULL;
	struct ast_node* n = NULL;
	char* op_name;

	location_init(loc);

	/* allocate ps{} a a{} */
	struct location loc_a;
	valid = power(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	if (!a) {
		valid = location_default(ps, loc) && valid;
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
		location_update_token(loc, op);
		/* test case: test case not needed */

        valid = consume_newline(ps) && valid;

		/* factor */
		/* allocate ps{} a a{} */
		struct location loc_b;
		valid = power(ps, &b, &loc_b) && valid;
		location_update(loc, &loc_b);

		if (!b) {
			/* allocate ps{} */
			valid = set_source_error(ps->el, &loc_b, "expected term after operator");
			/* test case: test_parse_mult_error_expected_term */
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
			struct ast_node* tu_a = a->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_a) {
				valid = set_source_error(ps->el, &loc_a, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_left_no_value */
			} else if (!is_numeric(tu_a->td)) {
				valid = set_source_error(ps->el, &loc_a, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_left_not_numeric */
			}

			if (!tu_b) {
				valid = set_source_error(ps->el, &loc_b, "%s operand has no value", op_name);
				/* test case: test_parse_mult_error_right_no_value*/
			} else if (!is_numeric(tu_b->td)) {
				valid = set_source_error(ps->el, &loc_b, "%s on non-numeric operand", op_name);
				/* test case: test_parse_mult_error_right_not_numeric */
			}

			if (valid) {
				struct ast_node* tu = ast_node_copy(tu_a);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					valid = set_source_error(ps->el, &op->loc, "invalid types for %s", op_name);
					/* test case: no test case needed */
				} else {
					n->tu = tu;
				}
			}

			left = n;
		}

		token_destroy(op);
		free(op);

	}

	if (valid) {
		*root = left;
	} else {
		ast_node_destroy(left);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

/* power -> array_subscript power' | e */
/* power' -> ^ array_subscript power' | e */
bool power(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;
	struct ast_node* b = NULL;

	location_init(loc);

	struct location loc_a;
	valid = subscript(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	if (!a) {
		valid = location_default(ps, loc) && valid;
		return valid;
	}

	struct ast_node* left = n = a ;
	struct location loc_left = loc_a;

	while (true) {
		struct token* t0 = NULL;
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		t0 = get_token(&ps->lookahead, 0);
		if (!t0 || t0->type != token_caret) {
			break;
		}

		struct token* caret = NULL;
		valid = match(ps, token_caret, "exprected a caret", &caret) && valid;
		location_update_token(loc, caret);
		/* test case: no test case needed */

        valid = consume_newline(ps) && valid;

		struct ast_node* b = NULL;
		struct location loc_b;
		valid = subscript(ps, &b, &loc_b) && valid;
		location_update(loc, &loc_b);

		if (!b) {
			valid = set_source_error(ps->el, &loc_b, "expected term after caret");
			/* test case: test_parse_power_error_expected_term */
		}

		if (valid) {
			ast_node_create(&n);
			n->type = ast_type_power;
			ast_node_add(n, left);
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
		}

		if (valid) {
			assert(left);
			assert(b);
			struct ast_node* tu_left = left->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_left) {
				valid = set_source_error(ps->el, &loc_left, "power operand has no value");
				/* test case: test_parse_power_error_left_no_value */
			} else if (!is_numeric(tu_left->td)) {
				valid = set_source_error(ps->el, &loc_left, "power on non-numeric operand");
				/* test case: test_parse_power_error_left_not_numeric */
			}

			if (!tu_b) {
				valid = set_source_error(ps->el, &loc_b, "power operand has no value");
				/* test case: test_parse_power_error_right_no_value */
			} else if (!is_numeric(tu_b->td)) {
				valid = set_source_error(ps->el, &loc_b, "power on non-numeric operand");
				/* test case: test_parse_power_error_right_not_numeric */
			}

			if (valid) {
				struct ast_node* tu = ast_node_copy(tu_left);
				if (!type_find_whole(ps->st, tu, tu_b)) {
					valid = set_source_error(ps->el, &loc_b, "invalid power types");
					/* test case: no test case needed */
				} else {
					n->tu = tu;
				}
			}

			left = n;
			#pragma warning(suppress:6001)
			loc_left = caret->loc;
		}

		token_destroy(caret);
		free(caret);
	}

	valid = location_default(ps, loc) && valid;

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

/* array_subscript -> factor array_subscript' */
/* array_subscript' -> [expr] array_subscript' | e */
/* dynamic-output ps{} root root{} */
bool subscript(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	int num;
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;

	location_init(loc);

	/* allocate ps{} n n{} */
	struct location loc_factor;
	valid = function_call(ps, &a, &loc_factor) && valid;
	location_update(loc, &loc_factor);

	struct ast_node* tu = NULL;
	struct ast_node* element_tu = NULL;

	struct location loc_last;
	location_init(&loc_last);
	while (true) {
		/* allocate ps{} */
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		if (!t0 || t0->type != token_left_square_bracket) {
			break;
		}

		if (valid) {
			if (!tu) {
				tu = a->tu;
			} else {
				tu = ast_node_get(tu, 0);
			}

			if (!tu) {
				valid = set_source_error(ps->el, &loc_factor, "subscripting expression with no type");
				/* test case: test_parse_subscript_error_no_type */
			} else if (tu->td->type != type_array) {
				valid = set_source_error(ps->el, &loc_factor, "subscripting expression that is not an array");
				/* test case: test_parse_subscript_error_not_array */
			}
		}

		/* allocate ps{} lsb lsb{} */
		struct token* lsb = NULL;
		valid = match(ps, token_left_square_bracket, "expecting array subscript operator", &lsb) && valid;
		location_update_token(loc, lsb);
		/* test case: no test cases needed */
		if (lsb) {
		#pragma warning(suppress:6001)
			loc_last = lsb->loc;
		}

		/* destroy lsb lsb{} */
		token_destroy(lsb);
		free(lsb);

        valid = consume_newline(ps) && valid;

		/* allocate b b{} */
		struct ast_node* b = NULL;
		struct location loc_expr;
		valid = expr(ps, &b, &loc_expr) && valid;
		location_update(loc, &loc_expr);

        valid = consume_newline(ps) && valid;

        /* allocate ps{} rsb rsb{} */
		struct token* rsb = NULL;
		valid = match(ps, token_right_square_bracket, "expected right-square-bracket", &rsb) && valid;
		location_update_token(loc, rsb);

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

	if (valid) {
		if (tu) {
			element_tu = ast_node_get(tu, 0);
		}

		if (n) {
			if (!element_tu) {
				valid = set_source_error(ps->el, &loc_last, "subscripting expression with no subtype");
				/* test case: test_parse_subscript_error_no_subtype */
			} else {
				n->tu = ast_node_copy(element_tu);
			}
		} else {
			if (a) {
				n = a;
			}
		}
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

/* function_call -> dot_nt function_call' */
/* function_call' -> ( cseq ) function_call' */
bool function_call(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	struct ast_node* dot_node = NULL;
	struct location dot_loc;
	valid = dot_nt(ps, &dot_node, &dot_loc);
	location_update(loc, &dot_loc);

	if (!dot_node) {
		valid = location_default(ps, loc) && valid;
		return valid;
	}

	struct ast_node* left = n = dot_node;
	struct location left_loc = dot_loc;

	while (true) {
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		if (!t0 || t0->type != token_left_paren) {
			break;
		}

		/* allocate ps{} lp lp{} */
		struct token* lp = NULL;
		valid = match(ps, token_left_paren, "expected left parenthesis", &lp) && valid;
		location_update_token(loc, lp);
		/* test case: test case not needed */

        valid = consume_newline(ps) && valid;

		/* allocate b b{} */
		struct ast_node* cseq_node = NULL;
		struct location loc_cseq;
		valid = cseq(ps, left->tu, &cseq_node, &loc_cseq) && valid;
		location_update(loc, &loc_cseq);

        valid = consume_newline(ps) && valid;

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		valid = match(ps, token_right_paren, "expected right parenthesis", &rp) && valid;
		location_update_token(loc, rp);
		/* test case: test_parse_call_error_right_paren */


		if (valid) {
			/* allocate n */
			ast_node_create(&n);
			n->type = ast_type_call;
			ast_node_add(n, left);
			ast_node_add(n, cseq_node);

		} else {
			/* destroy cseq_node cseq_node{} */
			ast_node_destroy(cseq_node);
		}

		if (valid) {
			struct ast_node* tu = left->tu;
			assert(tu);
			assert(tu->td);
			struct type_def* td = tu->td;
			if (td->type != type_function) {
				valid = set_source_error(ps->el, &left_loc, "not a function type");
				/* test case: test_parse_call_error_not_function */
			} else {
				struct ast_node* input = NULL;
				struct ast_node* output = NULL;
				get_function_children(tu, &input, &output);

				/* input */
				int tcount = 0;
				if (input) {
					tcount = ast_node_count_children(input);
				}
				int ccount = 0;
				if (cseq_node) {
					ccount = ast_node_count_children(cseq_node);
				}

				if (ccount < tcount) {
					valid = set_source_error(ps->el, &rp->loc, "not enough arguments in function call");
					/* test case: test_parse_call_error_not_enough_arguments */
				} else if (ccount > tcount) {
					valid = set_source_error(ps->el, &rp->loc, "too many arguments in function call");
					/* test case: test_parse_call_error_too_many_arguments */
				}

				/* output */
				if (output) {
					n->tu = ast_node_copy(ast_node_get(output, 0));
				}
			}

			left = n;
			#pragma warning(suppress:6001)
			left_loc = rp->loc;
		}

		/* destroy id id{} lp lp{} rp rp{} */
		token_destroy(lp);
		free(lp);
		token_destroy(rp);
		free(rp);

	}

	valid = location_default(ps, loc) && valid;

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

/* cseq -> expr cseq' | e */
/* cseq' -> , expr cseq' | e */
/* dynamic-output ps{} root root{} */
bool cseq(struct parse_state* ps, struct ast_node* tu, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	if (!tu || !tu->td || tu->td->type != type_function) {
		valid = location_default(ps, loc) && valid;
		valid = set_source_error(ps->el, loc, "not a function type");
		/* test case: no test case needed */
		return valid;
	}

	/* allocate n */
	ast_node_create(&n);
	n->type = ast_type_cseq;

	*root = n;

	/* allocate a a{} */
	struct ast_node* a = NULL;
	struct location loc_expr;
	valid = expr(ps, &a, &loc_expr) && valid;
	location_update(loc, &loc_expr);

	if (!a) {
		valid = location_default(ps, loc) && valid;
		return valid;
	}
	int i = 0;
	valid = check_input_type(ps, tu, i, a, &loc_expr) && valid;
	i++;

	/* transfer a -> n{} */
	ast_node_add(n, a);

	while (true) {
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		struct token* t0 = get_token(&ps->lookahead, 0);

		if (!t0 || t0->type != token_comma) {
			break;
		}

		/* allocate ps{} comma comma{} */
		struct token* comma = NULL;;
		valid = match(ps, token_comma, "expecting comma", &comma) && valid;
		location_update_token(loc, comma);
		/* test case: no test case needed */

		/* destroy comma comma{} */
		token_destroy(comma);
		free(comma);

        valid = consume_newline(ps) && valid;

		/* allocate a a{} */
		struct ast_node* a = NULL;
		valid = expr(ps, &a, &loc_expr) && valid;
		location_update(loc, &loc_expr);

		if (!a) {
			set_source_error(ps->el, &loc_expr, "expected expression after comma");
			/* test case: test_parse_call_error_expected_expression */
			valid = false;
		} else {
			/* transfer a -> parent */
			ast_node_add(n, a);

			valid = check_input_type(ps, tu, i, a, &loc_expr) && valid;
		}

		i++;
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool dot_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* a = NULL;

	location_init(loc);

	struct location loc_a;
	valid = factor(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	if (!a) {
		valid = location_default(ps, loc) && valid;
		return valid;
	}

	struct ast_node* left = n = a;
	struct location loc_left = loc_a;
	if (ps->qualifier.size > 0) buffer_add_char(&ps->qualifier, '.');
	buffer_copy(&a->value, &ps->qualifier);
	while (true) {
		struct token* t0 = NULL;
		int num;
		valid = get_lookahead(ps, 1, &num) && valid;
		t0 = get_token(&ps->lookahead, 0);
		if (!t0 || t0->type != token_dot) {
			break;
		}

		struct token* dot = NULL;
		valid = match(ps, token_dot, "expected a dot", &dot) && valid;
		location_update_token(loc, dot);
		/* test case: no test case needed */

        valid = consume_newline(ps) && valid;

		struct ast_node* b = NULL;
		struct location loc_b;
		valid = factor(ps, &b, &loc_b) && valid;
		location_update(loc, &loc_b);

		if (!b) {
			valid = set_source_error(ps->el, &loc_b, "expected term after dot");
			/* test case: test_parse_dot_error_expected_term */
		}

		if (valid) {
			assert(b);
			ast_node_create(&n);
			n->type = ast_type_dot;
			if (left->type == ast_type_dot) {
				buffer_copy(&left->value, &n->value);
				buffer_add_char(&n->value, '.');
				buffer_copy(&b->value, &n->value);
			} else if (left->type == ast_type_id) {
				buffer_copy(&left->value, &n->value);
				buffer_add_char(&n->value, '.');
				buffer_copy(&b->value, &n->value);
			}

			ast_node_add(n, left);
			ast_node_add(n, b);
		} else {
			ast_node_destroy(b);
		}

		if (valid) {
			assert(left);
			assert(b);
			struct ast_node* tu_left = left->tu;
			struct ast_node* tu_b = b->tu;

			if (!tu_left) {
				valid = set_source_error(ps->el, &loc_left, "dot operand has no value");
				/* test case: no test case necessary */
			} else if (tu_left->td->type != type_module && tu_left->td->type != type_struct) {
				valid = set_source_error(ps->el, &loc_left, "dot operand is not a module or struct");
				/* test case: test_parse_dot_error_left_non_module */
			}

			if (!tu_b) {
				valid = set_source_error(ps->el, &loc_b, "dot operand has no value");
				/* test case: test_parse_dot_error_non_module */
			}

			if (left == a) {
				if (left->type != ast_type_id) {
					valid = set_source_error(ps->el, &loc_left, "operand of dot operator not an identifier");
					/* test case: no test case necessary */
				}
			}

			if (b->type != ast_type_id) {
				valid = set_source_error(ps->el, &loc_b, "operand of dot operator not an identifier");
				/* test case: test_parse_dot_error_right_not_identifier */
			}

			if (valid) {
				n->tu = ast_node_copy(tu_b);
			}

			left = n;
			#pragma warning(suppress:6001)
			loc_left = dot->loc;
			if (ps->qualifier.size > 0) buffer_add_char(&ps->qualifier, '.');
			buffer_copy(&b->value, &ps->qualifier);
		}

		token_destroy(dot);
		free(dot);
	}

	valid = location_default(ps, loc) && valid;
	buffer_clear(&ps->qualifier);

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}
