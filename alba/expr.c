#include "result.h"
#include "token.h"
#include "dag.h"
#include "allocator.h"
#include "parse_tools.h"

/*
* expr -> term expr'
*/
enum result expr(struct allocator* al, struct token_state* ts, struct dag_node** root, char** message)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	struct dag_node* n = NULL;

	r = term(al, ts, &a, message);
	if (r == result_error) {
		goto function_error;
	}

	r = expr_prime(al, ts, &b, message);
	if (r == result_error) {
		goto function_error;
	}

	if (a && !b) {
		n = a;
		goto function_success;
	}

	if (b && !a) {
		n = b;
		goto function_success;
	}

	if (a && b && is_binary_operator(b)) {
		n = b;
		dag_push(n, a);
		goto function_success;
	} else {
		goto function_error;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* expr' -> + term expr'
*	     | - term expr'
*	     | e
*/
enum result expr_prime(struct allocator* al, struct token_state* ts, struct dag_node** root, char** message)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	enum dag_type type = dag_type_none;
	int num;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0 = get_token(&ts->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		goto function_success;
	}

	/* operator */
	if (t0->type == token_plus) {
		type = dag_type_plus;
	} else if (t0->type == token_minus) {
		type = dag_type_minus;
	} else {
		/* e */
		goto function_success;
	}

	r = dag_create_node(al, &n);
	if (r == result_error) {
		goto function_error;
	}

	n->type = type;
	r = match(al, ts, t0->type, "expecting + or -");
	if (r == result_error) {
		goto function_error;
	}

	/* term */
	r = term(al, ts, &a, message);
	if (r == result_error) {
		goto function_error;
	}

	/* expr' */
	r = expr_prime(al, ts, &b, message);
	if (r == result_error) {
		goto function_error;
	}

	if (a == NULL && b == NULL) {
		r = set_error("expecting expr'");
		goto function_error;
	}

	if (b && is_binary_operator(b)) {
		dag_push(b, a);
		dag_add_child(n, b);
		goto function_success;
	}

	if (a) {
		dag_add_child(n, a);
	}

	if (b) {
		dag_add_child(n, b);
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* term -> factor term_prime
*/
enum result term(struct allocator* al, struct token_state* ts, struct dag_node** root, char** message)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;

	r = factor(al, ts, &a, message);
	if (r == result_error) {
		goto function_error;
	}

	r = term_prime(al, ts, &b, message);
	if (r == result_error) {
		goto function_error;
	}

	if (a && !b) {
		n = a;
		goto function_success;
	}

	if (b && !a) {
		n = b;
		goto function_success;
	}

	if (a && b && is_binary_operator(b)) {
		n = b;
		dag_push(n, a);
		goto function_success;
	}

	goto function_success;

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* term' -> * factor term'
*	     | / factor term'
*	     | e
*/
enum result term_prime(struct allocator* al, struct token_state* ts, struct dag_node** root, char** message)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct dag_node* a = NULL;
	struct dag_node* b = NULL;
	enum dag_type type = dag_type_none;

	int num;
	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0 = get_token(&ts->lookahead, 0);

	/* e */
	if (t0 == NULL) {
		goto function_success;
	}

	/* operator */
	if (t0->type == token_mult) {
		type = dag_type_mult;
	} else if (t0->type == token_divide) {
		type = dag_type_divide;
	} else {
		goto function_success;
	}

	if (type == dag_type_none) {
		r = set_error("expecting expr'");
		goto function_error;
	}

	r = dag_create_node(al, &n);
	if (r == result_error) {
		goto function_error;
	}

	n->type = type;
	r = match(al, ts, t0->type, "expecting * or /");
	if (r == result_error) {
		goto function_error;
	}

	/* factor */
	r = factor(al, ts, &a, message);
	if (r == result_error) {
		goto function_error;
	}

	/* term' */
	r = term_prime(al, ts, &b, message);
	if (r == result_error) {
		goto function_error;
	}

	if (a == NULL && b == NULL) {
		r = set_error("expecting expr'");
		goto function_error;
	}

	if (b && is_binary_operator(b)) {
		dag_push(b, a);
		dag_add_child(n, b);
		goto function_success;
	}

	if (a) {
		dag_add_child(n, a);
	}

	if (b) {
		dag_add_child(n, b);
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* factor -> number
*	| + number
*	| - number
*	| word
*	| + word
*	| - word
*	| (expr)
*	| word(cseq)
*/
enum result factor(struct allocator* al, struct token_state* ts, struct dag_node** root, char** message)
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
		r = cseq(al, ts, &b, message);
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

		struct dag_node* right;
		r = dag_create_node(al, &right);
		if (r == result_error) {
			goto function_error;
		}

		if (t1->type == token_number) {
			right->type = dag_type_number;
		} else {
			right->type = dag_type_id;
		}

		r = match(al, ts, t1->type, "expecting number or word");
		if (r == result_error) {
			goto function_error;
		}

		r = buffer_copy(al, &t1->value, &right->value);
		if (r == result_error) {
			goto function_error;
		}

		dag_add_child(n, right);

		/* parenthesis */
	} else if (t0 && t0->type == token_left_paren) {
		r = match(al, ts, t0->type, "expecting left parenthesis");
		if (r == result_error) {
			goto function_error;
		}

		r = expr(al, ts, &n, message);
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
enum result cseq(struct allocator* al, struct token_state* ts, struct dag_node** root, char** message)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	struct dag_node* a = NULL;
	r = factor(al, ts, &a, message);
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

		r = cseq_prime(al, ts, n, message);
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
enum result cseq_prime(struct allocator* al, struct token_state* ts, struct dag_node* parent, char** message)
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
		r = factor(al, ts, &a, message);
		if (r == result_error) {
			goto function_error;
		}

		if (!a) {
			r = set_error("expected factor");
			goto function_error;
		}

		dag_add_child(parent, a);

		r = cseq_prime(al, ts, parent, message);
		if (r == result_error) {
			goto function_error;
		}
	}

function_success:
	return r;

function_error:
	return r;
}
