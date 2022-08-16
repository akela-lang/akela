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
#include "parse_stmts.h"

bool separator(struct parse_state* ps, int* has_separator);
bool stmt(struct parse_state* ps, struct dag_node** root);
bool while_nt(struct parse_state* ps, struct dag_node** root);
bool for_nt(struct parse_state* ps, struct dag_node** root);
bool for_range(struct parse_state* ps, struct dag_node** root);
bool for_range_start(struct parse_state* ps, struct dag_node** root);
bool for_range_finish(struct parse_state* ps, struct dag_node* n);
bool for_iteration(struct parse_state* ps, struct dag_node** root);
bool for_iteration_start(struct parse_state* ps, struct dag_node** root);
bool for_iteration_finish(struct parse_state* ps, struct dag_node* n);
bool function(struct parse_state* ps, struct dag_node** root);
bool function_start(struct parse_state* ps, struct dag_node** root);
bool function_finish(struct parse_state* ps, struct dag_node* fd);
bool elseif_nt(struct parse_state* ps, struct dag_node* parent);
bool else_nt(struct parse_state* ps, struct dag_node* parent);

/* stmts -> stmt stmts' */
/* stmts' -> separator stmt stmts' | e */
/* dynamic-output ps{} root root{} */
bool stmts(struct parse_state* ps, bool suppress_env, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	struct environment* saved = NULL;
	struct environment* env = NULL;
	if (!suppress_env) {
		/* transfer ps{top} -> saved */
		saved = ps->st->top;

		/* allocate env */
		malloc_safe((void**)&env, sizeof(struct environment));
		environment_init(env, saved);

		/* share env -> top */
		ps->st->top = env;
	}

	/* allocate n */
	dag_create_node(&n);
	n->type = dag_type_stmts;
	*root = n;

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	valid = stmt(ps, &a) && valid;

	/* transfer a -> n{} */
	if (a) {
		dag_add_child(n, a);
	}

	while (true) {
		/* allocate ps{} */
		int has_separator = 0;
		valid = separator(ps, &has_separator) && valid;

		if (!has_separator) {
			break;
		}

		/* allocate ps{} a a{} */
		struct dag_node* b = NULL;
		valid = stmt(ps, &b) && valid;

		if (b) {
			dag_add_child(n, b);
		}
	}

	if (!suppress_env) {
		/* destroy env env{} */
		environment_destroy(env);

		/* transfer saved -> ps{top} */
		ps->st->top = saved;
	}

	/* transfer n -> root */
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
	valid = get_lookahead(ps, 1, &num) && valid;

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
	valid = match(ps, type, "expecting newline or semicolon", &sep) && valid;

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
	valid = get_lookahead(ps, 2, &num) && valid;

	/* e */
	if (num <= 0) {
		return valid;
	}


	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	/* while */
	if (t0 && t0->type == token_while) {
		valid = while_nt(ps, &n) && valid;

	/* for */
	} else if (t0 && t0->type == token_for) {
		valid = for_nt(ps, &n) && valid;

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
		valid = valid && stmts(ps, false, &body);

		/* transfer body -> n{} */
		if (body) {
			dag_add_child(cb, body);
		}

		/* elseif_nt */
		/* allocate n{} */
		valid = valid && elseif_nt(ps, n);

		/* else_nt */
		/* allocate ps{] n{} */
		valid = valid && else_nt(ps, n);

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

bool while_nt(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} */
	struct token* whl = NULL;
	valid = match(ps, token_while, "expecting while", &whl) && valid;

	/* allocate ps{} */
	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} a a{} */
	struct dag_node* a = NULL;
	valid = expr(ps, &a) && valid;

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc, "expected expression after while");
	}

	/* allocate ps{} b b{} */
	struct dag_node* b = NULL;
	valid = stmts(ps, false, &b) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;

	if (valid) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_while;

		/* transfer a -> n{} */
		dag_add_child(n, a);

		/* transfer b -> n{} */
		dag_add_child(n, b);

		/* transfer n -> root */
		*root = n;

	} else {
		dag_destroy(a);
		dag_destroy(b);
	}

	token_destroy(whl);
	free(whl);
	token_destroy(end);
	free(end);

	return valid;
}

bool for_nt(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	int num;
	struct dag_node* n = NULL;

	/* allocate ps{} */
	valid = get_lookahead(ps, 3, &num) && valid;
	struct token* t1 = get_token(&ps->lookahead, 1);
	struct token* t2 = get_token(&ps->lookahead, 2);

	if (t1 && t1->type == token_id && t2 && t2->type == token_equal) {
		/* allocate ps{} n n{} */
		valid = for_range(ps, &n) && valid;
	} else if (t1 && t1->type == token_id && t2 && t2->type == token_in) {
		/* allocate ps{} n n{} */
		valid = for_iteration(ps, &n) && valid;
	} else {
		/* allocate ps{} */
		struct token* id = NULL;
		valid = match(ps, token_id, "expected identifier after for", &id) && valid;

		/* destroy id id{} */
		token_destroy(id);
		free(id);

		/* allocate ps{} */
		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc, "expected = or in after for and id");
	}

	if (valid) {
		*root = n;
	}

	return valid;
}

/* for_range -> for id = expr:expr stmts end */
/* dynamic-output ps{} root root{} */
bool for_range(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	valid = for_range_start(ps, &n);
	valid = for_range_finish(ps, n);

	if (valid) {
		*root = n;
	} else {
		dag_destroy(n);
	}

	ps->st->top = saved;
	environment_destroy(env);

	return valid;
}

bool for_range_start(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;
	struct dag_node* n = NULL;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = match(ps, token_for, "expected for", &f) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expected id", &id) && valid;

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	valid = match(ps, token_equal, "expected equal", &equal) && valid;

	/* allocate ps{} */
	struct location a_loc;
	valid = get_parse_location(ps, &a_loc) && valid;

	/* start expr */
	/* allocate b b{} */
	struct dag_node* a = NULL;
	valid = expr(ps, &a) && valid;
	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &a_loc, "expected range start after for-range");
	}

	/* allocate ps{} colon conlon{} */
	struct token* colon = NULL;
	valid = match(ps, token_colon, "expected colon", &colon) && valid;

	/* allocate ps{} */
	struct location b_loc;
	valid = get_parse_location(ps, &b_loc) && valid;

	/* end expr */
	/* allocate ps{} c c{} */
	struct dag_node* b = NULL;
	valid = expr(ps, &b) && valid;
	if (!b) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &b_loc, "expected range end after for-range");
	}

	if (valid) {
		/* allocate n */
		dag_create_node(&n);
		n->type = dag_type_for_range;

		/* id */
		/* allocate a */
		struct dag_node* id_node = NULL;
		dag_create_node(&id_node);
		id_node->type = dag_type_id;

		/* allocate a{} */
		buffer_copy(&id->value, &id_node->value);

		/* transfer a -> n{} */
		dag_add_child(n, id_node);
		dag_add_child(n, a);
		dag_add_child(n, b);
		*root = n;

		struct symbol* sym = NULL;
		malloc_safe(&sym, sizeof(struct symbol));
		symbol_init(sym);
		#pragma warning(suppress:6011)
		sym->tk_type = id->type;
		sym->dec = n;
		/* should get type information from list */
		#pragma warning(suppress:6011)
		environment_put(ps->st->top, &id->value, sym);

	} else {
		dag_destroy(a);
		dag_destroy(b);
	}

	/* destroy end end{} */
	token_destroy(f);
	free(f);
	token_destroy(id);
	free(id);
	token_destroy(equal);
	free(equal);
	token_destroy(colon);
	free(colon);

	return valid;
}

bool for_range_finish(struct parse_state* ps, struct dag_node* n)
{
	bool valid = true;

	if (!n) {
		valid = false;
	}

	/* stmts */
	/* allocate ps{} d d{} */
	struct dag_node* c = NULL;
	valid = stmts(ps, true, &c) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;

	if (valid) {
		dag_add_child(n, c);	
	} else {
		dag_destroy(c);
	}

	/* destroy end end{} */
	token_destroy(end);
	free(end);

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

	valid = for_iteration_start(ps, &n) && valid;
	valid = for_iteration_finish(ps, n) && valid;

	if (valid) {
		*root = n;
	} else {
		dag_destroy(n);
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
	valid = match(ps, token_for, "expecting for", &for_token) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting id", &id) && valid;

	/* allocate ps{} in in{} */
	struct token* in = NULL;
	valid = match(ps, token_in, "expecting in", &in) && valid;

	/* allocate ps{} */
	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* expr */
	/* allocate ps{} b b{} */
	struct dag_node* list = NULL;
	valid = expr(ps, &list) && valid;

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
		sym->dec = n;
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
	valid = stmts(ps, true, &stmts_node) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;

	if (valid) {
		/* transfer stmts_node -> n{} */
		dag_add_child(n, stmts_node);
	} else {
		dag_destroy(stmts_node);
	}

	token_destroy(end);
	free(end);

	return valid;
}

bool function(struct parse_state* ps, struct dag_node** root)
{
	bool valid = true;

	if (!symbol_table_is_global(ps->st)) {
		struct location loc;
		get_parse_location(ps, &loc);
		valid = set_source_error(ps->el, &loc, "function declaration is not in global scope");
	}

	/* shared ps{top} -> saved */
	struct environment* saved = ps->st->top;

	/* allocate env env{} */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct dag_node* fd = NULL;
	valid = function_start(ps, &fd) && valid;

	valid = function_finish(ps, fd) && valid;

	if (valid) {
		*root = fd;
	} else {
		dag_destroy(fd);
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
	valid = match(ps, token_function, "expecting function", &f) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting identifier", &id) && valid;
	struct symbol* sym = NULL;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expecting left parenthesis", &lp) && valid;

	/* allocate ps{} dseq_node dseq_node{} */
	struct dag_node* dseq_node = NULL;
	valid = dseq(ps, &dseq_node) && valid;

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expecting right parenthesis", &rp) && valid;

	struct dag_node* dret_node = NULL;
	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* next = get_token(&ps->lookahead, 0);
	if (next && next->type == token_double_colon) {
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expecting double colon", &dc) && valid;
		token_destroy(dc);
		free(dc);

		valid = type(ps, NULL, &dret_node) && valid;
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
	valid = stmts(ps, true, &stmts_node) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expecting end", &end) && valid;

	/* finish building nodes */
	if (valid) {
		/* transfer stmts_node -> n{} */
		dag_add_child(fd, stmts_node);

	} else {
		dag_destroy(stmts_node);
	}

	/* destroy end end{} */
	token_destroy(end);
	free(end);

	return valid;
}

/* elseif_nt -> elseif expr stmts elseif_nt | e */
/* dynamic-output ps{} parent{} */
bool elseif_nt(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int num;

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0->type == token_elseif) {
		/* allocate ps{} eit eit{} */
		struct token* eit = NULL;
		valid = match(ps, token_elseif, "expecting elseif", &eit) && valid;

		/* allocate ps{} */
		struct location loc;
		valid = get_parse_location(ps, &loc) && valid;

		/* allocate cb */
		struct dag_node* cb = NULL;
		dag_create_node(&cb);
		cb->type = dag_type_conditional_branch;

		/* allocate ps{} cond cond{} */
		struct dag_node* cond = NULL;
		valid = expr(ps, &cond) && valid;

		if (!cond) {
			/* allocate ps{} */
			valid = set_source_error(ps->el, &loc, "expecting condition after elseif");
		} else {
			/* transfer cond -> cb{} */
			dag_add_child(cb, cond);
		}


		/* allocate ps{} node node{} */
		struct dag_node* node = NULL;
		valid = stmts(ps, false, &node) && valid;

		/* transfer node -> cb{} */
		if (node) {
			dag_add_child(cb, node);
		}

		/* transfer cb -> parent{} */
		dag_add_child(parent, cb);

		/* allocate ps{} parent{} */
		valid = elseif_nt(ps, parent) && valid;
	}

	return valid;
}

/* else_nt -> else stmts | e */
/* dynamic-output ps{} parent{} */
bool else_nt(struct parse_state* ps, struct dag_node* parent)
{
	bool valid = true;
	int num;

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_else) {
		/* else */
		/* allocate ps{} et et{} */
		struct token* et = NULL;
		valid = match(ps, token_else, "expected else", &et) && valid;

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
		valid = stmts(ps, false, &node) && valid;

		/* transfer node -> cb{} */
		if (node) {
			dag_add_child(cb, node);
		}

		/* transfer cb -> parent{} */
		dag_add_child(parent, cb);
	}

	return valid;
}
