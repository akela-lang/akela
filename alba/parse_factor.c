#include "result.h"
#include "allocator.h"
#include "parse_tools.h"
#include "dag.h"
#include "token.h"
#include "types.h"
#include "parse_stmts.h"

/*
* factor -> id(cseq)
*		  | number
*		  | string
*		  | id
*		  | + factor
*		  | - factor
*		  | (expr)
*		  | ! factor
*		  | array_literal
*         | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
enum result factor(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;
	struct token* t;

	int num;
	r = get_lookahead(al, ts, 2, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0;
	struct token* t1;
	t0 = get_token(&ts->lookahead, 0);
	t1 = get_token(&ts->lookahead, 1);

	/* anonymous function */
	if (t0 && t0->type == token_function && t1 && t1->type == token_left_paren) {
		r = match(al, ts, token_function, "expected anonymous function", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, token_left_paren, "expected left parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_anonymous_function;

		struct dag_node* a = NULL;
		r = dseq(al, ts, &a);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, a);

		r = match(al, ts, token_right_paren, "expected right parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* b = NULL;
		r = stmts(al, ts, &b);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, b);

		r = match(al, ts, token_end, "expected end", &t);
		if (r == result_error) {
			goto function_error;
		}

		goto function_success;

		/* function call*/
	} else if (t0 && t0->type == token_id && t1 && t1->type == token_left_paren) {
		r = match(al, ts, token_id, "expecting id", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, token_left_paren, "expecting left parenthesis", &t);
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

		r = match(al, ts, token_right_paren, "expecting right parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		/* ! factor */
	} else if (t0 && t0->type == token_not) {
		r = match(al, ts, token_not, "expecting not", &t);
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

		/* number, id, string */
	} else if (t0 && (t0->type == token_number || t0->type == token_id || t0->type == token_string)) {
		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}

		if (t0->type == token_number) {
			n->type = dag_type_number;
		} else if (t0->type == token_id) {
			n->type = dag_type_id;
		} else if (t0->type == token_string) {
			n->type = dag_type_string;
		}
		r = buffer_copy(al, &t0->value, &n->value);
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, t0->type, "expecting number, id, or string", &t);
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

		r = match(al, ts, t0->type, "expecting unary plus or minus", &t);
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

		/* array literal */
	} else if (t0 && t0->type == token_left_square_bracket) {
		r = array_literal(al, ts, &n);
		if (r == result_error) {
			return r;
		}

		goto function_success;

		/* parenthesis */
	} else if (t0 && t0->type == token_left_paren) {
		r = match(al, ts, t0->type, "expecting left parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = expr(al, ts, &n);
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, token_right_paren, "expecting right parenthesis", &t);
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
	struct token* t;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ts->lookahead, 0);

	if (t0 && t0->type == token_comma) {
		r = match(al, ts, token_comma, "expecting comma", &t);
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

/*
* array_literal -> [aseq]
*/
enum result array_literal(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	int num;
	struct dag_node* n = NULL;
	struct token* t;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ts->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		r = match(al, ts, token_left_square_bracket, "expected left square bracket", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_array_literal;

		r = aseq(al, ts, n);
		if (r == result_error) {
			goto function_error;
		}

		r = match(al, ts, token_right_square_bracket, "expected right square bracket", &t);
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
* aseq -> factor aseq' | e
*/
enum result aseq(struct allocator* al, struct token_state* ts, struct dag_node* parent)
{
	enum result r = result_ok;

	struct dag_node* a = NULL;
	r = factor(al, ts, &a);
	if (r == result_error) {
		return r;
	}

	if (!a) {
		return r;
	}

	dag_add_child(parent, a);

	r = aseq_prime(al, ts, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}

/*
* aseq' = , factor aseq' | e
*/
enum result aseq_prime(struct allocator* al, struct token_state* ts, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;
	struct token* t;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ts->lookahead, 0);

	/* e */
	if (!t0 || t0->type != token_comma) {
		return r;
	}

	r = match(al, ts, token_comma, "expecting comma", &t);
	if (r == result_error) {
		return r;
	}

	struct dag_node* a = NULL;
	r = factor(al, ts, &a);
	if (r == result_error) {
		return r;
	}
	dag_add_child(parent, a);

	if (!a) {
		return set_error("expected factor");
	}

	r = aseq_prime(al, ts, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}
