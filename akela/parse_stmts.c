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
struct ast_node* parse_module(struct parse_state* ps, struct location* loc);
struct ast_node* parse_struct(struct parse_state* ps, struct location* loc);
struct ast_node* parse_return(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var_lseq(struct parse_state* ps, struct location* loc);
struct ast_node* parse_var_rseq(struct parse_state* ps, struct location* loc, struct list* l);
struct ast_node* parse_extern(struct parse_state* ps, struct location* loc);

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
        environment_begin(ps->st);
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
        environment_end(ps->st);
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
    assert(t0);

	if (t0->type == token_while) {
		n = parse_while(ps, loc);
	} else if (t0->type == token_for) {
		n = parse_for(ps, loc);
	} else if (t0->type == token_module) {
		n = parse_module(ps, loc);
	} else if (t0->type == token_struct) {
		n = parse_struct(ps, loc);
	} else if (t0->type == token_return) {
        n = parse_return(ps, loc);
    } else if (t0->type == token_var) {
        n = parse_var(ps, loc);
    } else if (t0->type == token_extern) {
        n = parse_extern(ps, loc);
	} else {
        n = parse_expr(ps, loc);
	}

    return n;
}

struct ast_node* parse_extern(struct parse_state* ps, struct location* loc)
{
    struct ast_node* n = NULL;
    get_location(ps, loc);

    struct token* ext = NULL;
    if (!match(ps, token_extern, "expected extern", &ext)) {
        /* test case: no test case needed */
        assert(false);
    }

    ast_node_create(&n);
    n->type = ast_type_extern;

    struct ast_node* proto = NULL;
    struct location proto_loc;
    bool has_id;
    proto = parse_prototype(ps, &has_id, &proto_loc);
    if (proto) {
        ast_node_add(n, proto);
        if (proto->type == ast_type_error) {
            n->type = ast_type_error;
        }
        struct ast_node* tu = ast_node_get(proto, 3);
        n->tu = ast_node_copy(tu);
    }

    if (!has_id) {
        error_list_set(ps->el, &proto_loc, "expected an identifier after extern");
        n->type = ast_type_error;
    }

    if (n->type != ast_type_error) {
        if (ps->st->top != ps->st->global) {
            error_list_set(ps->el, loc, "extern only allowed at top level scope");
            n->type = ast_type_error;
        }

        struct ast_node *id_node = ast_node_get(proto, 0);
        /* check and save symbol */
        struct symbol *search = environment_get_local(ps->st->top, &id_node->value);
        if (search) {
            buffer_finish(&id_node->value);
            error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", id_node->value.buf);
            n->type = ast_type_error;
        } else {
            struct symbol *sym = environment_get(ps->st->top, &id_node->value);
            if (sym && sym->td) {
                buffer_finish(&id_node->value);
                error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", id_node->value.buf);
                n->type = ast_type_error;
            } else {
                struct symbol *new_sym = NULL;
                malloc_safe((void **) &new_sym, sizeof(struct symbol));
                symbol_init(new_sym);
                new_sym->tk_type = token_id;
                new_sym->tu = ast_node_copy(n->tu);
                environment_put(ps->st->top, &id_node->value, new_sym);
                n->sym = new_sym;
            }
        }
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

    environment_begin(ps->st);

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

    environment_end(ps->st);

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

    environment_begin(ps->st);

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

	transfer_module_symbols(ps->st->top, ps->st->top->prev, &id->value);

    environment_end(ps->st);

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
    type_use = parse_type(ps, &type_use_loc);
    declare_type(ps, type_use, a);
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

        if (n->type != ast_type_error) {
            /* adjust rhs to the bit_size of lhs */
            if (a && type_use && b) {
                struct ast_node* rhs = b->head;
                while (rhs) {
                    Override_rhs(type_use, rhs);
                    rhs = rhs->next;
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

    token_destroy(id);
    free(id);

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
            break;
        }

        a = NULL;
        ast_node_create(&a);
        a->type = ast_type_id;
        buffer_copy(&id->value, &a->value);
        ast_node_add(n, a);
        a->loc = id->loc;

        token_destroy(id);
        free(id);
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

        struct token* comma = NULL;
        if (!match(ps, token_comma, "expected comma", &comma)) {
            assert(false && "should see comma");
        }

        token_destroy(comma);
        free(comma);

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