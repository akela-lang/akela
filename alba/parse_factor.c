#include "zinc/result.h"
#include "parse_tools.h"
#include "dag.h"
#include "token.h"
#include "parse_types.h"
#include "parse_stmts.h"
#include "parse_expr.h"
#include "source.h"
#include "scan.h"

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
/* dynamic-output ps{} root root{} */
enum result factor(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;

	/* allocate ps{} */
	int num;
	r = get_lookahead(ps, 2, &num);
	if (r == result_error) {
		return r;
	}
	struct token* t0;
	struct token* t1;
	t0 = get_token(&ps->lookahead, 0);
	t1 = get_token(&ps->lookahead, 1);

	/* anonymous function */
	if (t0 && t0->type == token_function && t1 && t1->type == token_left_paren) {

		/* allocate ps{} f f{} */
		struct token* f = NULL;
		r = match(ps, token_function, "expected anonymous function", &f);
		if (r == result_error) {
			return r;
		}

		/* destroy f f{} */
		token_destroy(f);
		free(f);

		struct token* lp = NULL;
		r = match(ps, token_left_paren, "expected left parenthesis", &lp);
		if (r == result_error) {
			return r;
		}

		/* destroy lp lp{} */
		token_destroy(lp);
		free(lp);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_anonymous_function;

		/* allocate a a{} */
		struct dag_node* a = NULL;
		r = dseq(ps, &a);
		if (r == result_error) {
			return r;
		}

		/* transfer a a{} -> n{} */
		dag_add_child(n, a);

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		r = match(ps, token_right_paren, "expected right parenthesis", &rp);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* destroy rp rp{} */
		token_destroy(rp);
		free(rp);

		/* allocate b b{} */
		struct dag_node* b = NULL;
		r = stmts(ps, &b);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* transfer b b{} -> n */
		dag_add_child(n, b);

		/* allocate ps{} end end{} */
		struct token* end = NULL;
		r = match(ps, token_end, "expected end", &end);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		/* destroy end end{} */
		token_destroy(end);
		free(end);

		/* function call*/
	} else if (t0 && t0->type == token_id && t1 && t1->type == token_left_paren) {

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		r = match(ps, token_id, "expecting id", &id);
		if (r == result_error) {
			return r;
		}

		/* allocate ps{} lp lp{} */
		struct token* lp = NULL;
		r = match(ps, token_left_paren, "expecting left parenthesis", &lp);
		if (r == result_error) {
			token_destroy(id);
			free(id);
			return r;
		}

		/* destroy lp lp{} */
		token_destroy(lp);
		free(lp);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_call;

		/* allocate a */
		struct dag_node* a = NULL;
		dag_create_node(&a);
		a->type = dag_type_id;

		/* allocate a{} */
		buffer_copy(&id->value, &a->value);

		/* transfer a a{} -> n{} */
		dag_add_child(n, a);

		/* destroy id id{} */
		token_destroy(id);
		free(id);

		/* allocate b b{} */
		struct dag_node* b = NULL;
		r = cseq(ps, &b);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* transfer b b{} -> n{} */
		if (b) {
			dag_add_child(n, b);
		}

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		r = match(ps, token_right_paren, "expecting right parenthesis", &rp);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		/* destroy rp rp{} */
		token_destroy(rp);
		free(rp);

		/* ! factor */
	} else if (t0 && t0->type == token_not) {

		/* allocate ps{} not not{} */
		struct token* not = NULL;
		r = match(ps, token_not, "expecting not", &not);
		if (r == result_error) {
			return r;
		}

		/* destroy not not{} */
		token_destroy(not);
		free(not);

		struct location loc;
		r = get_parse_location(ps, &loc);
		if (r == result_error) {
			return r;
		}

		/* allocate a a{} */
		struct dag_node* a = NULL;
		r = factor(ps, &a);

		if (!a) {
			/* allocate ps{} */
			r = set_source_error(ps->el, &loc, "expected factor after !");
			return r;
		}

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_not;

		/* transfer a -> n{} */
		dag_add_child(n, a);

		/* number, id, string */
	} else if (t0 && (t0->type == token_number || t0->type == token_id || t0->type == token_string)) {

		/* allocate ps{} x x{} */
		struct token* x = NULL;
		r = match(ps, t0->type, "expecting number, id, or string", &x);
		if (r == result_error) {
			/* destroy x x{} */
			token_destroy(x);
			free(x);
			return r;
		}

		/* allocate n */
		dag_create_node(&n);

		if (x->type == token_number) {
			n->type = dag_type_number;
		} else if (x->type == token_id) {
			n->type = dag_type_id;
		} else if (x->type == token_string) {
			n->type = dag_type_string;
		}
		
		/* allocate n{} */
		buffer_copy(&x->value, &n->value);

		/* destroy x x{} */
		token_destroy(x);
		free(x);
	}

	/* sign and number or word */
	else if (t0 && (t0->type == token_plus || t0->type == token_minus) && t1 && (t1->type == token_number || t1->type == token_id)) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_sign;

		/* allocate left */
		struct dag_node* left;
		dag_create_node(&left);

		if (t0->type == token_plus) {
			left->type = dag_type_plus;
		} else {
			left->type = dag_type_minus;
		}

		/* allocate sign */
		struct token* sign = NULL;
		r = match(ps, t0->type, "expecting unary plus or minus", &sign);
		if (r == result_error) {
			dag_destroy(n);
			dag_destroy(left);
			return r;
		}

		/* destroy sign */
		token_destroy(sign);

		/* transfer left -> n{} */
		dag_add_child(n, left);

		struct location loc;
		r = get_parse_location(ps, &loc);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		/* allocate right */
		struct dag_node* right = NULL;
		r = factor(ps, &right);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		if (!right) {
			r = set_source_error(ps->el, &loc, "expecting factor after sign");
			dag_destroy(n);
			return r;
		}

		/* transfer right -> n{} */
		dag_add_child(n, right);

		/* array literal */
	} else if (t0 && t0->type == token_left_square_bracket) {
		/* allocate n n{} */
		r = array_literal(ps, &n);
		if (r == result_error) {
			return r;
		}

		/* parenthesis */
	} else if (t0 && t0->type == token_left_paren) {
		/* allocate ps{} lp lp{} */
		struct token* lp = NULL;
		r = match(ps, t0->type, "expecting left parenthesis", &lp);
		if (r == result_error) {
			return r;
		}

		/* destroy lp lp{} */
		token_destroy(lp);
		free(lp);

		/* allocate n n{} */
		r = expr(ps, &n);
		if (r == result_error) {
			return r;
		}

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		r = match(ps, token_right_paren, "expecting right parenthesis", &rp);
		if (r == result_error) {
			dag_destroy(n);
			return r;
		}

		/* destroy rp rp{} */
		token_destroy(rp);
		free(rp);
	}

	/* transfer n -> root */
	*root = n;
	return r;
}

/*
* cseq -> factor cseq'
*		| e
*/
/* dynamic-output ps{} root root{} */
enum result cseq(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	/* allocate a a{} */
	struct dag_node* a = NULL;
	r = factor(ps, &a);
	if (r == result_error) {
		return r;
	}

	if (a) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_cseq;

		/* transfer a -> n{} */
		dag_add_child(n, a);

		/* allocate n{} */
		r = cseq_prime(ps, n);
		if (r == result_error) {
			return r;
		}
	}

	/* transfer n -> root */
	*root = n;
	return r;
}

/*
* cseq' -> , factor cseq'
*		 | e
*/
/* dynamic-output ps{} parent{} */
enum result cseq_prime(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;

	/* allocate ps{} */
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_comma) {

		/* allocate ps{} comma comma{} */
		struct token* comma = NULL;;
		r = match(ps, token_comma, "expecting comma", &comma);
		if (r == result_error) {
			return r;
		}

		/* destroy comma comma{} */
		token_destroy(comma);
		free(comma);

		struct location loc;
		r = get_parse_location(ps, &loc);
		if (r == result_error) {
			return r;
		}

		/* allocate a a{} */
		struct dag_node* a = NULL;
		r = factor(ps, &a);
		if (r == result_error) {
			return r;
		}

		if (!a) {
			r = set_source_error(ps->el, &loc, "expected factor after comma");
			return r;
		}

		/* transfer a -> parent */
		dag_add_child(parent, a);

		/* allocate ps{} parent{} */
		r = cseq_prime(ps, parent);
		if (r == result_error) {
			return r;
		}
	}

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

	/* allocate ps{} */
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		/* allocate ps{} lsb lsb{} */
		struct token* lsb = NULL;
		r = match(ps, token_left_square_bracket, "expected left square bracket", &lsb);
		if (r == result_error) {
			return r;
		}

		/* destroy lsb lsb{} */
		token_destroy(lsb);
		free(lsb);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_array_literal;

		/* allocate ps{} n{} */
		r = aseq(ps, n);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* allocate ps{} rsb rsb{] */
		struct token* rsb = NULL;
		r = match(ps, token_right_square_bracket, "expected right square bracket", &rsb);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* destroy rsb rsb{} */
		token_destroy(rsb);
		free(rsb);
	}

	/* transfer n -> root */
	*root = n;
	return r;
}

/*
* aseq -> factor aseq' | e
*/
/* dynamic-output ps{} parent{} */
enum result aseq(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	r = factor(ps, &a);
	if (r == result_error) {
		return r;
	}

	if (!a) {
		return r;
	}

	/* a -> parent{} */
	dag_add_child(parent, a);

	/* allocate parent{} */
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

	/* allocate ps{} */
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (!t0 || t0->type != token_comma) {
		return r;
	}

	/* allocate ps{} comma comma{} */
	struct token* comma = NULL;
	r = match(ps, token_comma, "expecting comma", &comma);
	if (r == result_error) {
		return r;
	}

	/* destroy comma comma{} */
	token_destroy(comma);
	free(comma);

	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		return r;
	}

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	r = factor(ps, &a);
	if (r == result_error) {
		return r;
	}

	if (!a) {
		return set_source_error(ps->el, &loc, "expected factor after comma");
	}

	/* transfer a -> parent */
	dag_add_child(parent, a);

	/* allocate ps{} parent{} */
	r = aseq_prime(ps, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}
