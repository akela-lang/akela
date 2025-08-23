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
#include "type.h"
#include <assert.h>

Ake_Ast* Ake_parse_stmt(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_while(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_for(struct Ake_parse_state* ps);
void Ake_parse_for_range(struct Ake_parse_state* ps, Ake_Ast* parent);
void Ake_parse_for_iteration(struct Ake_parse_state* ps, Ake_Ast* parent);
Ake_Ast* Ake_parse_module(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_struct(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_return(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_let(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_extern(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_impl(struct Ake_parse_state* ps);

/* stmts -> stmt stmts' */
/* stmts' -> separator stmt stmts' | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_stmts(Ake_parse_state* ps, bool suppress_env, bool is_global)
{
	Ake_Ast* n = NULL;
	Ake_Ast* last = NULL;

	struct Ake_environment* saved = NULL;
	struct Ake_environment* env = NULL;

	Ake_AstCreate(&n);
	n->kind = Ake_ast_type_stmts;

	if (!suppress_env) {
        Ake_begin_environment(ps->st, n);
		if (is_global) {
			ps->st->top->is_global = true;
		}
	}

	Ake_Ast* a = NULL;
	a = Ake_parse_stmt(ps);

	if (a) {
        Ake_AstAdd(n, a);
		last = a;
	}

	while (true) {
		bool has_separator = false;
		Ake_parse_separator(ps, n, &has_separator);

		if (!has_separator) {
			break;
		}

		Ake_Ast* b = NULL;
		b = Ake_parse_stmt(ps);

		if (b) {
            Ake_AstAdd(n, b);
			last = b;
		}
	}

	if (!suppress_env) {
        Ake_end_environment(ps->st);
	}

	if (!n->has_error) {
		if (last) {
			if (last->type) {
                n->type = Ake_TypeClone(last->type);
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
Ake_Ast* Ake_parse_stmt(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;

    struct Ake_token* t0 = Ake_get_lookahead(ps);
    assert(t0);

	if (t0->type == Ake_token_while) {
		n = Ake_parse_while(ps);
	} else if (t0->type == Ake_token_for) {
		n = Ake_parse_for(ps);
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

Ake_Ast* Ake_parse_extern(struct Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;
    Ake_AstCreate(&n);
    n->kind = Ake_ast_type_extern;

    struct Ake_token* ext = NULL;
    if (!Ake_match(ps, Ake_token_extern, "expected extern", &ext, n)) {
        /* test case: no test case needed */
        assert(false);
    }
    Ake_token_destroy(ext);
    free(ext);

    Ake_Ast* proto = NULL;
    bool has_id;
    proto = Ake_parse_prototype(ps, false, true, false, false, &has_id);
    if (proto) {
        Ake_AstAdd(n, proto);
        Ake_Type* type = Ake_proto2type_use(ps, proto, NULL);
        n->type = type;
    }

    if (!has_id) {
        Zinc_error_list_set(ps->el, &proto->loc, "expected an identifier after extern");
        n->has_error = true;
    }

    if (!n->has_error) {
        if (!ps->st->top->is_global) {
            Zinc_error_list_set(ps->el, &n->loc, "extern only allowed at top level scope");
            n->has_error = true;
        }

        Ake_Ast *id_node = Ake_AstGet(proto, 0);
        struct Ake_Symbol *new_sym = NULL;
        Zinc_malloc_safe((void **) &new_sym, sizeof(struct Ake_Symbol));
        Ake_SymbolInit(new_sym);
        new_sym->kind = AKE_SYMBOL_VARIABLE;
        Ake_Type* type = Ake_TypeClone(n->type);
        new_sym->tu = type;
        Ake_EnvironmentAdd(ps->st->top, &id_node->data.id.value, new_sym, n->loc.start);
    }

    if (!n->has_error) {
        Ake_Ast* id_node = Ake_AstGet(proto, 0);
        Zinc_string_list_add_bf(ps->extern_list, &id_node->data.id.value);
    }

    return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_while(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_AstCreate(&n);
    n->kind = Ake_ast_type_while;

	struct Ake_token* whl = NULL;
	if (!Ake_match(ps, Ake_token_while, "expecting while", &whl, n)) {
        n->has_error = true;
    }

	Ake_Ast* a = NULL;
    a = Ake_parse_expr(ps);

    if (a) {
        Ake_AstAdd(n, a);
    } else {
        struct Zinc_location a_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &a_loc, "expected expression after while");
        n->has_error = true;
		/* test case: test_parse_while_error_expected_expression */
	}

	Ake_Ast* b = NULL;
    b = Ake_parse_stmts(ps, false, false);

    if (b) {
        Ake_AstAdd(n, b);
    }

	struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        n->has_error = true;
        /* test case: test_parse_while_error_expected_end */
    }

	Ake_token_destroy(whl);
	free(whl);
	Ake_token_destroy(end);
	free(end);

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_for(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_AstCreate(&n);

	struct Ake_token* f = NULL;
	if (!Ake_match(ps, Ake_token_for, "expected for", &f, n)) {
        /* test case: test cases not needed */
        n->has_error = true;
    }

    Ake_begin_environment(ps->st, n);

    Ake_consume_newline(ps, n);

	Ake_Ast* dec = NULL;
    dec = Ake_parse_declaration(ps, true, false, true, true);

    if (dec) {
        Ake_AstAdd(n, dec);
    }

    Ake_consume_newline(ps, n);

	struct Ake_token* t0 = Ake_get_lookahead(ps);

	if (t0 && t0->type == Ake_token_equal) {
        if (n->kind == AKE_AST_NONE) {
            n->kind = Ake_ast_type_for_range;
        }
		Ake_parse_for_range(ps, n);

	} else if (t0 && t0->type == Ake_token_in) {
        if (n->kind == AKE_AST_NONE) {
            n->kind = Ake_ast_type_for_iteration;
        }
		Ake_parse_for_iteration(ps, n);

	} else {
		struct Zinc_location loc_error = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &loc_error, "expected '=' or 'in' after for element declaration");
        n->has_error = true;
		/* test case: test_parse_for_error_after_declaration */
	}

	Ake_Ast* c = NULL;
    c = Ake_parse_stmts(ps, true, false);

	struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        /* test case: test_parse_for_error_expected_end */
        n->has_error = true;
    }

    if (c) {
        Ake_AstAdd(n, c);
    }

	Ake_token_destroy(f);
	free(f);
	Ake_token_destroy(end);
	free(end);

    Ake_end_environment(ps->st);

	return n;
}

/* for_range -> for id = expr:expr stmts end */
void Ake_parse_for_range(struct Ake_parse_state* ps, Ake_Ast* parent)
{
	struct Ake_token* equal = NULL;
	if (!Ake_match(ps, Ake_token_equal, "expected equal", &equal, parent)) {
        /* test case: no test case needed */
        parent->has_error = true;
    }

    Ake_consume_newline(ps, parent);

	/* start expr */
	Ake_Ast* a = NULL;
    a = Ake_parse_expr(ps);

	if (!a) {
        struct Zinc_location a_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &a_loc, "expected range start");
        parent->has_error = true;
		/* test case: test_parse_for_error_expected_range_start */
	}

    Ake_consume_newline(ps, parent);

	struct Ake_token* colon = NULL;
	if (!Ake_match(ps, Ake_token_colon, "expected colon", &colon, parent)) {
        /* test case: test_parse_for_error_expected_colon */
        parent->has_error = true;
    }

    Ake_consume_newline(ps, parent);

	/* end expr */
	Ake_Ast* b = NULL;
    b = Ake_parse_expr(ps);

	if (!b) {
        struct Zinc_location b_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &b_loc, "expected range end");
        parent->has_error = true;
		/* test case: test_parse_for_error_expected_range_end */
	}

	if (a) {
        Ake_AstAdd(parent, a);
    }
    if (b) {
        Ake_AstAdd(parent, b);
	}

	Ake_token_destroy(equal);
	free(equal);
	Ake_token_destroy(colon);
	free(colon);

	if (!parent->has_error) {
		assert(a);
		if (!a->type) {
			Zinc_error_list_set(ps->el, &a->loc, "start range expression has no value");
            parent->has_error = true;
			/* test case: test_parse_for_range_error_start_no_value */
		} else {
			assert(a->type);
			if (!Ake_is_numeric(a->type)) {
				Zinc_error_list_set(ps->el, &a->loc, "start range expression is not numeric");
                parent->has_error = true;
				/* test case: test_parse_for_range_error_start_not_numeric */
			}
		}

		assert(b);
		if (!b->type) {
			Zinc_error_list_set(ps->el, &b->loc, "end range expression has no value");
            parent->has_error = true;
			/* test case: test_parse_for_range_error_end_no_value */
		} else {
			if (!Ake_is_numeric(b->type)) {
				Zinc_error_list_set(ps->el, &b->loc, "end range expression is not numeric");
                parent->has_error = true;
				/* test case: test_parse_for_range_error_end_not_numeric */
			}
		}
	}
}

/* for_iteration -> for id in expr stmts end */
void Ake_parse_for_iteration(struct Ake_parse_state* ps, Ake_Ast* parent)
{
	struct Ake_token* in = NULL;
	if (!Ake_match(ps, Ake_token_in, "expecting in", &in, parent)) {
        /* test case: no test case necessary */
        parent->has_error = true;
    }

    Ake_consume_newline(ps, parent);

	/* expr */
	Ake_Ast* list = NULL;
    list = Ake_parse_expr(ps);

	if (!list) {
        struct Zinc_location list_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &list_loc, "expected for iteration expression");
        parent->has_error = true;
        /* test case: test_parse_for_error_expected_iteration_expression */
	}

    if (list) {
        Ake_AstAdd(parent, list);
    }

	if (!parent->has_error) {
		Ake_Ast* element = Ake_AstGet(parent, 0);
		Ake_Ast* element_type_node = Ake_AstGet(element, 1);

		Ake_Type* list_type = list->type;

		if (!list_type) {
			Zinc_error_list_set(ps->el, &list->loc, "iteration expression has no value");
            parent->has_error = true;
			/* test case: test_parse_for_iteration_error_no_value */
		} else if (
			list_type->kind != AKE_TYPE_ARRAY
			&& list_type->kind != AKE_TYPE_SLICE) {
			Zinc_error_list_set(ps->el, &list->loc, "iteration expression is not an array or slice");
            parent->has_error = true;
			/* test case: test_parse_for_iteration_error_no_child_element */
		} else {
            Ake_Type* element_type2 = NULL;
			if (list_type->kind == AKE_TYPE_ARRAY) {
				element_type2 = Ake_TypeClone(list_type->data.array.type);
			} else if (list_type->kind == AKE_TYPE_SLICE) {
				element_type2 = Ake_TypeClone(list_type->data.slice.type);
			} else {
				assert(false && "expected array or slice");
			}
			if (!Ake_TypeMatch(element_type2, element_type_node->type, NULL)) {
                parent->has_error = true;
				Zinc_error_list_set(ps->el, &list->loc, "cannot cast list element");
				/* test case: test_parse_for_iteration_error_cannot_cast */
			}
            Ake_TypeDestroy(element_type2);
			free(element_type2);
		}
	}

	Ake_token_destroy(in);
	free(in);
}

/* parse_struct -> struct id struct_stmts end */
/* struct_stmts -> declaration struct_stmts | e */
/* struct_stmts' -> separator declaration | e */
Ake_Ast* Ake_parse_struct(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_AstCreate(&n);
    n->kind = Ake_ast_type_struct;

    Ake_token* st = NULL;
	if (!Ake_match(ps, Ake_token_struct, "expected struct", &st, n)) {
        /* test case: no test case needed */
        assert(false);
    }

	struct Ake_token* id = NULL;
	if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
        /* test case: test_parse_struct_error_expected_identifier */
        n->has_error = true;
    }
    if (id) {
        Zinc_string_copy(&id->value, &n->struct_value);
    }

	Ake_Ast* a = NULL;
    a = Ake_parse_declaration(ps, false, false, true, false);

	if (a) {
        Ake_AstAdd(n, a);
	}

	while (true) {
		bool has_separator;
		Ake_parse_separator(ps, n, &has_separator);

		if (!has_separator) {
			break;
		}

		Ake_Ast* b = NULL;
        b = Ake_parse_declaration(ps, false, false, true, false);

		if (b) {
            Ake_AstAdd(n, b);
		}
	}

	struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        /* test case: test_parse_struct_error_expected_end */
        n->has_error = true;
    }

	if (!n->has_error) {
		size_t seq = Ake_get_current_seq(ps);
		Ake_symbol* search = Ake_EnvironmentGetLocal(ps->st->top, &id->value, seq);
		if (search) {
			Zinc_string_finish(&id->value);
			Zinc_error_list_set(ps->el, &id->loc, "duplicate variable in scope: %s", id->value.buf);
            /* test case: test_parse_struct_error_duplicate */
            n->has_error = true;
		} else {
			Ake_Type* type = Ake_StructToType(n);
			Ake_symbol* sym = NULL;
			Ake_SymbolCreate(&sym);
			sym->kind = AKE_SYMBOL_TYPE;
			sym->td = type;
			Ake_EnvironmentAdd(ps->st->top, &id->value, sym, n->loc.start);
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
Ake_Ast* Ake_parse_return(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_AstCreate(&n);
    n->kind = Ake_ast_type_return;

	struct Ake_token* ret = NULL;
	if (!Ake_match(ps, Ake_token_return, "expected return", &ret, n)) {
        assert(false);
        /* test case: no test case needed */
    }

	Ake_Ast* a = NULL;
    a = Ake_parse_expr(ps);

	if (a) {
        Ake_AstAdd(n, a);
	}

	if (!n->has_error) {
		if (a) {
			if (!a->type) {
				Zinc_error_list_set(ps->el, &a->loc, "return expression has no value");
				/* test case: test_parse_return_error_no_value */
                n->has_error = true;
			} else {
				n->type = Ake_TypeClone(a->type);
				Ake_Ast* fd = Ake_get_current_function(ps->st);
				if (!fd) {
					Zinc_error_list_set(ps->el, &ret->loc, "return statement outside of function");
					/* test case: test_parse_return_error_outside_of_function */
                    n->has_error = true;
				} else {
                    Ake_Ast* proto = Ake_AstGet(fd, 0);
                    if (!Ake_check_return_type(ps, proto, n, &ret->loc)) {
                        /* test case: test_parse_return_error_type_does_not_match */
                        n->has_error = true;
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
Ake_Ast* Ake_parse_let(struct Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;
    Ake_AstCreate(&n);

	Ake_get_lookahead(ps);
	if (ps->lookahead->type == Ake_token_const) {
		Ake_token* t = NULL;
		if (!Ake_match(ps, Ake_token_const, "expected const", &t, n)) {
			assert(false && "expected const");
		}
		Ake_token_destroy(t);
		free(t);
		n->kind = Ake_ast_type_const;
	} else if (ps->lookahead->type == Ake_token_var) {
		Ake_token* t = NULL;
		if (!Ake_match(ps, Ake_token_var, "expected var", &t, n)) {
			assert(false && "expected var");;
		}
		Ake_token_destroy(t);
		free(t);
		n->kind = Ake_ast_type_var;
	} else {
		assert(false && "expected const or var");
	}

    Ake_consume_newline(ps, n);

	Ake_token* id = NULL;
	Ake_match(ps, Ake_token_id, "expected id", &id, n);

	Ake_Ast* id_node = NULL;
	Ake_AstCreate(&id_node);
	Ake_AstSet(id_node, AKE_AST_ID);
	if (id) {
		Zinc_string_add_string(&id_node->data.id.value, &id->value);
		Ake_token_destroy(id);
		free(id);
	}
    Ake_AstAdd(n, id_node);

    Ake_consume_newline(ps, n);

    struct Ake_token* dc = NULL;
    if (!Ake_match(ps, Ake_token_colon, "expected colon after variable(s)", &dc, n)) {
        n->has_error = true;
    }
    Ake_token_destroy(dc);
    free(dc);

    Ake_consume_newline(ps, n);

    Ake_Ast* type_node = NULL;
    type_node = Ake_parse_type(ps);
	if (type_node && !type_node->type) {
		Zinc_location loc = ps->lookahead->loc;
		Zinc_error_list_set(ps->el, &loc, "expected type identifier or fn");
		n->has_error = true;
	}
    Ake_declare_type(ps, type_node, id_node, n->kind == Ake_ast_type_const);
    if (type_node) {
        Ake_AstAdd(n, type_node);
    } else {
        struct Zinc_location type_use_loc = Ake_get_location(ps);
        Zinc_error_list_set(ps->el, &type_use_loc, "expected type");
        n->has_error = true;
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

        Ake_Ast* b = NULL;
        b = Ake_parse_expr(ps);

        if (b) {
            Ake_AstAdd(n, b);
        } else {
            struct Zinc_location b_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &b_loc, "expected expression");
            n->has_error = true;
        }

    	if (b && b->type == NULL) {
    		Zinc_error_list_set(ps->el, &b->loc, "cannot assign with operand that has no value");
    		n->has_error = true;
    	}

        if (!n->has_error) {
            bool cast = false;
            if (!Ake_TypeMatch(type_node->type, b->type, &cast)) {
                Zinc_error_list_set(ps->el, &b->loc, "values in assignment are not compatible");
                n->has_error = true;
            }
        }

        if (!n->has_error) {
            Ake_Override_rhs(type_node->type, b);
        }
    }

    return n;
}

Ake_Ast* Ake_parse_impl(struct Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;
    Ake_AstCreate(&n);
    n->kind = Ake_ast_type_impl;

    struct Ake_token* imp = NULL;
    if (!Ake_match(ps, Ake_token_impl, "expected impl", &imp, n)) {
        /* test case: no test case needed */
        assert(false);
    }
    Ake_token_destroy(imp);
    free(imp);

    struct Ake_token* id = NULL;
    if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
        n->has_error = true;
    }

    Ake_consume_newline(ps, n);

    Ake_Type* struct_type = NULL;
    if (id) {
    	size_t seq = Ake_get_current_seq(ps);
        Ake_symbol* sym = Ake_EnvironmentGet(ps->st->top, &id->value, seq);
        if (sym->kind == AKE_SYMBOL_TYPE) {
            if (sym->td->kind == AKE_TYPE_STRUCT) {
                struct_type = sym->td;
            }
        }
    }

    Ake_token_destroy(id);
    free(id);

    while (true) {
        struct Ake_token* t = Ake_get_lookahead(ps);
        if (t->type == Ake_token_fn) {
            Ake_Ast* func = Ake_parse_function(ps, true, struct_type);
            Ake_AstAdd(n, func);

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
                    n->has_error = true;
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
            n->has_error = true;
            break;
        }
    }

    return n;
}