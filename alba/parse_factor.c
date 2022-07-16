#include "zinc/result.h"
#include "parse_tools.h"
#include "dag.h"
#include "token.h"
#include "types.h"
#include "parse_stmts.h"
#include "parse_expr.h"
#include "source.h"
#include "scan.h"

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
enum result factor(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;
	struct token* t;

	int num;
	r = get_lookahead(ps, 2, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0;
	struct token* t1;
	t0 = get_token(&ps->lookahead, 0);
	t1 = get_token(&ps->lookahead, 1);

	/* anonymous function */
	if (t0 && t0->type == token_function && t1 && t1->type == token_left_paren) {
		r = match(ps, token_function, "expected anonymous function", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = match(ps, token_left_paren, "expected left parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		dag_create_node(&n);
		n->type = dag_type_anonymous_function;

		struct dag_node* a = NULL;
		r = dseq(ps, &a);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, a);

		r = match(ps, token_right_paren, "expected right parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* b = NULL;
		r = stmts(ps, &b);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, b);

		r = match(ps, token_end, "expected end", &t);
		if (r == result_error) {
			goto function_error;
		}

		goto function_success;

		/* function call*/
	} else if (t0 && t0->type == token_id && t1 && t1->type == token_left_paren) {
		r = match(ps, token_id, "expecting id", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = match(ps, token_left_paren, "expecting left parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		dag_create_node(&n);
		n->type = dag_type_call;

		struct dag_node* a = NULL;
		dag_create_node(&a);
		a->type = dag_type_id;
		buffer_copy(&t0->value, &a->value);
		dag_add_child(n, a);

		struct dag_node* b = NULL;
		r = cseq(ps, &b);
		if (r == result_error) {
			goto function_error;
		}
		if (b) {
			dag_add_child(n, b);
		}

		r = match(ps, token_right_paren, "expecting right parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		/* ! factor */
	} else if (t0 && t0->type == token_not) {
		r = match(ps, token_not, "expecting not", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* a = NULL;
		r = factor(ps, &a);

		if (!a) {
			r = set_source_error(ps->el, t0, ps->sns->lc, "expected factor after !");
			goto function_error;
		}

		dag_create_node(&n);

		n->type = dag_type_not;
		dag_add_child(n, a);
		goto function_success;

		/* number, id, string */
	} else if (t0 && (t0->type == token_number || t0->type == token_id || t0->type == token_string)) {
		dag_create_node(&n);

		if (t0->type == token_number) {
			n->type = dag_type_number;
		} else if (t0->type == token_id) {
			n->type = dag_type_id;
		} else if (t0->type == token_string) {
			n->type = dag_type_string;
		}
		r = buffer_copy(&t0->value, &n->value);
		if (r == result_error) {
			goto function_error;
		}

		r = match(ps, t0->type, "expecting number, id, or string", &t);
		if (r == result_error) {
			goto function_error;
		}
	}

	/* sign and number or word */
	else if (t0 && (t0->type == token_plus || t0->type == token_minus) && t1 && (t1->type == token_number || t1->type == token_id)) {
		dag_create_node(&n);

		n->type = dag_type_sign;

		struct dag_node* left;
		dag_create_node(&left);

		if (t0->type == token_plus) {
			left->type = dag_type_plus;
		} else {
			left->type = dag_type_minus;
		}

		r = match(ps, t0->type, "expecting unary plus or minus", &t);
		if (r == result_error) {
			goto function_error;
		}

		dag_add_child(n, left);

		struct dag_node* right = NULL;
		r = factor(ps, &right);
		if (r == result_error) {
			goto function_error;
		}

		if (!right) {
			r = set_source_error(ps->el, t0, ps->sns->lc, "expecting factor after sign");
			goto function_error;
		}

		dag_add_child(n, right);

		/* array literal */
	} else if (t0 && t0->type == token_left_square_bracket) {
		r = array_literal(ps, &n);
		if (r == result_error) {
			return r;
		}

		goto function_success;

		/* parenthesis */
	} else if (t0 && t0->type == token_left_paren) {
		r = match(ps, t0->type, "expecting left parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = expr(ps, &n);
		if (r == result_error) {
			goto function_error;
		}

		r = match(ps, token_right_paren, "expecting right parenthesis", &t);
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
enum result cseq(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	struct dag_node* a = NULL;
	r = factor(ps, &a);
	if (r == result_error) {
		goto function_error;
	}

	if (a) {
		dag_create_node(&n);
		n->type = dag_type_cseq;

		dag_add_child(n, a);

		r = cseq_prime(ps, n);
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
enum result cseq_prime(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;
	struct token* t;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_comma) {
		r = match(ps, token_comma, "expecting comma", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* a = NULL;
		r = factor(ps, &a);
		if (r == result_error) {
			goto function_error;
		}

		if (!a) {
			r = set_source_error(ps->el, t0, ps->sns->lc, "expected factor after comma");
			goto function_error;
		}

		dag_add_child(parent, a);

		r = cseq_prime(ps, parent);
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
enum result array_literal(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	int num;
	struct dag_node* n = NULL;
	struct token* t;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		r = match(ps, token_left_square_bracket, "expected left square bracket", &t);
		if (r == result_error) {
			goto function_error;
		}

		dag_create_node(&n);
		n->type = dag_type_array_literal;

		r = aseq(ps, n);
		if (r == result_error) {
			goto function_error;
		}

		r = match(ps, token_right_square_bracket, "expected right square bracket", &t);
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
enum result aseq(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;

	struct dag_node* a = NULL;
	r = factor(ps, &a);
	if (r == result_error) {
		return r;
	}

	if (!a) {
		return r;
	}

	dag_add_child(parent, a);

	r = aseq_prime(ps, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}

/*
* aseq' = , factor aseq' | e
*/
enum result aseq_prime(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;
	struct token* t;

	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (!t0 || t0->type != token_comma) {
		return r;
	}

	r = match(ps, token_comma, "expecting comma", &t);
	if (r == result_error) {
		return r;
	}

	struct dag_node* a = NULL;
	r = factor(ps, &a);
	if (r == result_error) {
		return r;
	}
	dag_add_child(parent, a);

	if (!a) {
		return set_source_error(ps->el, t0, ps->sns->lc, "expected factor after comma");
	}

	r = aseq_prime(ps, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}
