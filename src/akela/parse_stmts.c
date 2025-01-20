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

Ake_ast* Ake_parse_stmt(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_while(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_for(struct Ake_parse_state* ps);
void Ake_parse_for_range(struct Ake_parse_state* ps, Ake_ast* parent);
void Ake_parse_for_iteration(struct Ake_parse_state* ps, Ake_ast* parent);
Ake_ast* Ake_parse_module(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_struct(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_return(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_let(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_let_lseq(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_let_rseq(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_extern(struct Ake_parse_state* ps);
Ake_ast* Ake_parse_impl(struct Ake_parse_state* ps);

/* stmts -> stmt stmts' */
/* stmts' -> separator stmt stmts' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_stmts(struct Ake_parse_state* ps, bool suppress_env)
{
	Ake_ast* n = NULL;
	Ake_ast* last = NULL;

	struct Ake_environment* saved = NULL;
	struct Ake_environment* env = NULL;
	if (!suppress_env) {
        Ake_environment_begin(ps->st);
	}

    Ake_ast_create(&n);
	n->type = Ake_ast_type_stmts;

	Ake_ast* a = NULL;
	a = Ake_parse_stmt(ps);
    if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
		last = a;
	}

	while (true) {
		bool has_separator = false;
		Ake_parse_separator(ps, n, &has_separator);

		if (!has_separator) {
			break;
		}

		Ake_ast* b = NULL;
		b = Ake_parse_stmt(ps);
        if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

		if (b) {
            Ake_ast_add(n, b);
			last = b;
		}
	}

	if (!suppress_env) {
        Ake_environment_end(ps->st);
	}

	if (n->type != Ake_ast_type_error) {
		if (last) {
			if (last->tu) {
                n->tu = Ake_type_use_clone(last->tu);
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
Ake_ast* Ake_parse_stmt(struct Ake_parse_state* ps)
{
	Ake_ast* n = NULL;

    struct Ake_token* t0 = Ake_get_lookahead(ps);
    assert(t0);

	if (t0->type == Ake_token_while) {
		n = Ake_parse_while(ps);
	} else if (t0->type == Ake_token_for) {
		n = Ake_parse_for(ps);
	} else if (t0->type == Ake_token_module) {
		n = Ake_parse_module(ps);
	} else if (t0->type == Ake_token_struct) {
		n = Ake_parse_struct(ps);
	} else if (t0->type == Ake_token_return) {
        n = Ake_parse_return(ps);
    } else if (t0->type == Ake_token_const || t0->type == Ake_token_var) {
        n = Ake_parse_let(ps);
    } else if (t0->type == Ake_token_extern) {
        n = Ake_parse_extern(ps);
    } else if (t0->type == Ake_token_impl) {
        n = Ake_parse_impl(ps);
	} else {
        n = Ake_parse_expr(ps);
	}

    return n;
}

Ake_ast* Ake_parse_extern(struct Ake_parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_extern;

    struct Ake_token* ext = NULL;
    if (!Ake_match(ps, Ake_token_extern, "expected extern", &ext, n)) {
        /* test case: no test case needed */
        assert(false);
    }
    Ake_token_destroy(ext);
    free(ext);

    Ake_ast* proto = NULL;
    bool has_id;
    proto = Ake_parse_prototype(ps, false, true, false, false, &has_id);
    if (proto) {
        Ake_ast_add(n, proto);
        if (proto->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }
        Ake_type_use* tu = Ake_proto2type_use(ps->st, proto, NULL);
        n->tu = tu;
    }

    if (!has_id) {
        Zinc_error_list_set(ps->el, &proto->loc, "expected an identifier after extern");
        n->type = Ake_ast_type_error;
    }

    if (n->type != Ake_ast_type_error) {
        if (ps->st->top != ps->st->global) {
            Zinc_error_list_set(ps->el, &n->loc, "extern only allowed at top level scope");
            n->type = Ake_ast_type_error;
        }

        Ake_ast *id_node = Ast_node_get(proto, 0);
        /* check and save symbol */
        struct Ake_symbol *search = Ake_environment_get_local(ps->st->top, &id_node->value);
        if (search) {
            Zinc_string_finish(&id_node->value);
            Zinc_error_list_set(
                ps->el,
                &id_node->loc,
                "duplicate declaration in same scope: %s",
                id_node->value.buf);
            n->type = Ake_ast_type_error;
        } else {
            struct Ake_symbol *sym = Ake_environment_get(ps->st->top, &id_node->value);
            if (sym && sym->td) {
                Zinc_string_finish(&id_node->value);
                Zinc_error_list_set(
                    ps->el,
                    &id_node->loc,
                    "identifier reserved as a type: %s",
                    id_node->value.buf);
                n->type = Ake_ast_type_error;
            } else {
                struct Ake_symbol *new_sym = NULL;
                Zinc_malloc_safe((void **) &new_sym, sizeof(struct Ake_symbol));
                Ake_symbol_init(new_sym);
                new_sym->type = Ake_symbol_type_variable;
                new_sym->tu = Ake_type_use_clone(n->tu);
                Ake_environment_put(ps->st->top, &id_node->value, new_sym);
                n->sym = new_sym;
            }
        }
    }

    if (n->type != Ake_ast_type_error) {
        Ake_ast* id = Ast_node_get(proto, 0);
        Zinc_string_list_add_bf(ps->extern_list, &id->value);
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_while(struct Ake_parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_while;

	struct Ake_token* whl = NULL;
	if (!Ake_match(ps, Ake_token_while, "expecting while", &whl, n)) {
        n->type = Ake_ast_type_error;
    }

	Ake_ast* a = NULL;
    a = Ake_parse_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (a) {
        Ake_ast_add(n, a);
    } else {
        struct Zinc_location a_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &a_loc, "expected expression after while");
        n->type = Ake_ast_type_error;
		/* test case: test_parse_while_error_expected_expression */
	}

	Ake_ast* b = NULL;
    b = Ake_parse_stmts(ps, false);
	if (b && b->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (b) {
        Ake_ast_add(n, b);
    }

	struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        n->type = Ake_ast_type_error;
        /* test case: test_parse_while_error_expected_end */
    }

	Ake_token_destroy(whl);
	free(whl);
	Ake_token_destroy(end);
	free(end);

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_for(struct Ake_parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);

	struct Ake_token* f = NULL;
	if (!Ake_match(ps, Ake_token_for, "expected for", &f, n)) {
        /* test case: test cases not needed */
        n->type = Ake_ast_type_error;
    }

    Ake_environment_begin(ps->st);

    Ake_consume_newline(ps, n);

	Ake_ast* dec = NULL;
    dec = Ake_parse_declaration(ps, true, false, true);
	if (dec && dec->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (dec) {
        Ake_ast_add(n, dec);
    }

    Ake_consume_newline(ps, n);

	struct Ake_token* t0 = Ake_get_lookahead(ps);

	if (t0 && t0->type == Ake_token_equal) {
        if (n->type == Ake_ast_type_none) {
            n->type = Ake_ast_type_for_range;
        }
		Ake_parse_for_range(ps, n);

	} else if (t0 && t0->type == Ake_token_in) {
        if (n->type == Ake_ast_type_none) {
            n->type = Ake_ast_type_for_iteration;
        }
		Ake_parse_for_iteration(ps, n);

	} else {
		struct Zinc_location loc_error = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &loc_error, "expected '=' or 'in' after for element declaration");
        n->type = Ake_ast_type_error;
		/* test case: test_parse_for_error_after_declaration */
	}

	Ake_ast* c = NULL;
    c = Ake_parse_stmts(ps, true);
	if (c && c->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        /* test case: test_parse_for_error_expected_end */
        n->type = Ake_ast_type_error;
    }

    if (c) {
        Ake_ast_add(n, c);
    }

	Ake_token_destroy(f);
	free(f);
	Ake_token_destroy(end);
	free(end);

    Ake_environment_end(ps->st);

	return n;
}

/* for_range -> for id = expr:expr stmts end */
void Ake_parse_for_range(struct Ake_parse_state* ps, Ake_ast* parent)
{
	struct Ake_token* equal = NULL;
	if (!Ake_match(ps, Ake_token_equal, "expected equal", &equal, parent)) {
        /* test case: no test case needed */
        parent->type = Ake_ast_type_error;
    }

    Ake_consume_newline(ps, parent);

	/* start expr */
	Ake_ast* a = NULL;
    a = Ake_parse_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        parent->type = Ake_ast_type_error;
    }

	if (!a) {
        struct Zinc_location a_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &a_loc, "expected range start");
        parent->type = Ake_ast_type_error;
		/* test case: test_parse_for_error_expected_range_start */
	}

    Ake_consume_newline(ps, parent);

	struct Ake_token* colon = NULL;
	if (!Ake_match(ps, Ake_token_colon, "expected colon", &colon, parent)) {
        /* test case: test_parse_for_error_expected_colon */
        parent->type = Ake_ast_type_error;
    }

    Ake_consume_newline(ps, parent);

	/* end expr */
	Ake_ast* b = NULL;
    b = Ake_parse_expr(ps);
	if (b && b->type == Ake_ast_type_error) {
        parent->type = Ake_ast_type_error;
    }

	if (!b) {
        struct Zinc_location b_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &b_loc, "expected range end");
        parent->type = Ake_ast_type_error;
		/* test case: test_parse_for_error_expected_range_end */
	}

	if (a) {
        Ake_ast_add(parent, a);
    }
    if (b) {
        Ake_ast_add(parent, b);
	}

	Ake_token_destroy(equal);
	free(equal);
	Ake_token_destroy(colon);
	free(colon);

	if (parent->type != Ake_ast_type_error) {
		assert(a);
		if (!a->tu) {
			Zinc_error_list_set(ps->el, &a->loc, "start range expression has no value");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_range_error_start_no_value */
		} else {
			assert(a->tu->td);
			if (!Ake_is_numeric(a->tu->td)) {
				Zinc_error_list_set(ps->el, &a->loc, "start range expression is not numeric");
                parent->type = Ake_ast_type_error;
				/* test case: test_parse_for_range_error_start_not_numeric */
			}
		}

		assert(b);
		if (!b->tu) {
			Zinc_error_list_set(ps->el, &b->loc, "end range expression has no value");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_range_error_end_no_value */
		} else {
			assert(b->tu->td);
			if (!Ake_is_numeric(b->tu->td)) {
				Zinc_error_list_set(ps->el, &b->loc, "end range expression is not numeric");
                parent->type = Ake_ast_type_error;
				/* test case: test_parse_for_range_error_end_not_numeric */
			}
		}
	}
}

/* for_iteration -> for id in expr stmts end */
void Ake_parse_for_iteration(struct Ake_parse_state* ps, Ake_ast* parent)
{
	struct Ake_token* in = NULL;
	if (!Ake_match(ps, Ake_token_in, "expecting in", &in, parent)) {
        /* test case: no test case necessary */
        parent->type = Ake_ast_type_error;
    }

    Ake_consume_newline(ps, parent);

	/* expr */
	Ake_ast* list = NULL;
    list = Ake_parse_expr(ps);
	if (list && list->type == Ake_ast_type_error) {
        parent->type = Ake_ast_type_error;
    }

	if (!list) {
        struct Zinc_location list_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &list_loc, "expected for iteration expression");
        parent->type = Ake_ast_type_error;
        /* test case: test_parse_for_error_expected_iteration_expression */
	}

    if (list) {
        Ake_ast_add(parent, list);
    }

	if (parent->type != Ake_ast_type_error) {
		Ake_ast* element = Ast_node_get(parent, 0);
		Ake_ast* element_type_node = Ast_node_get(element, 1);

		Ake_type_use* list_tu = list->tu;

		if (!list_tu) {
			Zinc_error_list_set(ps->el, &list->loc, "iteration expression has no value");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_iteration_error_no_value */
		} else if (!list_tu->is_array) {
			Zinc_error_list_set(ps->el, &list->loc, "iteration expression is not an array");
            parent->type = Ake_ast_type_error;
			/* test case: test_parse_for_iteration_error_no_child_element */
		} else {
            Ake_type_use* element_tu2 = Ake_type_use_clone(list_tu);
            Ake_type_use_reduce_dimension(element_tu2);
			if (!Ake_type_use_can_cast(element_tu2, element_type_node->tu)) {
                parent->type = Ake_ast_type_error;
				Zinc_error_list_set(ps->el, &list->loc, "cannot cast list element");
				/* test case: test_parse_for_iteration_error_cannot_cast */
			}
            Ake_type_use_destroy(element_tu2);
		}
	}

	Ake_token_destroy(in);
	free(in);
}

/* parse_module -> module id stmts end */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_module(struct Ake_parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_module;

	struct Ake_token* module = NULL;
	if (!Ake_match(ps, Ake_token_module, "expected module", &module, n)) {
        assert(false);
        /* test case: no test case needed */
    }

	Ake_token_destroy(module);
	free(module);

    Ake_environment_begin(ps->st);

	struct Ake_token* id = NULL;
	if (!Ake_match(ps, Ake_token_id, "expected identifier after module", &id, n)) {
        /* test case: test_parse_module_expected_identifier */
        n->type = Ake_ast_type_error;
    }

	Ake_ast* a = NULL;
    a = Ake_parse_stmts(ps, true);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	Ake_transfer_module_symbols(ps->st->top, ps->st->top->prev, &id->value);

    Ake_environment_end(ps->st);

	struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        /* test case: test_parse_module_expected_end */
        n->type = Ake_ast_type_error;
    }

	Ake_token_destroy(end);
	free(end);

    Ake_ast* id_node = NULL;
    Ake_ast_create(&id_node);
    id_node->type = Ake_ast_type_id;
    if (id) {
        Zinc_string_copy(&id->value, &id_node->value);
    }
    Ake_ast_add(n, id_node);
    Ake_ast_add(n, a);

	if (n->type != Ake_ast_type_error) {
		struct Ake_symbol* sym = Ake_environment_get(ps->st->top, &id->value);
		if (sym) {
			Zinc_string_finish(&id->value);
			Zinc_error_list_set(ps->el, &id->loc, "variable already used: %s", id->value.buf);
            /* test case: test_parse_module_duplicate_declaration */
            n->type = Ake_ast_type_error;
		} else {
			struct Zinc_string bf;
			Zinc_string_init(&bf);
			Zinc_string_add_str(&bf, "Module");
			sym = Ake_environment_get(ps->st->top, &bf);
			Zinc_string_destroy(&bf);
			assert(sym);
			assert(sym->td);

			Ake_type_use* tu = NULL;
            Ake_type_use_create(&tu);
			tu->td = sym->td;

			struct Ake_symbol* new_sym = NULL;
			Zinc_malloc_safe((void**)&new_sym, sizeof(struct Ake_symbol));
			Ake_symbol_init(new_sym);
			new_sym->tk_type = Ake_token_id;
			new_sym->tu = tu;
			Ake_environment_put(ps->st->top, &id->value, new_sym);
		}
	}

	Ake_token_destroy(id);
	free(id);

	return n;
}

/* parse_struct -> struct id struct_stmts end */
/* struct_stmts -> declaration struct_stmts | e */
/* struct_stmts' -> separator declaration | e */
Ake_ast* Ake_parse_struct(struct Ake_parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_struct;

    struct Ake_token* st = NULL;
	if (!Ake_match(ps, Ake_token_struct, "expected struct", &st, n)) {
        /* test case: no test case needed */
        assert(false);
    }

	struct Ake_token* id = NULL;
	if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
        /* test case: test_parse_struct_error_expected_identifier */
        n->type = Ake_ast_type_error;
    }
    if (id) {
        Zinc_string_copy(&id->value, &n->value);
    }

	Ake_ast* a = NULL;
    a = Ake_parse_declaration(ps, false, false, true);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
	}

	while (true) {
		bool has_separator;
		Ake_parse_separator(ps, n, &has_separator);

		if (!has_separator) {
			break;
		}

		Ake_ast* b = NULL;
        b = Ake_parse_declaration(ps, false, false, true);
		if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

		if (b) {
            Ake_ast_add(n, b);
		}
	}

	struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        /* test case: test_parse_struct_error_expected_end */
        n->type = Ake_ast_type_error;
    }

	if (n->type != Ake_ast_type_error) {
		struct Ake_symbol* search = Ake_environment_get_local(ps->st->top, &id->value);
		if (search) {
			Zinc_string_finish(&id->value);
			Zinc_error_list_set(ps->el, &id->loc, "duplicate variable in scope: %s", id->value.buf);
            /* test case: test_parse_struct_error_duplicate */
            n->type = Ake_ast_type_error;
		} else {
			Ake_ast* tu = Ake_ast_clone(n);
			struct Ake_type_def* td = NULL;
			Zinc_malloc_safe((void**)&td, sizeof(struct Ake_type_def));
			Ake_type_def_init(td);
			td->type = Ake_type_struct;
			Zinc_string_copy(&id->value, &td->name);
			td->composite = tu;

			struct Ake_symbol* sym = NULL;
			Zinc_malloc_safe((void**)&sym, sizeof(struct Ake_symbol));
			Ake_symbol_init(sym);
			sym->type = Ake_symbol_type_type;
			sym->td = td;
			Ake_environment_put(ps->st->top, &id->value, sym);
            n->sym = sym;
		}
	}

	Ake_token_destroy(st);
	free(st);
	Ake_token_destroy(id);
	free(id);
	Ake_token_destroy(end);
	free(end);

	return n;
}

/* parse_return -> return expr | return */
Ake_ast* Ake_parse_return(struct Ake_parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_return;

	struct Ake_token* ret = NULL;
	if (!Ake_match(ps, Ake_token_return, "expected return", &ret, n)) {
        assert(false);
        /* test case: no test case needed */
    }

	Ake_ast* a = NULL;
    a = Ake_parse_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
	}

	if (n->type != Ake_ast_type_error) {
		if (a) {
			if (!a->tu) {
				Zinc_error_list_set(ps->el, &a->loc, "return expression has no value");
				/* test case: test_parse_return_error_no_value */
                n->type = Ake_ast_type_error;
			} else {
				n->tu = Ake_type_use_clone(a->tu);
				Ake_ast* fd = Ake_get_current_function(ps->st->top);
				if (!fd) {
					Zinc_error_list_set(ps->el, &ret->loc, "return statement outside of function");
					/* test case: test_parse_return_error_outside_of_function */
                    n->type = Ake_ast_type_error;
				} else {
                    Ake_ast* proto = Ast_node_get(fd, 0);
                    if (!Ake_check_return_type(ps, proto, n, &ret->loc)) {
                        /* test case: test_parse_return_error_type_does_not_match */
                        n->type = Ake_ast_type_error;
                    }
				}
			}
		}
	}

    Ake_token_destroy(ret);
    free(ret);

	return n;
}

/* parse_let = let let_lseq :: type | let let_lseq :: type = let_rseq */
Ake_ast* Ake_parse_let(struct Ake_parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);

	Ake_get_lookahead(ps);
	if (ps->lookahead->type == Ake_token_const) {
		Ake_token* t = NULL;
		if (!Ake_match(ps, Ake_token_const, "expected const", &t, n)) {
			assert(false && "expected const");
		}
		Ake_token_destroy(t);
		free(t);
		n->type = Ake_ast_type_const;
	} else if (ps->lookahead->type == Ake_token_var) {
		Ake_token* t = NULL;
		if (!Ake_match(ps, Ake_token_var, "expected var", &t, n)) {
			assert(false && "expected var");;
		}
		Ake_token_destroy(t);
		free(t);
		n->type = Ake_ast_type_var;
	} else {
		assert(false && "expected const or var");
	}

    Ake_consume_newline(ps, n);

    Ake_ast* a = NULL;
    a = Ake_parse_let_lseq(ps);
    if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    assert(a);
    Ake_ast_add(n, a);

    if (!a->head) {
        Zinc_error_list_set(ps->el, &a->loc, "expected variable(s) after let");
        n->type = Ake_ast_type_error;
    }

    Ake_consume_newline(ps, n);

    struct Ake_token* dc = NULL;
    if (!Ake_match(ps, Ake_token_colon, "expected colon after variable(s)", &dc, n)) {
        n->type = Ake_ast_type_error;
    }
    Ake_token_destroy(dc);
    free(dc);

    Ake_consume_newline(ps, n);

    Ake_ast* type_node = NULL;
    type_node = Ake_parse_type(ps);
    Ake_declare_type(ps, type_node, a);
    if (type_node && type_node->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }
    if (type_node) {
        Ake_ast_add(n, type_node);
    } else {
        struct Zinc_location type_use_loc = Ake_get_location(ps);
        Zinc_error_list_set(ps->el, &type_use_loc, "expected type");
        n->type = Ake_ast_type_error;
    }

    /* add variables */

    struct Ake_token* t0 = Ake_get_lookahead(ps);

    if (t0 && t0->type == Ake_token_equal) {
        struct Ake_token* equal = NULL;
        if (!Ake_match(ps, Ake_token_equal, "expected equal", &equal, n)) {
            /* test case: no test case needed */
            assert(false);
        }
        Ake_token_destroy(equal);
        free(equal);

        Ake_consume_newline(ps, n);

        Ake_ast* b = NULL;
        b = Ake_parse_let_rseq(ps);
        if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (b) {
            Ake_ast_add(n, b);
        } else {
            struct Zinc_location b_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &b_loc, "expected expression");
            n->type = Ake_ast_type_error;
        }

        if (n->type != Ake_ast_type_error) {
            size_t a_count = Ake_ast_count_children(a);
            size_t b_count = Ake_ast_count_children(b);
            if (a_count != b_count) {
                Zinc_error_list_set(ps->el, &a->loc, "lvalue count does not equal rvalue count");
                n->type = Ake_ast_type_error;
            } else {
                for (int i = 0; i < a_count; i++) {
                    Ake_ast* y = Ast_node_get(b, i);
                    if (!y->tu) {
                        Zinc_error_list_set(ps->el, &b->loc, "cannot assign with operand that has no value");
                        n->type = Ake_ast_type_error;
                    } else if (!Ake_type_use_can_cast(type_node->tu, y->tu)) {
                        Zinc_error_list_set(ps->el, &b->loc, "values in assignment are not compatible");
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
                    Ake_Override_rhs(type_node->tu, rhs);
                    rhs = rhs->next;
                }
            }
        }

        if (n->type != Ake_ast_type_error) {
            assert(a);
            Ake_ast* lhs = a->head;
            while (lhs) {
                assert(lhs->type == Ake_ast_type_id);
                struct Ake_symbol* sym = NULL;
                sym = Ake_environment_get(ps->st->top, &lhs->value);
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
Ake_ast* Ake_parse_let_lseq(struct Ake_parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_let_lseq;

    struct Ake_token* t0 = Ake_get_lookahead(ps);
    if (t0->type != Ake_token_mut && t0->type != Ake_token_id) {
        return n;
    }

    bool is_mut = false;
    if (t0->type == Ake_token_mut) {
        struct Ake_token* mut = NULL;
        if (!Ake_match(ps, Ake_token_mut, "expected mut", &mut, n)) {
            assert(false);
        }
        Ake_token_destroy(mut);
        free(mut);

        is_mut = true;
    }

    struct Ake_token* id = NULL;
    if (!Ake_match(ps, Ake_token_id, "expected an id", &id, n)) {
        n->type = Ake_ast_type_error;
    }

    Ake_ast* a = NULL;
    Ake_ast_create(&a);
    a->type = Ake_ast_type_id;
    a->is_mut = is_mut;
    Zinc_string_copy(&id->value, &a->value);
    Ake_ast_add(n, a);
    a->loc = id->loc;

    Ake_token_destroy(id);
    free(id);

    while (true) {
        t0 = Ake_get_lookahead(ps);
        if (!t0 || t0->type != Ake_token_comma) {
            break;
        }

        struct Ake_token* comma = NULL;
        if (!Ake_match(ps, Ake_token_comma, "expected comma", &comma, n)) {
            /* test case: no test case needed */
            assert(false);
        }
        Ake_token_destroy(comma);
        free(comma);

        id = NULL;
        if (!Ake_match(ps, Ake_token_id, "expected id", &id, n)) {
            Zinc_error_list_set(ps->el, &id->loc, "expected id");
            n->type = Ake_ast_type_error;
            break;
        }

        a = NULL;
        Ake_ast_create(&a);
        a->type = Ake_ast_type_id;
        Zinc_string_copy(&id->value, &a->value);
        Ake_ast_add(n, a);
        a->loc = id->loc;

        Ake_token_destroy(id);
        free(id);
    }

    return n;
}

/* let_rseq -> simple_expr let_rseq' */
/* let_rseq' -> , simple_expr let_rseq' */
Ake_ast* Ake_parse_let_rseq(struct Ake_parse_state* ps)
{
    Ake_ast* a = NULL;
    a = Ake_parse_simple_expr(ps);
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
        struct Ake_token* t0 = Ake_get_lookahead(ps);
        if (!t0 || t0->type != Ake_token_comma) {
            break;
        }

        struct Ake_token* comma = NULL;
        if (!Ake_match(ps, Ake_token_comma, "expected comma", &comma, n)) {
            assert(false && "should see comma");
        }

        Ake_token_destroy(comma);
        free(comma);

        Ake_ast* b = NULL;
        b = Ake_parse_simple_expr(ps);
        if (b && b->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (b) {
            Ake_ast_add(n, b);
        } else {
            struct Zinc_location b_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &b_loc, "expected an expression");
            n->type = Ake_ast_type_error;
        }
    }

    return n;
}

Ake_ast* Ake_parse_impl(struct Ake_parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_impl;

    struct Ake_token* imp = NULL;
    if (!Ake_match(ps, Ake_token_impl, "expected impl", &imp, n)) {
        /* test case: no test case needed */
        assert(false);
    }
    Ake_token_destroy(imp);
    free(imp);

    struct Ake_token* id = NULL;
    if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
        n->type = Ake_ast_type_error;
    }

    Ake_consume_newline(ps, n);

    Ake_ast* struct_type = NULL;
    if (id) {
        struct Ake_symbol* sym = Ake_environment_get(ps->st->top, &id->value);
        if (sym->type == Ake_symbol_type_type) {
            if (sym->td->type == Ake_type_struct) {
                Ake_ast* type_node = NULL;
                Ake_ast_create(&type_node);
                type_node->type = Ake_ast_type_type;
                Ake_type_use* tu = NULL;
                Ake_type_use_create(&tu);
                tu->td = sym->td;
                type_node->tu = tu;
                struct_type = type_node;
            }
        }
    }

    Ake_token_destroy(id);
    free(id);

    while (true) {
        struct Ake_token* t = Ake_get_lookahead(ps);
        if (t->type == Ake_token_fn) {
            Ake_ast* func = Ake_parse_function(ps, true, struct_type);
            Ake_ast_add(n, func);

            struct Ake_token* t2 = Ake_get_lookahead(ps);
            if (t2->type == Ake_token_end) {
                struct Ake_token* end = NULL;
                if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
                    assert(false);
                }
                Ake_token_destroy(end);
                free(end);
                break;
            } else {
                bool has_sep;
                Ake_parse_separator(ps, n, &has_sep);
                if (!has_sep) {
                    Zinc_error_list_set(ps->el, &t->loc, "expected separator");
                    n->type = Ake_ast_type_error;
                }
            }
        } else if (t->type == Ake_token_end) {
            struct Ake_token* end = NULL;
            if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
                assert(false);
            }
            Ake_token_destroy(end);
            free(end);
            break;
        } else {
            Zinc_error_list_set(ps->el, &t->loc, "expected fn or end");
            n->type = Ake_ast_type_error;
            break;
        }
    }

    Ake_ast_destroy(struct_type);

    return n;
}