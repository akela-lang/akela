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

struct ast_node* parse_not(struct parse_state* ps, struct location* loc);
struct ast_node* parse_literal(struct parse_state* ps, struct location* loc);
struct ast_node* parse_id(struct parse_state* ps, struct location* loc);
struct ast_node* parse_sign(struct parse_state* ps, struct location* loc);
struct ast_node* parse_array_literal(struct parse_state* ps, struct location* loc);
void parse_aseq(struct parse_state* ps, struct ast_node* parent, struct location* loc);
struct ast_node* parse_parenthesis(struct parse_state* ps, struct location* loc);
struct ast_node* parse_function(struct parse_state* ps, struct location* loc);
struct ast_node* parse_if(struct parse_state* ps, struct location* loc);
void parse_elseif(struct parse_state* ps, struct ast_node* parent, struct location* loc);
struct ast_node* parse_else(struct parse_state* ps, struct location* loc);

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! parse_factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
struct ast_node* parse_factor(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;

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

struct ast_node* parse_function(struct parse_state* ps, struct location* loc)
{
    struct ast_node* n = NULL;

    get_location(ps, loc);

    struct token* f = NULL;
    match(ps, token_fn, "expected fn", &f);
    consume_newline(ps);
    token_destroy(f);
    free(f);

    ast_node_create(&n);

    struct ast_node* proto = NULL;
    struct location proto_loc;
    bool has_id;
    proto = parse_prototype(ps, &has_id, &proto_loc);
    ast_node_add(n, proto);
    if (has_id) {
        n->type = ast_type_function;
    } else {
        n->type = ast_type_anonymous_function;
    }
    if (proto->type == ast_type_error) {
        n->type = ast_type_error;
    }

    environment_begin(ps->st);
    declare_params(ps, proto);
    set_current_function(ps->st->top, n);
    struct ast_node* tu = ast_node_get(proto, 3);
    n->tu = ast_node_copy(tu);

    struct ast_node* stmts_node = NULL;
	struct location loc_stmts;
    stmts_node = parse_stmts(ps, true, &loc_stmts);
	if (stmts_node && stmts_node->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (stmts_node) {
        ast_node_add(n, stmts_node);
    }

    environment_end(ps->st);

    struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_anonymous_function_expected_end */
        n->type = ast_type_error;
    }

	token_destroy(end);
	free(end);

    struct ast_node* id_node = ast_node_get(proto, 0);
    if (n->type != ast_type_error) {
        /* check and save symbol */
        struct symbol* search = environment_get_local(ps->st->top, &id_node->value);
        if (search) {
            buffer_finish(&id_node->value);
            error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", id_node->value.buf);
            n->type = ast_type_error;
            /* test case: test_parse_function_error_duplicate_declaration */
        } else {
            struct symbol* sym = environment_get(ps->st->top, &id_node->value);
            if (sym && sym->td) {
                buffer_finish(&id_node->value);
                error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", id_node->value.buf);
                n->type = ast_type_error;
                /* test case: test_parse_function_error_identifier_reserved */
            } else {
                struct symbol* new_sym = NULL;
                malloc_safe((void**)&new_sym, sizeof(struct symbol));
                symbol_init(new_sym);
                new_sym->tk_type = token_id;
                new_sym->tu = ast_node_copy(tu);
                environment_put(ps->st->top, &id_node->value, new_sym);
                n->sym = new_sym;
            }
        }

        /* check return type */
        struct ast_node* ret = ast_node_get(proto, 2);
        if (!check_return_type(ps, n, stmts_node, &ret->loc)) {
            n->type = ast_type_error;
        }
    }

	return n;
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
        if (b) {
            /* only return a value if else exists */
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

struct ast_node* parse_not(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_not;

    if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

    struct token* not = NULL;
	if (!match(ps, token_not, "expecting not", &not)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

	struct ast_node* a = NULL;
	struct location loc_factor;
	a = parse_expr(ps, &loc_factor);
    if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (!a) {
		error_list_set(ps->el, &loc_factor, "expected parse_factor after !");
		/* test case: test_parse_not_error_expected_factor */
        n->type = ast_type_error;
	}

	if (n->type != ast_type_error) {
		if (a) {
			ast_node_add(n, a);
		}
	}

	if (n->type != ast_type_error) {
		assert(a);
		struct ast_node* tu = a->tu;
		if (!tu) {
			error_list_set(ps->el, &not->loc, "! operator used on parse_factor with no value");
			/* test case: test_parse_not_error_no_value */
            n->type = ast_type_error;
		} else {
			assert(tu->td);
			if (tu->td->type != type_boolean) {
				error_list_set(ps->el, &not->loc, "not operator used on non-boolean");
				/* test case: test_parse_not_error_not_boolean */
                n->type = ast_type_error;
			} else {
				n->tu = ast_node_copy(tu);
			}
		}
	}

	token_destroy(not);
	free(not);

	return n;
}

struct ast_node* parse_literal(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
	char* type_name = NULL;
    ast_node_create(&n);

    if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

	struct token* t0 = get_lookahead(ps);

	struct token* x = NULL;
	if (!match(ps, t0->type, "expecting number, bool, or string", &x)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

    bool is_string = false;
	if (n->type != ast_type_error) {
		#pragma warning(suppress:6011)
		if (x->type == token_number) {
			n->type = ast_type_number;
			if (x->is_integer) {
				type_name = "Int64";
			} else if (x->is_float) {
				type_name = "Float64";
			}
		} else if (x->type == token_string) {
			n->type = ast_type_string;
			type_name = "UInt8";
            is_string = true;
		} else if (x->type == token_boolean) {
			n->type = ast_type_boolean;
			type_name = "Bool";
		} else {
            assert(false);
        }
		buffer_copy(&x->value, &n->value);
	}

	if (n->type != ast_type_error) {
		assert(type_name);
        struct buffer bf;
        buffer_init(&bf);
        buffer_copy_str(&bf, type_name);
        struct symbol* sym = environment_get(ps->st->top, &bf);
        assert(sym);
        assert(sym->td);
        struct ast_node* tu = NULL;
        ast_node_create(&tu);
        tu->type = ast_type_type;
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

	/* destroy x x{} */
	token_destroy(x);
	free(x);

	return n;
}

struct ast_node* parse_id(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_id;

    if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

    struct token* id = NULL;
    if (!match(ps, token_id, "expecting identifier", &id)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

	if (n->type != ast_type_error) {
		buffer_copy(&id->value, &n->value);
	}

    if (n->type != ast_type_error) {
		struct buffer full_id;
		buffer_init(&full_id);
		bool is_struct = false;
		struct symbol* sym = NULL;
		if (ps->qualifier.size > 0) {
			sym = environment_get(ps->st->top, &ps->qualifier);
			if (sym && sym->tu && sym->tu->td->type == type_struct) {
				buffer_copy(&ps->qualifier, &full_id);
				is_struct = true;
			} else {
				buffer_copy(&ps->qualifier, &full_id);
				buffer_add_char(&full_id, '.');
				buffer_copy(&id->value, &full_id);
			}
		} else {
			buffer_copy(&id->value, &full_id);
		}
		if (is_struct) {
			struct ast_node* tu = sym->tu->td->composite;
			assert(tu);
			bool found_id = false;
			struct ast_node* found_tu = NULL;
			struct ast_node* dec = tu->head;
			while (dec) {
				struct ast_node* field_id = ast_node_get(dec, 0);
				if (field_id) {
					if (buffer_compare(&id->value, &field_id->value)) {
						found_id = true;
						found_tu = ast_node_get(dec, 1);
					}
				}
				dec = dec->next;
			}

			if (found_id) {
				n->tu = ast_node_copy(found_tu);
			} else {
				buffer_finish(&id->value);
				error_list_set(ps->el, &id->loc, "variable not a field of struct: %s", id->value.buf);
                n->type = ast_type_error;
			}
		} else {
			struct symbol* sym2 = environment_get(ps->st->top, &full_id);
			if (!sym2) {
                buffer_finish(&full_id);
                error_list_set(ps->el, &id->loc, "variable not declared: %s", full_id.buf);
                /* test case: test_parse_types_missing_declaration */
                n->type = ast_type_error;
            } else if (sym2->td && sym2->td->type != type_struct) {
                error_list_set(ps->el, &id->loc, "using type as an identifier: %s", full_id.buf);
                n->type = ast_type_error;
			} else {
                struct symbol* sym3 = sym2;
				if (!sym3->tu) {
					sym3 = sym3->constructor;
				}
                assert(sym3);
				assert(sym3->tu);
				n->tu = ast_node_copy(sym3->tu);
                n->sym = sym3;
			}
		}

        buffer_destroy(&full_id);
	}

	/* destroy id id{} */
	token_destroy(id);
	free(id);

	return n;
}

struct ast_node* parse_sign(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;

    ast_node_create(&n);
    n->type = ast_type_sign;

    if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

	struct token* t0 = get_lookahead(ps);

	/* allocate sign */
	struct token* sign = NULL;
	if (!match(ps, t0->type, "expecting unary plus or minus", &sign)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

	struct ast_node* right = NULL;
	struct location loc_factor;
    right = parse_expr(ps, &loc_factor);
	if (right && right->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (!right) {
		error_list_set(ps->el, &loc_factor, "expected parse_factor after sign");
        n->type = ast_type_error;
	}

	if (n->type != ast_type_error) {
		struct ast_node* left;
		ast_node_create(&left);

		if (t0->type == token_plus) {
			left->type = ast_type_plus;
		} else {
			left->type = ast_type_minus;
		}

		ast_node_add(n, left);

		ast_node_add(n, right);

	}

	if (n->type != ast_type_error) {
		assert(right);
		struct ast_node* tu = right->tu;
		if (!tu) {
			error_list_set(ps->el, &sign->loc, "negative operator was used on expression with no value");
			/* test case: test_parse_sign_error */
            n->type = ast_type_error;
		} else {
			n->tu = ast_node_copy(tu);
		}
	}

	token_destroy(sign);
    free(sign);

	return n;
}

/*
* array_literal -> [aseq]
*/
struct ast_node* parse_array_literal(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
    get_location(ps, loc);

    ast_node_create(&n);
    n->type = ast_type_array_literal;

    struct token* lsb = NULL;
    if (!match(ps, token_left_square_bracket, "expected left square bracket", &lsb)) {
        n->type = ast_type_error;
        /* test case: no test case needed */
    }

    token_destroy(lsb);
    free(lsb);

    consume_newline(ps);

    struct location first_loc;
    parse_aseq(ps, n, &first_loc);

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

    struct token* rsb = NULL;
    if (!match(ps, token_right_square_bracket, "expected right square bracket", &rsb)) {
        /* test case: test_parse_array_literal_error_right_square_bracket */
        n->type = ast_type_error;
    }

    if (n->type != ast_type_error) {
        size_t count = 0;
        struct ast_node* first = n->head;

        if (!first) {
            error_list_set(ps->el, &rsb->loc, "array literal has no elements");
            /* test case: test_parse_array_literal_empty_error */
            n->type = ast_type_error;
        } else {
            struct ast_node* tu_first = ast_node_copy(first->tu);
            struct ast_node* x = first->next;
            struct ast_node* tu_x;
            count++;
            while (x) {
                tu_x = x->tu;
                if (!type_find_whole(ps->st, tu_first, tu_x)) {
                    error_list_set(ps->el, &first_loc, "array elements not the same type");
                    /* test case: test_parse_array_literal_mixed_error */
                    n->type = ast_type_error;
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
void parse_aseq(struct parse_state* ps, struct ast_node* parent, struct location *loc)
{
    get_location(ps, loc);

	struct ast_node* a = NULL;
	struct location loc_expr;
    a = parse_simple_expr(ps, &loc_expr);
	if (a && a->type == ast_type_error) {
        parent->type = ast_type_error;
    }

	if (a) {
		ast_node_add(parent, a);

		while (true) {
			struct token* t0 = get_lookahead(ps);
			if (!t0 || t0->type != token_comma) {
				break;
			}

			struct token* comma = NULL;
			if (!match(ps, token_comma, "expecting comma", &comma)) {
                /* test case: no test case needed */
                parent->type = ast_type_error;
            }

			token_destroy(comma);
			free(comma);

            if (!consume_newline(ps)) {
                parent->type = ast_type_error;
            }

			a = parse_simple_expr(ps, &loc_expr);
			if (a && a->type == ast_type_error) {
                parent->type = ast_type_error;
            }

			if (!a) {
				error_list_set(ps->el, &loc_expr, "expected expr after comma");
                parent->type = ast_type_error;
				/* test cases: test_parse_array_literal_error_expected_expr */
				break;
			}

			ast_node_add(parent, a);
		}
	}
}

struct ast_node* parse_parenthesis(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_parenthesis;

    if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	if (!match(ps, token_left_paren, "expecting left parenthesis", &lp)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

	/* allocate n n{} */
	struct ast_node* a = NULL;
	struct location loc_a;
    a = parse_expr(ps, &loc_a);
	if (a && a->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (!a) {
		error_list_set(ps->el, &loc_a, "empty parenthesis");
        n->type = ast_type_error;
		/* test case: test_parse_paren_error_empty */
	}

	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	if (!match(ps, token_right_paren, "expected right parenthesis", &rp)) {
        n->type = ast_type_error;
    }

	if (a) {
		ast_node_add(n, a);
	}

	if (n->type != ast_type_error) {
		assert(a);
		struct ast_node* tu = a->tu;
		if (!tu) {
			error_list_set(ps->el, &loc_a, "parenthesis on expression that has no value");
            n->type = ast_type_error;
		} else {
			n->tu = ast_node_copy(tu);
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
