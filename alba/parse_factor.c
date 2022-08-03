#include "zinc/result.h"
#include "parse_tools.h"
#include "dag.h"
#include "token.h"
#include "parse_types.h"
#include "parse_stmts.h"
#include "parse_expr.h"
#include "source.h"
#include "scan.h"
#include "zinc/memory.h"
#include "symbol_table.h"

bool anonymous_function(struct parse_state* ps, struct dag_node** root);
bool cseq(struct parse_state* ps, struct dag_node** root);
bool cseq_prime(struct parse_state* ps, struct dag_node* parent);
bool array_literal(struct parse_state* ps, struct dag_node** root);
bool aseq(struct parse_state* ps, struct dag_node* parent);
bool aseq_prime(struct parse_state* ps, struct dag_node* parent);

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
/* dynamic-output ps{} root root{} */
bool factor(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;

	/* allocate ps{} */
	int num;
	valid = valid && get_lookahead(ps, 2, &num);

	struct token* t0;
	struct token* t1;
	t0 = get_token(&ps->lookahead, 0);
	t1 = get_token(&ps->lookahead, 1);

	/* anonymous function */
	if (t0 && t0->type == token_function && t1 && t1->type == token_left_paren) {
		valid = valid && anonymous_function(ps, &n);

		/* function call*/
	} else if (t0 && t0->type == token_id && t1 && t1->type == token_left_paren) {

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		valid = valid && match(ps, token_id, "expecting id", &id);

		/* allocate ps{} lp lp{} */
		struct token* lp = NULL;
		valid = valid && match(ps, token_left_paren, "expecting left parenthesis", &lp);

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

		if (id) {
			/* allocate a{} */
			buffer_copy(&id->value, &a->value);
		}

		/* transfer a a{} -> n{} */
		dag_add_child(n, a);

		/* destroy id id{} */
		token_destroy(id);
		free(id);

		/* allocate b b{} */
		struct dag_node* b = NULL;
		valid = valid && cseq(ps, &b);

		/* transfer b b{} -> n{} */
		if (b) {
			dag_add_child(n, b);
		}

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		valid = valid && match(ps, token_right_paren, "expecting right parenthesis", &rp);

		/* destroy rp rp{} */
		token_destroy(rp);
		free(rp);

		/* ! factor */
	} else if (t0 && t0->type == token_not) {

		/* allocate ps{} not not{} */
		struct token* not = NULL;
		valid = valid && match(ps, token_not, "expecting not", &not);

		/* destroy not not{} */
		token_destroy(not);
		free(not);

		struct location loc;
		valid = valid && get_parse_location(ps, &loc);

		/* allocate a a{} */
		struct dag_node* a = NULL;
		valid = valid && factor(ps, &a);

		if (!a) {
			/* allocate ps{} */
			set_source_error(ps->el, &loc, "expected factor after !");
			valid = false;
		}

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_not;

		if (a) {
			/* transfer a -> n{} */
			dag_add_child(n, a);
		}

		/* number, id, string */
	} else if (t0 && (t0->type == token_number || t0->type == token_id || t0->type == token_string)) {

		/* allocate ps{} x x{} */
		struct token* x = NULL;
		valid = match(ps, t0->type, "expecting number, id, or string", &x);

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
		if (x) {
			buffer_copy(&x->value, &n->value);
		}

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
		valid = valid && match(ps, t0->type, "expecting unary plus or minus", &sign);

		/* destroy sign */
		token_destroy(sign);

		if (left) {
			/* transfer left -> n{} */
			dag_add_child(n, left);
		}

		struct location loc;
		valid = valid && get_parse_location(ps, &loc);

		/* allocate right */
		struct dag_node* right = NULL;
		valid = valid && factor(ps, &right);

		if (!right) {
			set_source_error(ps->el, &loc, "expecting factor after sign");
			valid = false;
		} else {
			/* transfer right -> n{} */
			dag_add_child(n, right);
		}

		/* array literal */
	} else if (t0 && t0->type == token_left_square_bracket) {
		/* allocate n n{} */
		valid = valid && array_literal(ps, &n);

		/* parenthesis */
	} else if (t0 && t0->type == token_left_paren) {
		/* allocate ps{} lp lp{} */
		struct token* lp = NULL;
		valid = valid && match(ps, t0->type, "expecting left parenthesis", &lp);

		/* destroy lp lp{} */
		token_destroy(lp);
		free(lp);

		/* allocate n n{} */
		valid = valid && expr(ps, &n);

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		valid = valid && match(ps, token_right_paren, "expecting right parenthesis", &rp);

		/* destroy rp rp{} */
		token_destroy(rp);
		free(rp);
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}

	return valid;
}

bool anonymous_function(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* shared ps{top} -> saved */
	struct environment* saved = ps->top;

	/* allocate env env{} */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);

	/* transfer env -> ps{top} */
	ps->top = env;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = valid && match(ps, token_function, "expected anonymous function", &f);

	/* destroy f f{} */
	token_destroy(f);
	free(f);

	struct token* lp = NULL;
	valid = valid && match(ps, token_left_paren, "expected left parenthesis", &lp);

	/* destroy lp lp{} */
	token_destroy(lp);
	free(lp);

	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_anonymous_function;

	/* allocate a a{} */
	struct dag_node* a = NULL;
	valid = valid && dseq(ps, &a);

	if (a) {
		/* transfer a a{} -> n{} */
		dag_add_child(n, a);
	}

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = valid && match(ps, token_right_paren, "expected right parenthesis", &rp);

	/* destroy rp rp{} */
	token_destroy(rp);
	free(rp);

	/* allocate b b{} */
	struct dag_node* b = NULL;
	valid = valid && stmts(ps, &b);

	if (b) {
		/* transfer b b{} -> n */
		dag_add_child(n, b);
	}

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = valid && match(ps, token_end, "expected end", &end);

	/* destroy end end{} */
	token_destroy(end);
	free(end);

	/* transfer saved -> ps{top} */
	ps->top = saved;

	/* destroy env env{} */
	environment_destroy(env);

	if (valid) {
		*root = n;
	}

	return valid;
}

/*
* cseq -> factor cseq'
*		| e
*/
/* dynamic-output ps{} root root{} */
bool cseq(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate a a{} */
	struct dag_node* a = NULL;
	valid = valid && factor(ps, &a);

	if (a) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_cseq;

		/* transfer a -> n{} */
		dag_add_child(n, a);

		/* allocate n{} */
		valid = valid && cseq_prime(ps, n);
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}
	return valid;
}

/*
* cseq' -> , factor cseq'
*		 | e
*/
/* dynamic-output ps{} parent{} */
bool cseq_prime(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_comma) {

		/* allocate ps{} comma comma{} */
		struct token* comma = NULL;;
		valid = valid && match(ps, token_comma, "expecting comma", &comma);

		/* destroy comma comma{} */
		token_destroy(comma);
		free(comma);

		struct location loc;
		valid = valid && get_parse_location(ps, &loc);

		/* allocate a a{} */
		struct dag_node* a = NULL;
		valid = valid && factor(ps, &a);

		if (!a) {
			set_source_error(ps->el, &loc, "expected factor after comma");
			valid = false;
		} else {
			/* transfer a -> parent */
			dag_add_child(parent, a);
		}

		/* allocate ps{} parent{} */
		valid = valid && cseq_prime(ps, parent);
	}

	return valid;
}

/*
* array_literal -> [aseq]
*/
bool array_literal(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	int num;
	struct dag_node* n = NULL;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		/* allocate ps{} lsb lsb{} */
		struct token* lsb = NULL;
		valid = valid && match(ps, token_left_square_bracket, "expected left square bracket", &lsb);

		/* destroy lsb lsb{} */
		token_destroy(lsb);
		free(lsb);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_array_literal;

		/* allocate ps{} n{} */
		valid = valid && aseq(ps, n);

		/* allocate ps{} rsb rsb{] */
		struct token* rsb = NULL;
		valid = valid && match(ps, token_right_square_bracket, "expected right square bracket", &rsb);

		/* destroy rsb rsb{} */
		token_destroy(rsb);
		free(rsb);
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}

	return valid;
}

/*
* aseq -> factor aseq' | e
*/
/* dynamic-output ps{} parent{} */
bool aseq(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	valid = valid && factor(ps, &a);

	if (a) {
		/* a -> parent{} */
		dag_add_child(parent, a);

		/* allocate parent{} */
		valid = valid && aseq_prime(ps, parent);
	}

	return valid;
}

/*
* aseq' = , factor aseq' | e
*/
/* dynamic-output ps{} parent{} */
bool aseq_prime(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);

	/* e */
	if (!t0 || t0->type != token_comma) {
		return valid;
	}

	/* allocate ps{} comma comma{} */
	struct token* comma = NULL;
	valid = valid && match(ps, token_comma, "expecting comma", &comma);

	/* destroy comma comma{} */
	token_destroy(comma);
	free(comma);

	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	valid = valid && factor(ps, &a);

	if (!a) {
		set_source_error(ps->el, &loc, "expected factor after comma");
		valid = false;
		return valid;
	}

	/* transfer a -> parent */
	dag_add_child(parent, a);

	/* allocate ps{} parent{} */
	valid = valid && aseq_prime(ps, parent);

	return valid;
}
