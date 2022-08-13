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
bool function_call(struct parse_state* ps, struct dag_node** root);
bool cseq(struct parse_state* ps, struct dag_node** root);
bool not(struct parse_state* ps, struct dag_node** root);
bool literal_nt(struct parse_state* ps, struct dag_node** root);
bool id_nt(struct parse_state* ps, struct dag_node** root);
bool sign(struct parse_state* ps, struct dag_node** root);
bool array_literal(struct parse_state* ps, struct dag_node** root);
bool aseq(struct parse_state* ps, struct dag_node* parent);
bool parenthesis(struct parse_state* ps, struct dag_node** root);

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
	valid = get_lookahead(ps, 2, &num) && valid;

	struct token* t0;
	struct token* t1;
	t0 = get_token(&ps->lookahead, 0);
	t1 = get_token(&ps->lookahead, 1);

	if (t0 && t0->type == token_function && t1 && t1->type == token_left_paren) {
		valid = anonymous_function(ps, &n) && valid;

	} else if (t0 && t0->type == token_id && t1 && t1->type == token_left_paren) {
		valid = function_call(ps, &n) && valid;

	} else if (t0 && t0->type == token_not) {
		valid = not(ps, &n) && valid;

	} else if (t0 && (t0->type == token_number || t0->type == token_string || t0->type == token_boolean)) {
		valid = literal_nt(ps, &n) && valid;

	} else if (t0 && t0->type == token_id) {
		valid = id_nt(ps, &n) && valid;

	} else if (t0 && (t0->type == token_plus || t0->type == token_minus)) {
		valid = sign(ps, &n) && valid;

	} else if (t0 && t0->type == token_left_square_bracket) {
		/* allocate n n{} */
		valid = array_literal(ps, &n) && valid;

	} else if (t0 && t0->type == token_left_paren) {
		valid = parenthesis(ps, &n) && valid;
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
	struct environment* saved = ps->st->top;

	/* allocate env env{} */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);

	/* transfer env -> ps{top} */
	ps->st->top = env;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = match(ps, token_function, "expected anonymous function", &f) && valid;

	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expected left parenthesis", &lp) && valid;

	/* allocate a a{} */
	struct dag_node* dseq_node = NULL;
	valid = dseq(ps, &dseq_node) && valid;

	/* allocate n */
	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expected right parenthesis", &rp) && valid;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct dag_node* dret_type = NULL;
	if (t0 && t0->type == token_double_colon) {
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expecting double colon", &dc) && valid;
		token_destroy(dc);
		free(dc);

		valid = type(ps, NULL, &dret_type) && valid;
	}

	/* allocate b b{} */
	struct dag_node* stmts_node = NULL;
	valid = stmts(ps, true, &stmts_node) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;

	if (valid) {
		dag_create_node(&n);
		n->type = dag_type_anonymous_function;

		/* transfer dseq_node dseq_node{} -> n{} */
		dag_add_child(n, dseq_node);

		struct dag_node* dret = NULL;
		dag_create_node(&dret);
		dret->type = dag_type_dret;

		if (dret_type) {
			dag_add_child(dret, dret_type);
		}
		dag_add_child(n, dret);

		/* transfer stmts_node stmts_node{} -> n */
		dag_add_child(n, stmts_node);
	} else {
		dag_destroy(dseq_node);
		dag_destroy(dret_type);
		dag_destroy(stmts_node);
	}

	if (valid) {
		struct dag_node* etype = af2etype(n);
		n->etype = etype;
	}

	/* destroy f f{} lp lp{} rp rp{} end end{} */
	token_destroy(f);
	free(f);
	token_destroy(lp);
	free(lp);
	token_destroy(rp);
	free(rp);
	token_destroy(end);
	free(end);

	/* transfer saved -> ps{top} */
	ps->st->top = saved;

	/* destroy env env{} */
	environment_destroy(env);

	if (valid) {
		/* transfer n -> root */
		*root = n;
	} else {
		dag_destroy(n);
	}

	return valid;
}

bool function_call(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting id", &id) && valid;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expecting left parenthesis", &lp) && valid;

	/* allocate b b{} */
	struct dag_node* cseq_node = NULL;
	valid = cseq(ps, &cseq_node) && valid;

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expecting right parenthesis", &rp) && valid;

	if (valid) {
		struct symbol* sym = environment_get(ps->st->top, &id->value);
		if (sym) {
			if (sym->dec->type != dag_type_function) {
				char* name;
				buffer2array(&id->value, &name);
				set_source_error(ps->el, &loc, "call of variable that is not a function: %s", name);
				free(name);
				valid = false;
			}
		} else {
			char* name;
			buffer2array(&id->value, &name);
			set_source_error(ps->el, &loc, "function is not declared: %s", name);
			free(name);
			valid = false;
		}
	}

	if (valid) {
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

		/* transfer b b{} -> n{} */
		if (cseq_node) {
			dag_add_child(n, cseq_node);
		}

		*root = n;

	} else {
		/* destroy cseq_node cseq_node{} */
		dag_destroy(cseq_node);
	}

	/* destroy id id{} lp lp{} rp rp{} */
	token_destroy(id);
	free(id);
	token_destroy(lp);
	free(lp);
	token_destroy(rp);
	free(rp);

	return valid;
}

/* cseq -> expr cseq' | e */
/* cseq' -> , expr cseq' | e */
/* dynamic-output ps{} root root{} */
bool cseq(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_cseq;

	*root = n;

	/* allocate a a{} */
	struct dag_node* a = NULL;
	valid = expr(ps, &a) && valid;

	if (!a) {
		return valid;
	}

	/* transfer a -> n{} */
	dag_add_child(n, a);

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

		/* destroy comma comma{} */
		token_destroy(comma);
		free(comma);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* allocate a a{} */
		struct dag_node* a = NULL;
		valid = expr(ps, &a) && valid;

		if (!a) {
			set_source_error(ps->el, &loc, "expected expression after comma");
			valid = false;
		} else {
			/* transfer a -> parent */
			dag_add_child(n, a);
		}
	}

	return valid;
}

bool not(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} not not{} */
	struct token* not = NULL;
	valid = match(ps, token_not, "expecting not", &not) && valid;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate a a{} */
	struct dag_node* a = NULL;
	valid = expr(ps, &a) && valid;

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc, "expected factor after !");
	}

	if (valid) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_not;

		if (a) {
			/* transfer a -> n{} */
			dag_add_child(n, a);
		}

	} else {
		dag_destroy(a);
	}

	if (valid) {
		#pragma warning(suppress:6011)
		if (a->etype) {
			struct dag_node* etype = NULL;
			if (type_is_boolean(a->etype)) {
				etype = dag_copy(a->etype);
				n->etype = etype;
			}
			if (!etype) {
				valid = set_source_error(ps->el, &loc, "used a ! operator but expression not a boolean");
			}
		} else {
			valid = set_source_error(ps->el, &loc, "expression type not set");
		}
	}

	/* destroy not not{} */
	token_destroy(not);
	free(not);

	if (valid) {
		*root = n;
	} else {
		dag_destroy(n);
	}

	return valid;
}

bool literal_nt(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} x x{} */
	struct token* x = NULL;
	valid = match(ps, t0->type, "expecting number, bool, or string", &x) && valid;

	if (valid) {
		/* allocate n */
		dag_create_node(&n);

		#pragma warning(suppress:6011)
		if (x->type == token_number) {
			n->type = dag_type_number;
		} else if (x->type == token_string) {
			n->type = dag_type_string;
		} else if (x->type == token_boolean) {
			n->type = dag_type_boolean;
		}
		/* allocate n{} */
		buffer_copy(&x->value, &n->value);
	}

	if (valid) {
		if (n->type == dag_type_number) {
			struct dag_node* etype = NULL;
			dag_create_node(&etype);
			etype->type = dag_type_type_name;
			if (x->is_integer) {
				buffer_copy_str(&etype->value, "Int64");
			} else if (x->is_float) {
				buffer_copy_str(&etype->value, "Float64");
			} else {
				valid = set_source_error(ps->el, &loc, "number token is not a integer or float");
			}
			n->etype = etype;
		} else if (n->type == dag_type_string) {
			struct dag_node* etype = NULL;
			dag_create_node(&etype);
			etype->type = dag_type_type_name;
			buffer_copy_str(&etype->value, "String");
			n->etype = etype;
		} else if (n->type == dag_type_boolean) {
			struct dag_node* etype = NULL;
			dag_create_node(&etype);
			etype->type = dag_type_type_name;
			buffer_copy_str(&etype->value, "Bool");
			n->etype = etype;
		}
	}

	/* destroy x x{} */
	token_destroy(x);
	free(x);

	if (valid) {
		*root = n;
	} else {
		dag_destroy(n);
	}

	return valid;
}

bool id_nt(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} x x{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting identifier", &id) && valid;

	if (valid) {
		/* allocate n */
		dag_create_node(&n);

		n->type = dag_type_id;

		/* allocate n{} */
		buffer_copy(&id->value, &n->value);

	}

	if (valid) {
		struct symbol* sym = environment_get(ps->st->top, &n->value);
		if (!sym) {
			char* a;
			buffer2array(&n->value, &a);
			valid = set_source_error(ps->el, &loc, "identifier not declared: %s", a);
			free(a);
		}

		if (sym) {
			struct dag_node* etype = dag_copy(sym->dec);
			n->etype = etype;
		}
	}

	/* destroy id id{} */
	token_destroy(id);
	free(id);

	if (valid) {
		*root = n;
	} else {
		dag_destroy(n);
	}

	return valid;
}

bool sign(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* allocate sign */
	struct token* sign = NULL;
	valid = match(ps, t0->type, "expecting unary plus or minus", &sign) && valid;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate right */
	struct dag_node* right = NULL;
	valid = factor(ps, &right) && valid;

	if (!right) {
		valid = set_source_error(ps->el, &loc, "expecting factor after sign");
	}

	if (valid) {
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

		/* transfer left -> n{} */
		dag_add_child(n, left);

		/* transfer right -> n{} */
		dag_add_child(n, right);

		*root = n;
	}

	/* destroy sign */
	token_destroy(sign);

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
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		/* allocate ps{} lsb lsb{} */
		struct token* lsb = NULL;
		valid = match(ps, token_left_square_bracket, "expected left square bracket", &lsb) && valid;

		/* destroy lsb lsb{} */
		token_destroy(lsb);
		free(lsb);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_array_literal;

		/* allocate ps{} n{} */
		valid = aseq(ps, n) && valid;

		/* allocate ps{} rsb rsb{] */
		struct token* rsb = NULL;
		valid = match(ps, token_right_square_bracket, "expected right square bracket", &rsb) && valid;

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

/* aseq -> expr aseq' | e */
/* aseq' = , expr aseq' | e */
/* dynamic-output ps{} parent{} */
bool aseq(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	valid = expr(ps, &a) && valid;

	if (a) {
		/* a -> parent{} */
		dag_add_child(parent, a);

		while (true) {
			/* allocate ps{} */
			int num;
			valid = get_lookahead(ps, 1, &num) && valid;
			struct token* t0 = get_token(&ps->lookahead, 0);
			if (!t0 || t0->type != token_comma) {
				break;
			}

			/* allocate ps{} comma comma{} */
			struct token* comma = NULL;
			valid = match(ps, token_comma, "expecting comma", &comma) && valid;

			/* destroy comma comma{} */
			token_destroy(comma);
			free(comma);

			struct location loc;
			valid = get_parse_location(ps, &loc) && valid;

			/* allocate ps{} a a{} */
			struct dag_node* a = NULL;
			valid = expr(ps, &a) && valid;

			if (!a) {
				valid = set_source_error(ps->el, &loc, "expected expr after comma");
				break;
			}

			/* transfer a -> parent */
			dag_add_child(parent, a);
		}
	}

	return valid;
}

bool parenthesis(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expecting left parenthesis", &lp) && valid;

	/* allocate n n{} */
	valid = valid && expr(ps, &n);

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expecting right parenthesis", &rp) && valid;

	if (valid) {
		*root = n;
	}

	/* destroy lp lp{} */
	token_destroy(lp);
	free(lp);

	/* destroy rp rp{} */
	token_destroy(rp);
	free(rp);

	return valid;
}
