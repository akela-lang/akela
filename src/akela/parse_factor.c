#include <assert.h>
#include "parse_tools.h"
#include "ast.h"
#include "token.h"
#include "parse_types.h"
#include "parse_stmts.h"
#include "parse_expr.h"
#include "lex.h"
#include "zinc/memory.h"
#include "symbol_table.h"
#include "parse_factor.h"
#include "type_def.h"
#include "symbol.h"

Ake_ast* parse_not(struct parse_state* ps);
Ake_ast* parse_literal(struct parse_state* ps);
Ake_ast* parse_id(struct parse_state* ps);
void parse_struct_literal_elements(
        struct parse_state* ps,
        struct Ake_ast* parent,
        struct type_def* td);
Ake_ast* parse_sign(struct parse_state* ps);
Ake_ast* parse_array_literal(struct parse_state* ps);
void parse_aseq(struct parse_state* ps, Ake_ast* parent);
Ake_ast* parse_parenthesis(struct parse_state* ps);
Ake_ast* parse_if(struct parse_state* ps);
void parse_elseif(struct parse_state* ps, Ake_ast* parent);
Ake_ast* parse_else(struct parse_state* ps);

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! parse_factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
Ake_ast* parse_factor(struct parse_state* ps)
{
	Ake_ast* n = NULL;

    struct token* t0;
	t0 = get_lookahead(ps);
    assert(t0);

	if (t0->type == token_fn) {
        n = parse_function(ps, false, NULL);

    } else if (t0->type == token_if) {
        n = parse_if(ps);

	} else if (t0->type == token_not) {
		n = parse_not(ps);

	} else if (t0->type == token_number || t0->type == token_string || t0->type == token_boolean) {
		n = parse_literal(ps);

	} else if (t0->type == token_id || t0->type == token_self) {
		n = parse_id(ps);

	} else if (t0->type == token_plus || t0->type == token_minus) {
		n = parse_sign(ps);

	} else if (t0->type == token_left_square_bracket) {
		n = parse_array_literal(ps);

	} else if (t0->type == token_left_paren) {
		n = parse_parenthesis(ps);

    }

	return n;
}

Ake_ast* parse_function(struct parse_state* ps, bool is_method, Ake_ast* struct_type)
{
    Ake_ast* n = NULL;

    Ake_ast_create(&n);
    n->type = Ake_ast_type_function;

    struct token* f = NULL;
    match(ps, token_fn, "expected fn", &f, n);
    consume_newline(ps, n);
    token_destroy(f);
    free(f);

    /* 0 prototype */
    Ake_ast* proto = NULL;
    bool has_id;
    proto = parse_prototype(ps, true, false, is_method, true, &has_id);
    Ake_ast_add(n, proto);
    if (proto->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    environment_begin(ps->st);
    declare_params(ps, proto, struct_type);
    set_current_function(ps->st->top, n);
    Type_use* tu = proto2type_use(ps->st, proto, struct_type);
    n->tu = tu;

    Ake_ast* stmts_node = NULL;
    stmts_node = parse_stmts(ps, true);
	if (stmts_node && stmts_node->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    /* 1 stmts */
    if (stmts_node) {
        Ake_ast_add(n, stmts_node);
    }

    environment_end(ps->st);

    struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end, n)) {
        /* test case: test_parse_anonymous_function_expected_end */
        n->type = Ake_ast_type_error;
    }

	token_destroy(end);
	free(end);

    if (n->type != Ake_ast_type_error) {
        /* check return type */
        Ake_ast* dret = Ast_node_get(proto, 2);
        if (!check_return_type(ps, proto, stmts_node, &dret->loc)) {
            n->type = Ake_ast_type_error;
        }
    }

    if (!is_method) {
        if (n->type != Ake_ast_type_error) {
            Ake_ast* id_node = Ast_node_get(proto, 0);
            /* check and save symbol */
            struct symbol* search = environment_get_local(ps->st->top, &id_node->value);
            if (search) {
                buffer_finish(&id_node->value);
                error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", id_node->value.buf);
                n->type = Ake_ast_type_error;
                /* test case: test_parse_function_error_duplicate_declaration */
            } else {
                struct symbol* sym = environment_get(ps->st->top, &id_node->value);
                if (sym && sym->td) {
                    buffer_finish(&id_node->value);
                    error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", id_node->value.buf);
                    n->type = Ake_ast_type_error;
                    /* test case: test_parse_function_error_identifier_reserved */
                } else {
                    struct symbol* new_sym = NULL;
                    malloc_safe((void**)&new_sym, sizeof(struct symbol));
                    symbol_init(new_sym);
                    new_sym->type = Symbol_type_variable;
                    new_sym->tu = Type_use_clone(tu);
                    environment_put(ps->st->top, &id_node->value, new_sym);
                    n->sym = new_sym;
                }
            }
        }
    }

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* parse_if(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_if;

    struct token* ift = NULL;
    if (!match(ps, token_if, "expecting if", &ift, n)) {
        /* test case: no test case necessary */
        n->type = Ake_ast_type_error;
    }

    token_destroy(ift);
    free(ift);

    Ake_ast* cb = NULL;
    Ake_ast_create(&cb);
    cb->type = Ake_ast_type_conditional_branch;

    Ake_ast_add(n, cb);

    Ake_ast* cond = NULL;
    cond = parse_expr(ps);
    if (cond && cond->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (cond == NULL) {
        struct location cond_loc = get_location(ps);
        error_list_set(ps->el, &cond_loc, "expected condition after if");
        /* test case: test_parse_if_error_expected_expression */
        n->type = Ake_ast_type_error;
        cb->type = Ake_ast_type_error;
    } else {
        Ake_ast_add(cb, cond);
    }

    Ake_ast* body = NULL;
    body = parse_stmts(ps, false);
    if (body && body->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
        cb->type = Ake_ast_type_error;
    }

    if (body) {
        cb->tu = Type_use_clone(body->tu);
        Ake_ast_add(cb, body);
    }

    parse_elseif(ps, n);

    Ake_ast* b = NULL;
    b = parse_else(ps);
    if (b && b->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (b) {
        Ake_ast_add(n, b);
    }

    struct token* end = NULL;
    if (!match(ps, token_end, "expected end", &end, n)) {
        /* test case: test_parse_if_error_expected_end */
        n->type = Ake_ast_type_error;
    }

    token_destroy(end);
    free(end);

    if (n->type != Ake_ast_type_error) {
        if (b) {
            /* only return a value if else exists */
            Ake_ast* p = n->head;
            Type_use* tu = NULL;
            if (p) {
                tu = Type_use_clone(p->tu);
                p = p->next;
            }
            while (p) {
                if (!type_find_whole(ps->st, tu, p->tu)) {
                    error_list_set(ps->el, &p->loc,
                                   "branch type does not match type of previous branch");
                    n->type = Ake_ast_type_error;
                    break;
                }
                p = p->next;
            }
            n->tu = tu;
        }
    }

    return n;
}

/* elseif-statement -> elseif expr stmts elseif | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
void parse_elseif(struct parse_state* ps, Ake_ast* parent)
{
    while (true) {
        struct token* t0 = get_lookahead(ps);
        if (t0->type != token_elseif) {
            break;
        }

        struct token *eit = NULL;
        if (!match(ps, token_elseif, "expecting elseif", &eit, parent)) {
            /* test case: no test case needed */
            assert(false);
        }

        token_destroy(eit);
        free(eit);

        Ake_ast *cb = NULL;
        Ake_ast_create(&cb);
        cb->type = Ake_ast_type_conditional_branch;

        Ake_ast *cond = NULL;
        cond = parse_expr(ps);
        if (cond && cond->type == Ake_ast_type_error) {
            cb->type = Ake_ast_type_error;
            parent->type = Ake_ast_type_error;
        }

        if (!cond) {
            struct location cond_loc = get_location(ps);
            error_list_set(ps->el, &cond_loc, "expected condition after elseif");
            /* test case: test_parse_if_error_expected_elseif_expression */
            cb->type = Ake_ast_type_error;
            parent->type = Ake_ast_type_error;
        } else {
            Ake_ast_add(cb, cond);
        }

        Ake_ast *body = NULL;
        body = parse_stmts(ps, false);
        if (body && body->type == Ake_ast_type_error) {
            cb->type = Ake_ast_type_error;
            parent->type = Ake_ast_type_error;
        }

        if (body) {
            Ake_ast_add(cb, body);
            cb->tu = Type_use_clone(body->tu);
        }

        Ake_ast_add(parent, cb);
    }
}

/* parse_else -> else stmts | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* parse_else(struct parse_state* ps)
{
    Ake_ast* n = NULL;

    struct token* t0 = get_lookahead(ps);
    if (t0 && t0->type == token_else) {
        Ake_ast_create(&n);
        n->type = Ake_ast_type_default_branch;

        struct token* et = NULL;
        if (!match(ps, token_else, "expected else", &et, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        token_destroy(et);
        free(et);

        /* stmts */
        Ake_ast* body = NULL;
        body = parse_stmts(ps, false);
        if (body && body->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (body) {
            n->tu = Type_use_clone(body->tu);
        }

        if (body) {
            Ake_ast_add(n, body);
        }
    }

    return n;
}

Ake_ast* parse_not(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_not;

    struct token* not = NULL;
	if (!match(ps, token_not, "expecting not", &not, n)) {
        /* test case: no test case needed */
        n->type = Ake_ast_type_error;
    }

    if (!consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

	Ake_ast* a = NULL;
	a = parse_expr(ps);
    if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (!a) {
        struct location a_loc = get_location(ps);
		error_list_set(ps->el, &a_loc, "expected parse_factor after !");
		/* test case: test_parse_not_error_expected_factor */
        n->type = Ake_ast_type_error;
	}

	if (n->type != Ake_ast_type_error) {
		if (a) {
            Ake_ast_add(n, a);
		}
	}

	if (n->type != Ake_ast_type_error) {
		assert(a);
		Type_use* tu = a->tu;
		if (!tu) {
			error_list_set(ps->el, &not->loc, "! operator used on parse_factor with no value");
			/* test case: test_parse_not_error_no_value */
            n->type = Ake_ast_type_error;
		} else {
			assert(tu->td);
			if (tu->td->type != type_boolean) {
				error_list_set(ps->el, &not->loc, "not operator used on non-boolean");
				/* test case: test_parse_not_error_not_boolean */
                n->type = Ake_ast_type_error;
			} else {
				n->tu = Type_use_clone(tu);
			}
		}
	}

	token_destroy(not);
	free(not);

	return n;
}

Ake_ast* parse_literal(struct parse_state* ps)
{
	Ake_ast* n = NULL;
	char* type_name = NULL;
    Ake_ast_create(&n);

	struct token* t0 = get_lookahead(ps);

	struct token* x = NULL;
	if (!match(ps, t0->type, "expecting number, bool, or string", &x, n)) {
        /* test case: no test case needed */
        n->type = Ake_ast_type_error;
    }

    bool is_string = false;
	if (n->type != Ake_ast_type_error) {
		#pragma warning(suppress:6011)
		if (x->type == token_number) {
			n->type = Ake_ast_type_number;
			if (x->is_integer) {
				type_name = "i64";
			} else if (x->is_float) {
				type_name = "f64";
			}
		} else if (x->type == token_string) {
			n->type = Ake_ast_type_string;
			type_name = "u8";
            is_string = true;
		} else if (x->type == token_boolean) {
			n->type = Ake_ast_type_boolean;
			type_name = "bool";
		} else {
            assert(false);
        }
		buffer_copy(&x->value, &n->value);
	}

	if (n->type != Ake_ast_type_error) {
		assert(type_name);
        struct buffer bf;
        buffer_init(&bf);
        buffer_copy_str(&bf, type_name);
        struct symbol* sym = environment_get(ps->st->top, &bf);
        assert(sym);
        assert(sym->td);
        Type_use* tu = NULL;
        Type_use_create(&tu);
        tu->td = sym->td;
        n->tu = tu;
        buffer_destroy(&bf);

        if (is_string) {
            tu->is_array = true;
            Type_dimension dim;
            dim.size = n->value.size + 1;
            dim.option = Array_element_const;
            VectorAdd(&tu->dim, &dim, 1);
        }
	}

	token_destroy(x);
	free(x);

	return n;
}

Ake_ast* parse_id(struct parse_state* ps)
{
    Ake_ast* n = NULL;
    Ake_ast_create(&n);

    struct token* t = get_lookahead(ps);

    struct token* id = NULL;

    if (t->type == token_id) {
        if (!match(ps, token_id, "expecting identifier", &id, n)) {
            /* test case: no test case needed */
            assert(false);
        }
    } else if (t->type == token_self) {
        if (!match(ps, token_self, "expecting self", &id, n)) {
            /* test case: no test case needed */
            assert(false);
        }
    }

    struct symbol* sym = environment_get(ps->st->top, &id->value);
    if (sym && sym->td && sym->td->type == type_struct) {
        /* struct literal */

        consume_newline(ps, n);

        n->type = Ake_ast_type_struct_literal;

        Type_use* tu = NULL;
        Type_use_create(&tu);
        tu->td = sym->td;
        n->tu = tu;
        n->sym = sym;

        parse_struct_literal_elements(ps, n, sym->td);

        struct token* end = NULL;
        if (!match(ps, token_end, "expected end", &end, n)) {
            n->type = Ake_ast_type_error;
        }
        token_destroy(end);
        free(end);

        token_destroy(id);
        free(id);
        return n;

    } else {
        /* id */
        n->type = Ake_ast_type_id;

        if (id) {
            buffer_copy(&id->value, &n->value);
        }

        if (!sym) {
            buffer_finish(&id->value);
            error_list_set(ps->el, &id->loc, "variable not declared: %s", id->value.buf);
            /* test case: test_parse_types_missing_declaration */
            n->type = Ake_ast_type_error;
        } else if (sym->td) {
            buffer_finish(&id->value);
            error_list_set(ps->el, &id->loc, "expected struct or variable but found type: %s", id->value.buf);
            /* test case: test_parse_types_missing_declaration */
            n->type = Ake_ast_type_error;
        } else {
            n->tu = Type_use_clone(sym->tu);
            n->sym = sym;
        }

        token_destroy(id);
        free(id);
        return n;
    }

}

typedef struct Struct_field_result {
    bool found;
    size_t index;
    Ake_ast* id;
    Type_use* tu;
} Struct_field_result;

Struct_field_result Get_struct_field(struct type_def* td, struct buffer* name) {
    assert(td->type == type_struct);
    assert(td->composite);
    size_t index = 0;
    Ake_ast* dec = td->composite->head;
    while (dec) {
        assert(dec->type == Ake_ast_type_declaration);
        Ake_ast* id = Ast_node_get(dec, 0);
        Ake_ast* type_node = Ast_node_get(dec, 1);
        if (buffer_compare(&id->value, name)) {
            Struct_field_result res = {true, index, id, type_node->tu};
            return res;
        }
        dec = dec->next;
        index++;
    }

    Struct_field_result res = {false, 0, NULL, NULL};
    return res;
}

void Find_missing_fields(struct parse_state* ps, struct type_def* td, Ake_ast* n) {
    assert(td->type == type_struct);
    assert(td->composite);
    Ake_ast *dec = td->composite->head;
    while (dec) {
        Ake_ast *id = Ast_node_get(dec, 0);
        bool found = false;
        Ake_ast *field = n->head;
        while (field) {
            Ake_ast *id2 = Ast_node_get(field, 0);
            if (buffer_compare(&id2->value, &id->value)) {
                found = true;
                break;
            }
            field = field->next;
        }
        if (!found) {
            error_list_set(ps->el, &id->loc, "struct field missing: %b", &id->value);
            n->type = Ake_ast_type_error;
        }
        dec = dec->next;
    }
}

void parse_struct_literal_elements(
        struct parse_state* ps,
        struct Ake_ast* parent,
        struct type_def* td)
{
    struct token* t0;

    while (true) {
        struct token* name = NULL;
        if (!match(ps, token_id, "expected a struct field identifier", &name, parent)) {
            parent->type = Ake_ast_type_error;
            break;
        }

        Ake_ast* field = NULL;
        Ake_ast_create(&field);
        field->type = Ake_ast_type_struct_literal_field;
        Ake_ast_add(parent, field);

        Struct_field_result sfr = Get_struct_field(td, &name->value);
        if (!sfr.found) {
            error_list_set(ps->el, &name->loc, "Not a valid field for %b: %b", &td->name, &name->value);
            parent->type = Ake_ast_type_error;
        }

        Ake_ast* id = NULL;
        Ake_ast_create(&id);
        id->type = Ake_ast_type_id;
        buffer_copy(&name->value, &id->value);
        Ake_ast_add(field, id);

        token_destroy(name);
        free(name);

        struct token* colon = NULL;
        if (!match(ps, token_colon, "expected a colon", &colon, parent)) {
            parent->type = Ake_ast_type_error;
            break;
        }
        token_destroy(colon);
        free(colon);

        struct Ake_ast* expr = parse_expr(ps);
        if (expr) {
            Ake_ast_add(field, expr);

            if (parent->type != Ake_ast_type_error) {
                if (!type_use_can_cast(sfr.tu, expr->tu)) {
                    error_list_set(ps->el, &expr->loc, "invalid type for field");
                    parent->type = Ake_ast_type_error;
                }

            }
        } else {
            struct location expr_loc = get_location(ps);
            error_list_set(ps->el, &expr_loc, "expected an expression");
            parent->type = Ake_ast_type_error;
        }

        t0 = get_lookahead(ps);
        if (t0->type == token_end) {
            break;
        }

        bool has_sep;
        parse_separator(ps, parent, &has_sep);

        t0 = get_lookahead(ps);
        if (t0->type == token_end) {
            break;
        }

        if (!has_sep) {
            struct location sep_loc = get_location(ps);
            error_list_set(ps->el, &sep_loc, "expected a separator");
            parent->type = Ake_ast_type_error;
        }
    }

    Find_missing_fields(ps, td, parent);
}

Ake_ast* parse_sign(struct parse_state* ps)
{
	Ake_ast* n = NULL;

    Ake_ast_create(&n);
    n->type = Ake_ast_type_sign;

	struct token* t0 = get_lookahead(ps);

	/* allocate sign */
	struct token* sign = NULL;
	if (!match(ps, t0->type, "expecting unary plus or minus", &sign, n)) {
        /* test case: no test case needed */
        n->type = Ake_ast_type_error;
    }

    if (!consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

	Ake_ast* right = NULL;
    right = parse_expr(ps);
	if (right && right->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (!right) {
        struct location right_loc = get_location(ps);
		error_list_set(ps->el, &right_loc, "expected parse_factor after sign");
        n->type = Ake_ast_type_error;
	}

	if (n->type != Ake_ast_type_error) {
		Ake_ast* left;
        Ake_ast_create(&left);

		if (t0->type == token_plus) {
			left->type = Ake_ast_type_plus;
		} else {
			left->type = Ake_ast_type_minus;
		}

        Ake_ast_add(n, left);

        Ake_ast_add(n, right);

	}

	if (n->type != Ake_ast_type_error) {
		assert(right);
		Type_use* tu = right->tu;
		if (!tu) {
			error_list_set(ps->el, &sign->loc, "negative operator was used on expression with no value");
			/* test case: test_parse_sign_error */
            n->type = Ake_ast_type_error;
		} else {
			n->tu = Type_use_clone(tu);
		}
	}

	token_destroy(sign);
    free(sign);

	return n;
}

/*
* array_literal -> [aseq]
*/
Ake_ast* parse_array_literal(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_array_literal;

    struct token* lsb = NULL;
    if (!match(ps, token_left_square_bracket, "expected left square bracket", &lsb, n)) {
        n->type = Ake_ast_type_error;
        /* test case: no test case needed */
    }

    token_destroy(lsb);
    free(lsb);

    consume_newline(ps, n);

    parse_aseq(ps, n);

    if (!consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

    struct token* rsb = NULL;
    if (!match(ps, token_right_square_bracket, "expected right square bracket", &rsb, n)) {
        /* test case: test_parse_array_literal_error_right_square_bracket */
        n->type = Ake_ast_type_error;
    }

    if (n->type != Ake_ast_type_error) {
        size_t count = 0;
        Ake_ast* first = n->head;

        if (!first) {
            error_list_set(ps->el, &rsb->loc, "array literal has no elements");
            /* test case: test_parse_array_literal_empty_error */
            n->type = Ake_ast_type_error;
        } else {
            Type_use* tu_first = Type_use_clone(first->tu);
            Ake_ast* x = first->next;
            Type_use* tu_x;
            count++;
            while (x) {
                tu_x = x->tu;
                if (!type_find_whole(ps->st, tu_first, tu_x)) {
                    error_list_set(ps->el, &first->loc, "array elements not the same type");
                    /* test case: test_parse_array_literal_mixed_error */
                    n->type = Ake_ast_type_error;
                    break;
                }
                count++;
                x = x->next;
            }
            n->tu = tu_first;
            n->tu->is_array = true;
            VectorAdd(&n->tu->dim, &count, 1);
        }
    }

    token_destroy(rsb);
    free(rsb);

	return n;
}

/* aseq -> expr aseq' | e */
/* aseq' = , expr aseq' | e */
void parse_aseq(struct parse_state* ps, Ake_ast* parent)
{
	Ake_ast* a = NULL;
    a = parse_simple_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        parent->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(parent, a);

		while (true) {
			struct token* t0 = get_lookahead(ps);
			if (!t0 || t0->type != token_comma) {
				break;
			}

			struct token* comma = NULL;
			if (!match(ps, token_comma, "expecting comma", &comma, parent)) {
                /* test case: no test case needed */
                parent->type = Ake_ast_type_error;
            }

			token_destroy(comma);
			free(comma);

            if (!consume_newline(ps, parent)) {
                parent->type = Ake_ast_type_error;
            }

			a = parse_simple_expr(ps);
			if (a && a->type == Ake_ast_type_error) {
                parent->type = Ake_ast_type_error;
            }

			if (!a) {
                struct location a_loc = get_location(ps);
				error_list_set(ps->el, &a_loc, "expected expr after comma");
                parent->type = Ake_ast_type_error;
				/* test cases: test_parse_array_literal_error_expected_expr */
				break;
			}

            Ake_ast_add(parent, a);
		}
	}
}

Ake_ast* parse_parenthesis(struct parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_parenthesis;

	struct token* lp = NULL;
	if (!match(ps, token_left_paren, "expecting left parenthesis", &lp, n)) {
        /* test case: no test case needed */
        n->type = Ake_ast_type_error;
    }

    if (!consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

	Ake_ast* a = NULL;
    a = parse_expr(ps);
	if (a && a->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (!a) {
        struct location a_loc = get_location(ps);
		error_list_set(ps->el, &a_loc, "empty parenthesis");
        n->type = Ake_ast_type_error;
		/* test case: test_parse_paren_error_empty */
	}

	struct token* rp = NULL;
	if (!match(ps, token_right_paren, "expected right parenthesis", &rp, n)) {
        n->type = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
	}

	if (n->type != Ake_ast_type_error) {
		assert(a);
		Type_use* tu = a->tu;
		if (!tu) {
			error_list_set(ps->el, &a->loc, "parenthesis on expression that has no value");
            n->type = Ake_ast_type_error;
		} else {
			n->tu = Type_use_clone(tu);
		}
	}

	/* destroy lp lp{} */
	token_destroy(lp);
	free(lp);

	/* destroy rp rp{} */
	token_destroy(rp);
	free(rp);

	return n;
}
