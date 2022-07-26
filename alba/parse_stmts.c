#include <stdbool.h>
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

bool stmts_prime(struct parse_state* ps, struct dag_node* parent);
bool separator(struct parse_state* ps, int* has_separator);
bool stmt(struct parse_state* ps, struct dag_node** root);

/* stmts -> stmt stmts' */
/* dynamic-output ps{} root root{} */
bool stmts(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	valid = valid && stmt(ps, &a);

	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_stmts;

	/* transfer a -> n{} */
	if (a) {
		dag_add_child(n, a);
	}

	/* allocate ps{} n{} */
	valid = valid && stmts_prime(ps, n);

	/* transfer n -> root */
	*root = n;
	return valid;
}

/* stmts' -> separator stmt stmts' | e */
/* dynamic-output ps{} parent{} */
bool stmts_prime(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int has_separator;

	/* allocate ps{} */
	valid = valid && separator(ps, &has_separator);

	if (!has_separator) {
		return valid;
	}

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	valid = valid && stmt(ps, &a);

	/* transfer a -> parent{} */
	if (a) {
		dag_add_child(parent, a);
	}

	/* allocate ps{} parent{} */
	valid = valid && stmts_prime(ps, parent);

	return valid;
}

/* separator -> \n | ; */
/* dynamic-output ps{} */
/* dynamic-temp sep sep{} */
bool separator(struct parse_state* ps, int* has_separator)
{
	bool valid = true;
	enum token_type type;
	int num;
	*has_separator = 0;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_newline) {
		type = token_newline;
		*has_separator = 1;
	} else if (t0 && t0->type == token_semicolon) {
		type = token_semicolon;
		*has_separator = 1;
	} else {
		return valid;
	}

	/* allocate ps{} sep sep{} */
	struct token* sep = NULL;
	valid = valid && match(ps, type, "expecting newline or semicolon", &sep);

	/* destroy sep sep{} */
	token_destroy(sep);
	free(sep);

	return valid;
}

/**
* @brief stmt -> id = expr
*		| function id (dseq) stmts end
*       | expr
*       | e
*/
/* @param dynamic-output ps{} root root{} */
bool stmt(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 2, &num);

	/* e */
	if (num <= 0) {
		return valid;
	}


	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	/* while */
	if (t0 && t0->type == token_while) {
		/* allocate ps{} */
		struct token* whl = NULL;
		valid = valid && match(ps, token_while, "expecting while", &whl);

		/* destroy whl whl{} */
		token_destroy(whl);
		free(whl);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_while;

		/* allocate ps{} */
		struct location loc;
		valid = valid && get_parse_location(ps, &loc);

		/* allocate ps{} a a{} */
		struct dag_node* a = NULL;
		valid = valid && expr(ps, &a);

		if (!a) {
			/* destroy n n{} */
			dag_destroy(n);
			/* allocate ps{} */
			set_source_error(ps->el, &loc, "expected expression after while");
			return valid;
		}

		/* transfer a -> n{} */
		dag_add_child(n, a);

		/* allocate ps{} b b{} */
		struct dag_node* b = NULL;
		valid = valid && stmts(ps, &b);
		if (!b) {
			/* destroy n n{} */
			dag_destroy(n);
			return valid;
		}

		/* transfer b -> n{} */
		dag_add_child(n, b);

		/* allocate ps{} end end{} */
		struct token* end = NULL;
		valid = valid && match(ps, token_end, "expected end", &end);

	/* for */
	} else if (t0 && t0->type == token_for) {
		/* allocate ps{} */
		valid = valid && get_lookahead(ps, 3, &num);

		struct token* t2 = get_token(&ps->lookahead, 2);

		if (t1 && t1->type == token_id && t2 && t2->type == token_equal) {
			/* allocate ps{} n n{} */
			valid = valid && for_range(ps, &n);
		} else if (t1 && t1->type == token_id && t2 && t2->type == token_in) {
			/* allocate ps{} n n{} */
			valid = valid && for_iteration(ps, &n);
		} else {
			/* allocate ps{} */
			struct token* id = NULL;
			valid = valid && match(ps, token_id, "expected id after for", &id);

			/* destroy id id{} */
			token_destroy(id);
			free(id);

			/* allocate ps{} */
			struct location loc;
			valid = valid && get_parse_location(ps, &loc);
			/* allocate ps{} */
			set_source_error(ps->el, &loc, "expected = or in after for and id");
			valid = false;
		}

		/* function word (seq) stmts end */
	} else if (t0 && t0->type == token_function) {
		/* allocate ps{} f f{} */
		struct token* f = NULL;
		valid = valid && match(ps, token_function, "expecting function", &f);

		/* destroy f f{} */
		token_destroy(f);
		free(f);

		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_function;

		/* allocate ps{} id id{} */
		struct token* id = NULL;
		valid = valid && match(ps, token_id, "expecting word", &id);

		/* allocate a a{} */
		struct dag_node* a;
		dag_create_node(&a);
		a->type = dag_type_id;
		if (id) {
			buffer_copy(&id->value, &a->value);
		}

		/* destroy id id{} */
		token_destroy(id);
		free(id);

		/* tranfer a -> n{} */
		dag_add_child(n, a);

		/* allocate ps{} lp lp{} */
		struct token* lp = NULL;
		valid = valid && match(ps, token_left_paren, "expecting left parenthesis", &lp);

		/* destroy lp lp{} */
		token_destroy(lp);
		free(lp);

		/* allocate ps{} b b{} */
		struct dag_node* b = NULL;
		valid = valid && dseq(ps, &b);

		/* transfer b -> n{} */
		if (b) {
			dag_add_child(n, b);
		}

		/* allocate ps{} rp rp{} */
		struct token* rp = NULL;
		valid = valid && match(ps, token_right_paren, "expecting right parenthesis", &rp);

		/* destroy rp rp{} */
		token_destroy(rp);
		free(rp);

		/* allocate ps{} c c{} */
		struct dag_node* c = NULL;
		valid = valid && stmts(ps, &c);

		/* transfer c -> n{} */
		if (c) {
			dag_add_child(n, c);
		}

		/* allocate ps{} end end{} */
		struct token* end = NULL;
		valid = valid && match(ps, token_end, "expecting end", &end);

		/* destroy end end{} */
		token_destroy(end);
		free(end);

	} else if (t0 && t0->type == token_if) {
		/* if */

		/* allocate ps{} ift ift{} */
		struct token* ift = NULL;
		valid = valid && match(ps, token_if, "expecting if", &ift);

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
		valid = valid && get_parse_location(ps, &loc);

		/* condition */
		/* allocate ps{} cond cond{} */
		struct dag_node* cond = NULL;
		valid = valid && expr(ps, &cond);

		if (cond == NULL) {
			set_source_error(ps->el, &loc, "expecting a condition after if");
			valid = false;
			return valid;
		} else {
			/* transfer cond -> n{} */
			dag_add_child(cb, cond);

		}

		/* stmts */
		/* allocate ps{} body body{} */
		struct dag_node* body = NULL;
		valid = valid && stmts(ps, &body);

		/* transfer body -> n{} */
		if (body) {
			dag_add_child(cb, body);
		}

		/* elseif_stmts */
		/* allocate n{} */
		valid = valid && elseif_stmts(ps, n);

		/* else_stmt */
		/* allocate ps{] n{} */
		valid = valid && else_stmt(ps, n);

		/* end */
		/* allocate ps{} end end{} */
		struct token* end = NULL;
		valid = valid && match(ps, token_end, "expected end", &end);

	/* expr */
	} else {
		/* allocate ps{} n n{} */
		valid = valid && expr(ps, &n);
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}
	return valid;
}

/* for_range -> for id = expr:expr stmts end */
/* dynamic-output ps{} root root{} */
enum result for_range(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = valid && match(ps, token_for, "expected for", &f);

	/* destroy f f{} */
	token_destroy(f);
	free(f);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = valid && match(ps, token_id, "expected id", &id);

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	valid = valid && match(ps, token_equal, "expected equal", &equal);

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
	if (id) {
		buffer_copy(&id->value, &a->value);
	}

	/* destroy id id{} */
	token_destroy(id);
	free(id);

	/* transfer a -> n{} */
	dag_add_child(n, a);

	/* allocate ps{} */
	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* start expr */
	/* allocate b b{} */
	struct dag_node* b = NULL;
	valid = valid && expr(ps, &b);

	if (!b) {
		/* allocate ps{} */
		set_source_error(ps->el, &loc, "expected range start after for-range");
	} else {
		/* transfer b -> n */
		dag_add_child(n, b);
	}

	/* allocate ps{} colon conlon{} */
	struct token* colon = NULL;
	valid = valid && match(ps, token_colon, "expected colon", &colon);

	/* allocate ps{} */
	valid = valid && get_parse_location(ps, &loc);

	/* end expr */
	/* allocate ps{} c c{} */
	struct dag_node* c = NULL;
	valid = valid && expr(ps, &c);

	if (!c) {
		set_source_error(ps->el, &loc, "expected range end after for-range");
	} else {
		/* transfer c -> n{} */
		dag_add_child(n, c);
	}

	/* stmts */
	/* allocate ps{} d d{} */
	struct dag_node* d = NULL;
	valid = valid && stmts(ps, &d);

	/* transfer d -> n{} */
	if (d) {
		dag_add_child(n, d);
	}

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = valid && match(ps, token_end, "expected end", &end);

	/* destroy end end{} */
	token_destroy(end);
	free(end);

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}

	return valid;
}

/* for_iteration -> for id in expr stmts end */
/* dynamic-output ps{} root root{} */
enum result for_iteration(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} fort fort{} */
	struct token* fort = NULL;
	valid = valid && match(ps, token_for, "expecting for", &fort);

	/* destroy fort fort{} */
	token_destroy(fort);
	free(fort);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = valid && match(ps, token_id, "expecting id", &id);

	/* allocate ps{} in in{} */
	struct token* in = NULL;
	valid = valid && match(ps, token_in, "expecting in", &in);

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
	if (id) {
		buffer_copy(&id->value, &a->value);
	}

	/* destroy id id{] */
	token_destroy(id);
	free(id);

	/* transfer a -> n{} */
	dag_add_child(n, a);

	/* allocate ps{} */
	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* expr */
	/* allocate ps{} b b{} */
	struct dag_node* b = NULL;
	valid = valid && expr(ps, &b);

	if (!b) {
		set_source_error(ps->el, &loc, "expected expression after for-iteration");
	} else {
		/* transfer b -> n{} */
		dag_add_child(n, b);
	}

	/* stmts */
	/* allocate ps{} c c{} */
	struct dag_node* c = NULL;
	valid = valid && stmts(ps, &c);

	if (c) {
		/* transfer c -> n{} */
		dag_add_child(n, c);
	}

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = valid && match(ps, token_end, "expected end", &end);

	if (valid) {
		/* transfer n -> root */
		*root = n;
	}

	return valid;
}

/* elseif_stmts -> elseif expr stmts elseif_stmts | e */
/* dynamic-output ps{} parent{} */
enum result elseif_stmts(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0->type == token_elseif) {
		/* allocate ps{} eit eit{} */
		struct token* eit = NULL;
		valid = valid && match(ps, token_elseif, "expecting elseif", &eit);

		/* allocate ps{} */
		struct location loc;
		valid = valid && get_parse_location(ps, &loc);

		/* allocate cb */
		struct dag_node* cb = NULL;
		dag_create_node(&cb);
		cb->type = dag_type_conditional_branch;

		/* allocate ps{} cond cond{} */
		struct dag_node* cond = NULL;
		valid = expr(ps, &cond);

		if (!cond) {
			/* allocate ps{} */
			set_source_error(ps->el, &loc, "expecting condition after elseif");
			valid = false;
		} else {
			/* transfer cond -> cb{} */
			dag_add_child(cb, cond);
		}


		/* allocate ps{} node node{} */
		struct dag_node* node = NULL;
		valid = valid && stmts(ps, &node);

		/* transfer node -> cb{} */
		if (node) {
			dag_add_child(cb, node);
		}

		/* transfer cb -> parent{} */
		dag_add_child(parent, cb);

		/* allocate ps{} parent{} */
		valid = valid && elseif_stmts(ps, parent);
	}

	return valid;
}

/* else_stmt -> else stmts | e */
/* dynamic-output ps{} parent{} */
enum result else_stmt(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int num;

	/* allocate ps{} */
	valid = valid && get_lookahead(ps, 1, &num);

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_else) {
		/* else */
		/* allocate ps{} et et{} */
		struct token* et = NULL;
		valid = valid && match(ps, token_else, "expected else", &et);

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
		valid = valid && stmts(ps, &node);

		/* transfer node -> cb{} */
		if (node) {
			dag_add_child(cb, node);
		}

		/* transfer cb -> parent{} */
		dag_add_child(parent, cb);
	}

	return valid;
}
