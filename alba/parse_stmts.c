#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
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
#include "type_use.h"
#include <assert.h>

bool separator(struct parse_state* ps, int* has_separator);
bool stmt(struct parse_state* ps, struct ast_node** root);
bool if_nt(struct parse_state* ps, struct ast_node** root);
bool elseif_nt(struct parse_state* ps, struct ast_node* parent);
bool else_nt(struct parse_state* ps, struct ast_node* parent);
bool while_nt(struct parse_state* ps, struct ast_node** root);
bool for_nt(struct parse_state* ps, struct ast_node** root);
bool for_range(struct parse_state* ps, struct ast_node* parent);
bool for_iteration(struct parse_state* ps, struct ast_node* parent);
bool function(struct parse_state* ps, struct ast_node** root);
bool function_start(struct parse_state* ps, struct ast_node** root);
bool function_finish(struct parse_state* ps, struct ast_node* fd);

/* stmts -> stmt stmts' */
/* stmts' -> separator stmt stmts' | e */
/* dynamic-output ps{} root root{} */
bool stmts(struct parse_state* ps, bool suppress_env, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* last = NULL;

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
	ast_node_create(&n);
	n->type = ast_type_stmts;
	*root = n;

	/* allocate ps{} a a{} */
	struct ast_node* a = NULL;
	valid = stmt(ps, &a) && valid;

	/* transfer a -> n{} */
	if (a) {
		ast_node_add(n, a);
		last = a;
	}

	while (true) {
		/* allocate ps{} */
		int has_separator = 0;
		valid = separator(ps, &has_separator) && valid;

		if (!has_separator) {
			break;
		}

		/* allocate ps{} a a{} */
		struct ast_node* b = NULL;
		valid = stmt(ps, &b) && valid;

		if (b) {
			ast_node_add(n, b);
			last = b;
		}
	}

	if (!suppress_env) {
		/* destroy env env{} */
		environment_destroy(env);

		/* transfer saved -> ps{top} */
		ps->st->top = saved;
	}

	if (valid) {
		if (last) {
			if (last->tu) {
				n->tu = type_use_copy(last->tu);
			}
		}
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
bool stmt(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;
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
		valid = function(ps, &n) && valid;

	} else if (t0 && t0->type == token_if) {
		valid = if_nt(ps, &n) && valid;

	/* expr */
	} else {
		/* allocate ps{} n n{} */
		struct location loc_expr;
		valid = valid && expr(ps, &n, &loc_expr);
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}
	return valid;
}

bool if_nt(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate ps{} ift ift{} */
	struct token* ift = NULL;
	valid = valid && match(ps, token_if, "expecting if", &ift);

	/* destroy ift ift{} */
	token_destroy(ift);
	free(ift);

	/* allocate n */
	ast_node_create(&n);
	n->type = ast_type_if;

	/* allocate cb */
	struct ast_node* cb = NULL;
	ast_node_create(&cb);
	cb->type = ast_type_conditional_branch;

	/* transfer cb -> n{} */
	ast_node_add(n, cb);

	/* allocate ps{} */
	struct location loc;
	valid = valid && get_parse_location(ps, &loc);

	/* condition */
	/* allocate ps{} cond cond{} */
	struct ast_node* cond = NULL;
	struct location loc_expr;
	valid = valid && expr(ps, &cond, &loc_expr);

	if (cond == NULL) {
		set_source_error(ps->el, &loc_expr, "expecting a condition after if");
		valid = false;
		return valid;
	} else {
		/* transfer cond -> n{} */
		ast_node_add(cb, cond);

	}

	/* stmts */
	/* allocate ps{} body body{} */
	struct ast_node* body = NULL;
	valid = valid && stmts(ps, false, &body);

	/* transfer body -> n{} */
	if (body) {
		ast_node_add(cb, body);
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

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

bool while_nt(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate ps{} */
	struct token* whl = NULL;
	valid = match(ps, token_while, "expecting while", &whl) && valid;

	/* allocate ps{} a a{} */
	struct ast_node* a = NULL;
	struct location loc_expr;
	valid = expr(ps, &a, &loc_expr) && valid;

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_expr, "expected expression after while");
	}

	/* allocate ps{} b b{} */
	struct ast_node* b = NULL;
	valid = stmts(ps, false, &b) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;

	if (valid) {
		/* allocate n */
		ast_node_create(&n);
		n->type = ast_type_while;

		/* transfer a -> n{} */
		ast_node_add(n, a);

		/* transfer b -> n{} */
		ast_node_add(n, b);

		/* transfer n -> root */
		*root = n;

	} else {
		ast_node_destroy(a);
		ast_node_destroy(b);
	}

	token_destroy(whl);
	free(whl);
	token_destroy(end);
	free(end);

	return valid;
}

bool for_nt(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	int num;
	struct ast_node* n = NULL;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = match(ps, token_for, "expected for", &f) && valid;

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct ast_node* dec = NULL;
	struct location loc_dec;
	valid = declaration(ps, &dec, &loc_dec) && valid;

	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	if (t0 && t0->type == token_equal) {
		ast_node_create(&n);
		n->type = ast_type_for_range;
		if (dec) {
			ast_node_add(n, dec);
		}
		valid = for_range(ps, n) && valid;

	} else if (t0 && t0->type == token_in) {
		ast_node_create(&n);
		n->type = ast_type_for_iteration;
		if (dec) {
			ast_node_add(n, dec);
		}
		valid = for_iteration(ps, n) && valid;

	} else {
		valid = set_source_error(ps->el, &loc, "expected an = or in after for element declaration");
	}

	struct ast_node* c = NULL;
	valid = stmts(ps, true, &c) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;

	if (valid) {
		ast_node_add(n, c);
	}


	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(dec);
		ast_node_destroy(c);
	}

	token_destroy(f);
	free(f);
	token_destroy(end);
	free(end);

	ps->st->top = saved;
	environment_destroy(env);

	return valid;
}

/* for_range -> for id = expr:expr stmts end */
/* dynamic-output ps{} root root{} */
bool for_range(struct parse_state* ps, struct ast_node* parent)
{
	bool valid = true;

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	valid = match(ps, token_equal, "expected equal", &equal) && valid;

	/* start expr */
	/* allocate b b{} */
	struct ast_node* a = NULL;
	struct location loc_a;
	valid = expr(ps, &a, &loc_a) && valid;
	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_a, "expected range start after for-range");
	}

	/* allocate ps{} colon conlon{} */
	struct token* colon = NULL;
	valid = match(ps, token_colon, "expected colon", &colon) && valid;

	/* end expr */
	/* allocate ps{} c c{} */
	struct ast_node* b = NULL;
	struct location loc_b;
	valid = expr(ps, &b, &loc_b) && valid;
	if (!b) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_b, "expected range end after for-range");
	}

	if (valid) {
		ast_node_add(parent, a);
		ast_node_add(parent, b);
	} else {
		ast_node_destroy(a);
		ast_node_destroy(b);
	}

	/* destroy end end{} */
	token_destroy(equal);
	free(equal);
	token_destroy(colon);
	free(colon);

	if (valid) {
	}

	return valid;
}

/* for_iteration -> for id in expr stmts end */
/* dynamic-output ps{} root root{} */
bool for_iteration(struct parse_state* ps, struct ast_node* parent)
{
	bool valid = true;

	/* allocate ps{} in in{} */
	struct token* in = NULL;
	valid = match(ps, token_in, "expecting in", &in) && valid;

	/* expr */
	/* allocate ps{} b b{} */
	struct ast_node* list = NULL;
	struct location loc_list;
	valid = expr(ps, &list, &loc_list) && valid;

	if (!list) {
		set_source_error(ps->el, &loc_list, "expected expression after for-iteration");
		valid = false;
	}

	if (valid) {
		ast_node_add(parent, list);
	} else {
		ast_node_destroy(list);
	}

	if (valid) {
	}

	token_destroy(in);
	free(in);

	return valid;
}

bool function(struct parse_state* ps, struct ast_node** root)
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

	struct ast_node* fd = NULL;
	valid = function_start(ps, &fd) && valid;

	valid = function_finish(ps, fd) && valid;

	if (valid) {
		*root = fd;
	} else {
		ast_node_destroy(fd);
	}

	/* transfer saved -> ps->st->top */
	ps->st->top = saved;

	/* destroy env env{} */
	environment_destroy(env);

	return valid;
}

bool function_start(struct parse_state* ps, struct ast_node** root)
{
	bool valid = true;
	struct ast_node* n = NULL;

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = match(ps, token_function, "expecting function", &f) && valid;

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting identifier", &id) && valid;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expecting left parenthesis", &lp) && valid;

	/* allocate ps{} dseq_node dseq_node{} */
	struct ast_node* dseq_node = NULL;
	struct location loc_dseq;
	valid = dseq(ps, &dseq_node, &loc_dseq) && valid;

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expecting right parenthesis", &rp) && valid;

	struct ast_node* dret_node = NULL;
	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* next = get_token(&ps->lookahead, 0);
	if (next && next->type == token_double_colon) {
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expecting double colon", &dc) && valid;
		token_destroy(dc);
		free(dc);

		struct location loc_ret;
		valid = type(ps, NULL, &dret_node, &loc_ret) && valid;
	}

	/* start building nodes */
	if (valid) {
		/* allocate n */
		ast_node_create(&n);
		n->type = ast_type_function;

		/* allocate a a{} */
		struct ast_node* a;
		ast_node_create(&a);
		a->type = ast_type_id;
		buffer_copy(&id->value, &a->value);

		/* tranfer a -> n{} */
		ast_node_add(n, a);

		/* transfer dseq_node -> n{} */
		ast_node_add(n, dseq_node);

		struct ast_node* b;
		ast_node_create(&b);
		b->type = ast_type_dret;

		if (dret_node) {
			ast_node_add(b, dret_node);
		}

		ast_node_add(n, b);

	} else {
		ast_node_destroy(dseq_node);
	}

	if (valid) {
		struct symbol* search = environment_get_local(ps->st->top->prev, &id->value);
		if (search) {
			struct location loc;
			get_token_location(id, &loc);
			char* a;
			buffer2array(&id->value, &a);
			valid = set_source_error(ps->el, &loc, "duplicate declaration in same scope: %s", a);
			free(a);
		} else {
			struct symbol* sym = environment_get(ps->st->top, &id->value);
			if (sym && sym->td) {
				struct location loc;
				get_token_location(id, &loc);
				char* a;
				buffer2array(&id->value, &a);
				valid = set_source_error(ps->el, &loc, "identifier reserved as a type: %s", a);
				free(a);
			} else {
				struct type_use* tu = function2type(ps->st, n);
				struct symbol* new_sym = NULL;
				malloc_safe((void**)&new_sym, sizeof(struct symbol));
				symbol_init(new_sym);
				new_sym->tk_type = id->type;
				new_sym->tu = tu;
				environment_put(ps->st->top->prev, &id->value, new_sym);
				n->tu = type_use_copy(tu);
			}
		}
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

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

bool function_finish(struct parse_state* ps, struct ast_node* fd)
{
	bool valid = true;

	if (!fd) {
		valid = false;
	}

	struct location loc;
	valid = get_parse_location(ps, &loc) && valid;

	/* allocate ps{} stmts_node stmts_node{} */
	struct ast_node* stmts_node = NULL;
	valid = stmts(ps, true, &stmts_node) && valid;

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expecting end", &end) && valid;

	/* finish building nodes */
	if (valid) {
		/* transfer stmts_node -> n{} */
		ast_node_add(fd, stmts_node);

	} else {
		ast_node_destroy(stmts_node);
	}

	/* destroy end end{} */
	token_destroy(end);
	free(end);

	if (valid) {
		check_return_type(ps, fd, stmts_node, &loc, &valid);
	}

	return valid;
}

/* elseif_nt -> elseif expr stmts elseif_nt | e */
/* dynamic-output ps{} parent{} */
bool elseif_nt(struct parse_state* ps, struct ast_node* parent)
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

		/* allocate cb */
		struct ast_node* cb = NULL;
		ast_node_create(&cb);
		cb->type = ast_type_conditional_branch;

		/* allocate ps{} cond cond{} */
		struct ast_node* cond = NULL;
		struct location loc_cond;
		valid = expr(ps, &cond, &loc_cond) && valid;

		if (!cond) {
			/* allocate ps{} */
			valid = set_source_error(ps->el, &loc_cond, "expecting condition after elseif");
		} else {
			/* transfer cond -> cb{} */
			ast_node_add(cb, cond);
		}


		/* allocate ps{} node node{} */
		struct ast_node* node = NULL;
		valid = stmts(ps, false, &node) && valid;

		/* transfer node -> cb{} */
		if (node) {
			ast_node_add(cb, node);
		}

		/* transfer cb -> parent{} */
		ast_node_add(parent, cb);

		/* allocate ps{} parent{} */
		valid = elseif_nt(ps, parent) && valid;
	}

	return valid;
}

/* else_nt -> else stmts | e */
/* dynamic-output ps{} parent{} */
bool else_nt(struct parse_state* ps, struct ast_node* parent)
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
		struct ast_node* cb = NULL;
		ast_node_create(&cb);
		cb->type = ast_type_default_branch;

		/* stmts */
		/* allocate node node{} */
		struct ast_node* node = NULL;
		valid = stmts(ps, false, &node) && valid;

		/* transfer node -> cb{} */
		if (node) {
			ast_node_add(cb, node);
		}

		/* transfer cb -> parent{} */
		ast_node_add(parent, cb);
	}

	return valid;
}
