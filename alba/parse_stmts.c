#include "zinc/result.h"
#include "token.h"
#include "dag.h"
#include "zinc/buffer.h"
#include "parse_expr.h"
#include "input.h"
#include "scan.h"
#include "parse_tools.h"
#include "parse_types.h"
#include "source.h"

/* stmts -> stmt stmts' */
/* dynamic-output ps{} root root{} */
enum result stmts(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	r = stmt(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_stmts;

	/* transfer a -> n{} */
	if (a) {
		dag_add_child(n, a);
	}

	/* allocate ps{} n{} */
	r = stmts_prime(ps, n);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* transfer n -> root */
	*root = n;
	return r;
}

/* stmts' -> separator stmt stmts' | e */
/* dynamic-output ps{} parent{} */
enum parse_result stmts_prime(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int has_separator;

	/* allocate ps{} */
	r = separator(ps, &has_separator);
	if (r == result_error) {
		return r;
	}

	if (!has_separator) {
		return r;
	}

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	r = stmt(ps, &a);
	if (r == result_error) {
		return r;
	}

	/* transfer a -> parent{} */
	if (a) {
		dag_add_child(parent, a);
	}

	/* allocate ps{} parent{} */
	r = stmts_prime(ps, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}

/* separator -> \n | ; */
/* dynamic-output ps{} */
/* dynamic-temp sep sep{} */
enum parse_result separator(struct parse_state* ps, int* has_separator)
{
	enum result r = result_ok;
	enum token_type type;
	int num;
	*has_separator = 0;

	/* allocate ps{} */
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_newline) {
		type = token_newline;
		*has_separator = 1;
	} else if (t0 && t0->type == token_semicolon) {
		type = token_semicolon;
		*has_separator = 1;
	} else {
		return r;
	}

	/* allocate ps{} sep sep{} */
	struct token* sep;
	r = match(ps, type, "expecting newline or semicolon", &sep);
	if (r == result_error) {
		return r;
	}

	/* destroy sep sep{} */
	token_destroy(sep);
	free(sep);

	return r;
}

/**
* @brief stmt -> id = expr
*		| function id (dseq) stmts end
*       | expr
*       | e
*/
/* @param dynamic-output ps{} root root{} */
enum result stmt(struct parse_state* ps, struct dag_node** root)
{
	enum result r;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	r = get_lookahead(ps, 2, &num);
	if (r == result_error) {
		return r;
	}

	/* e */
	if (num <= 0) {
		return r;
	}


	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	/* while */
	if (t0 && t0->type == token_while) {
		/* allocate ps{} */
		struct token* whl = NULL;
		r = match(ps, token_while, "expecting while", &whl);
		if (r == result_error) {
			return r;
		}

		/* destroy whl whl{} */
		token_destroy(whl);
		free(whl);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_while;

		/* allocate ps{} */
		struct location loc;
		r = get_parse_location(ps, &loc);
		if (r == result_error) {
			return r;
		}

		/* allocate ps{} a a{} */
		struct dag_node* a = NULL;
		r = expr(ps, &a);
		if (r == result_error) {
			return r;
		}

		if (!a) {
			/* destroy n n{} */
			dag_destroy(n);
			/* allocate ps{} */
			r = set_source_error(ps->el, &loc, "expected expression after while");
			return r;
		}

		/* transfer a -> n{} */
		dag_add_child(n, a);

		/* allocate ps{} b b{} */
		struct dag_node* b = NULL;
		r = stmts(ps, &b);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* transfer b -> n{} */
		dag_add_child(n, b);

		/* allocate ps{} end end{} */
		struct token* end = NULL;
		r = match(ps, token_end, "expected end", &end);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

	/* for */
	} else if (t0 && t0->type == token_for) {
		/* allocate ps{} */
		r = get_lookahead(ps, 3, &num);
		if (r == result_error) {
			return r;
		}

		struct token* t2 = get_token(&ps->lookahead, 2);

		if (t1 && t1->type == token_id && t2 && t2->type == token_equal) {
			/* allocate ps{} n n{} */
			r = for_range(ps, &n);
			if (r == result_error) {
				return r;
			}
		} else if (t1 && t1->type == token_id && t2 && t2->type == token_in) {
			/* allocate ps{} n n{} */
			r = for_iteration(ps, &n);
			if (r == result_error) {
				return r;
			}
		} else {
			/* allocate ps{} */
			struct token* id = NULL;
			r = match(ps, token_id, "expected id after for", &id);
			if (r == result_error) {
				return r;
			}

			/* destroy id id{} */
			token_destroy(id);
			free(id);

			/* allocate ps{} */
			struct location loc;
			r = get_parse_location(ps, &loc);
			if (r == result_error) {
				return r;
			}
			/* allocate ps{} */
			r = set_source_error(ps->el, &loc, "expected = or in after for and id");
			return r;
		}

		/* function word (seq) stmts end */
	} else if (t0 && t0->type == token_function) {
		/* allocate ps{} f f{} */
		struct token* f = NULL;
		r = match(ps, token_function, "expecting function", &f);
		if (r == result_error) {
			return r;
		}

		/* destroy f f{} */
		token_destroy(f);
		free(f);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_function;

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		r = match(ps, token_id, "expecting word", &id);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* allocate a a{} */
		struct dag_node* a;
		dag_create_node(&a);
		a->type = dag_type_id;
		buffer_copy(&id->value, &a->value);

		/* destroy id id{} */
		token_destroy(id);
		free(id);

		/* tranfer a -> n{} */
		dag_add_child(n, a);

		/* allocate ps{} lp lp{} */
		struct token* lp = NULL;
		r = match(ps, token_left_paren, "expecting left parenthesis", &lp);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* destroy lp lp{} */
		token_destroy(lp);
		free(lp);

		/* allocate ps{} b b{} */
		struct dag_node* b;
		r = dseq(ps, &b);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* transfer b -> n{} */
		if (b) {
			dag_add_child(n, b);
		}

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		r = match(ps, token_right_paren, "expecting right parenthesis", &rp);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* destroy rp rp{} */
		token_destroy(rp);
		free(rp);

		/* allocate ps{} c c{} */
		struct dag_node* c;
		r = stmts(ps, &c);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* transfer c -> n{} */
		if (c) {
			dag_add_child(n, c);
		}

		/* allocate ps{} end end{} */
		struct token* end = NULL;
		r = match(ps, token_end, "expecting end", &end);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* destroy end end{} */
		token_destroy(end);
		free(end);

	} else if (t0 && t0->type == token_if) {
		/* if */

		/* allocate ps{} ift ift{} */
		struct token* ift = NULL;
		r = match(ps, token_if, "expecting if", &ift);
		if (r == result_error) {
			return r;
		}

		/* destroy ift ift{} */
		token_destroy(ift);
		free(ift);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_if;

		/* allocate cb */
		struct dag_node* cb = NULL;
		dag_create_node(&cb);
		cb->type = dag_type_conditional_branch;

		/* transfer cb -> n{} */
		dag_add_child(n, cb);

		/* allocate ps{} */
		struct location loc;
		r = get_parse_location(ps, &loc);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* condition */
		/* allocate ps{} cond cond{} */
		struct dag_node* cond = NULL;
		r = expr(ps, &cond);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		if (cond == NULL) {
			/* destroy n n{}*/
			dag_destroy(n);
			/* allocate ps{} */
			r = set_source_error(ps->el, &loc, "expecting a condition after if");
			return r;
		}

		/* transfer cond -> n{} */
		dag_add_child(cb, cond);

		/* stmts */
		/* allocate ps{} body body{} */
		struct dag_node* body = NULL;
		r = stmts(ps, &body);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* transfer body -> n{} */
		if (body) {
			dag_add_child(cb, body);
		}

		/* elseif_stmts */
		/* allocate n{} */
		r = elseif_stmts(ps, n);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* else_stmt */
		/* allocate ps{] n{} */
		r = else_stmt(ps, n);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

		/* end */
		/* allocate ps{} end end{} */
		struct token* end = NULL;
		r = match(ps, token_end, "expected end", &end);
		if (r == result_error) {
			/* destroy n n{} */
			dag_destroy(n);
			return r;
		}

	/* expr */
	} else {
		/* allocate ps{} n n{} */
		r = expr(ps, &n);
		if (r == result_error) {
			return r;
		}
	}

	/* transfer n -> root */
	*root = n;
	return result_ok;
}

/* for_range -> for id = expr:expr stmts end */
/* dynamic-output ps{} root root{} */
enum result for_range(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	r = match(ps, token_for, "expected for", &f);
	if (r == result_error) {
		return r;
	}

	/* destroy f f{} */
	token_destroy(f);
	free(f);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	r = match(ps, token_id, "expected id", &id);
	if (r == result_error) {
		return r;
	}

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	r = match(ps, token_equal, "expected equal", &equal);
	if (r == result_error) {
		token_destroy(id);
		free(id);
		return r;
	}

	/* destroy equal equal{} */
	token_destroy(equal);
	free(equal);

	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_for_range;

	/* id */
	/* allocate a */
	struct dag_node* a = NULL;
	dag_create_node(&a);
	a->type = dag_type_id;

	/* allocate a{} */
	buffer_copy(&id->value, &a->value);

	/* destroy id id{} */
	token_destroy(id);
	free(id);

	/* transfer a -> n{} */
	dag_add_child(n, a);

	/* allocate ps{} */
	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* start expr */
	/* allocate b b{} */
	struct dag_node* b = NULL;
	r = expr(ps, &b);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	if (!b) {
		/* destroy n n{} */
		dag_destroy(n);

		/* allocate ps{} */
		r = set_source_error(ps->el, &loc, "expected range start after for-range");
		return r;
	}

	/* transfer b -> n */
	dag_add_child(n, b);

	/* allocate ps{} colon conlon{} */
	struct token* colon = NULL;
	r = match(ps, token_colon, "expected colon", &colon);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* allocate ps{} */
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* end expr */
	/* allocate ps{} c c{} */
	struct dag_node* c = NULL;
	r = expr(ps, &c);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	if (!c) {
		/* destroyo n n{} */
		dag_destroy(n);
		/* allocate ps{} */
		r = set_source_error(ps->el, &loc, "expected range end after for-range");
		return r;
	}

	/* transfer c -> n{} */
	dag_add_child(n, c);

	/* stmts */
	/* allocate ps{} d d{} */
	struct dag_node* d = NULL;
	r = stmts(ps, &d);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* transfer d -> n{} */
	dag_add_child(n, d);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	r = match(ps, token_end, "expected end", &end);
	if (r == result_error) {
		/* allocate n n{} */
		dag_destroy(n);
		return r;
	}

	/* destroy end end{} */
	token_destroy(end);
	free(end);

	/* transfer n -> root */
	*root = n;
	return r;
}

/* for_iteration -> for id in expr stmts end */
/* dynamic-output ps{} root root{} */
enum result for_iteration(struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	/* allocate ps{} fort fort{} */
	struct token* fort = NULL;
	r = match(ps, token_for, "expecting for", &fort);
	if (r == result_error) {
		return r;
	}

	/* destroy fort fort{} */
	token_destroy(fort);
	free(fort);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	r = match(ps, token_id, "expecting id", &id);
	if (r == result_error) {
		return r;
	}

	/* allocate ps{} in in{} */
	struct token* in = NULL;
	r = match(ps, token_in, "expecting in", &in);
	if (r == result_error) {
		/* destroy id id{} */
		token_destroy(id);
		free(id);
		return r;
	}

	/* destroy in in{} */
	token_destroy(in);
	free(in);

	/* for */
	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_for_iteration;

	/* id */
	/* allocate a */
	struct dag_node* a = NULL;
	dag_create_node(&a);
	a->type = dag_type_id;

	/* allocate a{} */
	buffer_copy(&id->value, &a->value);

	/* destroy id id{] */
	token_destroy(id);
	free(id);

	/* transfer a -> n{} */
	dag_add_child(n, a);

	/* allocate ps{} */
	struct location loc;
	r = get_parse_location(ps, &loc);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* expr */
	/* allocate ps{} b b{} */
	struct dag_node* b = NULL;
	r = expr(ps, &b);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	if (!b) {
		/* destroy n n{} */
		dag_destroy(n);
		/* allocate ps{} */
		r = set_source_error(ps->el, &loc, "expected expression after for-iteration");
		return r;
	}

	/* transfer b -> n{} */
	dag_add_child(n, b);

	/* stmts */
	/* allocate ps{} c c{} */
	struct dag_node* c = NULL;
	r = stmts(ps, &c);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* transfer c -> n{} */
	dag_add_child(n, c);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	r = match(ps, token_end, "expected end", &end);
	if (r == result_error) {
		/* destroy n n{} */
		dag_destroy(n);
		return r;
	}

	/* transfer n -> root */
	*root = n;
	return r;
}

/* elseif_stmts -> elseif expr stmts elseif_stmts | e */
/* dynamic-output ps{} parent{} */
enum result elseif_stmts(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;

	/* allocate ps{} */
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0->type == token_elseif) {
		/* allocate ps{} eit eit{} */
		struct token* eit = NULL;
		r = match(ps, token_elseif, "expecting elseif", &eit);
		if (r == result_error) {
			return r;
		}

		/* allocate ps{} */
		struct location loc;
		r = get_parse_location(ps, &loc);
		if (r == result_error) {
			return r;
		}

		/* allocate cb */
		struct dag_node* cb = NULL;
		dag_create_node(&cb);
		cb->type = dag_type_conditional_branch;

		/* allocate ps{} cond cond{} */
		struct dag_node* cond = NULL;
		r = expr(ps, &cond);
		if (r == result_error) {
			/* destroy cb */
			dag_destroy(cb);
			return r;
		}

		if (cond == NULL) {
			dag_destroy(cb);
			/* allocate ps{} */
			r = set_source_error(ps->el, &loc, "expecting condition after elseif");
			return r;
		}

		/* transfer cond -> cb{} */
		dag_add_child(cb, cond);

		/* allocate ps{} node node{} */
		struct dag_node* node = NULL;
		r = stmts(ps, &node);
		if (r == result_error) {
			/* destroy cb cb{} */
			dag_destroy(cb);
			return r;
		}

		/* transfer node -> cb{} */
		if (node) {
			dag_add_child(cb, node);
		}

		/* transfer cb -> parent{} */
		dag_add_child(parent, cb);

		/* allocate ps{} parent{} */
		r = elseif_stmts(ps, parent);
		if (r == result_error) {
			return r;
		}
	}

	return r;
}

/* else_stmt -> else stmts | e */
/* dynamic-output ps{} parent{} */
enum result else_stmt(struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;

	/* allocate ps{} */
	r = get_lookahead(ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_else) {
		/* else */
		/* allocate ps{} et et{} */
		struct token* et = NULL;
		r = match(ps, token_else, "expected else", &et);
		if (r == result_error) {
			return r;
		}

		/* destroy et et{} */
		token_destroy(et);
		free(et);

		/* allocate cb */
		struct dag_node* cb = NULL;
		dag_create_node(&cb);
		cb->type = dag_type_default_branch;

		/* stmts */
		/* allocate node node{} */
		struct dag_node* node = NULL;
		r = stmts(ps, &node);
		if (r == result_error) {
			/* destroy cb cb{} */
			dag_destroy(cb);
			return r;
		}


		/* transfer node -> cb{} */
		if (node) {
			dag_add_child(cb, node);
		}

		/* transfer cb -> parent{} */
		dag_add_child(parent, cb);
	}

	return r;
}
