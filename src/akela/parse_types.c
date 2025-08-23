#include <stdbool.h>
#include "zinc/zstring.h"
#include "token.h"
#include "lex.h"
#include "parse_tools.h"
#include "ast.h"
#include "symbol_table.h"
#include "symbol.h"
#include "zinc/memory.h"
#include "parse_types.h"
#include "type.h"
#include <assert.h>

bool Ake_token_is_type(struct Ake_parse_state* ps, struct Ake_token* t);
Ake_Type* Ake_Type_use_add_proto(
        Ake_parse_state* ps,
        Ake_Type* func,
        Ake_Ast* proto,
        Ake_Type* struct_type);
Ake_Type* Ake_parse_type_dispatch(Ake_parse_state* ps, Ake_Ast* n);
Ake_Type* Ake_parse_type_array(Ake_parse_state* ps, Ake_Ast* n);
Ake_Type* Ake_parse_type_pointer(Ake_parse_state* ps, Ake_Ast* n);
Ake_Type* Ake_parse_type_function(Ake_parse_state* ps, Ake_Ast* n);
Ake_Type* Ake_parse_type_id(Ake_parse_state* ps, Ake_Ast* n);

/**
 * Parse a function prototype
 * @param ps
 * @param has_id
 * @return struct ast_node*
 */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_prototype(
        struct Ake_parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id)
{
    Ake_Ast* n = NULL;

    Ake_AstCreate(&n);
    n->kind = Ake_ast_type_prototype;

    /* 0 id */
    Ake_Ast* id_node = NULL;
    struct Ake_token* t0 = Ake_get_lookahead(ps);
    if (t0->type == Ake_token_id) {
        struct Ake_token* id = NULL;
        if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
            assert(false);
        }
        Ake_AstCreate(&id_node);
        Ake_AstSet(id_node, AKE_AST_ID);
        Zinc_string_copy(&id->value, &id_node->data.id.value);
        Ake_AstAdd(n, id_node);
        Ake_token_destroy(id);
        free(id);
        Ake_consume_newline(ps, n);
        *has_id = true;
    } else {
        Ake_AstCreate(&id_node);
        Ake_AstSet(id_node, AKE_AST_ID);
        if (is_function) {
            Zinc_string_add_format(
                    &id_node->data.id.value,
                    "__anonymous_function_%zu",
                    Ake_symbol_table_generate_id(ps->st)
            );
        }
        Ake_AstAdd(n, id_node);
        *has_id = false;
    }

    struct Ake_token* lp = NULL;
    if (!Ake_match(ps, Ake_token_left_paren, "expected left parenthesis", &lp, n)) {
        /* test case: no test case needed */
        n->has_error = true;
    }
    Ake_token_destroy(lp);
    free(lp);

    Ake_consume_newline(ps, n);

    /* 1 dseq */
    Ake_Ast* dseq_node = NULL;
    dseq_node = Ake_parse_dseq(ps, require_param_name, is_extern, is_method);

    if (dseq_node) {
        Ake_AstAdd(n, dseq_node);
    }

    Ake_consume_newline(ps, n);

    struct Ake_token* rp = NULL;
    if (!Ake_match(ps, Ake_token_right_paren, "expected right parenthesis", &rp, n)) {
        /* test case: test_parse_anonymous_function_expected_right_paren */
        n->has_error = true;
    }
    Ake_token_destroy(rp);
    free(rp);

    /* 2 ret */
    t0 = Ake_get_lookahead(ps);
    Ake_Ast* ret_type = NULL;
    if (t0 && t0->type == Ake_token_arrow) {
        struct Ake_token* dc = NULL;
        if (!Ake_match(ps, Ake_token_arrow, "expecting ->", &dc, n)) {
            /* test case: no test case needed */
            n->has_error = true;
        }
        Ake_token_destroy(dc);
        free(dc);

        Ake_consume_newline(ps, n);

        ret_type = Ake_parse_type(ps);

        if (!ret_type) {
            struct Zinc_location ret_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &ret_loc, "expected a type");
            n->has_error = true;
        }
    }

    Ake_Ast* ret = NULL;
    Ake_AstCreate(&ret);
    ret->kind = Ake_ast_type_dret;
    if (ret_type) {
        Ake_AstAdd(ret, ret_type);
    }

    Ake_AstAdd(n, ret);

    return n;
}

/**
 * Declare the parameters of a prototype
 * @param ps
 * @param proto
 */
void Ake_declare_params(Ake_parse_state* ps, Ake_Ast* proto, Ake_Type* struct_type)
{
    Ake_Ast* dseq = Ake_AstGet(proto, 1);
    Ake_Ast* dec = dseq->head;
    while (dec) {
        Ake_Ast* id_node = Ake_AstGet(dec, 0);
        Ake_Ast* type_node = Ake_AstGet(dec, 1);
        if (dec->kind == Ake_ast_type_self) {
            if (struct_type) {
                type_node->type = Ake_TypeClone(struct_type);
            } else {
                dec = dec->next;
                continue;
            }
        }
        if (!dec->has_error && !type_node->has_error) {
            Ake_declare_type(ps, type_node, id_node, true);
        }
        dec = dec->next;
    }
}

/**
 * Parse the prototype parameters
 * @param ps
 * @return struct ast_node*
 */
/* dseq -> declaration dseq' | e */
/* dseq' -> , declaration dseq' | , ... | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_dseq(
        struct Ake_parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method)
{
	Ake_Ast* n = NULL;
    Ake_AstCreate(&n);
	n->kind = Ake_ast_type_dseq;

	Ake_Ast* dec = NULL;
	dec = Ake_parse_declaration(ps, false, is_method, require_param_name, true);

	if (!dec) {
		return n;
	}

    Ake_AstAdd(n, dec);

	while (true)
	{
		struct Ake_token* t0 = Ake_get_lookahead(ps);
		if (!t0 || t0->type != Ake_token_comma) {
			break;
		}

		struct Ake_token* comma = NULL;
		if (!Ake_match(ps, Ake_token_comma, "expecting comma", &comma, n)) {
            assert(false);
            /* test case: no test case needed */
        }

		Ake_token_destroy(comma);
		free(comma);

        Ake_consume_newline(ps, n);

        struct Ake_token* t = Ake_get_lookahead(ps);
        if (t->type == Ake_token_ellipsis) {
            struct Ake_token* eps = NULL;
            if (!Ake_match(ps, Ake_token_ellipsis, "expected ellipsis", &eps, n)) {
                assert(false);
            }
            Ake_token_destroy(eps);
            free(eps);
            if (is_extern) {
                Ake_Ast* ellipsis = NULL;
                Ake_AstCreate(&ellipsis);
                ellipsis->kind = Ake_ast_type_ellipsis;
                Ake_AstAdd(n, ellipsis);
                break;
            } else {
                Zinc_error_list_set(ps->el, &eps->loc,
                       "Found ellipsis but variadic functions are only supported in extern declarations");
                n->has_error = true;
                break;
            }
        }

		dec = Ake_parse_declaration(ps, false, is_method, require_param_name, true);

		if (!dec) {
            struct Zinc_location dec_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &dec_loc, "expected declaration after comma");
			/* test case: test_parse_error_dseq_comma */
            n->has_error = true;
			break;
		}

        Ake_AstAdd(n, dec);
	}

	return n;
}

/**
 * Is the token the start of a type
 * @param ps
 * @param t
 * @return true if a type, otherwise false
 */
bool Ake_token_is_type(struct Ake_parse_state* ps, struct Ake_token* t)
{
    /* array type */
    if (t->type == Ake_token_left_square_bracket) {
        return true;
    }

    /* function type */
    if (t->type == Ake_token_fn) {
        return true;
    }

    /* type id */
    if (t->type == Ake_token_id) {
        Ake_get_lookahead(ps);
        size_t seq = ps->lookahead->loc.start;
        Ake_symbol* sym = Ake_EnvironmentGet(ps->st->top, &t->value, seq);
        if (sym && sym->td) {
            return true;
        }
    }

    return false;
}

/**
 * Parse a declaration which consists of an id and type
 * @param ps
 * @param add_symbol
 * @return struct ast_node*
 */
/* declaration -> id :: type | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_declaration(
        struct Ake_parse_state* ps,
        bool add_symbol,
        bool is_method,
        bool require_param_name,
        bool is_const)
{
	Ake_Ast* n = NULL;

	struct Ake_token* t0 = Ake_get_lookahead(ps);
    bool type_only = !require_param_name && Ake_token_is_type(ps, t0);
    bool is_self = is_method && t0->type == Ake_token_self;
    if (t0->type == Ake_token_id || type_only || is_self) {
        Ake_AstCreate(&n);
        n->kind = Ake_ast_type_declaration;

        if (is_self) {
            n->kind = Ake_ast_type_self;
            struct Ake_token *self = NULL;
            if (!Ake_match(ps, Ake_token_self, "expected self", &self, n)) {
                /* test case: no test case needed */
                assert(false);
            }

            Ake_Ast* id_node = NULL;
            Ake_AstCreate(&id_node);
            Ake_AstSet(id_node, AKE_AST_ID);
            Zinc_string_add_str(&id_node->data.id.value, "self");
            Ake_AstAdd(n, id_node);

            Ake_Ast* type_node = NULL;
            Ake_AstCreate(&type_node);
            type_node->kind = Ake_ast_type_type;
            Ake_AstAdd(n, type_node);

            Ake_token_destroy(self);
            free(self);

        } else if (type_only) {
            Ake_Ast* id_node = NULL;
            Ake_AstCreate(&id_node);
            Ake_AstSet(id_node, AKE_AST_ID);
            Ake_AstAdd(n, id_node);

            Ake_Ast* type_use = NULL;
            type_use = Ake_parse_type(ps);

            if (!type_use) {
                struct Zinc_location type_use_loc = Ake_get_location(ps);
                Zinc_error_list_set(ps->el, &type_use_loc, "expected a type");
                /* test case: test_parse_error_declaration_type */
                n->has_error = true;
            }

            if (type_use) {
                Ake_AstAdd(n, type_use);
            }
        } else if (t0->type == Ake_token_id) {
            Ake_Ast* id_node = NULL;
            Ake_AstCreate(&id_node);
            Ake_AstSet(id_node, AKE_AST_ID);
            Ake_AstAdd(n, id_node);

            struct Ake_token *id = NULL;
            if (!Ake_match(ps, Ake_token_id, "expected id", &id, n)) {
                assert(false);
                /* test case: no test case needed */
            }
            Zinc_string_copy(&id->value, &id_node->data.id.value);
            Ake_token_destroy(id);
            free(id);

            Ake_consume_newline(ps, n);

            struct Ake_token *dc = NULL;
            if (!Ake_match(ps, Ake_token_colon, "expected colon", &dc, n)) {
                /* test case: test_parse_error_declaration_double_colon */
                n->has_error = true;
            }
            Ake_token_destroy(dc);
            free(dc);

            Ake_consume_newline(ps, n);

            Ake_Ast* type_use = NULL;
            type_use = Ake_parse_type(ps);
            if (!type_use) {
                Zinc_location loc = Ake_get_location(ps);
                Zinc_error_list_set(ps->el, &loc, "expected type identifier or fn");
                n->has_error = true;
            }
            if (add_symbol) {
                if (!n->has_error) {
                    Ake_declare_type(ps, type_use, id_node, is_const);
                }
            }

            if (type_use) {
                Ake_AstAdd(n, type_use);
            }
        } else {
            assert(false);
        }
    }

	return n;
}

/**
 * Parse a type
 * @param ps
 * @return ast
 */
/* type -> id | id { tseq } */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Ast* Ake_parse_type(Ake_parse_state* ps)
{
	Ake_Ast* n = NULL;
    Ake_AstCreate(&n);
    n->kind = Ake_ast_type_type;
    n->type = Ake_parse_type_dispatch(ps, n);
	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Type* Ake_parse_type_dispatch(Ake_parse_state* ps, Ake_Ast* n)
{
    Ake_token* t = Ake_get_lookahead(ps);

    if (t->type == Ake_token_left_square_bracket) {
        return Ake_parse_type_array(ps, n);
    }

    if (t->type == Ake_token_mult) {
        return Ake_parse_type_pointer(ps, n);
    }

    if (t->type == Ake_token_fn) {
        return Ake_parse_type_function(ps, n);
    }

    if (t->type == Ake_token_id) {
        return Ake_parse_type_id(ps, n);
    }

    return NULL;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Type* Ake_parse_type_array(Ake_parse_state* ps, Ake_Ast* n)
{
    Ake_Type* type = NULL;
    Ake_TypeCreate(&type);

    Ake_token *lsb = NULL;
    if (!Ake_match(ps, Ake_token_left_square_bracket, "expected left square bracket", &lsb, n)) {
        assert(false);
    }

    Ake_token_destroy(lsb);
    free(lsb);

    bool has_number = false;
    bool has_const = false;
    size_t dim_size_number = 0;

    Ake_token* t0 = Ake_get_lookahead(ps);
    if (t0->type == Ake_token_number) {
        Ake_token *dim_size = NULL;
        if (!Ake_match(ps, Ake_token_number, "expected number", &dim_size, n)) {
            /* test case: no test case needed */
            assert(false);
        }
        has_number = true;
        Zinc_string_finish(&dim_size->value);
        dim_size_number = (size_t) strtol(dim_size->value.buf, NULL, 10);
        Ake_token_destroy(dim_size);
        free(dim_size);
    }

    t0 = Ake_get_lookahead(ps);
    if (t0->type == Ake_token_const) {
        Ake_token* const_token = NULL;
        if (!Ake_match(ps, Ake_token_const, "expected const", &const_token, n)) {
            /* test case: no test case needed */
            assert(false);
        }
        has_const = true;
        Ake_token_destroy(const_token);
        free(const_token);
    }

    Ake_token* rsb = NULL;
    if (!Ake_match(ps, Ake_token_right_square_bracket, "expected right square bracket", &rsb, n)) {
        n->has_error = true;
    }
    Ake_token_destroy(rsb);
    free(rsb);

    if (has_number) {
        Ake_TypeSet(type, AKE_TYPE_ARRAY);
        type->data.array.is_const = has_const;
        type->data.array.dim = dim_size_number;
        Ake_Type* type2 = Ake_parse_type_dispatch(ps, n);
        if (type2) {
            type->data.array.type = type2;
        } else {
            Zinc_error_list_set(ps->el, &n->loc, "expected array element type");
            n->has_error = true;
        }
    } else {
        Ake_TypeSet(type, AKE_TYPE_SLICE);
        Ake_Type* type2 = Ake_parse_type_dispatch(ps, n);
        if (type) {
            type->data.slice.type = type2;
        } else {
            Zinc_error_list_set(ps->el, &n->loc, "expected slice element type");
            n->has_error = true;
        }
    }

    return type;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Type* Ake_parse_type_pointer(Ake_parse_state* ps, Ake_Ast* n)
{
    Ake_Type* type = NULL;
    Ake_TypeCreate(&type);
    Ake_TypeSet(type, AKE_TYPE_POINTER);
    Ake_token *ast = NULL;
    if (!Ake_match(ps, Ake_token_mult, "expected asterisk", &ast, n)) {
        assert(false && "not possible");
    }
    Ake_token_destroy(ast);
    free(ast);
    Ake_Type* type2 = Ake_parse_type_dispatch(ps, n);
    if (type2) {
        type->data.pointer.type = type2;
    } else {
        Zinc_error_list_set(ps->el, &n->loc, "expected pointer type");
        n->has_error = true;
    }
    return type;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_Type* Ake_parse_type_function(Ake_parse_state* ps, Ake_Ast* n)
{
    Ake_Type* type = NULL;
    Ake_TypeCreate(&type);
    Ake_TypeSet(type, AKE_TYPE_FUNCTION);

    Ake_token* fn = NULL;
    if (!Ake_match(ps, Ake_token_fn, "expected fn", &fn, n)) {
        assert(false);
    }

    Ake_token_destroy(fn);
    free(fn);

    bool has_id;
    Ake_Ast* proto = Ake_parse_prototype(ps, false, false, false, false, &has_id);
    if (has_id) {
        Zinc_error_list_set(ps->el, &proto->loc, "function type has name");
    }
    if (proto->has_error) {
        n->has_error = true;
    } else {
        Ake_Type_use_add_proto(ps, type, proto, NULL);
    }
    Ake_AstDestroy(proto);

    return type;
}

Ake_Type* Ake_parse_type_id(Ake_parse_state* ps, Ake_Ast* n)
{
    Ake_Type* type = NULL;

    struct Ake_token* id = NULL;
    if (!Ake_match(ps, Ake_token_id, "expected type identifier", &id, n)) {
        n->has_error = true;
    }

    struct Ake_Symbol* sym = NULL;
    if (!n->has_error) {
        Ake_get_lookahead(ps);
        size_t seq = ps->lookahead->loc.start;
        sym = Ake_EnvironmentGet(ps->st->top, &id->value, seq);
        if (!sym) {
            char* a;
            Zinc_string_create_str(&id->value, &a);
            Zinc_error_list_set(ps->el, &id->loc, "type not defined: %s", a);
            free(a);
            n->has_error = true;
            /* test case: test_parse_error_type_not_defined */
        } else if (!sym->td) {
            char *a;
            Zinc_string_create_str(&id->value, &a);
            Zinc_error_list_set(ps->el, &id->loc, "identifier is not a type: %s", a);
            free(a);
            n->has_error = true;
            /* test case: test_parse_error_not_a_type */
        } else {
            if (!n->has_error) {
                type = Ake_TypeClone(sym->td);
            }
        }
    }

    Ake_token_destroy(id);
    free(id);

    return type;
}

void Ake_create_variable_symbol(Ake_parse_state* ps, Zinc_string* name, Ake_Type* type, size_t seq, bool is_const)
{
    Ake_symbol* new_sym = NULL;
    Ake_SymbolCreate(&new_sym);
    new_sym->kind = AKE_SYMBOL_VARIABLE;
    new_sym->tu = Ake_TypeClone(type);
    new_sym->is_const = is_const;
    Ake_EnvironmentAdd(ps->st->top, name, new_sym, seq);
}

void Ake_declare_type(Ake_parse_state* ps, Ake_Ast* type_node, Ake_Ast* id_node, bool is_const)
{
    if (type_node && !type_node->has_error) {
        if (id_node) {
            if (id_node->kind == AKE_AST_ID) {
                Ake_create_variable_symbol(ps, &id_node->data.id.value, type_node->type, type_node->loc.start, is_const);
            } else {
                assert(false);
            }
        }
    }
}

Ake_Type* Ake_proto2type_use(Ake_parse_state* ps, Ake_Ast* proto, Ake_Type* struct_type) {
    Ake_Type *func = NULL;
    Ake_TypeCreate(&func);
    Ake_TypeSet(func, AKE_TYPE_FUNCTION);
    return Ake_Type_use_add_proto(ps, func, proto, struct_type);
}

Ake_Type* Ake_Type_use_add_proto(
    Ake_parse_state* ps,
    Ake_Type* func,
    Ake_Ast* proto,
    Ake_Type* struct_type)
{
    Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "Function");
    Ake_get_lookahead(ps);
    Zinc_string_destroy(&bf);
    func->kind = AKE_TYPE_FUNCTION;

    Ake_Ast* id_node = Ake_AstGet(proto, 0);
    Ake_Ast* dseq = Ake_AstGet(proto, 1);
    Ake_Ast* dret = Ake_AstGet(proto, 2);

    Zinc_string_copy(&id_node->data.id.value, &func->name);

    if (dseq->head) {
        Ake_Ast* dec = dseq->head;
        while (dec) {
            Ake_Ast* id_node = Ake_AstGet(dec, 0);
            Ake_Ast* type_node = Ake_AstGet(dec, 1);

            Ake_TypeParam* tp = NULL;
            Ake_TypeParamCreate(&tp);
            Zinc_string_add_string(&tp->name, &id_node->data.id.value);

            if (dec->kind == Ake_ast_type_self) {
                tp->kind = AKE_TYPE_PARAM_SELF;
                tp->type = Ake_TypeClone(struct_type);
            } else if (dec->kind == Ake_ast_type_ellipsis) {
                tp->kind = AKE_TYPE_PARAM_ELLIPSIS;
            } else {
                tp->type = Ake_TypeClone(type_node->type);
            }

            Ake_TypeInputAdd(func, tp);

            dec = dec->next;
        }
    }

    Ake_Ast* ret_type_node = Ake_AstGet(dret, 0);
    if (ret_type_node) {
        func->data.function.output = Ake_TypeClone(ret_type_node->type);
    }

    return func;
}

bool Ake_check_return_type(Ake_parse_state* ps, Ake_Ast* proto, Ake_Ast* stmts_node, Zinc_location* loc)
{
    bool valid = true;

    if (!proto->has_error && !stmts_node->has_error) {
        Ake_Ast *dret = Ake_AstGet(proto, 2);
        Ake_Ast *ret_type = Ake_AstGet(dret, 0);
        if (ret_type) {
            if (!Ake_TypeMatch(ret_type->type, stmts_node->type, NULL)) {
                valid = Zinc_error_list_set(ps->el, loc, "returned type does not match function return type");
            }
        }
    }

    return valid;
}

Ake_TypeParam* Ake_get_function_input_type(Ake_Type* func, int index)
{
	Ake_TypeParam* p = func->data.function.input_head;
	int i = 0;
	while (p) {
		if (i == index) {
            return p;
        }
		p = p->next;
		i++;
	}

	return NULL;
}

bool Ake_check_input_type(
    Ake_parse_state* ps,
    Ake_Type* func,
    int index,
    Ake_Ast* a)
{
	bool valid = true;

	if (func) {
		Ake_TypeParam* tp = Ake_get_function_input_type(func, index);
		if (tp) {
		    Ake_Type* tu0 = tp->type;
			Ake_Type* call_tu0 = a->type;
			if (call_tu0) {
			    bool cast = false;
				if (!Ake_TypeMatch(tu0, call_tu0, &cast)) {
					valid = Zinc_error_list_set(ps->el, &a->loc, "parameter and arguments types do not match");
					/* test case: test_parse_types_error_param */
				}
			} else {
				valid = Zinc_error_list_set(ps->el, &a->loc, "argument expression has no value");
				/* test case: test_parse_types_error_param_no_value */
			}
		}
	}

	return valid;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Ake_Override_rhs(Ake_Type* type, Ake_Ast* rhs)
{
    if (rhs->kind == AKE_AST_SIGN) {
        Ake_Ast* p = rhs->data.sign.right;
        if (p->kind == Ake_ast_type_number) {
            Ake_TypeCopy(type, p->type);
            Ake_TypeCopy(type, rhs->type);
        }
        return;
    }

    if (rhs->kind == Ake_ast_type_number) {
        if (Ake_is_numeric(type)) {
            Ake_TypeCopy(type, rhs->type);
        }
        return;
    }

    if (rhs->kind == Ake_ast_type_array_literal) {
        Ake_TypeCopy(type, rhs->type);

        if (type->kind == AKE_TYPE_ARRAY) {
            if (type->data.array.is_const && !rhs->type->data.array.is_const) {
                rhs->type->data.array.is_const = true;
            }
            Ake_Ast* p = rhs->head;
            while (p) {
                Ake_Override_rhs(type->data.array.type, p);
                p = p->next;
            }
        }
    }
}

bool Ake_is_lvalue(enum Ake_AstKind type)
{
    if (type == AKE_AST_ID) {
        return true;
    }

    if (type == Ake_ast_type_array_subscript) {
        return true;
    }

    if (type == Ake_ast_type_eseq) {
        return true;
    }

    if (type == Ake_ast_type_dot) {
        return true;
    }

    return false;
}

bool Ake_check_lvalue(Ake_parse_state* ps, Ake_Ast* n, Zinc_location* loc)
{
    Ake_Ast* p = n;
    Ake_symbol* sym = NULL;
    Ake_Ast* first = NULL;
    while (p) {
        if (!Ake_is_lvalue(p->kind)) {
            Zinc_error_list_set(ps->el, loc, "invalid lvalue");
            n->has_error = true;
            p->has_error = true;
            return false;
        }
        if (!p->head) {
            assert(p->type);
            if (p->kind != AKE_AST_ID) {
                Zinc_error_list_set(ps->el, loc, "invalid lvalue");
                n->has_error = true;
                p->has_error = true;
                return false;
            }
            Ake_get_lookahead(ps);
            size_t seq = ps->lookahead->loc.start;
            sym = Ake_EnvironmentGet(ps->st->top, &p->data.id.value, seq);
            if (sym->is_const) {
                Zinc_error_list_set(ps->el, loc, "immutable variable changed in assignment");
                n->has_error = true;
            }
            first = p;
        }
        p = p->head;
    }

    if (!n->type) {
        Zinc_error_list_set(ps->el, loc, "invalid lvalue");
        n->has_error = true;
        return false;
    }

    if (!sym) {
        Zinc_error_list_set(ps->el, loc, "invalid lvalue");
        n->has_error = true;
        return false;
    }

    if (!n->has_error) {
        if (n->kind == Ake_ast_type_array_subscript) {
            Ake_Ast* left = n->head;
            if (left->type->kind == AKE_TYPE_ARRAY && left->type->data.array.is_const) {
                Zinc_error_list_set(ps->el, loc, "immutable variable changed in assignment");
                n->has_error = true;
            }

        }
    }

    return true;
}

bool Ake_is_placeholder_node(Ake_Ast* n)
{
    if (n->kind == AKE_AST_ID && Zinc_string_compare_str(&n->data.id.value, "_")) {
        return true;
    } else {
        return false;
    }
}

bool Ake_is_placeholder_token(struct Ake_token* t)
{
    if (t->type == Ake_token_id && Zinc_string_compare_str(&t->value, "_")) {
        return true;
    } else {
        return false;
    }
}

Ake_Type* Ake_StructToType(Ake_Ast* n)
{
    Ake_Type* type = NULL;
    Ake_TypeCreate(&type);
    Ake_TypeSet(type, AKE_TYPE_STRUCT);
    Zinc_string_add_string(&type->name, &n->struct_value);
    Ake_Ast* dec = n->head;
    while (dec) {
        Ake_TypeField* tf = NULL;
        Ake_TypeFieldCreate(&tf);

        Ake_Ast* id_node = dec->head;
        assert(id_node);

        Zinc_string_add_string(&tf->name, &id_node->data.id.value);

        Ake_Ast* type_node = id_node->next;
        assert(type_node);

        tf->type = Ake_TypeClone(type_node->type);
        Ake_TypeStructAdd(type, tf);

        dec = dec->next;
    }
    return type;
}