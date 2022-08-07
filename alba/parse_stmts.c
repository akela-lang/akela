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
#include "symbol_table.h"
#include "zinc/memory.h"
#include "symbol_table.h"

bool stmts_prime(struct parse_state* ps, struct dag_node* parent);
bool separator(struct parse_state* ps, int* has_separator);
bool stmt(struct parse_state* ps, struct dag_node** root);
bool function(struct parse_state* ps, struct dag_node** root);
bool function_start(struct parse_state* ps, struct dag_node** root);
bool function_finish(struct parse_state* ps, struct dag_node* fd);
bool for_range(struct parse_state* ps, struct dag_node** root);
bool for_iteration(struct parse_state* ps, struct dag_node** root);
bool for_iteration_start(struct parse_state* ps, struct dag_node** root);
bool for_iteration_finish(struct parse_state* ps, struct dag_node* n);
bool elseif_stmts(struct parse_state* ps, struct dag_node* parent);
bool else_stmt(struct parse_state* ps, struct dag_node* parent);

/* stmts -> stmt stmts' */
/* dynamic-output ps{} root root{} */
bool stmts(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* transfer ps{top} -> saved */
	struct environment* saved = ps->st->top;

	/* allocate env */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);

	/* share env -> top */
	ps->st->top = env;

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

	/* destroy env env{} */
	environment_destroy(env);

	/* transfer saved -> ps{top} */
	ps->st->top = saved;

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
	enum token_enum type;
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
			valid = valid && match(ps, token_id, "expected identifier after for", &id);

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
		valid = valid && function(ps, &n);

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

bool function(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;

	/* shared ps{top} -> saved */
	struct environment* saved = ps->st->top;

	/* allocate env env{} */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct dag_node* fd = NULL;
	valid = valid && function_start(ps, &fd);

	valid = valid && function_finish(ps, fd);

	if (valid) {
		*root = fd;
	}

	/* transfer saved -> ps->st->top */
	ps->st->top = saved;

	/* destroy env env{} */
	environment_destroy(env);

	return valid;
}

bool function_start(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = valid && match(ps, token_function, "expecting function", &f);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = valid && match(ps, token_id, "expecting identifier", &id);
	struct symbol* sym = NULL;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = valid && match(ps, token_left_paren, "expecting left parenthesis", &lp);

	/* allocate ps{} dseq_node dseq_node{} */
	struct dag_node* dseq_node = NULL;
	valid = valid && dseq(ps, &dseq_node);

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = valid && match(ps, token_right_paren, "expecting right parenthesis", &rp);

	struct dag_node* dret_node = NULL;
	int num;
	valid = valid && get_lookahead(ps, 1, &num);
	struct token* next = get_token(&ps->lookahead, 0);
	if (next && next->type == token_double_colon) {
		struct token* dc = NULL;
		valid = valid && match(ps, token_double_colon, "expecting double colon", &dc);
		token_destroy(dc);
		free(dc);

		valid = valid && type(ps, NULL, &dret_node);
	}

	/* start building nodes */
	if (valid) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_function;

		/* allocate a a{} */
		struct dag_node* a;
		dag_create_node(&a);
		a->type = dag_type_id;
		buffer_copy(&id->value, &a->value);

		/* tranfer a -> n{} */
		dag_add_child(n, a);

		/* transfer dseq_node -> n{} */
		dag_add_child(n, dseq_node);

		struct dag_node* b;
		dag_create_node(&b);
		b->type = dag_type_dret;

		if (dret_node) {
			dag_add_child(b, dret_node);
		}

		dag_add_child(n, b);

		struct symbol* search = environment_get_local(ps->st->top->prev, &id->value);
		if (search) {
			struct location loc;
			get_token_location(id, &loc);
			char* a;
			buffer2array(&id->value, &a);
			valid = set_source_error(ps->el, &loc, "duplicate declaration in same scope: %s", a);
			free(a);
		} else {
			malloc_safe((void**)&sym, sizeof(struct symbol));
			symbol_init(sym);
			sym->tk_type = id->type;
			sym->dec = n;
			environment_put(ps->st->top->prev, &id->value, sym);
		}

		*root = n;
	} else {
		dag_destroy(dseq_node);
	}

	/* destroy f f{} id id{} lp lp{} rp rp{} */
	token_destroy(f);
	free(f);
	token_destroy(id);
	free(id);
	token_destroy(lp);
	free(lp);
	token_destroy(rp);
	free(rp);

	return valid;
}

bool function_finish(struct parse_state* ps, struct dag_node* fd)
{
	bool valid = true;

	/* allocate ps{} stmts_node stmts_node{} */
	struct dag_node* stmts_node = NULL;
	valid = valid && stmts(ps, &stmts_node);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = valid && match(ps, token_end, "expecting end", &end);

	/* finish building nodes */
	if (valid) {
		/* transfer stmts_node -> n{} */
		dag_add_child(fd, stmts_node);

	} else {
		dag_destroy(fd);
		dag_destroy(stmts_node);
	}

	/* destroy end end{} */
	token_destroy(end);
	free(end);

	return valid;
}

/* for_range -> for id = expr:expr stmts end */
/* dynamic-output ps{} root root{} */
bool for_range(struct parse_state* ps, struct dag_node** root)
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
bool for_iteration(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	valid = valid && for_iteration_start(ps, &n);
	valid = valid && for_iteration_finish(ps, n);

	if (valid) {
		*root = n;
	}

	ps->st->top = saved;
	environment_destroy(env);

	return valid;
}

bool for_iteration_start(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} fort fort{} */
	struct token* for_token = NULL;
	valid = valid && match(ps, token_for, "expecting for", &for_token);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = valid && match(ps, token_id, "expecting id", &id);

	/* allocate ps{} in in{} */
	struct token* in = NULL;
	valid = valid && match(ps, token_in, "expecting in", &in);

	/* allocate ps{} */
	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* expr */
	/* allocate ps{} b b{} */
	struct dag_node* list = NULL;
	valid = valid && expr(ps, &list);

	if (!list) {
		set_source_error(ps->el, &loc, "expected expression after for-iteration");
		valid = false;
	}

	if (valid) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_for_iteration;

		/* allocate element */
		struct dag_node* element = NULL;
		dag_create_node(&element);
		element->type = dag_type_id;

		/* allocate element{} */
		if (id) {
			buffer_copy(&id->value, &element->value);
		}

		/* transfer element -> n{} */
		dag_add_child(n, element);

		/* transfer list -> n{} */
		dag_add_child(n, list);

		/* transfer n -> root */
		*root = n;

		struct symbol* sym = NULL;
		malloc_safe(&sym, sizeof(struct symbol));
		symbol_init(sym);
		#pragma warning(suppress:6011)
		sym->tk_type = id->type;
		/* should get type information from list */
		#pragma warning(suppress:6011)
		environment_put(ps->st->top, &id->value, sym);

	} else {
		dag_destroy(list);
	}

	token_destroy(for_token);
	free(for_token);
	token_destroy(id);
	free(id);
	token_destroy(in);
	free(in);

	return valid;
}

bool for_iteration_finish(struct parse_state* ps, struct dag_node* n)
{
	bool valid = true;

	if (!n) {
		valid = false;
	}

	/* stmts */
	/* allocate ps{} c c{} */
	struct dag_node* stmts_node = NULL;
	valid = valid && stmts(ps, &stmts_node);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = valid && match(ps, token_end, "expected end", &end);

	if (valid) {
		/* transfer stmts_node -> n{} */
		dag_add_child(n, stmts_node);
	} else {
		dag_destroy(n);
		dag_destroy(stmts_node);
	}

	token_destroy(end);
	free(end);

	return valid;
}

/* elseif_stmts -> elseif expr stmts elseif_stmts | e */
/* dynamic-output ps{} parent{} */
bool elseif_stmts(struct parse_state* ps, struct dag_node* parent)
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
bool else_stmt(struct parse_state* ps, struct dag_node* parent)
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
