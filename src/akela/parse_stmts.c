#include <stdbool.h>
#include "token.h"
#include "ast.h"
#include "parse_expr.h"
#include "lex.h"
#include "parse_tools.h"
#include "parse_types.h"
#include "symbol_table.h"
#include "zinc/memory.h"
#include "symbol.h"
#include "parse_stmts.h"
#include "parse_factor.h"
#include "type_def.h"
#include <assert.h>

Ake_ast* parse_stmt(struct parse_state* ps);
Ake_ast* parse_while(struct parse_state* ps);
Ake_ast* parse_for(struct parse_state* ps);
void parse_for_range(struct parse_state* ps, Ake_ast* parent);
void parse_for_iteration(struct parse_state* ps, Ake_ast* parent);
Ake_ast* parse_module(struct parse_state* ps);
Ake_ast* parse_struct(struct parse_state* ps);
Ake_ast* parse_return(struct parse_state* ps);
Ake_ast* parse_let(struct parse_state* ps);
Ake_ast* parse_let_lseq(struct parse_state* ps);
Ake_ast* parse_let_rseq(struct parse_state* ps);
Ake_ast* parse_extern(struct parse_state* ps);
Ake_ast* parse_impl(struct parse_state* ps);

/* stmts -> stmt stmts' */
/* stmts' -> separator stmt stmts' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* parse_stmts(struct parse_state* ps, bool suppress_env)
{
	Ake_ast* n = NULL;
	Ake_ast* last = NULL;

	struct environment* saved = NULL;
	struct environment* env = NULL;
	if (!suppress_env) {
        environment_begin(ps->st);
	}

    Ake_ast_create(&n);
	n->type = Ake_ast_type_stmts;

	Ake_ast* a = NULL;
	a = parse_stmt(ps);
    if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
		last = a;
	}

	while (true) {
		bool has_separator = false;
		parse_separator(ps, n, &has_separator);

		if (!has_separator) {
			break;
		}

		Ake_ast* b = NULL;
		b = parse_stmt(ps);
        if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

		if (b) {
            Ake_ast_add(n, b);
			last = b;
		}
	}

	if (!suppress_env) {
        environment_end(ps->st);
	}

	if (n->type != Ake_ast_type_error) {
		if (last) {
			if (last->tu) {
                n->tu = Type_use_clone(last->tu);
			}
		}
	}

	return n;
}

/**
* @brief stmt -> id = expr
*		| function id (dseq) parse_stmts end
*       | expr
*       | e
*/
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* parse_stmt(struct parse_state* ps)
{
	Ake_ast* n = NULL;

    struct token* t0 = get_lookahead(ps);
    assert(t0);

	if (t0->type == token_while) {
		n = parse_while(ps);
	} else if (t0->type == token_for) {
		n = parse_for(ps);
	} else if (t0->type == token_module) {
		n = parse_module(ps);
	} else if (t0->type == token_struct) {
		n = parse_struct(ps);
	} else if (t0->type == token_return) {
        n = parse_return(ps);
    } else if (t0->type == token_let) {
        n = parse_let(ps);
    } else if (t0->type == token_extern) {
        n = parse_extern(ps);
    } else if (t0->type == token_impl) {
        n = parse_impl(ps);
	} else {
        n = parse_expr(ps);
	}

    return n;
}

Ake_ast* parse_extern(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_extern;

    struct token* ext = NULL;
    if (!match(ps, token_extern, "expected extern", &ext, n)) {
        /* test case: no test case needed */
        assert(false);
    }
    token_destroy(ext);
    free(ext);

    Ake_ast* proto = NULL;
    bool has_id;
    proto = parse_prototype(ps, false, true, false, false, &has_id);
    if (proto) {
        Ake_ast_add(n, proto);
        if (proto->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }
        Type_use* tu = proto2type_use(ps->st, proto, NULL);
        n->tu = tu;
    }

    if (!has_id) {
        error_list_set(ps->el, &proto->loc, "expected an identifier after extern");
        n->type = Ake_ast_type_error;
    }

    if (n->type != Ake_ast_type_error) {
        if (ps->st->top != ps->st->global) {
            error_list_set(ps->el, &n->loc, "extern only allowed at top level scope");
            n->type = Ake_ast_type_error;
        }

        Ake_ast *id_node = Ast_node_get(proto, 0);
        /* check and save symbol */
        struct symbol *search = environment_get_local(ps->st->top, &id_node->value);
        if (search) {
            buffer_finish(&id_node->value);
            error_list_set(
                ps->el,
                &id_node->loc,
                "duplicate declaration in same scope: %s",
                id_node->value.buf);
            n->type = Ake_ast_type_error;
        } else {
            struct symbol *sym = environment_get(ps->st->top, &id_node->value);
            if (sym && sym->td) {
                buffer_finish(&id_node->value);
                error_list_set(
                    ps->el,
                    &id_node->loc,
                    "identifier reserved as a type: %s",
                    id_node->value.buf);
                n->type = Ake_ast_type_error;
            } else {
                struct symbol *new_sym = NULL;
                malloc_safe((void **) &new_sym, sizeof(struct symbol));
                symbol_init(new_sym);
                new_sym->type = Symbol_type_variable;
                new_sym->tu = Type_use_clone(n->tu);
                environment_put(ps->st->top, &id_node->value, new_sym);
                n->sym = new_sym;
            }
        }
    }

    if (n->type != Ake_ast_type_error) {
        Ake_ast* id = Ast_node_get(proto, 0);
        buffer_list_add_bf(ps->extern_list, &id->value);
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* parse_while(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_while;

	struct token* whl = NULL;
	if (!match(ps, token_while, "expecting while", &whl, n)) {
        n->type = Ake_ast_type_error;
    }

	Ake_ast* a = NULL;
    a = parse_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (a) {
        Ake_ast_add(n, a);
    } else {
        struct location a_loc = get_location(ps);
		error_list_set(ps->el, &a_loc, "expected expression after while");
        n->type = Ake_ast_type_error;
		/* test case: test_parse_while_error_expected_expression */
	}

	Ake_ast* b = NULL;
    b = parse_stmts(ps, false);
	if (b && b->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (b) {
        Ake_ast_add(n, b);
    }

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end, n)) {
        n->type = Ake_ast_type_error;
        /* test case: test_parse_while_error_expected_end */
    }

	token_destroy(whl);
	free(whl);
	token_destroy(end);
	free(end);

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* parse_for(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);

	struct token* f = NULL;
	if (!match(ps, token_for, "expected for", &f, n)) {
        /* test case: test cases not needed */
        n->type = Ake_ast_type_error;
    }

    environment_begin(ps->st);

    consume_newline(ps, n);

	Ake_ast* dec = NULL;
    dec = parse_declaration(ps, true, false, true);
	if (dec && dec->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (dec) {
        Ake_ast_add(n, dec);
    }

    consume_newline(ps, n);

	struct token* t0 = get_lookahead(ps);

	if (t0 && t0->type == token_equal) {
        if (n->type == Ake_ast_type_none) {
            n->type = Ake_ast_type_for_range;
        }
		parse_for_range(ps, n);

	} else if (t0 && t0->type == token_in) {
        if (n->type == Ake_ast_type_none) {
            n->type = Ake_ast_type_for_iteration;
        }
		parse_for_iteration(ps, n);

	} else {
		struct location loc_error = get_location(ps);
		error_list_set(ps->el, &loc_error, "expected '=' or 'in' after for element declaration");
        n->type = Ake_ast_type_error;
		/* test case: test_parse_for_error_after_declaration */
	}

	Ake_ast* c = NULL;
    c = parse_stmts(ps, true);
	if (c && c->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end, n)) {
        /* test case: test_parse_for_error_expected_end */
        n->type = Ake_ast_type_error;
    }

    if (c) {
        Ake_ast_add(n, c);
    }

	token_destroy(f);
	free(f);
	token_destroy(end);
	free(end);

    environment_end(ps->st);

	return n;
}

/* for_range -> for id = expr:expr stmts end */
void parse_for_range(struct parse_state* ps, Ake_ast* parent)
{
	struct token* equal = NULL;
	if (!match(ps, token_equal, "expected equal", &equal, parent)) {
        /* test case: no test case needed */
        parent->type = Ake_ast_type_error;
    }

    consume_newline(ps, parent);

	/* start expr */
	Ake_ast* a = NULL;
    a = parse_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        parent->type = Ake_ast_type_error;
    }

	if (!a) {
        struct location a_loc = get_location(ps);
		error_list_set(ps->el, &a_loc, "expected range start");
        parent->type = Ake_ast_type_error;
		/* test case: test_parse_for_error_expected_range_start */
	}

    consume_newline(ps, parent);

	struct token* colon = NULL;
	if (!match(ps, token_colon, "expected colon", &colon, parent)) {
        /* test case: test_parse_for_error_expected_colon */
        parent->type = Ake_ast_type_error;
    }

    consume_newline(ps, parent);

	/* end expr */
	Ake_ast* b = NULL;
    b = parse_expr(ps);
	if (b && b->type == Ake_ast_type_error) {
        parent->type = Ake_ast_type_error;
    }

	if (!b) {
        struct location b_loc = get_location(ps);
		error_list_set(ps->el, &b_loc, "expected range end");
        parent->type = Ake_ast_type_error;
		/* test case: test_parse_for_error_expected_range_end */
	}

	if (a) {
        Ake_ast_add(parent, a);
    }
    if (b) {
        Ake_ast_add(parent, b);
	}

	token_destroy(equal);
	free(equal);
	token_destroy(colon);
	free(colon);

	if (parent->type != Ake_ast_type_error) {
		assert(a);
		if (!a->tu) {
			error_list_set(ps->el, &a->loc, "start range expression has no value");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_range_error_start_no_value */
		} else {
			assert(a->tu->td);
			if (!is_numeric(a->tu->td)) {
				error_list_set(ps->el, &a->loc, "start range expression is not numeric");
                parent->type = Ake_ast_type_error;
				/* test case: test_parse_for_range_error_start_not_numeric */
			}
		}

		assert(b);
		if (!b->tu) {
			error_list_set(ps->el, &b->loc, "end range expression has no value");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_range_error_end_no_value */
		} else {
			assert(b->tu->td);
			if (!is_numeric(b->tu->td)) {
				error_list_set(ps->el, &b->loc, "end range expression is not numeric");
                parent->type = Ake_ast_type_error;
				/* test case: test_parse_for_range_error_end_not_numeric */
			}
		}
	}
}

/* for_iteration -> for id in expr stmts end */
void parse_for_iteration(struct parse_state* ps, Ake_ast* parent)
{
	struct token* in = NULL;
	if (!match(ps, token_in, "expecting in", &in, parent)) {
        /* test case: no test case necessary */
        parent->type = Ake_ast_type_error;
    }

    consume_newline(ps, parent);

	/* expr */
	Ake_ast* list = NULL;
    list = parse_expr(ps);
	if (list && list->type == Ake_ast_type_error) {
        parent->type = Ake_ast_type_error;
    }

	if (!list) {
        struct location list_loc = get_location(ps);
		error_list_set(ps->el, &list_loc, "expected for iteration expression");
        parent->type = Ake_ast_type_error;
        /* test case: test_parse_for_error_expected_iteration_expression */
	}

    if (list) {
        Ake_ast_add(parent, list);
    }

	if (parent->type != Ake_ast_type_error) {
		Ake_ast* element = Ast_node_get(parent, 0);
		Ake_ast* element_type_node = Ast_node_get(element, 1);

		Type_use* list_tu = list->tu;

		if (!list_tu) {
			error_list_set(ps->el, &list->loc, "iteration expression has no value");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_iteration_error_no_value */
		} else if (!list_tu->is_array) {
			error_list_set(ps->el, &list->loc, "iteration expression is not an array");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_iteration_error_no_child_element */
		} else {
            Type_use* element_tu2 = Type_use_clone(list_tu);
            Type_use_reduce_dimension(element_tu2);
			if (!type_use_can_cast(element_tu2, element_type_node->tu)) {
                parent->type = Ake_ast_type_error;
				error_list_set(ps->el, &list->loc, "cannot cast list element");
				/* test case: test_parse_for_iteration_error_cannot_cast */
			}
            Type_use_destroy(element_tu2);
		}
	}

	token_destroy(in);
	free(in);
}

/* parse_module -> module id stmts end */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* parse_module(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_module;

	struct token* module = NULL;
	if (!match(ps, token_module, "expected module", &module, n)) {
        assert(false);
        /* test case: no test case needed */
    }

	token_destroy(module);
	free(module);

    environment_begin(ps->st);

	struct token* id = NULL;
	if (!match(ps, token_id, "expected identifier after module", &id, n)) {
        /* test case: test_parse_module_expected_identifier */
        n->type = Ake_ast_type_error;
    }

	Ake_ast* a = NULL;
    a = parse_stmts(ps, true);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	transfer_module_symbols(ps->st->top, ps->st->top->prev, &id->value);

    environment_end(ps->st);

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end, n)) {
        /* test case: test_parse_module_expected_end */
        n->type = Ake_ast_type_error;
    }

	token_destroy(end);
	free(end);

    Ake_ast* id_node = NULL;
    Ake_ast_create(&id_node);
    id_node->type = Ake_ast_type_id;
    if (id) {
        buffer_copy(&id->value, &id_node->value);
    }
    Ake_ast_add(n, id_node);
    Ake_ast_add(n, a);

	if (n->type != Ake_ast_type_error) {
		struct symbol* sym = environment_get(ps->st->top, &id->value);
		if (sym) {
			buffer_finish(&id->value);
			error_list_set(ps->el, &id->loc, "variable already used: %s", id->value.buf);
            /* test case: test_parse_module_duplicate_declaration */
            n->type = Ake_ast_type_error;
		} else {
			struct buffer bf;
			buffer_init(&bf);
			buffer_copy_str(&bf, "Module");
			sym = environment_get(ps->st->top, &bf);
			buffer_destroy(&bf);
			assert(sym);
			assert(sym->td);

			Type_use* tu = NULL;
            Type_use_create(&tu);
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
Ake_ast* parse_struct(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_struct;

    struct token* st = NULL;
	if (!match(ps, token_struct, "expected struct", &st, n)) {
        /* test case: no test case needed */
        assert(false);
    }

	struct token* id = NULL;
	if (!match(ps, token_id, "expected identifier", &id, n)) {
        /* test case: test_parse_struct_error_expected_identifier */
        n->type = Ake_ast_type_error;
    }
    if (id) {
        buffer_copy(&id->value, &n->value);
    }

	Ake_ast* a = NULL;
    a = parse_declaration(ps, false, false, true);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
	}

	while (true) {
		bool has_separator;
		parse_separator(ps, n, &has_separator);

		if (!has_separator) {
			break;
		}

		Ake_ast* b = NULL;
        b = parse_declaration(ps, false, false, true);
		if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

		if (b) {
            Ake_ast_add(n, b);
		}
	}

	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end, n)) {
        /* test case: test_parse_struct_error_expected_end */
        n->type = Ake_ast_type_error;
    }

	if (n->type != Ake_ast_type_error) {
		struct symbol* search = environment_get_local(ps->st->top, &id->value);
		if (search) {
			buffer_finish(&id->value);
			error_list_set(ps->el, &id->loc, "duplicate variable in scope: %s", id->value.buf);
            /* test case: test_parse_struct_error_duplicate */
            n->type = Ake_ast_type_error;
		} else {
			Ake_ast* tu = Ast_node_clone(n);
			struct type_def* td = NULL;
			malloc_safe((void**)&td, sizeof(struct type_def));
			type_def_init(td);
			td->type = type_struct;
			buffer_copy(&id->value, &td->name);
			td->composite = tu;

			struct symbol* sym = NULL;
			malloc_safe((void**)&sym, sizeof(struct symbol));
			symbol_init(sym);
			sym->type = Symbol_type_type;
			sym->td = td;
			environment_put(ps->st->top, &id->value, sym);
            n->sym = sym;
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

/* parse_return -> return expr | return */
Ake_ast* parse_return(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_return;

	struct token* ret = NULL;
	if (!match(ps, token_return, "expected return", &ret, n)) {
        assert(false);
        /* test case: no test case needed */
    }

	Ake_ast* a = NULL;
    a = parse_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
	}

	if (n->type != Ake_ast_type_error) {
		if (a) {
			if (!a->tu) {
				error_list_set(ps->el, &a->loc, "return expression has no value");
				/* test case: test_parse_return_error_no_value */
                n->type = Ake_ast_type_error;
			} else {
				n->tu = Type_use_clone(a->tu);
				Ake_ast* fd = get_current_function(ps->st->top);
				if (!fd) {
					error_list_set(ps->el, &ret->loc, "return statement outside of function");
					/* test case: test_parse_return_error_outside_of_function */
                    n->type = Ake_ast_type_error;
				} else {
                    Ake_ast* proto = Ast_node_get(fd, 0);
                    if (!check_return_type(ps, proto, n, &ret->loc)) {
                        /* test case: test_parse_return_error_type_does_not_match */
                        n->type = Ake_ast_type_error;
                    }
				}
			}
		}
	}

    token_destroy(ret);
    free(ret);

	return n;
}

/* parse_let = let let_lseq :: type | let let_lseq :: type = let_rseq */
Ake_ast* parse_let(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_let;

    struct token* vrt = NULL;
    if (!match(ps, token_let, "expected let", &vrt, n)) {
        /* test case: no test case needed */
        assert(false);
    }

    token_destroy(vrt);
    free(vrt);

    consume_newline(ps, n);

    Ake_ast* a = NULL;
    a = parse_let_lseq(ps);
    if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    assert(a);
    Ake_ast_add(n, a);

    if (!a->head) {
        error_list_set(ps->el, &a->loc, "expected variable(s) after let");
        n->type = Ake_ast_type_error;
    }

    consume_newline(ps, n);

    struct token* dc = NULL;
    if (!match(ps, token_colon, "expected colon after variable(s)", &dc, n)) {
        n->type = Ake_ast_type_error;
    }
    token_destroy(dc);
    free(dc);

    consume_newline(ps, n);

    Ake_ast* type_node = NULL;
    type_node = parse_type(ps);
    declare_type(ps, type_node, a);
    if (type_node && type_node->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }
    if (type_node) {
        Ake_ast_add(n, type_node);
    } else {
        struct location type_use_loc = get_location(ps);
        error_list_set(ps->el, &type_use_loc, "expected type");
        n->type = Ake_ast_type_error;
    }

    /* add variables */

    struct token* t0 = get_lookahead(ps);

    if (t0 && t0->type == token_equal) {
        struct token* equal = NULL;
        if (!match(ps, token_equal, "expected equal", &equal, n)) {
            /* test case: no test case needed */
            assert(false);
        }
        token_destroy(equal);
        free(equal);

        consume_newline(ps, n);

        Ake_ast* b = NULL;
        b = parse_let_rseq(ps);
        if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (b) {
            Ake_ast_add(n, b);
        } else {
            struct location b_loc = get_location(ps);
            error_list_set(ps->el, &b_loc, "expected expression");
            n->type = Ake_ast_type_error;
        }

        if (n->type != Ake_ast_type_error) {
            size_t a_count = Ast_node_count_children(a);
            size_t b_count = Ast_node_count_children(b);
            if (a_count != b_count) {
                error_list_set(ps->el, &a->loc, "lvalue count does not equal rvalue count");
                n->type = Ake_ast_type_error;
            } else {
                for (int i = 0; i < a_count; i++) {
                    Ake_ast* y = Ast_node_get(b, i);
                    if (!y->tu) {
                        error_list_set(ps->el, &b->loc, "cannot assign with operand that has no value");
                        n->type = Ake_ast_type_error;
                    } else if (!type_use_can_cast(type_node->tu, y->tu)) {
                        error_list_set(ps->el, &b->loc, "values in assignment are not compatible");
                        n->type = Ake_ast_type_error;
                    }
                }
            }
        }

        if (n->type != Ake_ast_type_error) {
            /* adjust rhs to the bit_size of lhs */
            if (a && type_node && b) {
                Ake_ast* rhs = b->head;
                while (rhs) {
                    Override_rhs(type_node->tu, rhs);
                    rhs = rhs->next;
                }
            }
        }

        if (n->type != Ake_ast_type_error) {
            assert(a);
            Ake_ast* lhs = a->head;
            while (lhs) {
                assert(lhs->type == Ake_ast_type_id);
                struct symbol* sym = NULL;
                sym = environment_get(ps->st->top, &lhs->value);
                assert(sym);
                sym->assign_count++;
                lhs = lhs->next;
            }
        }
    }

    return n;
}

/* let_lseq -> id let_lseq' */
/* let_lseq' -> , id let_lseq' */
Ake_ast* parse_let_lseq(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_let_lseq;

    struct token* t0 = get_lookahead(ps);
    if (t0->type != token_mut && t0->type != token_id) {
        return n;
    }

    bool is_mut = false;
    if (t0->type == token_mut) {
        struct token* mut = NULL;
        if (!match(ps, token_mut, "expected mut", &mut, n)) {
            assert(false);
        }
        token_destroy(mut);
        free(mut);

        is_mut = true;
    }

    struct token* id = NULL;
    if (!match(ps, token_id, "expected an id", &id, n)) {
        n->type = Ake_ast_type_error;
    }

    Ake_ast* a = NULL;
    Ake_ast_create(&a);
    a->type = Ake_ast_type_id;
    a->is_mut = is_mut;
    buffer_copy(&id->value, &a->value);
    Ake_ast_add(n, a);
    a->loc = id->loc;

    token_destroy(id);
    free(id);

    while (true) {
        t0 = get_lookahead(ps);
        if (!t0 || t0->type != token_comma) {
            break;
        }

        struct token* comma = NULL;
        if (!match(ps, token_comma, "expected comma", &comma, n)) {
            /* test case: no test case needed */
            assert(false);
        }
        token_destroy(comma);
        free(comma);

        id = NULL;
        if (!match(ps, token_id, "expected id", &id, n)) {
            error_list_set(ps->el, &id->loc, "expected id");
            n->type = Ake_ast_type_error;
            break;
        }

        a = NULL;
        Ake_ast_create(&a);
        a->type = Ake_ast_type_id;
        buffer_copy(&id->value, &a->value);
        Ake_ast_add(n, a);
        a->loc = id->loc;

        token_destroy(id);
        free(id);
    }

    return n;
}

/* let_rseq -> simple_expr let_rseq' */
/* let_rseq' -> , simple_expr let_rseq' */
Ake_ast* parse_let_rseq(struct parse_state* ps)
{
    Ake_ast* a = NULL;
    a = parse_simple_expr(ps);
    if (!a) {
        return NULL;
    }

    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_let_rseq;

    if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    Ake_ast_add(n, a);

    while (true) {
        struct token* t0 = get_lookahead(ps);
        if (!t0 || t0->type != token_comma) {
            break;
        }

        struct token* comma = NULL;
        if (!match(ps, token_comma, "expected comma", &comma, n)) {
            assert(false && "should see comma");
        }

        token_destroy(comma);
        free(comma);

        Ake_ast* b = NULL;
        b = parse_simple_expr(ps);
        if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (b) {
            Ake_ast_add(n, b);
        } else {
            struct location b_loc = get_location(ps);
            error_list_set(ps->el, &b_loc, "expected an expression");
            n->type = Ake_ast_type_error;
        }
    }

    return n;
}

Ake_ast* parse_impl(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_impl;

    struct token* imp = NULL;
    if (!match(ps, token_impl, "expected impl", &imp, n)) {
        /* test case: no test case needed */
        assert(false);
    }
    token_destroy(imp);
    free(imp);

    struct token* id = NULL;
    if (!match(ps, token_id, "expected identifier", &id, n)) {
        n->type = Ake_ast_type_error;
    }

    consume_newline(ps, n);

    Ake_ast* struct_type = NULL;
    if (id) {
        struct symbol* sym = environment_get(ps->st->top, &id->value);
        if (sym->type == Symbol_type_type) {
            if (sym->td->type == type_struct) {
                Ake_ast* type_node = NULL;
                Ake_ast_create(&type_node);
                type_node->type = Ake_ast_type_type;
                Type_use* tu = NULL;
                Type_use_create(&tu);
                tu->td = sym->td;
                type_node->tu = tu;
                struct_type = type_node;
            }
        }
    }

    token_destroy(id);
    free(id);

    while (true) {
        struct token* t = get_lookahead(ps);
        if (t->type == token_fn) {
            Ake_ast* func = parse_function(ps, true, struct_type);
            Ake_ast_add(n, func);

            struct token* t2 = get_lookahead(ps);
            if (t2->type == token_end) {
                struct token* end = NULL;
                if (!match(ps, token_end, "expected end", &end, n)) {
                    assert(false);
                }
                token_destroy(end);
                free(end);
                break;
            } else {
                bool has_sep;
                parse_separator(ps, n, &has_sep);
                if (!has_sep) {
                    error_list_set(ps->el, &t->loc, "expected separator");
                    n->type = Ake_ast_type_error;
                }
            }
        } else if (t->type == token_end) {
            struct token* end = NULL;
            if (!match(ps, token_end, "expected end", &end, n)) {
                assert(false);
            }
            token_destroy(end);
            free(end);
            break;
        } else {
            error_list_set(ps->el, &t->loc, "expected fn or end");
            n->type = Ake_ast_type_error;
            break;
        }
    }

    Ake_ast_destroy(struct_type);

    return n;
}