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
#include "parse_factor.h"
#include "type_def.h"
#include <assert.h>
#include "zinc/list.h"

bool separator(struct parse_state* ps, bool* has_separator, struct location* loc);
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
struct ast_node* parse_struct(struct parse_state* ps, struct location* loc);
struct ast_node* parse_return(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var_lseq(struct parse_state* ps, struct location* loc, struct token_list* tl);
struct ast_node* parse_var_rseq(struct parse_state* ps, struct location* loc, struct list* l);

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
		bool has_separator = false;
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
bool separator(struct parse_state* ps, bool* has_separator, struct location* loc)
{
	bool valid = true;
	enum token_enum type;
	int num;
	*has_separator = false;

	location_init(loc);

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_newline) {
		type = token_newline;
		*has_separator = true;
	} else if (t0 && t0->type == token_semicolon) {
		type = token_semicolon;
		*has_separator = true;
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

	/* while */
	if (t0 && t0->type == token_while) {
		valid = while_nt(ps, &n, loc) && valid;

	/* for */
	} else if (t0 && t0->type == token_for) {
		valid = for_nt(ps, &n, loc) && valid;

		/* function word (seq) stmts end */
	} else if (t0 && t0->type == token_function) {
		valid = function(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_if) {
		valid = if_nt(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_module) {
		valid = module_nt(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_struct) {
		n = parse_struct(ps, loc);
        if (n->type == ast_type_error) {
            valid = false;
        }

	} else if (t0 && t0->type == token_return) {
        n = parse_return(ps, loc);
        if (n->type == ast_type_error) {
            valid = false;
        }

    } else if (t0 && t0->type == token_var) {
        n = parse_var(ps, loc);
        if (n->type == ast_type_error) {
            valid = false;
        }

	/* expr */
	} else {
		/* allocate ps{} n n{} */
        n = parse_expr(ps, loc);
        if (n && n->type == ast_type_error) {
            valid = false;
        }
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
    a = parse_expr(ps, &loc_expr);
	if (a && a->type == ast_type_error) {
        valid = false;
    }
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
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

    valid = consume_newline(ps) && valid;

	struct ast_node* dec = NULL;
	struct location loc_dec;
	valid = declaration(ps, true, &dec, &loc_dec) && valid;
	location_update(loc, &loc_dec);

    valid = consume_newline(ps) && valid;

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

    valid = consume_newline(ps) && valid;

	/* start expr */
	/* allocate b b{} */
	struct ast_node* a = NULL;
	struct location loc_a;
    a = parse_expr(ps, &loc_a);
	if (a && a->type == ast_type_error) {
        valid = false;
    }
	location_update(loc, &loc_a);

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_a, "expected range start");
		/* test case: test_parse_for_error_expected_range_start */
	}

    valid = consume_newline(ps) && valid;

	/* allocate ps{} colon conlon{} */
	struct token* colon = NULL;
	valid = match(ps, token_colon, "expected colon", &colon) && valid;
	location_update_token(loc, colon);
	/* test case: test_parse_for_error_expected_colon */

    valid = consume_newline(ps) && valid;

	/* end expr */
	/* allocate ps{} c c{} */
	struct ast_node* b = NULL;
	struct location loc_b;
    b = parse_expr(ps, &loc_b);
	if (b && b->type == ast_type_error) {
        valid = false;
    }
	location_update(loc, &loc_b);

	if (!b) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_b, "expected range end");
		/* test case: test_parse_for_error_expected_range_end */
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
		assert(a);
		if (!a->tu) {
			valid = set_source_error(ps->el, &loc_a, "start range expression has no value");
			/* test case: test_parse_for_range_error_start_no_value */
		} else {
			assert(a->tu->td);
			if (!is_numeric(a->tu->td)) {
				valid = set_source_error(ps->el, &loc_a, "start range expression is not numeric");
				/* test case: test_parse_for_range_error_start_not_numeric */
			}
		}

		assert(b);
		if (!b->tu) {
			valid = set_source_error(ps->el, &loc_b, "end range expression has no value");
			/* test case: test_parse_for_range_error_end_no_value */
		} else {
			assert(b->tu->td);
			if (!is_numeric(b->tu->td)) {
				valid = set_source_error(ps->el, &loc_b, "end range expression is not numeric");
				/* test case: test_parse_for_range_error_end_not_numeric */
			}
		}
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

    valid = consume_newline(ps) && valid;

	/* expr */
	/* allocate ps{} b b{} */
	struct ast_node* list = NULL;
	struct location loc_list;
    list = parse_expr(ps, &loc_list);
	if (list && list->type == ast_type_error) {
        valid = false;
    }
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
		struct ast_node* element = ast_node_get(parent, 0);
		struct ast_node* element_tu = ast_node_get(element, 1);

		struct ast_node* list_tu = list->tu;

		if (!list_tu) {
			valid = set_source_error(ps->el, &loc_list, "iteration expression has no value");
			/* test case: test_parse_for_iteration_error_no_value */
		} else if (!list_tu->head) {
			valid = set_source_error(ps->el, &loc_list, "iteration expression has no child element");
			/* test case: test_parse_for_iteration_error_no_child_element */
		} else {
			struct ast_node* element_tu2 = ast_node_get(list_tu, 0);
			if (!type_use_can_cast(element_tu2, element_tu)) {
				valid = set_source_error(ps->el, &loc_list, "cannot cast list element");
				/* test case: test_parse_for_iteration_error_cannot_cast */
			}
		}
	}

	token_destroy(in);
	free(in);

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool function(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
    struct ast_node* n = NULL;

	location_init(loc);

    struct token* f = NULL;
    valid = match(ps, token_function, "expected function", &f) && valid;
    location_update_token(loc, f);
    token_destroy(f);
    free(f);

    valid = consume_newline(ps) && valid;

    int num;
    valid = get_lookahead(ps, 1, &num) && valid;
    struct token* t0 = get_token(&ps->lookahead, 0);
    if (t0 && t0->type == token_id) {
        /* shared ps{top} -> saved */
        struct environment* saved = ps->st->top;

        /* allocate env env{} */
        struct environment* env = NULL;
        malloc_safe((void**)&env, sizeof(struct environment));
        environment_init(env, saved);
        ps->st->top = env;

        struct location loc_start;
        valid = function_start(ps, &n, &loc_start) && valid;
        location_update(loc, &loc_start);

        struct location loc_finish;
        valid = function_finish(ps, n, &loc_finish) && valid;
        location_update(loc, &loc_finish);

        /* transfer saved -> ps->st->top */
        ps->st->top = saved;

        /* destroy env env{} */
        environment_destroy(env);
    } else if (t0 && t0->type == token_left_paren) {
        struct location af_loc;
        n = parse_anonymous_function(ps, &af_loc);
        if (n->type == ast_type_error) {
            valid = false;
        }
    } else {
        valid = set_source_error(ps->el, loc, "expected function name or parenthesis");
    }

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool function_start(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} id id{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting identifier", &id) && valid;
	location_update_token(loc, id);
	/* test case: no test case needed */

    valid = consume_newline(ps) && valid;

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expected left parenthesis", &lp) && valid;
	location_update_token(loc, lp);
	/* test case: test_parse_function_error_expected_left_parenthesis */

    valid = consume_newline(ps) && valid;

	/* allocate ps{} dseq_node dseq_node{} */
	struct ast_node* dseq_node = NULL;
	struct location loc_dseq;
	valid = dseq(ps, &dseq_node, &loc_dseq) && valid;
	location_update(loc, &loc_dseq);

    valid = consume_newline(ps) && valid;

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expected right parenthesis", &rp) && valid;
	location_update_token(loc, rp);
	/* test case: test_parse_function_error_expected_right_parenthesis */

    valid = consume_newline(ps) && valid;

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

        valid = consume_newline(ps) && valid;

		struct location loc_ret;
		valid = parse_type(ps, NULL, &dret_node, &loc_ret) && valid;
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

	/* destroy id id{} lp lp{} rp rp{} */
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
	set_current_function(ps->st->top, fd);
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
	/* test case: no test case necessary */

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
    cond = parse_expr(ps, &loc_expr);
	if (cond && cond->type == ast_type_error) {
        valid = false;
    }
	location_update(loc, &loc_expr);

	if (cond == NULL) {
		valid = set_source_error(ps->el, &loc_expr, "expected condition after if");
		/* test case: test_parse_if_error_expected_expression */
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
	/* test case: test_parse_if_error_expected_end */

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
	if (t0 && t0->type == token_elseif) {
		/* allocate ps{} eit eit{} */
		struct token* eit = NULL;
		valid = match(ps, token_elseif, "expecting elseif", &eit) && valid;
		location_update_token(loc, eit);
		/* test case: no test case neeeded */

		/* allocate cb */
		struct ast_node* cb = NULL;
		ast_node_create(&cb);
		cb->type = ast_type_conditional_branch;

		/* allocate ps{} cond cond{} */
		struct ast_node* cond = NULL;
		struct location loc_cond;
        cond = parse_expr(ps, &loc_cond);
		if (cond && cond->type == ast_type_error) {
            valid = false;
        }
		location_update(loc, &loc_cond);

		if (!cond) {
			/* allocate ps{} */
			valid = set_source_error(ps->el, &loc_cond, "expected condition after elseif");
			/* test case: test_parse_if_error_expected_elseif_expression */
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
		/* test case: no test case needed */

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

/* module_nt -> module id stmts end */
bool module_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	struct token* module = NULL;
	valid = match(ps, token_module, "expected module", &module) && valid;
	location_update_token(loc, module);
	/* test case: no test case needed */

	token_destroy(module);
	free(module);

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct token* id = NULL;
	valid = match(ps, token_id, "expected identifier after module", &id) && valid;
	location_update_token(loc, id);
	/* test case: test_parse_module_expected_identifier */

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
	/* test case: test_parse_module_expected_end */

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
			valid = set_source_error(ps->el, &id->loc, "variable already used: %s", id->value.buf);
			/* test case: test_parse_module_duplicate_declaration */
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
			malloc_safe((void**)&new_sym, sizeof(struct symbol));
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

/* struct_np -> struct id struct_stmts end */
/* struct_stmts -> declaration struct_stmts | e */
/* struct_stmts' -> separator declaration | e */
struct ast_node* parse_struct(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_struct;

    struct token* st = NULL;
	if (!match(ps, token_struct, "expected struct", &st)) {
        /* test case: no test case needed */
        assert(false);
    }

	struct token* id = NULL;
	if (!match(ps, token_id, "expected identifier", &id)) {
        /* test case: test_parse_struct_error_expected_identifier */
        n->type = ast_type_error;
    }

	struct ast_node* a = NULL;
	struct location a_loc;
	if (!declaration(ps, false, &a, &a_loc)) {
        n->type = ast_type_error;
    }

	if (a) {
		ast_node_add(n, a);
	}

	while (true) {
		bool has_separator;
		struct location sep_loc;
		if (!separator(ps, &has_separator, &sep_loc)) {
            n->type = ast_type_error;
        }

		if (!has_separator) {
			break;
		}

		struct ast_node* b = NULL;
		struct location b_loc;
		if (!declaration(ps, false, &b, &b_loc)) {
            n->type = ast_type_error;
        }

		if (b) {
			ast_node_add(n, b);
		}
	}

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_struct_error_expected_end */
        n->type = ast_type_error;
    }

	if (n->type != ast_type_error) {
		struct symbol* search = environment_get_local(ps->st->top, &id->value);
		if (search) {
			buffer_finish(&id->value);
			set_source_error(ps->el, &id->loc, "duplicate variable in scope: %s", id->value.buf);
            /* test case: test_parse_struct_error_duplicate */
            n->type = ast_type_error;
		} else {
			struct ast_node* tu = ast_node_copy(n);
			struct type_def* td = NULL;
			malloc_safe((void**)&td, sizeof(struct type_def));
			type_def_init(td);
			td->type = type_struct;
			buffer_copy(&id->value, &td->name);
			td->composite = tu;
			struct ast_node* root = make_constructor(td);
			struct ast_node* root_tu = function2type(ps->st, root);

			struct symbol* constructor_sym = NULL;
			malloc_safe((void**)&constructor_sym, sizeof(struct symbol));
			symbol_init(constructor_sym);
			constructor_sym->tk_type = token_id;
			constructor_sym->tu = root_tu;
			constructor_sym->root = root;

			struct symbol* sym = NULL;
			malloc_safe((void**)&sym, sizeof(struct symbol));
			symbol_init(sym);
			sym->tk_type = token_id;
			sym->td = td;
			sym->constructor = constructor_sym;
			environment_put(ps->st->top, &id->value, sym);
		}
	}

	token_destroy(st);
	free(st);
	token_destroy(id);
	free(id);
	token_destroy(end);
	free(end);

	return n;
}

/* pr-return -> return expr | return */
struct ast_node* parse_return(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_return;

	struct token* ret = NULL;
	if (!match(ps, token_return, "expected return", &ret)) {
        assert(false);
        /* test case: no test case needed */
    }

	struct ast_node* a = NULL;
	struct location a_loc;
    a = parse_expr(ps, &a_loc);
	if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (a) {
		ast_node_add(n, a);
	}

	if (n->type != ast_type_error) {
		if (a) {
			if (!a->tu) {
				set_source_error(ps->el, &a_loc, "return expression has no value");
				/* test case: test_parse_return_error_no_value */
                n->type = ast_type_error;
			} else {
				n->tu = ast_node_copy(a->tu);
				struct ast_node* fd = get_current_function(ps->st->top);
				if (!fd) {
					set_source_error(ps->el, &ret->loc, "return statement outside of function");
					/* test case: test_parse_return_error_outside_of_function */
                    n->type = ast_type_error;
				} else {
                    bool valid = true;
					check_return_type(ps, fd, n, &ret->loc, &valid);
					/* test case: test_parse_return_error_type_does_not_match */
                    if (!valid) {
                        n->type = ast_type_error;
                    }
				}
			}
		}
	}

	return n;
}

void location_item_destroy(struct location* loc)
{
    free(loc);
}

/* parse_var = var var_lseq :: type | var var_lseq :: type = var_rseq */
struct ast_node* parse_var(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

    struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_var;

    struct token* vrt = NULL;
    if (!match(ps, token_var, "expected var", &vrt)) {
        /* test case: no test case needed */
        assert(false);
    }

    token_destroy(vrt);
    free(vrt);

    consume_newline(ps);

    struct ast_node* a = NULL;
    struct location a_loc;
    struct token_list a_tl;
    token_list_init(&a_tl);
    a = parse_var_lseq(ps, &a_loc, &a_tl);
    if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (a) {
        ast_node_add(n, a);
    } else {
        set_source_error(ps->el, &a_loc, "expected variable(s) after var");
        n->type = ast_type_error;
    }

    consume_newline(ps);

    struct token* dc = NULL;
    if (!match(ps, token_double_colon, "expected :: after variable(s)", &dc)) {
        n->type = ast_type_error;
    }
    token_destroy(dc);
    free(dc);

    consume_newline(ps);

    struct ast_node* type_use = NULL;
    struct location type_use_loc;
    if (!parse_type(ps, &a_tl, &type_use, &type_use_loc)) {
        n->type = ast_type_error;
    }
    if (type_use) {
        ast_node_add(n, type_use);
    } else {
        set_source_error(ps->el, &type_use_loc, "expected type");
        n->type = ast_type_error;
    }

    /* add variables */

    get_lookahead_one(ps);
    struct token* t0 = ps->lookahead.head;

    if (t0 && t0->type == token_equal) {
        struct token* equal = NULL;
        if (!match(ps, token_equal, "expected equal", &equal)) {
            /* test case: no test case needed */
            assert(false);
        }
        token_destroy(equal);
        free(equal);

        consume_newline(ps);

        struct ast_node* b = NULL;
        struct location b_loc;
        struct list b_l;
        list_init(&b_l);
        b = parse_var_rseq(ps, &b_loc, &b_l);
        if (b && b->type == ast_type_error) {
            n->type = ast_type_error;
        }

        if (b) {
            ast_node_add(n, b);
        } else {
            set_source_error(ps->el, &b_loc, "expected expression");
            n->type = ast_type_error;
        }

        if (n->type != ast_type_error) {
            int a_count = ast_node_count_children(a);
            int b_count = ast_node_count_children(b);
            if (a_count != b_count) {
                set_source_error(ps->el, &a_loc, "lvalue count does not equal rvalue count");
                n->type = ast_type_error;
            } else {
                for (int i = 0; i < a_count; i++) {
                    struct ast_node* x = ast_node_get(a, i);
                    struct ast_node* y = ast_node_get(b, i);
                    struct location* y_loc = list_get(&b_l, i);
                    if (!y->tu) {
                        set_source_error(ps->el, y_loc, "cannot assign with operand that has no value");
                        n->type = ast_type_error;
                    } else if (!type_use_can_cast(type_use, y->tu)) {
                        set_source_error(ps->el, y_loc, "values in assignment not compatible");
                        n->type = ast_type_error;
                    }
                }
            }
        }

        list_destroy(&b_l, (list_node_destroy)location_item_destroy);
    }

    token_list_destroy(&a_tl);

    return n;
}

/* var_lseq -> id var_lseq' */
/* var_lseq' -> , id var_lseq' */
struct ast_node* parse_var_lseq(struct parse_state* ps, struct location* loc, struct token_list* tl)
{
    get_location(ps, loc);

    get_lookahead_one(ps);
    struct token* t0 = ps->lookahead.head;
    if (!t0 || t0->type != token_id) {
        return NULL;
    }

    struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_var_lseq;

    struct token* id = NULL;
    if (!match(ps, token_id, "expected an id", &id)) {
        /* test case: no test case needed */
        assert(false);
    }

    token_list_add(tl, id);

    struct ast_node* a = NULL;
    ast_node_create(&a);
    a->type = ast_type_id;
    buffer_copy(&id->value, &a->value);
    ast_node_add(n, a);

    while (true) {
        int num;
        get_lookahead_one(ps);
        t0 = ps->lookahead.head;
        if (!t0 || t0->type != token_comma) {
            break;
        }

        struct token* comma = NULL;
        if (!match(ps, token_comma, "expected comma", &comma)) {
            /* test case: no test case needed */
            assert(false);
        }
        token_destroy(comma);
        free(comma);

        id = NULL;
        if (!match(ps, token_id, "expected id", &id)) {
            set_source_error(ps->el, &id->loc, "expected id");
            n->type = ast_type_error;
            token_destroy(id);
            free(id);
            break;
        }

        a = NULL;
        ast_node_create(&a);
        a->type = ast_type_id;
        buffer_copy(&id->value, &a->value);
        ast_node_add(n, a);
        token_list_add(tl, id);
    }

    return n;
}

/* var_rseq -> simple_expr var_rseq' */
/* var_rseq' -> , simple_expr var_rseq' */
struct ast_node* parse_var_rseq(struct parse_state* ps, struct location* loc, struct list* l)
{
    get_location(ps, loc);

    struct ast_node* a = NULL;
    struct location a_loc;
    ast_node_create(&a);
    a = parse_simple_expr(ps, &a_loc);
    if (!a) {
        return NULL;
    }

    struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_var_rseq;

    if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

    struct location* temp = NULL;
    location_create(&temp);
    *temp = a_loc;
    list_add_item(l, temp);

    ast_node_add(n, a);

    while (true) {
        int num;
        get_lookahead(ps, 1, &num);
        struct token* t0 = get_token(&ps->lookahead, 0);
        if (!t0 || t0->type != token_comma) {
            break;
        }

        struct ast_node* b = NULL;
        struct location b_loc;
        b = parse_simple_expr(ps, &b_loc);
        if (b && b->type == ast_type_error) {
            n->type = ast_type_error;
        }

        location_create(&temp);
        *temp = b_loc;
        list_add_item(l, temp);
        if (b) {
            ast_node_add(n, b);
        } else {
            set_source_error(ps->el, &b_loc, "expected an expression");
            n->type = ast_type_error;
        }
    }

    return n;
}