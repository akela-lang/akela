#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "zinc/buffer.h"
#include "parse_expr.h"
#include "lex.h"
#include "parse_tools.h"
#include "parse_types.h"
#include "symbol_table.h"
#include "zinc/memory.h"
#include "symbol_table.h"
#include "parse_stmts.h"
#include "parse_factor.h"
#include "type_def.h"
#include <assert.h>
#include "zinc/list.h"

void parse_separator(struct parse_state* ps, bool* has_separator, struct location* loc);
struct ast_node* parse_stmt(struct parse_state* ps, struct location* loc);
struct ast_node* parse_while(struct parse_state* ps, struct location* loc);
struct ast_node* parse_for(struct parse_state* ps, struct location* loc);
void parse_for_range(struct parse_state* ps, struct ast_node* parent, struct location* loc);
void parse_for_iteration(struct parse_state* ps, struct ast_node* parent, struct location* loc);
struct ast_node* parse_function(struct parse_state* ps, struct location* loc);
void parse_function_start(struct parse_state* ps, struct ast_node* n, struct location* loc);
void parse_function_finish(struct parse_state* ps, struct ast_node* fd, struct location* loc);
struct ast_node* parse_if(struct parse_state* ps, struct location* loc);
void parse_elseif(struct parse_state* ps, struct ast_node* parent, struct location* loc);
struct ast_node* parse_else(struct parse_state* ps, struct location* loc);
struct ast_node* parse_module(struct parse_state* ps, struct location* loc);
struct ast_node* parse_struct(struct parse_state* ps, struct location* loc);
struct ast_node* parse_return(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var_lseq(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var_rseq(struct parse_state* ps, struct location* loc, struct list* l);

/* stmts -> stmt stmts' */
/* stmts' -> separator stmt stmts' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_stmts(struct parse_state* ps, bool suppress_env, struct location* loc)
{
	struct ast_node* n = NULL;
	struct ast_node* last = NULL;

    get_location(ps, loc);

	struct environment* saved = NULL;
	struct environment* env = NULL;
	if (!suppress_env) {
		saved = ps->st->top;

		malloc_safe((void**)&env, sizeof(struct environment));
		environment_init(env, saved);

		ps->st->top = env;
	}

	ast_node_create(&n);
	n->type = ast_type_stmts;

	struct ast_node* a = NULL;
	struct location loc_a;
	a = parse_stmt(ps, &loc_a);
    if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (a) {
		ast_node_add(n, a);
		last = a;
	}

	while (true) {
		bool has_separator = false;
		struct location loc_sep;
		parse_separator(ps, &has_separator, &loc_sep);

		if (!has_separator) {
			break;
		}

		/* allocate ps{} a a{} */
		struct ast_node* b = NULL;
		struct location loc_b;
		b = parse_stmt(ps, &loc_b);
        if (b && b->type == ast_type_error) {
            n->type = ast_type_error;
        }

		if (b) {
			ast_node_add(n, b);
			last = b;
		}
	}

	if (!suppress_env) {
        env->prev = ps->st->deactivated;
        ps->st->deactivated = env;
		//environment_destroy(env);
		ps->st->top = saved;
	}

	if (n->type != ast_type_error) {
		if (last) {
			if (last->tu) {
				n->tu = ast_node_copy(last->tu);
			}
		}
	}

	return n;
}

/* separator -> \n | ; */
/* dynamic-output ps{} */
/* dynamic-temp sep sep{} */
void parse_separator(struct parse_state* ps, bool* has_separator, struct location* loc)
{
	enum token_enum type;
	*has_separator = false;

    get_location(ps, loc);

	struct token* t0 = get_lookahead(ps);
	if (t0 && t0->type == token_newline) {
		type = token_newline;
		*has_separator = true;
	} else if (t0 && t0->type == token_semicolon) {
		type = token_semicolon;
		*has_separator = true;
	} else {
		return;
	}

	struct token* sep = NULL;
	if (!match(ps, type, "expecting newline or semicolon", &sep)) {
        assert(false);
        /* test case: no test case necessary */
    }

	token_destroy(sep);
	free(sep);
}

/**
* @brief stmt -> id = expr
*		| function id (dseq) parse_stmts end
*       | expr
*       | e
*/
/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_stmt(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;

    get_location(ps, loc);

	location_init(loc);

    struct token* t0 = get_lookahead(ps);

    /* e */
	if (!t0) {
		return n;
	}


	/* while */
	if (t0 && t0->type == token_while) {
		n = parse_while(ps, loc);

	/* for */
	} else if (t0 && t0->type == token_for) {
		n = parse_for(ps, loc);

		/* function word (seq) stmts end */
	} else if (t0 && t0->type == token_function) {
		n = parse_function(ps, loc);

	} else if (t0 && t0->type == token_if) {
		n = parse_if(ps, loc);

	} else if (t0 && t0->type == token_module) {
		n = parse_module(ps, loc);

	} else if (t0 && t0->type == token_struct) {
		n = parse_struct(ps, loc);

	} else if (t0 && t0->type == token_return) {
        n = parse_return(ps, loc);

    } else if (t0 && t0->type == token_var) {
        n = parse_var(ps, loc);

	/* expr */
	} else {
		/* allocate ps{} n n{} */
        n = parse_expr(ps, loc);
	}

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_while(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_while;

	struct token* whl = NULL;
	if (!match(ps, token_while, "expecting while", &whl)) {
        n->type = ast_type_error;
    }

	struct ast_node* a = NULL;
	struct location loc_expr;
    a = parse_expr(ps, &loc_expr);
	if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (a) {
        ast_node_add(n, a);
    } else {
		error_list_set(ps->el, &loc_expr, "expected expression after while");
        n->type = ast_type_error;
		/* test case: test_parse_while_error_expected_expression */
	}

	struct ast_node* b = NULL;
	struct location loc_stmts;
    b = parse_stmts(ps, false, &loc_stmts);
	if (b && b->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (b) {
        ast_node_add(n, b);
    }

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        n->type = ast_type_error;
        /* test case: test_parse_while_error_expected_end */
    }

	token_destroy(whl);
	free(whl);
	token_destroy(end);
	free(end);

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_for(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
    ast_node_create(&n);

    get_location(ps, loc);

	struct token* f = NULL;
	if (!match(ps, token_for, "expected for", &f)) {
        /* test case: test cases not needed */
        n->type = ast_type_error;
    }

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

    consume_newline(ps);

	struct ast_node* dec = NULL;
	struct location loc_dec;
    dec = parse_declaration(ps, true, &loc_dec);
	if (dec && dec->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (dec) {
        ast_node_add(n, dec);
    }

    consume_newline(ps);

	struct token* t0 = get_lookahead(ps);

	if (t0 && t0->type == token_equal) {
        if (n->type == ast_type_none) {
            n->type = ast_type_for_range;
        }
		struct location loc_range;
		parse_for_range(ps, n, &loc_range);

	} else if (t0 && t0->type == token_in) {
        if (n->type == ast_type_none) {
            n->type = ast_type_for_iteration;
        }
		struct location loc_iteration;
		parse_for_iteration(ps, n, &loc_iteration);

	} else {
		struct location loc_error;
		get_location(ps, &loc_error);
		error_list_set(ps->el, &loc_error, "expected '=' or 'in' after for element declaration");
        n->type = ast_type_error;
		/* test case: test_parse_for_error_after_declaration */
	}

	struct ast_node* c = NULL;
	struct location loc_stmts;
    c = parse_stmts(ps, true, &loc_stmts);
	if (c && c->type == ast_type_error) {
        n->type = ast_type_error;
    }

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_for_error_expected_end */
        n->type = ast_type_error;
    }

    if (c) {
        ast_node_add(n, c);
    }

	token_destroy(f);
	free(f);
	token_destroy(end);
	free(end);

	ps->st->top = saved;
	environment_destroy(env);

	return n;
}

/* for_range -> for id = expr:expr stmts end */
void parse_for_range(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
    get_location(ps, loc);

	struct token* equal = NULL;
	if (!match(ps, token_equal, "expected equal", &equal)) {
        /* test case: no test case needed */
        parent->type = ast_type_error;
    }

    consume_newline(ps);

	/* start expr */
	struct ast_node* a = NULL;
	struct location loc_a;
    a = parse_expr(ps, &loc_a);
	if (a && a->type == ast_type_error) {
        parent->type = ast_type_error;
    }

	if (!a) {
		error_list_set(ps->el, &loc_a, "expected range start");
        parent->type = ast_type_error;
		/* test case: test_parse_for_error_expected_range_start */
	}

    consume_newline(ps);

	struct token* colon = NULL;
	if (!match(ps, token_colon, "expected colon", &colon)) {
        /* test case: test_parse_for_error_expected_colon */
        parent->type = ast_type_error;
    }

    consume_newline(ps);

	/* end expr */
	struct ast_node* b = NULL;
	struct location loc_b;
    b = parse_expr(ps, &loc_b);
	if (b && b->type == ast_type_error) {
        parent->type = ast_type_error;
    }

	if (!b) {
		error_list_set(ps->el, &loc_b, "expected range end");
        parent->type = ast_type_error;
		/* test case: test_parse_for_error_expected_range_end */
	}

	if (a) {
        ast_node_add(parent, a);
    }
    if (b) {
		ast_node_add(parent, b);
	}

	token_destroy(equal);
	free(equal);
	token_destroy(colon);
	free(colon);

	if (parent->type != ast_type_error) {
		assert(a);
		if (!a->tu) {
			error_list_set(ps->el, &loc_a, "start range expression has no value");
            parent->type = ast_type_error;
			/* test case: test_parse_for_range_error_start_no_value */
		} else {
			assert(a->tu->td);
			if (!is_numeric(a->tu->td)) {
				error_list_set(ps->el, &loc_a, "start range expression is not numeric");
                parent->type = ast_type_error;
				/* test case: test_parse_for_range_error_start_not_numeric */
			}
		}

		assert(b);
		if (!b->tu) {
			error_list_set(ps->el, &loc_b, "end range expression has no value");
            parent->type = ast_type_error;
			/* test case: test_parse_for_range_error_end_no_value */
		} else {
			assert(b->tu->td);
			if (!is_numeric(b->tu->td)) {
				error_list_set(ps->el, &loc_b, "end range expression is not numeric");
                parent->type = ast_type_error;
				/* test case: test_parse_for_range_error_end_not_numeric */
			}
		}
	}
}

/* for_iteration -> for id in expr stmts end */
/* dynamic-output ps{} root root{} */
void parse_for_iteration(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
	get_location(ps, loc);

	struct token* in = NULL;
	if (!match(ps, token_in, "expecting in", &in)) {
        /* test case: no test case necessary */
        parent->type = ast_type_error;
    }

    consume_newline(ps);

	/* expr */
	struct ast_node* list = NULL;
	struct location loc_list;
    list = parse_expr(ps, &loc_list);
	if (list && list->type == ast_type_error) {
        parent->type = ast_type_error;
    }

	if (!list) {
		error_list_set(ps->el, &loc_list, "expected for iteration expression");
        parent->type = ast_type_error;
        /* test case: test_parse_for_error_expected_iteration_expression */
	}

    if (list) {
        ast_node_add(parent, list);
    }

	if (parent->type != ast_type_error) {
		struct ast_node* element = ast_node_get(parent, 0);
		struct ast_node* element_tu = ast_node_get(element, 1);

		struct ast_node* list_tu = list->tu;

		if (!list_tu) {
			error_list_set(ps->el, &loc_list, "iteration expression has no value");
            parent->type = ast_type_error;
			/* test case: test_parse_for_iteration_error_no_value */
		} else if (!list_tu->head) {
			error_list_set(ps->el, &loc_list, "iteration expression has no child element");
            parent->type = ast_type_error;
			/* test case: test_parse_for_iteration_error_no_child_element */
		} else {
			struct ast_node* element_tu2 = ast_node_get(list_tu, 0);
			if (!type_use_can_cast(element_tu2, element_tu)) {
                parent->type = ast_type_error;
				error_list_set(ps->el, &loc_list, "cannot cast list element");
				/* test case: test_parse_for_iteration_error_cannot_cast */
			}
		}
	}

	token_destroy(in);
	free(in);
}

/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_function(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

    struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_function;

	location_init(loc);

    struct token* f = NULL;
    if (!match(ps, token_function, "expected function", &f)) {
        n->type = ast_type_error;
    }
    token_destroy(f);
    free(f);

    consume_newline(ps);

    struct token* t0 = get_lookahead(ps);
    if (t0 && t0->type == token_id) {
        struct environment* saved = ps->st->top;

        struct environment* env = NULL;
        malloc_safe((void**)&env, sizeof(struct environment));
        environment_init(env, saved);
        ps->st->top = env;

        struct location loc_start;
        parse_function_start(ps, n, &loc_start);

        if (n) {
            struct location loc_finish;
            parse_function_finish(ps, n, &loc_finish);
            if (n->type == ast_type_error) {
                n->type = ast_type_error;
            }
        }

        ps->st->top = saved;

        environment_destroy(env);
    } else if (t0 && t0->type == token_left_paren) {
        struct location af_loc;
        parse_anonymous_function(ps, n, &af_loc);
    } else {
        n->type = ast_type_error;
        error_list_set(ps->el, loc, "expected function name or parenthesis");
    }

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
/* function_start -> id ( dseq ) | id ( dseq ) :: type */
void parse_function_start(struct parse_state* ps, struct ast_node* n, struct location* loc)
{
    get_location(ps, loc);

	struct token* id = NULL;
	if (!match(ps, token_id, "expecting identifier", &id)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

    struct ast_node* a;
    ast_node_create(&a);
    a->type = ast_type_id;
    if (id) {
        buffer_copy(&id->value, &a->value);
    }
    ast_node_add(n, a);

    consume_newline(ps);

	struct token* lp = NULL;
	if (!match(ps, token_left_paren, "expected left parenthesis", &lp)) {
        /* test case: test_parse_function_error_expected_left_parenthesis */
        n->type = ast_type_error;
    }

    consume_newline(ps);

	struct ast_node* dseq_node = NULL;
	struct location loc_dseq;
    dseq_node = parse_dseq(ps, &loc_dseq);
	if (dseq_node && dseq_node->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (dseq_node) {
        ast_node_add(n, dseq_node);
    }

    consume_newline(ps);

	struct token* rp = NULL;
	if (!match(ps, token_right_paren, "expected right parenthesis", &rp)) {
        /* test case: test_parse_function_error_expected_right_parenthesis */
        n->type = ast_type_error;
    }

    consume_newline(ps);

	struct ast_node* dret_node = NULL;
	struct token* next = get_lookahead(ps);
	if (next && next->type == token_double_colon) {
		struct token* dc = NULL;
		if (!match(ps, token_double_colon, "expecting double colon", &dc)) {
            /* test case: no test case needed */
            n->type = ast_type_error;
        }
		token_destroy(dc);
		free(dc);

        consume_newline(ps);

		struct location loc_ret;
        dret_node = parse_type(ps, NULL, &loc_ret);
		if (dret_node && dret_node->type == ast_type_error) {
            n->type = ast_type_error;
        }
	}

    struct ast_node* b;
    ast_node_create(&b);
    b->type = ast_type_dret;

    if (dret_node) {
        ast_node_add(b, dret_node);
    }

    ast_node_add(n, b);


	if (n->type != ast_type_error) {
		struct symbol* search = environment_get_local(ps->st->top->prev, &id->value);
		if (search) {
			buffer_finish(&id->value);
			error_list_set(ps->el, &id->loc, "duplicate declaration in same scope: %s", id->value.buf);
            n->type = ast_type_error;
			/* test case: test_parse_function_error_duplicate_declaration */
		} else {
			struct symbol* sym = environment_get(ps->st->top, &id->value);
			if (sym && sym->td) {
                buffer_finish(&id->value);
				error_list_set(ps->el, &id->loc, "identifier reserved as a type: %s", id->value.buf);
                n->type = ast_type_error;
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
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void parse_function_finish(struct parse_state* ps, struct ast_node* fd, struct location* loc)
{
	get_location(ps, loc);

	set_current_function(ps->st->top, fd);
	struct ast_node* stmts_node = NULL;
	struct location loc_stmts;
    stmts_node = parse_stmts(ps, true, &loc_stmts);
	if (stmts_node && stmts_node->type == ast_type_error) {
        fd->type = ast_type_error;
    }

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_function_error_expected_end */
        fd->type = ast_type_error;
    }

	/* finish building nodes */
	if (stmts_node) {
		ast_node_add(fd, stmts_node);
	}

	token_destroy(end);
	free(end);

    if (fd->type != ast_type_error) {
        if (!check_return_type(ps, fd, stmts_node, &loc_stmts)) {
            fd->type = ast_type_error;
        }
    }
}

/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_if(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_if;

	struct token* ift = NULL;
	if (!match(ps, token_if, "expecting if", &ift)) {
        /* test case: no test case necessary */
        n->type = ast_type_error;
    }

	token_destroy(ift);
	free(ift);

	struct ast_node* cb = NULL;
	ast_node_create(&cb);
	cb->type = ast_type_conditional_branch;

	ast_node_add(n, cb);

	struct ast_node* cond = NULL;
	struct location loc_expr;
    cond = parse_expr(ps, &loc_expr);
	if (cond && cond->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (cond == NULL) {
		error_list_set(ps->el, &loc_expr, "expected condition after if");
		/* test case: test_parse_if_error_expected_expression */
        n->type = ast_type_error;
        cb->type = ast_type_error;
	} else {
		ast_node_add(cb, cond);
	}

	struct ast_node* body = NULL;
	struct location loc_stmts;
    body = parse_stmts(ps, false, &loc_stmts);
	if (body && body->type == ast_type_error) {
        n->type = ast_type_error;
        cb->type = ast_type_error;
    }

    if (body) {
        cb->tu = ast_node_copy(body->tu);
		ast_node_add(cb, body);
	}

	struct location loc_elseif;
	parse_elseif(ps, n, &loc_elseif);

	struct location b_loc;
    struct ast_node* b = NULL;
	b = parse_else(ps, &b_loc);
    if (b && b->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (b) {
        ast_node_add(n, b);
    }

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_if_error_expected_end */
        n->type = ast_type_error;
    }

    token_destroy(end);
    free(end);

    if (n->type != ast_type_error) {
        struct ast_node* p = n->head;
        struct ast_node* tu = NULL;
        if (p) {
            tu = ast_node_copy(p->tu);
            p = p->next;
        }
        while (p) {
            if (!type_find_whole(ps->st, tu, p->tu)) {
                error_list_set(ps->el, &p->loc,
                    "branch type does not match type of previous branch");
                n->type = ast_type_error;
                break;
            }
            p = p->next;
        }
        if (b) {
            /* only return a value if else exists */
            n->tu = tu;
        }
    }

	return n;
}

/* elseif-statement -> elseif expr stmts elseif | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
void parse_elseif(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
    get_location(ps, loc);

    while (true) {
        struct token* t0 = get_lookahead(ps);
        if (t0->type != token_elseif) {
            break;
        }

        struct token *eit = NULL;
        if (!match(ps, token_elseif, "expecting elseif", &eit)) {
            /* test case: no test case needed */
            assert(false);
        }

        token_destroy(eit);
        free(eit);

        struct ast_node *cb = NULL;
        ast_node_create(&cb);
        cb->type = ast_type_conditional_branch;

        struct ast_node *cond = NULL;
        struct location loc_cond;
        cond = parse_expr(ps, &loc_cond);
        if (cond && cond->type == ast_type_error) {
            cb->type = ast_type_error;
            parent->type = ast_type_error;
        }

        if (!cond) {
            error_list_set(ps->el, &loc_cond, "expected condition after elseif");
            /* test case: test_parse_if_error_expected_elseif_expression */
            cb->type = ast_type_error;
            parent->type = ast_type_error;
        } else {
            ast_node_add(cb, cond);
        }

        struct ast_node *body = NULL;
        struct location loc_node;
        body = parse_stmts(ps, false, &loc_node);
        if (body && body->type == ast_type_error) {
            cb->type = ast_type_error;
            parent->type = ast_type_error;
        }

        if (body) {
            ast_node_add(cb, body);
            cb->tu = ast_node_copy(body->tu);
        }

        ast_node_add(parent, cb);

        struct location loc_elseif;
    }
}

/* parse_else -> else stmts | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_else(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

    struct ast_node* n = NULL;

	struct token* t0 = get_lookahead(ps);
	if (t0 && t0->type == token_else) {
        ast_node_create(&n);
        n->type = ast_type_default_branch;

		struct token* et = NULL;
		if (!match(ps, token_else, "expected else", &et)) {
            /* test case: no test case needed */
            assert(false);
        }

		token_destroy(et);
		free(et);

		/* stmts */
		struct ast_node* body = NULL;
		struct location body_loc;
        body = parse_stmts(ps, false, &body_loc);
		if (body && body->type == ast_type_error) {
            n->type = ast_type_error;
        }

        if (body) {
            n->tu = ast_node_copy(body->tu);
        }

		if (body) {
			ast_node_add(n, body);
		}
	}

	return n;
}

/* parse_module -> module id stmts end */
/* NOLINTNEXTLINE(misc-no-recursion) */
struct ast_node* parse_module(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_module;

	struct token* module = NULL;
	if (!match(ps, token_module, "expected module", &module)) {
        assert(false);
        /* test case: no test case needed */
    }

	token_destroy(module);
	free(module);

	struct environment* saved = ps->st->top;
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);
	ps->st->top = env;

	struct token* id = NULL;
	if (!match(ps, token_id, "expected identifier after module", &id)) {
        /* test case: test_parse_module_expected_identifier */
        n->type = ast_type_error;
    }

	struct ast_node* a = NULL;
	struct location loc_stmts;
    a = parse_stmts(ps, true, &loc_stmts);
	if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

	transfer_module_symbols(env, saved, &id->value);

	ps->st->top = saved;
	environment_destroy(env);

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_module_expected_end */
        n->type = ast_type_error;
    }

	token_destroy(end);
	free(end);

    struct ast_node* id_node = NULL;
    ast_node_create(&id_node);
    id_node->type = ast_type_id;
    if (id) {
        buffer_copy(&id->value, &id_node->value);
    }
    ast_node_add(n, id_node);
    ast_node_add(n, a);

	if (n->type != ast_type_error) {
		struct symbol* sym = environment_get(ps->st->top, &id->value);
		if (sym) {
			buffer_finish(&id->value);
			error_list_set(ps->el, &id->loc, "variable already used: %s", id->value.buf);
            /* test case: test_parse_module_duplicate_declaration */
            n->type = ast_type_error;
		} else {
			struct buffer bf;
			buffer_init(&bf);
			buffer_copy_str(&bf, "Module");
			sym = environment_get(ps->st->top, &bf);
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

	token_destroy(id);
	free(id);

	return n;
}

/* parse_struct -> struct id struct_stmts end */
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
    a = parse_declaration(ps, false, &a_loc);
	if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (a) {
		ast_node_add(n, a);
	}

	while (true) {
		bool has_separator;
		struct location sep_loc;
		parse_separator(ps, &has_separator, &sep_loc);

		if (!has_separator) {
			break;
		}

		struct ast_node* b = NULL;
		struct location b_loc;
        b = parse_declaration(ps, false, &b_loc);
		if (b && b->type == ast_type_error) {
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
			error_list_set(ps->el, &id->loc, "duplicate variable in scope: %s", id->value.buf);
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
				error_list_set(ps->el, &a_loc, "return expression has no value");
				/* test case: test_parse_return_error_no_value */
                n->type = ast_type_error;
			} else {
				n->tu = ast_node_copy(a->tu);
				struct ast_node* fd = get_current_function(ps->st->top);
				if (!fd) {
					error_list_set(ps->el, &ret->loc, "return statement outside of function");
					/* test case: test_parse_return_error_outside_of_function */
                    n->type = ast_type_error;
				} else {
					if (!check_return_type(ps, fd, n, &ret->loc)) {
                        /* test case: test_parse_return_error_type_does_not_match */
                            n->type = ast_type_error;
                    }
				}
			}
		}
	}

    token_destroy(ret);
    free(ret);

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
    a = parse_var_lseq(ps, &a_loc);
    if (a) {
        a->loc = a_loc;
    }
    if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (a) {
        ast_node_add(n, a);
    } else {
        error_list_set(ps->el, &a_loc, "expected variable(s) after var");
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
    type_use = parse_type(ps, a, &type_use_loc);
    if (type_use && type_use->type == ast_type_error) {
        n->type = ast_type_error;
    }
    if (type_use) {
        ast_node_add(n, type_use);
    } else {
        error_list_set(ps->el, &type_use_loc, "expected type");
        n->type = ast_type_error;
    }

    /* add variables */

    struct token* t0 = get_lookahead(ps);

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
            error_list_set(ps->el, &b_loc, "expected expression");
            n->type = ast_type_error;
        }

        if (n->type != ast_type_error) {
            int a_count = ast_node_count_children(a);
            int b_count = ast_node_count_children(b);
            if (a_count != b_count) {
                error_list_set(ps->el, &a_loc, "lvalue count does not equal rvalue count");
                n->type = ast_type_error;
            } else {
                for (int i = 0; i < a_count; i++) {
                    struct ast_node* x = ast_node_get(a, i);
                    struct ast_node* y = ast_node_get(b, i);
                    struct location* y_loc = list_get(&b_l, i);
                    if (!y->tu) {
                        error_list_set(ps->el, y_loc, "cannot assign with operand that has no value");
                        n->type = ast_type_error;
                    } else if (!type_use_can_cast(type_use, y->tu)) {
                        error_list_set(ps->el, y_loc, "values in assignment not compatible");
                        n->type = ast_type_error;
                    }
                }
            }
        }

        list_destroy(&b_l, (list_node_destroy)location_item_destroy);
    }

    return n;
}

/* var_lseq -> id var_lseq' */
/* var_lseq' -> , id var_lseq' */
struct ast_node* parse_var_lseq(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

    struct token* t0 = get_lookahead(ps);
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

    struct ast_node* a = NULL;
    ast_node_create(&a);
    a->type = ast_type_id;
    buffer_copy(&id->value, &a->value);
    ast_node_add(n, a);
    a->loc = id->loc;

    while (true) {
        t0 = get_lookahead(ps);
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
            error_list_set(ps->el, &id->loc, "expected id");
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
        a->loc = id->loc;
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
        struct token* t0 = get_lookahead(ps);
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
            error_list_set(ps->el, &b_loc, "expected an expression");
            n->type = ast_type_error;
        }
    }

    return n;
}