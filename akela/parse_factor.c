#include <assert.h>
#include "zinc/result.h"
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

Ast_node* parse_not(struct parse_state* ps, struct location* loc);
Ast_node* parse_literal(struct parse_state* ps, struct location* loc);
Ast_node* parse_id(struct parse_state* ps, struct location* loc);
void parse_struct_literal_elements(
        struct parse_state* ps,
        struct Ast_node* parent,
        struct type_def* td,
        struct location* loc);
Ast_node* parse_sign(struct parse_state* ps, struct location* loc);
Ast_node* parse_array_literal(struct parse_state* ps, struct location* loc);
void parse_aseq(struct parse_state* ps, Ast_node* parent, struct location* loc);
Ast_node* parse_parenthesis(struct parse_state* ps, struct location* loc);
Ast_node* parse_function(struct parse_state* ps, struct location* loc);
Ast_node* parse_if(struct parse_state* ps, struct location* loc);
void parse_elseif(struct parse_state* ps, Ast_node* parent, struct location* loc);
Ast_node* parse_else(struct parse_state* ps, struct location* loc);

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! parse_factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
Ast_node* parse_factor(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;

    struct token* t0;
	t0 = get_lookahead(ps);
    assert(t0);

	if (t0->type == token_fn) {
        n = parse_function(ps, loc);

    } else if (t0->type == token_if) {
        n = parse_if(ps, loc);

	} else if (t0->type == token_not) {
		n = parse_not(ps, loc);

	} else if (t0->type == token_number || t0->type == token_string || t0->type == token_boolean) {
		n = parse_literal(ps, loc);

	} else if (t0->type == token_id) {
		n = parse_id(ps, loc);

	} else if (t0->type == token_plus || t0->type == token_minus) {
		n = parse_sign(ps, loc);

	} else if (t0->type == token_left_square_bracket) {
		n = parse_array_literal(ps, loc);

	} else if (t0->type == token_left_paren) {
		n = parse_parenthesis(ps, loc);

    }

	return n;
}

Ast_node* parse_function(struct parse_state* ps, struct location* loc)
{
    Ast_node* n = NULL;

    get_location(ps, loc);

    struct token* f = NULL;
    match(ps, token_fn, "expected fn", &f);
    consume_newline(ps);
    token_destroy(f);
    free(f);

    Ast_node_create(&n);

    /* 0 prototype */
    Ast_node* proto = NULL;
    struct location proto_loc;
    bool has_id;
    proto = parse_prototype(ps, true, false, true, &has_id, &proto_loc);
    Ast_node_add(n, proto);
    n->type = Ast_type_function;
    if (proto->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

    environment_begin(ps->st);
    declare_params(ps, proto);
    set_current_function(ps->st->top, n);
    Ast_node* tu = proto2type(ps->st, proto);
    n->tu = tu;

    Ast_node* stmts_node = NULL;
	struct location loc_stmts;
    stmts_node = parse_stmts(ps, true, &loc_stmts);
	if (stmts_node && stmts_node->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

    /* 1 stmts */
    if (stmts_node) {
        Ast_node_add(n, stmts_node);
    }

    environment_end(ps->st);

    struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_anonymous_function_expected_end */
        n->type = Ast_type_error;
    }

	token_destroy(end);
	free(end);

    Ast_node* id_node = Ast_node_get(proto, 0);
    if (n->type != Ast_type_error) {
        /* check and save symbol */
        struct symbol* search = environment_get_local(ps->st->top, &id_node->value);
        if (search) {
            buffer_finish(&id_node->value);
            error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", id_node->value.buf);
            n->type = Ast_type_error;
            /* test case: test_parse_function_error_duplicate_declaration */
        } else {
            struct symbol* sym = environment_get(ps->st->top, &id_node->value);
            if (sym && sym->td) {
                buffer_finish(&id_node->value);
                error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", id_node->value.buf);
                n->type = Ast_type_error;
                /* test case: test_parse_function_error_identifier_reserved */
            } else {
                struct symbol* new_sym = NULL;
                malloc_safe((void**)&new_sym, sizeof(struct symbol));
                symbol_init(new_sym);
                new_sym->tk_type = token_id;
                new_sym->tu = Ast_node_copy(tu);
                environment_put(ps->st->top, &id_node->value, new_sym);
                n->sym = new_sym;
            }
        }

        /* check return type */
        Ast_node* dret = Ast_node_get(proto, 2);
        if (!check_return_type(ps, proto, stmts_node, &dret->loc)) {
            n->type = Ast_type_error;
        }
    }

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ast_node* parse_if(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

    Ast_node* n = NULL;
    Ast_node_create(&n);
    n->type = Ast_type_if;

    struct token* ift = NULL;
    if (!match(ps, token_if, "expecting if", &ift)) {
        /* test case: no test case necessary */
        n->type = Ast_type_error;
    }

    token_destroy(ift);
    free(ift);

    Ast_node* cb = NULL;
    Ast_node_create(&cb);
    cb->type = Ast_type_conditional_branch;

    Ast_node_add(n, cb);

    Ast_node* cond = NULL;
    struct location loc_expr;
    cond = parse_expr(ps, &loc_expr);
    if (cond && cond->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

    if (cond == NULL) {
        error_list_set(ps->el, &loc_expr, "expected condition after if");
        /* test case: test_parse_if_error_expected_expression */
        n->type = Ast_type_error;
        cb->type = Ast_type_error;
    } else {
        Ast_node_add(cb, cond);
    }

    Ast_node* body = NULL;
    struct location loc_stmts;
    body = parse_stmts(ps, false, &loc_stmts);
    if (body && body->type == Ast_type_error) {
        n->type = Ast_type_error;
        cb->type = Ast_type_error;
    }

    if (body) {
        cb->tu = Ast_node_copy(body->tu);
        Ast_node_add(cb, body);
    }

    struct location loc_elseif;
    parse_elseif(ps, n, &loc_elseif);

    struct location b_loc;
    Ast_node* b = NULL;
    b = parse_else(ps, &b_loc);
    if (b && b->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

    if (b) {
        Ast_node_add(n, b);
    }

    struct token* end = NULL;
    if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_if_error_expected_end */
        n->type = Ast_type_error;
    }

    token_destroy(end);
    free(end);

    if (n->type != Ast_type_error) {
        if (b) {
            /* only return a value if else exists */
            Ast_node* p = n->head;
            Ast_node* tu = NULL;
            if (p) {
                tu = Ast_node_copy(p->tu);
                p = p->next;
            }
            while (p) {
                if (!type_find_whole(ps->st, tu, p->tu)) {
                    error_list_set(ps->el, &p->loc,
                                   "branch type does not match type of previous branch");
                    n->type = Ast_type_error;
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
void parse_elseif(struct parse_state* ps, Ast_node* parent, struct location* loc)
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

        Ast_node *cb = NULL;
        Ast_node_create(&cb);
        cb->type = Ast_type_conditional_branch;

        Ast_node *cond = NULL;
        struct location loc_cond;
        cond = parse_expr(ps, &loc_cond);
        if (cond && cond->type == Ast_type_error) {
            cb->type = Ast_type_error;
            parent->type = Ast_type_error;
        }

        if (!cond) {
            error_list_set(ps->el, &loc_cond, "expected condition after elseif");
            /* test case: test_parse_if_error_expected_elseif_expression */
            cb->type = Ast_type_error;
            parent->type = Ast_type_error;
        } else {
            Ast_node_add(cb, cond);
        }

        Ast_node *body = NULL;
        struct location loc_node;
        body = parse_stmts(ps, false, &loc_node);
        if (body && body->type == Ast_type_error) {
            cb->type = Ast_type_error;
            parent->type = Ast_type_error;
        }

        if (body) {
            Ast_node_add(cb, body);
            cb->tu = Ast_node_copy(body->tu);
        }

        Ast_node_add(parent, cb);

        struct location loc_elseif;
    }
}

/* parse_else -> else stmts | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ast_node* parse_else(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

    Ast_node* n = NULL;

    struct token* t0 = get_lookahead(ps);
    if (t0 && t0->type == token_else) {
        Ast_node_create(&n);
        n->type = Ast_type_default_branch;

        struct token* et = NULL;
        if (!match(ps, token_else, "expected else", &et)) {
            /* test case: no test case needed */
            assert(false);
        }

        token_destroy(et);
        free(et);

        /* stmts */
        Ast_node* body = NULL;
        struct location body_loc;
        body = parse_stmts(ps, false, &body_loc);
        if (body && body->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (body) {
            n->tu = Ast_node_copy(body->tu);
        }

        if (body) {
            Ast_node_add(n, body);
        }
    }

    return n;
}

Ast_node* parse_not(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
    Ast_node_create(&n);
    n->type = Ast_type_not;

    if (!get_location(ps, loc)) {
        n->type = Ast_type_error;
    }

    struct token* not = NULL;
	if (!match(ps, token_not, "expecting not", &not)) {
        /* test case: no test case needed */
        n->type = Ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = Ast_type_error;
    }

	Ast_node* a = NULL;
	struct location loc_factor;
	a = parse_expr(ps, &loc_factor);
    if (a && a->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

	if (!a) {
		error_list_set(ps->el, &loc_factor, "expected parse_factor after !");
		/* test case: test_parse_not_error_expected_factor */
        n->type = Ast_type_error;
	}

	if (n->type != Ast_type_error) {
		if (a) {
            Ast_node_add(n, a);
		}
	}

	if (n->type != Ast_type_error) {
		assert(a);
		Ast_node* tu = a->tu;
		if (!tu) {
			error_list_set(ps->el, &not->loc, "! operator used on parse_factor with no value");
			/* test case: test_parse_not_error_no_value */
            n->type = Ast_type_error;
		} else {
			assert(tu->td);
			if (tu->td->type != type_boolean) {
				error_list_set(ps->el, &not->loc, "not operator used on non-boolean");
				/* test case: test_parse_not_error_not_boolean */
                n->type = Ast_type_error;
			} else {
				n->tu = Ast_node_copy(tu);
			}
		}
	}

	token_destroy(not);
	free(not);

	return n;
}

Ast_node* parse_literal(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
	char* type_name = NULL;
    Ast_node_create(&n);

    if (!get_location(ps, loc)) {
        n->type = Ast_type_error;
    }

	struct token* t0 = get_lookahead(ps);

	struct token* x = NULL;
	if (!match(ps, t0->type, "expecting number, bool, or string", &x)) {
        /* test case: no test case needed */
        n->type = Ast_type_error;
    }

    bool is_string = false;
	if (n->type != Ast_type_error) {
		#pragma warning(suppress:6011)
		if (x->type == token_number) {
			n->type = Ast_type_number;
			if (x->is_integer) {
				type_name = "i64";
			} else if (x->is_float) {
				type_name = "f64";
			}
		} else if (x->type == token_string) {
			n->type = Ast_type_string;
			type_name = "u8";
            is_string = true;
		} else if (x->type == token_boolean) {
			n->type = Ast_type_boolean;
			type_name = "bool";
		} else {
            assert(false);
        }
		buffer_copy(&x->value, &n->value);
	}

	if (n->type != Ast_type_error) {
		assert(type_name);
        struct buffer bf;
        buffer_init(&bf);
        buffer_copy_str(&bf, type_name);
        struct symbol* sym = environment_get(ps->st->top, &bf);
        assert(sym);
        assert(sym->td);
        Ast_node* tu = NULL;
        Ast_node_create(&tu);
        tu->type = Ast_type_type;
        tu->td = sym->td;
        n->tu = tu;
        buffer_destroy(&bf);

        if (is_string) {
            tu->to.is_array = true;
            Type_dimension dim;
            buffer_finish(&n->value);
            dim.size = n->value.size + 1;
            dim.option = Array_element_const;
            VectorAdd(&tu->to.dim, &dim, 1);
        }
	}

	token_destroy(x);
	free(x);

	return n;
}

Ast_node* parse_id(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);


    struct token* id = NULL;
    if (!match(ps, token_id, "expecting identifier", &id)) {
        /* test case: no test case needed */
        assert(false);
    }

    struct symbol* sym = environment_get(ps->st->top, &id->value);
    if (sym && sym->td && sym->td->type == type_struct) {
        /* struct literal */

        consume_newline(ps);

        Ast_node* n = NULL;
        Ast_node_create(&n);
        n->type = Ast_type_struct_literal;

        Ast_node* tu = NULL;
        Ast_node_create(&tu);
        tu->type = Ast_type_type;
        tu->td = sym->td;
        n->tu = tu;
        n->sym = sym;

        struct location elements_loc;
        parse_struct_literal_elements(ps, n, sym->td, &elements_loc);

        struct token* end = NULL;
        if (!match(ps, token_end, "expected end", &end)) {
            n->type = Ast_type_error;
        }
        token_destroy(end);
        free(end);

        token_destroy(id);
        free(id);
        return n;

    } else {
        /* id */
        Ast_node* n = NULL;
        Ast_node_create(&n);
        n->type = Ast_type_id;

        if (id) {
            buffer_copy(&id->value, &n->value);
        }

        if (!sym) {
            buffer_finish(&id->value);
            error_list_set(ps->el, &id->loc, "variable not declared: %s", id->value.buf);
            /* test case: test_parse_types_missing_declaration */
            n->type = Ast_type_error;
        } else if (sym->td) {
            buffer_finish(&id->value);
            error_list_set(ps->el, &id->loc, "expected struct or variable but found type: %s", id->value.buf);
            /* test case: test_parse_types_missing_declaration */
            n->type = Ast_type_error;
        } else {
            n->tu = Ast_node_copy(sym->tu);
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
    Ast_node* id;
    Ast_node* tu;
} Struct_field_result;

Struct_field_result Get_struct_field(struct type_def* td, struct buffer* name) {
    assert(td->type == type_struct);
    assert(td->composite);
    size_t index = 0;
    Ast_node* dec = td->composite->head;
    while (dec) {
        assert(dec->type == Ast_type_declaration);
        Ast_node* id = Ast_node_get(dec, 0);
        Ast_node* tu = Ast_node_get(dec, 1);
        if (buffer_compare(&id->value, name)) {
            Struct_field_result res = {true, index, id, tu};
            return res;
        }
        dec = dec->next;
        index++;
    }

    Struct_field_result res = {false, 0, NULL, NULL};
    return res;
}

void Find_missing_fields(struct parse_state* ps, struct type_def* td, Ast_node* n) {
    assert(td->type == type_struct);
    assert(td->composite);
    size_t index = 0;
    Ast_node *dec = td->composite->head;
    while (dec) {
        Ast_node *id = Ast_node_get(dec, 0);
        bool found = false;
        Ast_node *field = n->head;
        while (field) {
            Ast_node *id2 = Ast_node_get(field, 0);
            if (buffer_compare(&id2->value, &id->value)) {
                found = true;
                break;
            }
            field = field->next;
        }
        if (!found) {
            error_list_set(ps->el, &id->loc, "struct field missing: %b", &id->value);
            n->type = Ast_type_error;
        }
        dec = dec->next;
    }
}

void parse_struct_literal_elements(
        struct parse_state* ps,
        struct Ast_node* parent,
        struct type_def* td,
        struct location* loc)
{
    get_location(ps, loc);

    struct token* t0;

    while (true) {
        struct token* name = NULL;
        if (!match(ps, token_id, "expected a struct field identifier", &name)) {
            parent->type = Ast_type_error;
            break;
        }

        Ast_node* field = NULL;
        Ast_node_create(&field);
        field->type = Ast_type_struct_literal_field;
        Ast_node_add(parent, field);

        Struct_field_result sfr = Get_struct_field(td, &name->value);
        if (!sfr.found) {
            error_list_set(ps->el, &name->loc, "Not a valid field for %b: %b", &td->name, &name->value);
            parent->type = Ast_type_error;
        }

        Ast_node* id = NULL;
        Ast_node_create(&id);
        id->type = Ast_type_id;
        buffer_copy(&name->value, &id->value);
        Ast_node_add(field, id);

        token_destroy(name);
        free(name);

        struct token* colon = NULL;
        if (!match(ps, token_colon, "expected a colon", &colon)) {
            parent->type = Ast_type_error;
            break;
        }
        token_destroy(colon);
        free(colon);

        struct location expr_loc;
        struct Ast_node* expr = parse_expr(ps, &expr_loc);
        if (expr) {
            Ast_node_add(field, expr);

            if (parent->type != Ast_type_error) {
                if (!type_use_can_cast(sfr.tu, expr->tu)) {
                    error_list_set(ps->el, &expr_loc, "invalid type for field");
                    parent->type = Ast_type_error;
                }

            }
        } else {
            error_list_set(ps->el, &expr_loc, "expected an expression");
            parent->type = Ast_type_error;
        }


        t0 = get_lookahead(ps);
        if (t0->type == token_end) {
            break;
        }

        bool has_sep;
        struct location sep_loc;
        parse_separator(ps, &has_sep, &sep_loc);

        t0 = get_lookahead(ps);
        if (t0->type == token_end) {
            break;
        }

        if (!has_sep) {
            error_list_set(ps->el, &sep_loc, "expected a separator");
            parent->type = Ast_type_error;
        }
    }

    Find_missing_fields(ps, td, parent);
}

Ast_node* parse_sign(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;

    Ast_node_create(&n);
    n->type = Ast_type_sign;

    if (!get_location(ps, loc)) {
        n->type = Ast_type_error;
    }

	struct token* t0 = get_lookahead(ps);

	/* allocate sign */
	struct token* sign = NULL;
	if (!match(ps, t0->type, "expecting unary plus or minus", &sign)) {
        /* test case: no test case needed */
        n->type = Ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = Ast_type_error;
    }

	Ast_node* right = NULL;
	struct location loc_factor;
    right = parse_expr(ps, &loc_factor);
	if (right && right->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

	if (!right) {
		error_list_set(ps->el, &loc_factor, "expected parse_factor after sign");
        n->type = Ast_type_error;
	}

	if (n->type != Ast_type_error) {
		Ast_node* left;
        Ast_node_create(&left);

		if (t0->type == token_plus) {
			left->type = Ast_type_plus;
		} else {
			left->type = Ast_type_minus;
		}

        Ast_node_add(n, left);

        Ast_node_add(n, right);

	}

	if (n->type != Ast_type_error) {
		assert(right);
		Ast_node* tu = right->tu;
		if (!tu) {
			error_list_set(ps->el, &sign->loc, "negative operator was used on expression with no value");
			/* test case: test_parse_sign_error */
            n->type = Ast_type_error;
		} else {
			n->tu = Ast_node_copy(tu);
		}
	}

	token_destroy(sign);
    free(sign);

	return n;
}

/*
* array_literal -> [aseq]
*/
Ast_node* parse_array_literal(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
    get_location(ps, loc);

    Ast_node_create(&n);
    n->type = Ast_type_array_literal;

    struct token* lsb = NULL;
    if (!match(ps, token_left_square_bracket, "expected left square bracket", &lsb)) {
        n->type = Ast_type_error;
        /* test case: no test case needed */
    }

    token_destroy(lsb);
    free(lsb);

    consume_newline(ps);

    struct location first_loc;
    parse_aseq(ps, n, &first_loc);

    if (!consume_newline(ps)) {
        n->type = Ast_type_error;
    }

    struct token* rsb = NULL;
    if (!match(ps, token_right_square_bracket, "expected right square bracket", &rsb)) {
        /* test case: test_parse_array_literal_error_right_square_bracket */
        n->type = Ast_type_error;
    }

    if (n->type != Ast_type_error) {
        size_t count = 0;
        Ast_node* first = n->head;

        if (!first) {
            error_list_set(ps->el, &rsb->loc, "array literal has no elements");
            /* test case: test_parse_array_literal_empty_error */
            n->type = Ast_type_error;
        } else {
            Ast_node* tu_first = Ast_node_copy(first->tu);
            Ast_node* x = first->next;
            Ast_node* tu_x;
            count++;
            while (x) {
                tu_x = x->tu;
                if (!type_find_whole(ps->st, tu_first, tu_x)) {
                    error_list_set(ps->el, &first_loc, "array elements not the same type");
                    /* test case: test_parse_array_literal_mixed_error */
                    n->type = Ast_type_error;
                    break;
                }
                count++;
                x = x->next;
            }
            n->tu = tu_first;
            n->tu->to.is_array = true;
            VectorAdd(&n->tu->to.dim, &count, 1);
        }
    }

    token_destroy(rsb);
    free(rsb);

	return n;
}

/* aseq -> expr aseq' | e */
/* aseq' = , expr aseq' | e */
void parse_aseq(struct parse_state* ps, Ast_node* parent, struct location *loc)
{
    get_location(ps, loc);

	Ast_node* a = NULL;
	struct location loc_expr;
    a = parse_simple_expr(ps, &loc_expr);
	if (a && a->type == Ast_type_error) {
        parent->type = Ast_type_error;
    }

	if (a) {
        Ast_node_add(parent, a);

		while (true) {
			struct token* t0 = get_lookahead(ps);
			if (!t0 || t0->type != token_comma) {
				break;
			}

			struct token* comma = NULL;
			if (!match(ps, token_comma, "expecting comma", &comma)) {
                /* test case: no test case needed */
                parent->type = Ast_type_error;
            }

			token_destroy(comma);
			free(comma);

            if (!consume_newline(ps)) {
                parent->type = Ast_type_error;
            }

			a = parse_simple_expr(ps, &loc_expr);
			if (a && a->type == Ast_type_error) {
                parent->type = Ast_type_error;
            }

			if (!a) {
				error_list_set(ps->el, &loc_expr, "expected expr after comma");
                parent->type = Ast_type_error;
				/* test cases: test_parse_array_literal_error_expected_expr */
				break;
			}

            Ast_node_add(parent, a);
		}
	}
}

Ast_node* parse_parenthesis(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;
    Ast_node_create(&n);
    n->type = Ast_type_parenthesis;

    if (!get_location(ps, loc)) {
        n->type = Ast_type_error;
    }

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	if (!match(ps, token_left_paren, "expecting left parenthesis", &lp)) {
        /* test case: no test case needed */
        n->type = Ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = Ast_type_error;
    }

	/* allocate n n{} */
	Ast_node* a = NULL;
	struct location loc_a;
    a = parse_expr(ps, &loc_a);
	if (a && a->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

	if (!a) {
		error_list_set(ps->el, &loc_a, "empty parenthesis");
        n->type = Ast_type_error;
		/* test case: test_parse_paren_error_empty */
	}

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	if (!match(ps, token_right_paren, "expected right parenthesis", &rp)) {
        n->type = Ast_type_error;
    }

	if (a) {
        Ast_node_add(n, a);
	}

	if (n->type != Ast_type_error) {
		assert(a);
		Ast_node* tu = a->tu;
		if (!tu) {
			error_list_set(ps->el, &loc_a, "parenthesis on expression that has no value");
            n->type = Ast_type_error;
		} else {
			n->tu = Ast_node_copy(tu);
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
