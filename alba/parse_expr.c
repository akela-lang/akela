#include "result.h"
#include "token.h"
#include "dag.h"
#include "allocator.h"
#include "parse_tools.h"

/*
* expr -> not_operator
*/
enum result expr(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;

	r = get_lookahead(al, ts, 2, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0 = get_token(&ts->lookahead, 0);
	struct token* t1 = get_token(&ts->lookahead, 1);

	/* id = expr */
	if (t0 && t0->type == token_id && t1 && t1->type == token_equal) {
		r = match(al, ts, token_id, "expected word");
		if (r == result_error) {
			goto function_error;
		}
		r = match(al, ts, token_equal, "expected equal");
		if (r == result_error) {
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_assign;

		struct dag_node* a;
		r = dag_create_node(al, &a);
		if (r == result_error) {
			goto function_error;
		}
		a->type = dag_type_id;
		r = buffer_copy(al, &t0->value, &a->value);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, a);

		struct dag_node* b;
		r = expr(al, ts, &b);
		if (r == result_error) {
			goto function_error;
		}
		if (b == NULL) {
			r = set_error("expect expression");
			goto function_error;
		}
		dag_add_child(n, b);
		goto function_success;
	}

	r = boolean(al, ts, &n);
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

enum result boolean(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct dag_node* n = NULL;

	r = comparison(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	r = boolean_prime(al, ts, &b, &c);
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
enum result boolean_prime(struct allocator* al, struct token_state* ts, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ts->lookahead, 0);

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

	r = dag_create_node(al, &n);
	if (r == result_error) {
		return r;
	}

	n->type = type;
	r = match(al, ts, t0->type, "expecting + or -");
	if (r == result_error) {
		return r;
	}

	/* comparison */
	r = comparison(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	/* boolean' */
	r = boolean_prime(al, ts, &b, &c);
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
enum result comparison(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct dag_node* n = NULL;

	r = add(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	r = comparison_prime(al, ts, &b, &c);
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
enum result comparison_prime(struct allocator* al, struct token_state* ts, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ts->lookahead, 0);

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

	r = dag_create_node(al, &n);
	if (r == result_error) {
		return r;
	}

	n->type = type;
	r = match(al, ts, t0->type, "expecting + or -");
	if (r == result_error) {
		return r;
	}

	/* add */
	r = add(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	/* comparison' */
	r = comparison_prime(al, ts, &b, &c);
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
enum result add(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	struct dag_node* n = NULL;

	r = mult(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	r = add_prime(al, ts, &b, &c);
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
* add' -> + mult add'
*	    | - mult add'
*	    | e
*/
enum result add_prime(struct allocator* al, struct token_state* ts, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;
	int num;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ts->lookahead, 0);

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

	r = dag_create_node(al, &n);
	if (r == result_error) {
		return r;
	}

	n->type = type;
	r = match(al, ts, t0->type, "expecting + or -");
	if (r == result_error) {
		return r;
	}

	/* term */
	r = mult(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	/* add' */
	r = add_prime(al, ts, &b, &c);
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
* mult -> factor mult_prime
*/
enum result mult(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;

	r = factor(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	r = mult_prime(al, ts, &b, &c);
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
* term' -> * factor mult'
*	     | / factor mult'
*	     | e
*/
enum result mult_prime(struct allocator* al, struct token_state* ts, struct dag_node** root, struct dag_node** insert_point)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* c = NULL;
	enum dag_type type = dag_type_none;

	int num;
	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0 = get_token(&ts->lookahead, 0);

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

	if (type == dag_type_none) {
		r = set_error("expecting expr'");
		return r;
	}

	r = dag_create_node(al, &n);
	if (r == result_error) {
		return r;
	}

	n->type = type;
	r = match(al, ts, t0->type, "expecting * or /");
	if (r == result_error) {
		return r;
	}

	/* factor */
	r = factor(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	/* mult' */
	r = mult_prime(al, ts, &b, &c);
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
* factor -> id(cseq)
*		  | number
*		  | id
*		  | + factor
*		  | - factor
*		  | (expr)
*		  | ! factor
* note: type system should catch incompatible sign or not factors
*/
enum result factor(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;

	int num;
	r = get_lookahead(al, ts, 2, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0;
	struct token* t1;
	t0 = get_token(&ts->lookahead, 0);
	t1 = get_token(&ts->lookahead, 1);

	/* function call*/
	if (t0 && t0->type == token_id && t1 && t1->type == token_left_paren) {
		r = match(al, ts, token_id, "expecting id");
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, token_left_paren, "expecting left parenthesis");
		if (r == result_error) {
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_call;

		struct dag_node* a = NULL;
		r = dag_create_node(al, &a);
		if (r == result_error) {
			goto function_error;
		}
		a->type = dag_type_id;
		buffer_copy(al, &t0->value, &a->value);
		dag_add_child(n, a);

		struct dag_node* b = NULL;
		r = cseq(al, ts, &b);
		if (r == result_error) {
			goto function_error;
		}
		if (b) {
			dag_add_child(n, b);
		}

		r = match(al, ts, token_right_paren, "expecting right parenthesis");
		if (r == result_error) {
			goto function_error;
		}

		/* ! factor */
	} else if (t0 && t0->type == token_not) {
		r = match(al, ts, token_not, "expecting not");
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* a = NULL;
		r = factor(al, ts, &a);

		if (!a) {
			r = set_error("expected factor");
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}

		n->type = dag_type_not;
		dag_add_child(n, a);
		goto function_success;

		/* number or word */
	} else if (t0 && (t0->type == token_number || t0->type == token_id)) {
		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}

		if (t0->type == token_number) {
			n->type = dag_type_number;
		} else if (t0->type == token_id) {
			n->type = dag_type_id;
		}
		r = buffer_copy(al, &t0->value, &n->value);
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, t0->type, "expecting number or word");
		if (r == result_error) {
			goto function_error;
		}
	}

	/* sign and number or word */
	else if (t0 && (t0->type == token_plus || t0->type == token_minus) && t1 && (t1->type == token_number || t1->type == token_id)) {
		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}

		n->type = dag_type_sign;

		struct dag_node* left;
		r = dag_create_node(al, &left);
		if (r == result_error) {
			goto function_error;
		}

		if (t0->type == token_plus) {
			left->type = dag_type_plus;
		} else {
			left->type = dag_type_minus;
		}

		r = match(al, ts, t0->type, "expecting unary plus or minus");
		if (r == result_error) {
			goto function_error;
		}

		dag_add_child(n, left);

		struct dag_node* right = NULL;
		r = factor(al, ts, &right);
		if (r == result_error) {
			goto function_error;
		}

		if (!right) {
			r = set_error("expecting factor after sign");
			goto function_error;
		}

		dag_add_child(n, right);

		/* parenthesis */
	} else if (t0 && t0->type == token_left_paren) {
		r = match(al, ts, t0->type, "expecting left parenthesis");
		if (r == result_error) {
			goto function_error;
		}

		r = expr(al, ts, &n);
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, token_right_paren, "expecting right parenthesis");
		if (r == result_error) {
			goto function_error;
		}

		goto function_success;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* cseq -> factor cseq'
*		| e
*/
enum result cseq(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	struct dag_node* a = NULL;
	r = factor(al, ts, &a);
	if (r == result_error) {
		goto function_error;
	}

	if (a) {
		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_cseq;

		dag_add_child(n, a);

		r = cseq_prime(al, ts, n);
		if (r == result_error) {
			goto function_error;
		}
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* cseq' -> , factor cseq'
*		 | e
*/
enum result cseq_prime(struct allocator* al, struct token_state* ts, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ts->lookahead, 0);

	if (t0 && t0->type == token_comma) {
		r = match(al, ts, token_comma, "expecting comma");
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* a = NULL;
		r = factor(al, ts, &a);
		if (r == result_error) {
			goto function_error;
		}

		if (!a) {
			r = set_error("expected factor");
			goto function_error;
		}

		dag_add_child(parent, a);

		r = cseq_prime(al, ts, parent);
		if (r == result_error) {
			goto function_error;
		}
	}

function_success:
	return r;

function_error:
	return r;
}
