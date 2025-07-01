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
#include "type.h"
#include "symbol.h"

Ake_Ast* Ake_parse_not(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_literal(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_id(struct Ake_parse_state* ps);
void Ake_parse_struct_literal_elements(
        Ake_parse_state* ps,
        Ake_Ast* parent,
        Ake_Type* type);
Ake_Ast* Ake_parse_sign(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_array_literal(struct Ake_parse_state* ps);
void Ake_parse_aseq(struct Ake_parse_state* ps, Ake_Ast* parent);
Ake_Ast* Ake_parse_parenthesis(struct Ake_parse_state* ps);
Ake_Ast* Ake_parse_if(struct Ake_parse_state* ps);
void Ake_parse_elseif(struct Ake_parse_state* ps, Ake_Ast* parent);
Ake_Ast* Ake_parse_else(struct Ake_parse_state* ps);

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! parse_factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
Ake_Ast* Ake_parse_factor(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;

    struct Ake_token* t0;
	t0 = Ake_get_lookahead(ps);
    assert(t0);

	if (t0->type == Ake_token_fn) {
        n = Ake_parse_function(ps, false, NULL);

    } else if (t0->type == Ake_token_if) {
        n = Ake_parse_if(ps);

	} else if (t0->type == Ake_token_not) {
		n = Ake_parse_not(ps);

	} else if (t0->type == Ake_token_number || t0->type == Ake_token_string || t0->type == Ake_token_boolean) {
		n = Ake_parse_literal(ps);

	} else if (t0->type == Ake_token_id || t0->type == Ake_token_self) {
		n = Ake_parse_id(ps);

	} else if (t0->type == Ake_token_plus || t0->type == Ake_token_minus) {
		n = Ake_parse_sign(ps);

	} else if (t0->type == Ake_token_left_square_bracket) {
		n = Ake_parse_array_literal(ps);

	} else if (t0->type == Ake_token_left_paren) {
		n = Ake_parse_parenthesis(ps);

    }

	return n;
}

Ake_Ast* Ake_parse_function(struct Ake_parse_state* ps, bool is_method, Ake_Type* struct_type)
{
    Ake_Ast* n = NULL;

    Ake_ast_create(&n);
    n->kind = Ake_ast_type_function;

    Ake_token* f = NULL;
    Ake_match(ps, Ake_token_fn, "expected fn", &f, n);
    Ake_consume_newline(ps, n);
    Ake_token_destroy(f);
    free(f);

    /* 0 prototype */
    Ake_Ast* proto = NULL;
    bool has_id;
    proto = Ake_parse_prototype(ps, true, false, is_method, true, &has_id);
    Ake_ast_add(n, proto);
    if (proto->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

    Ake_begin_environment(ps->st, n);
    Ake_declare_params(ps, proto, struct_type);
    Ake_set_current_function(ps->st, n);
    Ake_Type* type = Ake_proto2type_use(ps, proto, struct_type);
    n->type = type;

    Ake_Ast* stmts_node = NULL;
    stmts_node = Ake_parse_stmts(ps, true, false);
	if (stmts_node && stmts_node->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

    /* 1 stmts */
    if (stmts_node) {
        Ake_ast_add(n, stmts_node);
    }

    Ake_end_environment(ps->st);

    struct Ake_token* end = NULL;
	if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        /* test case: test_parse_anonymous_function_expected_end */
        n->kind = Ake_ast_type_error;
    }

	Ake_token_destroy(end);
	free(end);

    if (n->kind != Ake_ast_type_error) {
        /* check return type */
        Ake_Ast* dret = Ast_node_get(proto, 2);
        if (!Ake_check_return_type(ps, proto, stmts_node, &dret->loc)) {
            n->kind = Ake_ast_type_error;
        }
    }

    if (!is_method) {
        if (n->kind != Ake_ast_type_error) {
            Ake_Ast* id_node = Ast_node_get(proto, 0);
            struct Ake_Symbol* new_sym = NULL;
            Zinc_malloc_safe((void**)&new_sym, sizeof(struct Ake_Symbol));
            Ake_SymbolInit(new_sym);
            new_sym->kind = AKE_SYMBOL_VARIABLE;
            new_sym->tu = Ake_TypeClone(type);
            Ake_EnvironmentAdd(ps->st->top, &id_node->value, new_sym, n->loc.start);
        }
    }

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_if(struct Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;
    Ake_ast_create(&n);
    n->kind = Ake_ast_type_if;

    struct Ake_token* ift = NULL;
    if (!Ake_match(ps, Ake_token_if, "expecting if", &ift, n)) {
        /* test case: no test case necessary */
        n->kind = Ake_ast_type_error;
    }

    Ake_token_destroy(ift);
    free(ift);

    Ake_Ast* cb = NULL;
    Ake_ast_create(&cb);
    cb->kind = Ake_ast_type_conditional_branch;

    Ake_ast_add(n, cb);

    Ake_Ast* cond = NULL;
    cond = Ake_parse_expr(ps);
    if (cond && cond->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

    if (cond == NULL) {
        struct Zinc_location cond_loc = Ake_get_location(ps);
        Zinc_error_list_set(ps->el, &cond_loc, "expected condition after if");
        /* test case: test_parse_if_error_expected_expression */
        n->kind = Ake_ast_type_error;
        cb->kind = Ake_ast_type_error;
    } else {
        Ake_ast_add(cb, cond);
    }

    Ake_Ast* body = NULL;
    body = Ake_parse_stmts(ps, false, false);
    if (body && body->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
        cb->kind = Ake_ast_type_error;
    }

    if (body) {
        cb->type = Ake_TypeClone(body->type);
        Ake_ast_add(cb, body);
    }

    Ake_parse_elseif(ps, n);

    Ake_Ast* b = NULL;
    b = Ake_parse_else(ps);
    if (b && b->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

    if (b) {
        Ake_ast_add(n, b);
    }

    struct Ake_token* end = NULL;
    if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
        /* test case: test_parse_if_error_expected_end */
        n->kind = Ake_ast_type_error;
    }

    Ake_token_destroy(end);
    free(end);

    if (n->kind != Ake_ast_type_error) {
        if (b) {
            /* only return a value if else exists */
            Ake_Ast* p = n->head;
            Ake_Type* type = NULL;
            if (p) {
                type = Ake_TypeClone(p->type);
                p = p->next;
            }
            while (p) {
                if (!Ake_TypeMatch(type, p->type, NULL)) {
                    Zinc_error_list_set(ps->el, &p->loc,
                                   "branch type does not match type of previous branch");
                    n->kind = Ake_ast_type_error;
                    break;
                }
                p = p->next;
            }
            n->type = type;
        }
    }

    return n;
}

/* elseif-statement -> elseif expr stmts elseif | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_parse_elseif(struct Ake_parse_state* ps, Ake_Ast* parent)
{
    while (true) {
        struct Ake_token* t0 = Ake_get_lookahead(ps);
        if (t0->type != Ake_token_elseif) {
            break;
        }

        struct Ake_token *eit = NULL;
        if (!Ake_match(ps, Ake_token_elseif, "expecting elseif", &eit, parent)) {
            /* test case: no test case needed */
            assert(false);
        }

        Ake_token_destroy(eit);
        free(eit);

        Ake_Ast *cb = NULL;
        Ake_ast_create(&cb);
        cb->kind = Ake_ast_type_conditional_branch;

        Ake_Ast *cond = NULL;
        cond = Ake_parse_expr(ps);
        if (cond && cond->kind == Ake_ast_type_error) {
            cb->kind = Ake_ast_type_error;
            parent->kind = Ake_ast_type_error;
        }

        if (!cond) {
            struct Zinc_location cond_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &cond_loc, "expected condition after elseif");
            /* test case: test_parse_if_error_expected_elseif_expression */
            cb->kind = Ake_ast_type_error;
            parent->kind = Ake_ast_type_error;
        } else {
            Ake_ast_add(cb, cond);
        }

        Ake_Ast *body = NULL;
        body = Ake_parse_stmts(ps, false, false);
        if (body && body->kind == Ake_ast_type_error) {
            cb->kind = Ake_ast_type_error;
            parent->kind = Ake_ast_type_error;
        }

        if (body) {
            Ake_ast_add(cb, body);
            cb->type = Ake_TypeClone(body->type);
        }

        Ake_ast_add(parent, cb);
    }
}

/* parse_else -> else stmts | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_else(struct Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;

    struct Ake_token* t0 = Ake_get_lookahead(ps);
    if (t0 && t0->type == Ake_token_else) {
        Ake_ast_create(&n);
        n->kind = Ake_ast_type_default_branch;

        struct Ake_token* et = NULL;
        if (!Ake_match(ps, Ake_token_else, "expected else", &et, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        Ake_token_destroy(et);
        free(et);

        /* stmts */
        Ake_Ast* body = NULL;
        body = Ake_parse_stmts(ps, false, false);
        if (body && body->kind == Ake_ast_type_error) {
            n->kind = Ake_ast_type_error;
        }

        if (body) {
            n->type = Ake_TypeClone(body->type);
        }

        if (body) {
            Ake_ast_add(n, body);
        }
    }

    return n;
}

Ake_Ast* Ake_parse_not(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_ast_create(&n);
    n->kind = Ake_ast_type_not;

    struct Ake_token* not = NULL;
	if (!Ake_match(ps, Ake_token_not, "expecting not", &not, n)) {
        /* test case: no test case needed */
        n->kind = Ake_ast_type_error;
    }

    if (!Ake_consume_newline(ps, n)) {
        n->kind = Ake_ast_type_error;
    }

	Ake_Ast* a = NULL;
	a = Ake_parse_expr(ps);
    if (a && a->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

	if (!a) {
        struct Zinc_location a_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &a_loc, "expected parse_factor after !");
		/* test case: test_parse_not_error_expected_factor */
        n->kind = Ake_ast_type_error;
	}

	if (n->kind != Ake_ast_type_error) {
		if (a) {
            Ake_ast_add(n, a);
		}
	}

	if (n->kind != Ake_ast_type_error) {
		assert(a);
		Ake_Type* type = a->type;
		if (!type) {
			Zinc_error_list_set(ps->el, &not->loc, "! operator used on parse_factor with no value");
			/* test case: test_parse_not_error_no_value */
            n->kind = Ake_ast_type_error;
		} else {
			if (type->kind != AKE_TYPE_BOOLEAN) {
				Zinc_error_list_set(ps->el, &not->loc, "not operator used on non-boolean");
				/* test case: test_parse_not_error_not_boolean */
                n->kind = Ake_ast_type_error;
			} else {
				n->type = Ake_TypeClone(type);
			}
		}
	}

	Ake_token_destroy(not);
	free(not);

	return n;
}

Ake_Ast* Ake_parse_literal(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
	char* type_name = NULL;
    Ake_ast_create(&n);

	struct Ake_token* t0 = Ake_get_lookahead(ps);

	struct Ake_token* x = NULL;
	if (!Ake_match(ps, t0->type, "expecting number, bool, or string", &x, n)) {
        /* test case: no test case needed */
        n->kind = Ake_ast_type_error;
    }

    bool is_string = false;
	if (n->kind != Ake_ast_type_error) {
		#pragma warning(suppress:6011)
		if (x->type == Ake_token_number) {
			n->kind = Ake_ast_type_number;
			if (x->is_integer) {
			    if (ps->context.is_subscript) {
			        type_name = "Nat64";
			    } else {
			        type_name = "Int32";
			    }
			} else if (x->is_float) {
				type_name = "Real64";
			}
		} else if (x->type == Ake_token_string) {
			n->kind = Ake_ast_type_string;
			type_name = "Nat8";
            is_string = true;
		} else if (x->type == Ake_token_boolean) {
			n->kind = Ake_ast_type_boolean;
			type_name = "Bool";
		} else {
            assert(false);
        }
		Zinc_string_copy(&x->value, &n->value);
	}

	if (n->kind != Ake_ast_type_error) {
		assert(type_name);
        struct Zinc_string bf;
        Zinc_string_init(&bf);
        Zinc_string_add_str(&bf, type_name);
	    size_t seq = Ake_get_current_seq(ps);
        Ake_symbol* sym = Ake_EnvironmentGet(ps->st->top, &bf, seq);
        assert(sym);
        if (sym->kind != AKE_SYMBOL_TYPE) {
            Zinc_error_list_set(ps->el, &n->loc, "expected a type");
            n->kind = Ake_ast_type_error;
        }
        Zinc_string_destroy(&bf);

        if (is_string) {
            Ake_Type* type = NULL;
            Ake_TypeCreate(&type);
            type->kind = AKE_TYPE_ARRAY;
            type->data.array.is_const = true;
            type->data.array.dim = n->value.size + 1;
            type->data.array.type = Ake_TypeClone(sym->td);;
            n->type = type;
        } else {
            n->type = Ake_TypeClone(sym->td);
        }
	}

	Ake_token_destroy(x);
	free(x);

	return n;
}

Ake_Ast* Ake_parse_id(Ake_parse_state* ps)
{
    Ake_Ast* n = NULL;
    Ake_ast_create(&n);

    Ake_token* t = Ake_get_lookahead(ps);

    Ake_token* id = NULL;

    if (t->type == Ake_token_id) {
        if (!Ake_match(ps, Ake_token_id, "expecting identifier", &id, n)) {
            /* test case: no test case needed */
            assert(false);
        }
    } else if (t->type == Ake_token_self) {
        if (!Ake_match(ps, Ake_token_self, "expecting self", &id, n)) {
            /* test case: no test case needed */
            assert(false);
        }
    }

    size_t seq = Ake_get_current_seq(ps);
    Ake_symbol* sym = Ake_EnvironmentGet(ps->st->top, &id->value, seq);
    if (sym && sym->kind == AKE_SYMBOL_TYPE && sym->td && sym->td->kind == AKE_TYPE_STRUCT) {
        /* struct literal */

        Ake_consume_newline(ps, n);

        n->kind = Ake_ast_type_struct_literal;
        n->type = Ake_TypeClone(sym->td);

        Ake_parse_struct_literal_elements(ps, n, sym->td);

        struct Ake_token* end = NULL;
        if (!Ake_match(ps, Ake_token_end, "expected end", &end, n)) {
            n->kind = Ake_ast_type_error;
        }
        Ake_token_destroy(end);
        free(end);

        Ake_token_destroy(id);
        free(id);
        return n;

    } else {
        /* id */
        n->kind = Ake_ast_type_id;

        if (id) {
            Zinc_string_copy(&id->value, &n->value);
        }

        if (!sym) {
            Zinc_string_finish(&id->value);
            Zinc_error_list_set(ps->el, &id->loc, "variable not declared: %s", id->value.buf);
            /* test case: test_parse_types_missing_declaration */
            n->kind = Ake_ast_type_error;
        } else if (sym->td) {
            Zinc_string_finish(&id->value);
            Zinc_error_list_set(ps->el, &id->loc, "expected struct or variable but found type: %s", id->value.buf);
            /* test case: test_parse_types_missing_declaration */
            n->kind = Ake_ast_type_error;
        } else {
            n->type = Ake_TypeClone(sym->tu);
        }

        Ake_token_destroy(id);
        free(id);
        return n;
    }

}

typedef struct Ake_struct_field_result {
    bool found;
    size_t index;
    Ake_Type* type;
} Ake_struct_field_result;

Ake_struct_field_result Ake_get_struct_field(Ake_Type* type, Zinc_string* name) {
    assert(type->kind == AKE_TYPE_STRUCT);
    size_t index = 0;
    Ake_TypeField* tf = type->data.fields.head;
    while (tf) {
        if (Zinc_string_compare(&tf->name, name)) {
            return (Ake_struct_field_result) {true, index, tf->type};
        }
        tf = tf->next;
        index++;
    }

    return (Ake_struct_field_result) {false, 0, NULL};
}

void Ake_find_missing_fields(Ake_parse_state* ps, Ake_Type* type, Ake_Ast* n) {
    assert(type->kind == AKE_TYPE_STRUCT);
    Ake_TypeField *tf = type->data.fields.head;
    while (tf) {
        bool found = false;
        Ake_Ast *field = n->head;
        while (field) {
            Ake_Ast *id2 = Ast_node_get(field, 0);
            if (Zinc_string_compare(&id2->value, &tf->name)) {
                found = true;
                break;
            }
            field = field->next;
        }
        if (!found) {
            Zinc_error_list_set(ps->el, &n->loc, "struct field missing: %bf", &tf->name);
            n->kind = Ake_ast_type_error;
        }
        tf = tf->next;
    }
}

void Ake_parse_struct_literal_elements(
        Ake_parse_state* ps,
        Ake_Ast* parent,
        Ake_Type* type)
{
    Ake_token* t0;

    while (true) {
        struct Ake_token* name = NULL;
        if (!Ake_match(ps, Ake_token_id, "expected a struct field identifier", &name, parent)) {
            parent->kind = Ake_ast_type_error;
            break;
        }

        Ake_Ast* field = NULL;
        Ake_ast_create(&field);
        field->kind = Ake_ast_type_struct_literal_field;
        Ake_ast_add(parent, field);

        Ake_struct_field_result sfr = Ake_get_struct_field(type, &name->value);
        if (!sfr.found) {
            Zinc_error_list_set(ps->el, &name->loc, "Not a valid field for %bf: %bf", &type->name, &name->value);
            parent->kind = Ake_ast_type_error;
        }

        Ake_Ast* id = NULL;
        Ake_ast_create(&id);
        id->kind = Ake_ast_type_id;
        Zinc_string_copy(&name->value, &id->value);
        Ake_ast_add(field, id);

        Ake_token_destroy(name);
        free(name);

        struct Ake_token* colon = NULL;
        if (!Ake_match(ps, Ake_token_colon, "expected a colon", &colon, parent)) {
            parent->kind = Ake_ast_type_error;
            break;
        }
        Ake_token_destroy(colon);
        free(colon);

        struct Ake_Ast* expr = Ake_parse_expr(ps);
        if (expr) {
            Ake_ast_add(field, expr);

            if (parent->kind != Ake_ast_type_error) {
                bool cast = false;
                if (!Ake_TypeMatch(sfr.type, expr->type, &cast)) {
                    Zinc_error_list_set(ps->el, &expr->loc, "invalid type for field");
                    parent->kind = Ake_ast_type_error;
                }

            }
        } else {
            struct Zinc_location expr_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &expr_loc, "expected an expression");
            parent->kind = Ake_ast_type_error;
        }

        t0 = Ake_get_lookahead(ps);
        if (t0->type == Ake_token_end) {
            break;
        }

        bool has_sep;
        Ake_parse_separator(ps, parent, &has_sep);

        t0 = Ake_get_lookahead(ps);
        if (t0->type == Ake_token_end) {
            break;
        }

        if (!has_sep) {
            struct Zinc_location sep_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &sep_loc, "expected a separator");
            parent->kind = Ake_ast_type_error;
        }
    }

    Ake_find_missing_fields(ps, type, parent);
}

Ake_Ast* Ake_parse_sign(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;

    Ake_ast_create(&n);
    n->kind = Ake_ast_type_sign;

	struct Ake_token* t0 = Ake_get_lookahead(ps);

	/* allocate sign */
	struct Ake_token* sign = NULL;
	if (!Ake_match(ps, t0->type, "expecting unary plus or minus", &sign, n)) {
        /* test case: no test case needed */
        n->kind = Ake_ast_type_error;
    }

    if (!Ake_consume_newline(ps, n)) {
        n->kind = Ake_ast_type_error;
    }

	Ake_Ast* right = NULL;
    right = Ake_parse_complex_operators(ps);
	if (right && right->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

	if (!right) {
        struct Zinc_location right_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &right_loc, "expected parse_factor after sign");
        n->kind = Ake_ast_type_error;
	}

	if (n->kind != Ake_ast_type_error) {
		Ake_Ast* left;
        Ake_ast_create(&left);

		if (t0->type == Ake_token_plus) {
			left->kind = Ake_ast_type_plus;
		} else {
			left->kind = Ake_ast_type_minus;
		}

        Ake_ast_add(n, left);

        Ake_ast_add(n, right);

	}

	if (n->kind != Ake_ast_type_error) {
		assert(right);
		Ake_Type* type = right->type;
		if (!type) {
			Zinc_error_list_set(ps->el, &sign->loc, "negative operator was used on expression with no value");
			/* test case: test_parse_sign_error */
            n->kind = Ake_ast_type_error;
		} else {
			n->type = Ake_TypeClone(type);
		}
	}

	Ake_token_destroy(sign);
    free(sign);

	return n;
}

/*
* array_literal -> [aseq]
*/
Ake_Ast* Ake_parse_array_literal(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_ast_create(&n);
    n->kind = Ake_ast_type_array_literal;

    struct Ake_token* lsb = NULL;
    if (!Ake_match(ps, Ake_token_left_square_bracket, "expected left square bracket", &lsb, n)) {
        n->kind = Ake_ast_type_error;
        /* test case: no test case needed */
    }

    Ake_token_destroy(lsb);
    free(lsb);

    Ake_consume_newline(ps, n);

    Ake_parse_aseq(ps, n);

    if (!Ake_consume_newline(ps, n)) {
        n->kind = Ake_ast_type_error;
    }

    struct Ake_token* rsb = NULL;
    if (!Ake_match(ps, Ake_token_right_square_bracket, "expected right square bracket", &rsb, n)) {
        /* test case: test_parse_array_literal_error_right_square_bracket */
        n->kind = Ake_ast_type_error;
    }

    if (n->kind != Ake_ast_type_error) {
        size_t count = 0;
        Ake_Ast* first = n->head;

        if (!first) {
            Zinc_error_list_set(ps->el, &rsb->loc, "array literal has no elements");
            /* test case: test_parse_array_literal_empty_error */
            n->kind = Ake_ast_type_error;
        } else {
            Ake_Type* tu_first = Ake_TypeClone(first->type);
            Ake_Ast* x = first->next;
            Ake_Type* tu_x;
            count++;
            while (x) {
                tu_x = x->type;
                bool cast = false;
                if (!Ake_TypeMatch(tu_first, tu_x, &cast)) {
                    Zinc_error_list_set(ps->el, &first->loc, "array elements not the same type");
                    /* test case: test_parse_array_literal_mixed_error */
                    n->kind = Ake_ast_type_error;
                    break;
                }
                count++;
                x = x->next;
            }
            Ake_Type* type = NULL;
            Ake_TypeCreate(&type);
            Ake_TypeSet(type, AKE_TYPE_ARRAY);
            type->data.array.dim = count;
            type->data.array.type = Ake_TypeClone(tu_first);
            n->type = type;
        }
    }

    Ake_token_destroy(rsb);
    free(rsb);

	return n;
}

/* aseq -> expr aseq' | e */
/* aseq' = , expr aseq' | e */
void Ake_parse_aseq(struct Ake_parse_state* ps, Ake_Ast* parent)
{
	Ake_Ast* a = NULL;
    a = Ake_parse_simple_expr(ps);
	if (a && a->kind == Ake_ast_type_error) {
        parent->kind = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(parent, a);

		while (true) {
			struct Ake_token* t0 = Ake_get_lookahead(ps);
			if (!t0 || t0->type != Ake_token_comma) {
				break;
			}

			struct Ake_token* comma = NULL;
			if (!Ake_match(ps, Ake_token_comma, "expecting comma", &comma, parent)) {
                /* test case: no test case needed */
                parent->kind = Ake_ast_type_error;
            }

			Ake_token_destroy(comma);
			free(comma);

            if (!Ake_consume_newline(ps, parent)) {
                parent->kind = Ake_ast_type_error;
            }

			a = Ake_parse_simple_expr(ps);
			if (a && a->kind == Ake_ast_type_error) {
                parent->kind = Ake_ast_type_error;
            }

			if (!a) {
                struct Zinc_location a_loc = Ake_get_location(ps);
				Zinc_error_list_set(ps->el, &a_loc, "expected expr after comma");
                parent->kind = Ake_ast_type_error;
				/* test cases: test_parse_array_literal_error_expected_expr */
				break;
			}

            Ake_ast_add(parent, a);
		}
	}
}

Ake_Ast* Ake_parse_parenthesis(struct Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_ast_create(&n);
    n->kind = Ake_ast_type_parenthesis;

	struct Ake_token* lp = NULL;
	if (!Ake_match(ps, Ake_token_left_paren, "expecting left parenthesis", &lp, n)) {
        /* test case: no test case needed */
        n->kind = Ake_ast_type_error;
    }

    if (!Ake_consume_newline(ps, n)) {
        n->kind = Ake_ast_type_error;
    }

	Ake_Ast* a = NULL;
    a = Ake_parse_expr(ps);
	if (a && a->kind == Ake_ast_type_error) {
        n->kind = Ake_ast_type_error;
    }

	if (!a) {
        struct Zinc_location a_loc = Ake_get_location(ps);
		Zinc_error_list_set(ps->el, &a_loc, "empty parenthesis");
        n->kind = Ake_ast_type_error;
		/* test case: test_parse_paren_error_empty */
	}

	struct Ake_token* rp = NULL;
	if (!Ake_match(ps, Ake_token_right_paren, "expected right parenthesis", &rp, n)) {
        n->kind = Ake_ast_type_error;
    }

	if (a) {
        Ake_ast_add(n, a);
	}

	if (n->kind != Ake_ast_type_error) {
		assert(a);
		Ake_Type* type = a->type;
		if (!type) {
			Zinc_error_list_set(ps->el, &a->loc, "parenthesis on expression that has no value");
            n->kind = Ake_ast_type_error;
		} else {
			n->type = Ake_TypeClone(type);
		}
	}

	Ake_token_destroy(lp);
	free(lp);

	Ake_token_destroy(rp);
	free(rp);

	return n;
}
