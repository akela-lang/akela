#include "zinc/result.h"
#include "parse_tools.h"
#include "ast.h"
#include "token.h"
#include "parse_types.h"
#include "parse_stmts.h"
#include "parse_expr.h"
#include "source.h"
#include "scan.h"
#include "zinc/memory.h"
#include "symbol_table.h"
#include "parse_factor.h"

bool anonymous_function(struct parse_state* ps, struct ast_node** root);
bool function_call(struct parse_state* ps, struct ast_node** root);
bool cseq(struct parse_state* ps, struct ast_node** root);
bool not_nt(struct parse_state* ps, struct ast_node** root);
bool literal_nt(struct parse_state* ps, struct ast_node** root);
bool id_nt(struct parse_state* ps, struct ast_node** root);
bool sign(struct parse_state* ps, struct ast_node** root);
bool array_literal(struct parse_state* ps, struct ast_node** root);
bool aseq(struct parse_state* ps, struct ast_node* parent);
bool parenthesis(struct parse_state* ps, struct ast_node** root);

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
/* dynamic-output ps{} root root{} */
bool factor(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct ast_node* n = NULL;

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
		valid = not_nt(ps, &n) && valid;

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

bool anonymous_function(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

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
	struct ast_node* dseq_node = NULL;
	valid = dseq(ps, &dseq_node) && valid;

	/* allocate n */
	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expected right parenthesis", &rp) && valid;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct ast_node* dret_type = NULL;
	if (t0 && t0->type == token_double_colon) {
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expecting double colon", &dc) && valid;
		token_destroy(dc);
		free(dc);

		valid = type(ps, NULL, &dret_type) && valid;
	}

	/* allocate b b{} */
	struct ast_node* stmts_node = NULL;
	valid = stmts(ps, true, &stmts_node) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;

	if (valid) {
		ast_node_create(&n);
		n->type = ast_type_anonymous_function;

		/* transfer dseq_node dseq_node{} -> n{} */
		ast_node_add(n, dseq_node);

		struct ast_node* dret = NULL;
		ast_node_create(&dret);
		dret->type = ast_type_dret;

		if (dret_type) {
			ast_node_add(dret, dret_type);
		}
		ast_node_add(n, dret);

		/* transfer stmts_node stmts_node{} -> n */
		ast_node_add(n, stmts_node);
	} else {
		ast_node_destroy(dseq_node);
		ast_node_destroy(dret_type);
		ast_node_destroy(stmts_node);
	}

	if (valid) {
		struct ast_node* etype = af2etype(n);
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
		ast_node_destroy(n);
	}

	return valid;
}

bool function_call(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting id", &id) && valid;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expecting left parenthesis", &lp) && valid;

	/* allocate b b{} */
	struct ast_node* cseq_node = NULL;
	valid = cseq(ps, &cseq_node) && valid;

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expecting right parenthesis", &rp) && valid;

	if (valid) {
		struct symbol* sym = environment_get(ps->st->top, &id->value);
		if (sym) {
			if (sym->dec->type != ast_type_function) {
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
		ast_node_create(&n);
		n->type = ast_type_call;

		/* allocate a */
		struct ast_node* a = NULL;
		ast_node_create(&a);
		a->type = ast_type_id;

		/* allocate a{} */
		buffer_copy(&id->value, &a->value);

		/* transfer a a{} -> n{} */
		ast_node_add(n, a);

		/* transfer b b{} -> n{} */
		if (cseq_node) {
			ast_node_add(n, cseq_node);
		}

		*root = n;

	} else {
		/* destroy cseq_node cseq_node{} */
		ast_node_destroy(cseq_node);
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
bool cseq(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate n */
	ast_node_create(&n);
	n->type = ast_type_cseq;

	*root = n;

	/* allocate a a{} */
	struct ast_node* a = NULL;
	valid = expr(ps, &a) && valid;

	if (!a) {
		return valid;
	}

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

		/* destroy comma comma{} */
		token_destroy(comma);
		free(comma);

		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* allocate a a{} */
		struct ast_node* a = NULL;
		valid = expr(ps, &a) && valid;

		if (!a) {
			set_source_error(ps->el, &loc, "expected expression after comma");
			valid = false;
		} else {
			/* transfer a -> parent */
			ast_node_add(n, a);
		}
	}

	return valid;
}

bool not_nt(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate ps{} not not{} */
	struct token* not = NULL;
	valid = match(ps, token_not, "expecting not", &not) && valid;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate a a{} */
	struct ast_node* a = NULL;
	valid = expr(ps, &a) && valid;

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc, "expected factor after !");
	}

	if (valid) {
		/* allocate n */
		ast_node_create(&n);
		n->type = ast_type_not;

		if (a) {
			/* transfer a -> n{} */
			ast_node_add(n, a);
		}

	} else {
		ast_node_destroy(a);
	}

	if (valid) {
		#pragma warning(suppress:6011)
		if (a->etype) {
			struct ast_node* etype = NULL;
			if (a->etype->type == ast_type_type_name) {
				if (buffer_str_compare(&a->etype->value, "Bool")) {
					etype = ast_node_copy(a->etype);
					n->etype = etype;
				}
			}
			if (!etype) {
				valid = set_source_error(ps->el, &loc, "used a ! operator but expression not a boolean");
			}
		} else {
			valid = set_source_error(ps->el, &loc, "cannot perform operation on expression with no value");
		}
	}

	/* destroy not not{} */
	token_destroy(not);
	free(not);

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

bool literal_nt(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

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
		ast_node_create(&n);

		#pragma warning(suppress:6011)
		if (x->type == token_number) {
			n->type = ast_type_number;
		} else if (x->type == token_string) {
			n->type = ast_type_string;
		} else if (x->type == token_boolean) {
			n->type = ast_type_boolean;
		}
		/* allocate n{} */
		buffer_copy(&x->value, &n->value);
	}

	if (valid) {
		if (n->type == ast_type_number) {
			struct ast_node* etype = NULL;
			ast_node_create(&etype);
			etype->type = ast_type_type_name;
			if (x->is_integer) {
				buffer_copy_str(&etype->value, "Int64");
			} else if (x->is_float) {
				buffer_copy_str(&etype->value, "Float64");
			} else {
				valid = set_source_error(ps->el, &loc, "number token is not a integer or float");
			}
			n->etype = etype;
		} else if (n->type == ast_type_string) {
			struct ast_node* etype = NULL;
			ast_node_create(&etype);
			etype->type = ast_type_type_name;
			buffer_copy_str(&etype->value, "String");
			n->etype = etype;
		} else if (n->type == ast_type_boolean) {
			struct ast_node* etype = NULL;
			ast_node_create(&etype);
			etype->type = ast_type_type_name;
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
		ast_node_destroy(n);
	}

	return valid;
}

bool id_nt(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

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
		ast_node_create(&n);

		n->type = ast_type_id;

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
			struct ast_node* etype = ast_node_copy(sym->dec);
			n->etype = etype;
		}
	}

	/* destroy id id{} */
	token_destroy(id);
	free(id);

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

bool sign(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* allocate sign */
	struct token* sign = NULL;
	valid = match(ps, t0->type, "expecting unary plus or minus", &sign) && valid;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate right */
	struct ast_node* right = NULL;
	valid = factor(ps, &right) && valid;

	if (!right) {
		valid = set_source_error(ps->el, &loc, "expecting factor after sign");
	}

	if (valid) {
		ast_node_create(&n);
		n->type = ast_type_sign;

		/* allocate left */
		struct ast_node* left;
		ast_node_create(&left);

		if (t0->type == token_plus) {
			left->type = ast_type_plus;
		} else {
			left->type = ast_type_minus;
		}

		/* transfer left -> n{} */
		ast_node_add(n, left);

		/* transfer right -> n{} */
		ast_node_add(n, right);

	}

	if (valid) {
		#pragma warning(suppress:6011)
		if (right->etype) {
			n->etype = ast_node_copy(right->etype);
		} else {
			valid = set_source_error(ps->el, &loc, "cannot perform operation on expression with no value");
		}
	}

	/* destroy sign */
	token_destroy(sign);

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

/*
* array_literal -> [aseq]
*/
bool array_literal(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	int num;
	struct ast_node* n = NULL;

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
		ast_node_create(&n);
		n->type = ast_type_array_literal;

		struct location loc;
		get_parse_location(ps, &loc);

		/* allocate ps{} n{} */
		valid = aseq(ps, n) && valid;

		/* allocate ps{} rsb rsb{] */
		struct token* rsb = NULL;
		valid = match(ps, token_right_square_bracket, "expected right square bracket", &rsb) && valid;

		/* destroy rsb rsb{} */
		token_destroy(rsb);
		free(rsb);

		if (valid) {
			struct ast_node* first = ast_node_get(n, 0);
			if (!first) {
				valid = set_source_error(ps->el, &loc, "array literal is empty");
			} else if (!first->etype) {
				valid = set_source_error(ps->el, &loc, "array element has no value");
			} else {
				struct ast_node* element_etype = ast_node_copy(first->etype);
				struct ast_node* p = ast_node_get(n, 1);
				bool m = true;
				while (p) {
					if (!p->etype) {
						valid = set_source_error(ps->el, &loc, "array element has no value");
						break;
					}
					if (!type_match(ps->st, element_etype, p->etype)) {
						m = false;
						break;
					}
					p = p->next;
				}
				if (!m) {
					valid = set_source_error(ps->el, &loc, "array elements not one type");
					ast_node_destroy(element_etype);
				} else {
					struct ast_node* etype = NULL;
					ast_node_create(&etype);
					etype->type = ast_type_array;

					struct ast_node* a = NULL;
					ast_node_create(&a);
					a->type = ast_type_array_type_name;
					buffer_copy_str(&a->value, "Vector");
					ast_node_add(etype, a);

					ast_node_add(etype, element_etype);

					n->etype = etype;
				}
			}
		}
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
bool aseq(struct parse_state* ps, struct ast_node* parent)
{
	bool valid = true;

	/* allocate ps{} a a{} */
	struct ast_node* a = NULL;
	valid = expr(ps, &a) && valid;

	if (a) {
		/* a -> parent{} */
		ast_node_add(parent, a);

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
			struct ast_node* a = NULL;
			valid = expr(ps, &a) && valid;

			if (!a) {
				valid = set_source_error(ps->el, &loc, "expected expr after comma");
				break;
			}

			/* transfer a -> parent */
			ast_node_add(parent, a);
		}
	}

	return valid;
}

bool parenthesis(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expecting left parenthesis", &lp) && valid;

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate n n{} */
	struct ast_node* a = NULL;
	valid = valid && expr(ps, &a);

	if (!a) {
		valid = set_source_error(ps->el, &loc, "empty parenthesis");
	}

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expecting right parenthesis", &rp) && valid;

	if (valid) {
		ast_node_create(&n);
		n->type = ast_type_parenthesis;

		ast_node_add(n, a);
	} else {
		ast_node_destroy(a);
	}

	if (valid) {
		#pragma warning(suppress:6011)
		if (!a->etype) {
			valid = set_source_error(ps->el, &loc, "parenthesis used on expression with no value");
		} else {
			n->etype = ast_node_copy(a->etype);
		}
	}

	/* destroy lp lp{} */
	token_destroy(lp);
	free(lp);

	/* destroy rp rp{} */
	token_destroy(rp);
	free(rp);

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}
