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
#include "type_def.h"
#include <assert.h>

bool separator(struct parse_state* ps, int* has_separator, struct location* loc);
bool stmt(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool if_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool elseif_nt(struct parse_state* ps, struct ast_node* parent, struct location* loc);
bool else_nt(struct parse_state* ps, struct ast_node* parent, struct location* loc);
bool while_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool for_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool for_range(struct parse_state* ps, struct ast_node* parent, struct location* loc);
bool for_iteration(struct parse_state* ps, struct ast_node* parent, struct location* loc);
bool function(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool function_start(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool function_finish(struct parse_state* ps, struct ast_node* fd, struct location* loc);
bool module_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);

/* stmts -> stmt stmts' */
/* stmts' -> separator stmt stmts' | e */
/* dynamic-output ps{} root root{} */
bool stmts(struct parse_state* ps, bool suppress_env, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;
	struct ast_node* last = NULL;

	location_init(loc);

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
	struct location loc_a;
	valid = stmt(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	/* transfer a -> n{} */
	if (a) {
		ast_node_add(n, a);
		last = a;
	}

	while (true) {
		/* allocate ps{} */
		int has_separator = 0;
		struct location loc_sep;
		valid = separator(ps, &has_separator, &loc_sep) && valid;

		if (!has_separator) {
			break;
		}

		/* allocate ps{} a a{} */
		struct ast_node* b = NULL;
		struct location loc_b;
		valid = stmt(ps, &b, &loc_b) && valid;
		location_update(loc, &loc_b);

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
				n->tu = ast_node_copy(last->tu);
			}
		}
	}

	valid = location_default(ps, loc) && valid;

	/* transfer n -> root */
	return valid;
}

/* separator -> \n | ; */
/* dynamic-output ps{} */
/* dynamic-temp sep sep{} */
bool separator(struct parse_state* ps, int* has_separator, struct location* loc)
{
	bool valid = true;
	enum token_enum type;
	int num;
	*has_separator = 0;

	location_init(loc);

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
	location_update_token(loc, sep);
	/* test case: no test case necessary */

	/* destroy sep sep{} */
	token_destroy(sep);
	free(sep);

	valid = location_default(ps, loc) && valid;

	return valid;
}

/**
* @brief stmt -> id = expr
*		| function id (dseq) stmts end
*       | expr
*       | e
*/
/* @param dynamic-output ps{} root root{} */
bool stmt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;
	int num;

	location_init(loc);

	/* allocate ps{} */
	valid = get_lookahead(ps, 2, &num) && valid;

	/* e */
	if (num <= 0) {
		valid = valid && location_default(ps, loc) && valid;
		return valid;
	}


	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	/* while */
	if (t0 && t0->type == token_while) {
		valid = while_nt(ps, &n, loc) && valid;

	/* for */
	} else if (t0 && t0->type == token_for) {
		valid = for_nt(ps, &n, loc) && valid;

		/* function word (seq) stmts end */
	} else if (t0 && t0->type == token_function && t1 && t1->type == token_id) {
		valid = function(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_if) {
		valid = if_nt(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_module) {
		valid = module_nt(ps, &n, loc) && valid;

	/* expr */
	} else {
		/* allocate ps{} n n{} */
		valid = valid && expr(ps, &n, loc);
	}

	valid = valid && location_default(ps, loc) && valid;

	/* transfer n -> root */
	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}
	return valid;
}

bool while_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} */
	struct token* whl = NULL;
	valid = match(ps, token_while, "expecting while", &whl) && valid;
	location_update_token(loc, whl);

	/* allocate ps{} a a{} */
	struct ast_node* a = NULL;
	struct location loc_expr;
	valid = expr(ps, &a, &loc_expr) && valid;
	location_update(loc, &loc_expr);

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_expr, "expected expression after while");
		/* test case: test_parse_while_error_expected_expression */
	}

	/* allocate ps{} b b{} */
	struct ast_node* b = NULL;
	struct location loc_stmts;
	valid = stmts(ps, false, &b, &loc_stmts) && valid;
	location_update(loc, &loc_stmts);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;
	location_update_token(loc, end);
	/* test case: test_parse_while_error_expected_end */

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

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool for_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	int num;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = match(ps, token_for, "expected for", &f) && valid;
	location_update_token(loc, f);
	/* test case: test cases not needed */

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct ast_node* dec = NULL;
	struct location loc_dec;
	valid = declaration(ps, &dec, &loc_dec) && valid;
	location_update(loc, &loc_dec);

	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_equal) {
		ast_node_create(&n);
		n->type = ast_type_for_range;
		if (dec) {
			ast_node_add(n, dec);
		}
		struct location loc_range;
		valid = for_range(ps, n, &loc_range) && valid;
		location_update(loc, &loc_range);

	} else if (t0 && t0->type == token_in) {
		ast_node_create(&n);
		n->type = ast_type_for_iteration;
		if (dec) {
			ast_node_add(n, dec);
		}
		struct location loc_iteration;
		valid = for_iteration(ps, n, &loc_iteration) && valid;
		location_update(loc, &loc_iteration);

	} else {
		struct location loc_error;
		location_init(&loc_error);
		get_parse_location(ps, &loc_error);
		valid = set_source_error(ps->el, &loc_error, "expected '=' or 'in' after for element declaration");
		/* test case: test_parse_for_error_after_declaration */
	}

	struct ast_node* c = NULL;
	struct location loc_stmts;
	valid = stmts(ps, true, &c, &loc_stmts) && valid;
	location_update(loc, &loc_stmts);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;
	location_update_token(loc, end);
	/* test case: test_parse_for_error_expected_end */

	valid = location_default(ps, loc) && valid;

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
bool for_range(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
	bool valid = true;

	location_init(loc);

	/* allocate ps{} equal equal{} */
	struct token* equal = NULL;
	valid = match(ps, token_equal, "expected equal", &equal) && valid;
	location_update_token(loc, equal);
	/* test case: no test case needed */

	/* start expr */
	/* allocate b b{} */
	struct ast_node* a = NULL;
	struct location loc_a;
	valid = expr(ps, &a, &loc_a) && valid;
	location_update(loc, &loc_a);

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_a, "expected range start");
		/* test case: test_parse_for_error_expected_range_start */
	}

	/* allocate ps{} colon conlon{} */
	struct token* colon = NULL;
	valid = match(ps, token_colon, "expected colon", &colon) && valid;
	location_update_token(loc, colon);
	/* test case: test_parse_for_error_expected_colon */

	/* end expr */
	/* allocate ps{} c c{} */
	struct ast_node* b = NULL;
	struct location loc_b;
	valid = expr(ps, &b, &loc_b) && valid;
	location_update(loc, &loc_b);

	if (!b) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_b, "expected range end");
		/* test case: */
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

	valid = location_default(ps, loc) && valid;

	return valid;
}

/* for_iteration -> for id in expr stmts end */
/* dynamic-output ps{} root root{} */
bool for_iteration(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
	bool valid = true;

	location_init(loc);

	/* allocate ps{} in in{} */
	struct token* in = NULL;
	valid = match(ps, token_in, "expecting in", &in) && valid;
	location_update_token(loc, in);
	/* test case: no test case necessary */

	/* expr */
	/* allocate ps{} b b{} */
	struct ast_node* list = NULL;
	struct location loc_list;
	valid = expr(ps, &list, &loc_list) && valid;
	location_update(loc, &loc_list);

	if (!list) {
		set_source_error(ps->el, &loc_list, "expected for iteration expression");
		valid = false;
		/* test case: test_parse_for_error_expected_iteration_expression */
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

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool function(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;

	location_init(loc);

	/* shared ps{top} -> saved */
	struct environment* saved = ps->st->top;

	/* allocate env env{} */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct ast_node* fd = NULL;
	struct location loc_start;
	valid = function_start(ps, &fd, &loc_start) && valid;
	location_update(loc, &loc_start);

	struct location loc_finish;
	valid = function_finish(ps, fd, &loc_finish) && valid;
	location_update(loc, &loc_finish);

	if (valid) {
		*root = fd;
	} else {
		ast_node_destroy(fd);
	}

	/* transfer saved -> ps->st->top */
	ps->st->top = saved;

	/* destroy env env{} */
	environment_destroy(env);

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool function_start(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} f f{} */
	struct token* f = NULL;
	valid = match(ps, token_function, "expecting function", &f) && valid;
	location_update_token(loc, f);
	/* test case: no test case needed */

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting identifier", &id) && valid;
	location_update_token(loc, id);
	/* test case: no test case needed */

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expected left parenthesis", &lp) && valid;
	location_update_token(loc, lp);
	/* test case: test_parse_function_error_expected_left_parenthesis */

	/* allocate ps{} dseq_node dseq_node{} */
	struct ast_node* dseq_node = NULL;
	struct location loc_dseq;
	valid = dseq(ps, &dseq_node, &loc_dseq) && valid;
	location_update(loc, &loc_dseq);

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expected right parenthesis", &rp) && valid;
	location_update_token(loc, rp);
	/* test case: test_parse_function_error_expected_right_parenthesis */

	struct ast_node* dret_node = NULL;
	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* next = get_token(&ps->lookahead, 0);
	if (next && next->type == token_double_colon) {
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expecting double colon", &dc) && valid;
		location_update_token(loc, dc);
		token_destroy(dc);
		free(dc);
		/* test case: no test case needed */

		struct location loc_ret;
		valid = type(ps, NULL, &dret_node, &loc_ret) && valid;
		location_update(loc, &loc_ret);
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
			valid = set_source_error(ps->el, &id->loc, "duplicate declaration in same scope: %s", a);
			free(a);
			/* test case: test_parse_function_error_duplicate_declaration */
		} else {
			struct symbol* sym = environment_get(ps->st->top, &id->value);
			if (sym && sym->td) {
				struct location loc;
				get_token_location(id, &loc);
				char* a;
				buffer2array(&id->value, &a);
				valid = set_source_error(ps->el, &id->loc, "identifier reserved as a type: %s", a);
				free(a);
				/* test case: test_parse_function_error_identifier_reserved */
			} else {
				struct ast_node* tu = function2type(ps->st, n);
				struct symbol* new_sym = NULL;
				malloc_safe((void**)&new_sym, sizeof(struct symbol));
				symbol_init(new_sym);
				new_sym->tk_type = id->type;
				new_sym->tu = tu;
				environment_put(ps->st->top->prev, &id->value, new_sym);
				n->tu = ast_node_copy(tu);
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

	valid = location_default(ps, loc) && valid;
	
	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

bool function_finish(struct parse_state* ps, struct ast_node* fd, struct location* loc)
{
	bool valid = true;

	location_init(loc);

	if (!fd) {
		valid = false;
	}

	/* allocate ps{} stmts_node stmts_node{} */
	struct ast_node* stmts_node = NULL;
	struct location loc_stmts;
	valid = stmts(ps, true, &stmts_node, &loc_stmts) && valid;
	location_update(loc, &loc_stmts);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;
	location_update_token(loc, end);
	/* test case: test_parse_function_error_expected_end */

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
		check_return_type(ps, fd, stmts_node, &loc_stmts, &valid);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool if_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} ift ift{} */
	struct token* ift = NULL;
	valid = valid && match(ps, token_if, "expecting if", &ift);
	location_update_token(loc, ift);

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

	/* condition */
	/* allocate ps{} cond cond{} */
	struct ast_node* cond = NULL;
	struct location loc_expr;
	valid = valid && expr(ps, &cond, &loc_expr);
	location_update(loc, &loc_expr);

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
	struct location loc_stmts;
	valid = valid && stmts(ps, false, &body, &loc_stmts);
	location_update(loc, &loc_stmts);

	/* transfer body -> n{} */
	if (body) {
		ast_node_add(cb, body);
	}

	/* elseif_nt */
	/* allocate n{} */
	struct location loc_elseif;
	valid = valid && elseif_nt(ps, n, &loc_elseif);
	location_update(loc, &loc_elseif);

	/* else_nt */
	/* allocate ps{] n{} */
	struct location loc_else;
	valid = valid && else_nt(ps, n, &loc_else);
	location_update(loc, &loc_else);

	/* end */
	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = valid && match(ps, token_end, "expected end", &end);
	location_update_token(loc, end);

	valid = location_default(ps, loc) && valid;

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	return valid;
}

/* elseif_nt -> elseif expr stmts elseif_nt | e */
/* dynamic-output ps{} parent{} */
bool elseif_nt(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
	bool valid = true;
	int num;

	location_init(loc);

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0->type == token_elseif) {
		/* allocate ps{} eit eit{} */
		struct token* eit = NULL;
		valid = match(ps, token_elseif, "expecting elseif", &eit) && valid;
		location_update_token(loc, eit);

		/* allocate cb */
		struct ast_node* cb = NULL;
		ast_node_create(&cb);
		cb->type = ast_type_conditional_branch;

		/* allocate ps{} cond cond{} */
		struct ast_node* cond = NULL;
		struct location loc_cond;
		valid = expr(ps, &cond, &loc_cond) && valid;
		location_update(loc, &loc_cond);

		if (!cond) {
			/* allocate ps{} */
			valid = set_source_error(ps->el, &loc_cond, "expecting condition after elseif");
		} else {
			/* transfer cond -> cb{} */
			ast_node_add(cb, cond);
		}


		/* allocate ps{} node node{} */
		struct ast_node* node = NULL;
		struct location loc_node;
		valid = stmts(ps, false, &node, &loc_node) && valid;
		location_update(loc, &loc_node);

		/* transfer node -> cb{} */
		if (node) {
			ast_node_add(cb, node);
		}

		/* transfer cb -> parent{} */
		ast_node_add(parent, cb);

		/* allocate ps{} parent{} */
		struct location loc_elseif;
		valid = elseif_nt(ps, parent, &loc_elseif) && valid;
		location_update(loc, &loc_elseif);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

/* else_nt -> else stmts | e */
/* dynamic-output ps{} parent{} */
bool else_nt(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
	bool valid = true;
	int num;

	location_init(loc);

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_else) {
		/* else */
		/* allocate ps{} et et{} */
		struct token* et = NULL;
		valid = match(ps, token_else, "expected else", &et) && valid;
		location_update_token(loc, et);

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
		struct location loc_node;
		valid = stmts(ps, false, &node, &loc_node) && valid;
		location_update(loc, &loc_node);

		/* transfer node -> cb{} */
		if (node) {
			ast_node_add(cb, node);
		}

		/* transfer cb -> parent{} */
		ast_node_add(parent, cb);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

/* module_nt -> module stmts end */
bool module_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	struct token* module = NULL;
	valid = match(ps, token_module, "expected module", &module) && valid;
	location_update_token(loc, module);

	token_destroy(module);
	free(module);

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe(&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct token* id = NULL;
	valid = match(ps, token_id, "expected identifier after module", &id);
	location_update_token(loc, id);

	struct ast_node* a = NULL;
	struct location loc_stmts;
	valid = stmts(ps, true, &a, &loc_stmts) && valid;
	location_update(loc, &loc_stmts);

	transfer_module_symbols(env, saved, &id->value);

	ps->st->top = saved;
	environment_destroy(env);

	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;
	location_update_token(loc, end);

	token_destroy(end);
	free(end);

	if (valid) {
		ast_node_create(&n);
		n->type = ast_type_module;

		struct ast_node* id_node = NULL;
		ast_node_create(&id_node);
		id_node->type = ast_type_id;
		buffer_copy(&id->value, &id_node->value);
		ast_node_add(n, id_node);

		ast_node_add(n, a);
	} else {
		ast_node_destroy(a);
	}

	if (valid) {
		struct symbol* sym = environment_get(ps->st->top, &id->value);
		if (sym) {
			buffer_finish(&id->value);
			valid = set_source_error(ps->el, &id->loc, "variable aready used: %s", &id->value);
		} else {
			struct buffer bf;
			buffer_init(&bf);
			buffer_copy_str(&bf, "Module");
			struct symbol* sym = environment_get(ps->st->top, &bf);
			buffer_destroy(&bf);
			assert(sym);
			assert(sym->td);

			struct ast_node* tu = NULL;
			ast_node_create(&tu);
			tu->type = ast_type_type;
			tu->td = sym->td;

			struct symbol* new_sym = NULL;
			malloc_safe(&new_sym, sizeof(struct symbol));
			symbol_init(new_sym);
			new_sym->tk_type = token_id;
			new_sym->tu = tu;
			environment_put(ps->st->top, &id->value, new_sym);
		}
	}

	valid = location_default(ps, loc) && valid;

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	token_destroy(id);
	free(id);

	return valid;
}
