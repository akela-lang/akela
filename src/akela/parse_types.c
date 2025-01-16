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
#include "type_def.h"
#include <assert.h>

bool Ake_token_is_type(struct Ake_parse_state* ps, struct Ake_token* t);
Ake_type_use* Ake_Type_use_add_proto(
        struct Ake_symbol_table* st,
        Ake_type_use* func,
        Ake_ast* proto,
        Ake_ast* struct_type);

/**
 * Parse a function prototype
 * @param ps
 * @param has_id
 * @param loc
 * @return struct ast_node*
 */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_prototype(
        struct Ake_parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id)
{
    Ake_ast* n = NULL;

    Ake_ast_create(&n);
    n->type = Ake_ast_type_prototype;

    /* 0 id */
    Ake_ast* id_node = NULL;
    struct Ake_token* t0 = Ake_get_lookahead(ps);
    if (t0->type == Ake_token_id) {
        struct Ake_token* id = NULL;
        if (!Ake_match(ps, Ake_token_id, "expected identifier", &id, n)) {
            assert(false);
        }
        Ake_ast_create(&id_node);
        id_node->type = Ake_ast_type_id;
        Zinc_string_copy(&id->value, &id_node->value);
        Ake_ast_add(n, id_node);
        Ake_token_destroy(id);
        free(id);
        Ake_consume_newline(ps, n);
        *has_id = true;
    } else {
        Ake_ast_create(&id_node);
        id_node->type = Ake_ast_type_id;
        if (is_function) {
            Zinc_string_add_format(
                    &id_node->value,
                    "__anonymous_function_%zu",
                    Ake_symbol_table_generate_id(ps->st)
            );
        }
        Ake_ast_add(n, id_node);
        *has_id = false;
    }

    struct Ake_token* lp = NULL;
    if (!Ake_match(ps, Ake_token_left_paren, "expected left parenthesis", &lp, n)) {
        /* test case: no test case needed */
        n->type = Ake_ast_type_error;
    }
    Ake_token_destroy(lp);
    free(lp);

    Ake_consume_newline(ps, n);

    /* 1 dseq */
    Ake_ast* dseq_node = NULL;
    dseq_node = Ake_parse_dseq(ps, require_param_name, is_extern, is_method);
    if (dseq_node && dseq_node->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

    if (dseq_node) {
        Ake_ast_add(n, dseq_node);
    }

    if (!Ake_consume_newline(ps, n)) {
        n->type = Ake_ast_type_error;
    }

    struct Ake_token* rp = NULL;
    if (!Ake_match(ps, Ake_token_right_paren, "expected right parenthesis", &rp, n)) {
        /* test case: test_parse_anonymous_function_expected_right_paren */
        n->type = Ake_ast_type_error;
    }
    Ake_token_destroy(rp);
    free(rp);

    /* 2 ret */
    t0 = Ake_get_lookahead(ps);
    Ake_ast* ret_type = NULL;
    if (t0 && t0->type == Ake_token_arrow) {
        struct Ake_token* dc = NULL;
        if (!Ake_match(ps, Ake_token_arrow, "expecting ->", &dc, n)) {
            /* test case: no test case needed */
            n->type = Ake_ast_type_error;
        }
        Ake_token_destroy(dc);
        free(dc);

        if (!Ake_consume_newline(ps, n)) {
            n->type = Ake_ast_type_error;
        }

        ret_type = Ake_parse_type(ps);
        if (ret_type && ret_type->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

        if (!ret_type) {
            struct Zinc_location ret_loc = Ake_get_location(ps);
            Zinc_error_list_set(ps->el, &ret_loc, "expected a type");
            n->type = Ake_ast_type_error;
        }
    }

    Ake_ast* ret = NULL;
    Ake_ast_create(&ret);
    ret->type = Ake_ast_type_dret;
    if (ret_type) {
        Ake_ast_add(ret, ret_type);
    }

    Ake_ast_add(n, ret);

    return n;
}

/**
 * Declare the parameters of a prototype
 * @param ps
 * @param proto
 */
void Ake_declare_params(struct Ake_parse_state* ps, Ake_ast* proto, Ake_ast* struct_type)
{
    Ake_ast* dseq = Ast_node_get(proto, 1);
    Ake_ast* dec = dseq->head;
    while (dec) {
        Ake_ast* id_node = Ast_node_get(dec, 0);
        Ake_ast* type_node = Ast_node_get(dec, 1);
        if (dec->type == Ake_ast_type_self) {
            if (struct_type) {
                type_node = struct_type;
            } else {
                dec = dec->next;
                continue;
            }
        }
        if (dec->type != Ake_ast_type_error && type_node->type != Ake_ast_type_error) {
            Ake_declare_type(ps, type_node, id_node);
        }
        dec = dec->next;
    }
}

/**
 * Parse the prototype parameters
 * @param ps
 * @param loc
 * @return struct ast_node*
 */
/* dseq -> declaration dseq' | e */
/* dseq' -> , declaration dseq' | , ... | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_dseq(
        struct Ake_parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
	n->type = Ake_ast_type_dseq;

	Ake_ast* dec = NULL;
	dec = Ake_parse_declaration(ps, false, is_method, require_param_name);
    if (dec && dec->type == Ake_ast_type_error) {
        n->type = Ake_ast_type_error;
    }

	if (!dec) {
		return n;
	}

    Ake_ast_add(n, dec);

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
                Ake_ast* ellipsis = NULL;
                Ake_ast_create(&ellipsis);
                ellipsis->type = Ake_ast_type_ellipsis;
                Ake_ast_add(n, ellipsis);
                break;
            } else {
                Zinc_error_list_set(ps->el, &eps->loc,
                       "Found ellipsis but variadic functions are only supported in extern declarations");
                n->type = Ake_ast_type_error;
                break;
            }
        }

		dec = Ake_parse_declaration(ps, false, is_method, require_param_name);
        if (dec && dec->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        }

		if (!dec) {
            struct Zinc_location dec_loc = Ake_get_location(ps);
			Zinc_error_list_set(ps->el, &dec_loc, "expected declaration after comma");
			/* test case: test_parse_error_dseq_comma */
            n->type = Ake_ast_type_error;
			break;
		}

        Ake_ast_add(n, dec);
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
        struct Ake_symbol* sym = Ake_environment_get(ps->st->top, &t->value);
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
 * @param loc
 * @return struct ast_node*
 */
/* declaration -> id :: type | e */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_declaration(
        struct Ake_parse_state* ps,
        bool add_symbol,
        bool is_method,
        bool require_param_name)
{
	Ake_ast* n = NULL;

	struct Ake_token* t0 = Ake_get_lookahead(ps);
    bool type_only = !require_param_name && Ake_token_is_type(ps, t0);
    bool is_self = is_method && t0->type == Ake_token_self;
    if (t0->type == Ake_token_id || type_only || is_self) {
        Ake_ast_create(&n);
        n->type = Ake_ast_type_declaration;

        if (is_self) {
            n->type = Ake_ast_type_self;
            struct Ake_token *self = NULL;
            if (!Ake_match(ps, Ake_token_self, "expected self", &self, n)) {
                /* test case: no test case needed */
                assert(false);
            }

            Ake_ast* id_node = NULL;
            Ake_ast_create(&id_node);
            id_node->type = Ake_ast_type_id;
            Zinc_string_add_str(&id_node->value, "self");
            Ake_ast_add(n, id_node);

            Ake_token_destroy(self);
            free(self);

        } else if (type_only) {
            Ake_ast* id_node = NULL;
            Ake_ast_create(&id_node);
            id_node->type = Ake_ast_type_id;
            Ake_ast_add(n, id_node);

            Ake_ast* type_use = NULL;
            type_use = Ake_parse_type(ps);
            if (type_use && type_use->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }

            if (!type_use) {
                struct Zinc_location type_use_loc = Ake_get_location(ps);
                Zinc_error_list_set(ps->el, &type_use_loc, "expected a type");
                /* test case: test_parse_error_declaration_type */
                n->type = Ake_ast_type_error;
            }

            if (type_use) {
                Ake_ast_add(n, type_use);
            }
        } else if (t0->type == Ake_token_id) {
            Ake_ast* id_node = NULL;
            Ake_ast_create(&id_node);
            id_node->type = Ake_ast_type_id;
            Ake_ast_add(n, id_node);

            struct Ake_token *id = NULL;
            if (!Ake_match(ps, Ake_token_id, "expected id", &id, n)) {
                assert(false);
                /* test case: no test case needed */
            }
            Zinc_string_copy(&id->value, &id_node->value);
            Ake_token_destroy(id);
            free(id);

            Ake_consume_newline(ps, n);

            struct Ake_token *dc = NULL;
            if (!Ake_match(ps, Ake_token_colon, "expected colon", &dc, n)) {
                /* test case: test_parse_error_declaration_double_colon */
                n->type = Ake_ast_type_error;
            }
            Ake_token_destroy(dc);
            free(dc);

            Ake_consume_newline(ps, n);

            Ake_ast* type_use = NULL;
            type_use = Ake_parse_type(ps);
            if (type_use && type_use->type == Ake_ast_type_error) {
                n->type = Ake_ast_type_error;
            }
            if (add_symbol) {
                if (n->type != Ake_ast_type_error) {
                    Ake_declare_type(ps, type_use, id_node);
                }
            }

            if (!type_use) {
                struct Zinc_location type_use_loc = Ake_get_location(ps);
                Zinc_error_list_set(ps->el, &type_use_loc, "expected a type");
                /* test case: test_parse_error_declaration_type */
                n->type = Ake_ast_type_error;
            }

            if (type_use) {
                Ake_ast_add(n, type_use);
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
 * @param loc
 * @return ast
 */
/* type -> id | id { tseq } */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ake_ast* Ake_parse_type(struct Ake_parse_state* ps)
{
	Ake_ast* n = NULL;
    Ake_ast_create(&n);
    n->type = Ake_ast_type_type;

    Ake_type_use* tu = NULL;
    Ake_type_use_create(&tu);
    n->tu = tu;

    /* handle array dimensions */
	struct Ake_token* t0 = Ake_get_lookahead(ps);
    while (t0->type == Ake_token_left_square_bracket) {
        struct Ake_token *lsb = NULL;
        if (!Ake_match(ps, Ake_token_left_square_bracket, "expected left square bracket", &lsb, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        Ake_token_destroy(lsb);
        free(lsb);

        bool has_number = false;
        bool has_const = false;
        size_t dim_size_number;
        t0 = Ake_get_lookahead(ps);

        if (t0->type == Ake_token_number) {
            struct Ake_token *dim_size = NULL;
            if (!Ake_match(ps, Ake_token_number, "expected number", &dim_size, n)) {
                /* test case: no test case needed */
                assert(false);
            }
            has_number = true;
            Zinc_string_finish(&dim_size->value);
            dim_size_number = (size_t) strtol(dim_size->value.buf, NULL, 10);
            Ake_token_destroy(dim_size);
            free(dim_size);
            t0 = Ake_get_lookahead(ps);
        }

        if (t0->type == Ake_token_const) {
            struct Ake_token* const_token = NULL;
            if (!Ake_match(ps, Ake_token_const, "expected const", &const_token, n)) {
                /* test case: no test case needed */
                assert(false);
            }
            has_const = true;
            Ake_token_destroy(const_token);
            free(const_token);
        }

        struct Ake_token* rsb = NULL;
        if (!Ake_match(ps, Ake_token_right_square_bracket, "expected right square bracket", &rsb, n)) {
            n->type = Ake_ast_type_error;
        }

        Ake_token_destroy(rsb);
        free(rsb);

        if (has_number) {
            n->tu->is_array = true;
            Ake_type_dimension dim;
            dim.size =  dim_size_number;
            if (has_const) {
                dim.option = Ake_array_element_const;
            } else {
                dim.option = Ake_array_element_default;
            }
            VectorAdd(&n->tu->dim, &dim, 1);
        } else {
            n->tu->is_slice = true;
            Ake_type_dimension dim;
            dim.size = 0;
            if (has_const) {
                dim.option = Ake_array_element_const;
            } else {
                dim.option = Ake_array_element_default;
            }
            VectorAdd(&n->tu->dim, &dim, 1);
        }

        t0 = Ake_get_lookahead(ps);
    }

    if (t0->type == Ake_token_fn) {
        struct Ake_token* fn = NULL;
        if (!Ake_match(ps, Ake_token_fn, "expected fn", &fn, n)) {
            assert(false);
        }

        Ake_token_destroy(fn);
        free(fn);

        bool has_id;
        Ake_ast* proto = Ake_parse_prototype(ps, false, false, false, false, &has_id);
        if (has_id) {
            Zinc_error_list_set(ps->el, &proto->loc, "function type has name");
        }
        if (proto->type == Ake_ast_type_error) {
            n->type = Ake_ast_type_error;
        } else {
            Ake_Type_use_add_proto(ps->st, n->tu, proto, NULL);
        }
        Ake_ast_destroy(proto);

    } else if (t0->type == Ake_token_id) {
        /* handle type or array element */
        struct Ake_token* name = NULL;
        if (!Ake_match(ps, t0->type, "expected type identifier", &name, n)) {
            n->type = Ake_ast_type_error;
        }

        struct Ake_symbol* sym = NULL;
        if (n->type != Ake_ast_type_error) {
            sym = Ake_environment_get(ps->st->top, &name->value);
            if (!sym) {
                char* a;
                Zinc_string_create_str(&name->value, &a);
                Zinc_error_list_set(ps->el, &name->loc, "type not defined: %s", a);
                free(a);
                n->type = Ake_ast_type_error;
                /* test case: test_parse_error_type_not_defined */
            } else if (!sym->td) {
                char *a;
                Zinc_string_create_str(&name->value, &a);
                Zinc_error_list_set(ps->el, &name->loc, "identifier is not a type: %s", a);
                free(a);
                n->type = Ake_ast_type_error;
                /* test case: test_parse_error_not_a_type */
            } else if (sym->td == ps->st->function_type_def) {
                Zinc_error_list_set(
                        ps->el,
                        &name->loc,
                        "can not directly use Function to declare a function; use fn syntax to declare a function");
                n->type = Ake_ast_type_error;
            } else {
                if (n->type != Ake_ast_type_error) {
                    n->tu->td = sym->td;
                }
            }
        }

        Ake_token_destroy(name);
        free(name);

    } else {
        t0 = Ake_get_lookahead(ps);
        if (t0->type != Ake_token_fn && t0->type != Ake_token_id) {
            Zinc_error_list_set(ps->el, &t0->loc, "expected type identifier or fn");
            n->type = Ake_ast_type_error;
        }
    }

	return n;
}

/**
 * Check ID node and create symbol
 * @param ps parse state
 * @param n type node
 * @param id_node ID node
 */
void Ake_create_variable_symbol(struct Ake_parse_state* ps, Ake_ast* type_node, Ake_ast* id_node)
{
    struct Ake_symbol* dup = Ake_environment_get_local(ps->st->top, &id_node->value);
    if (dup) {
        char* a;
        Zinc_string_create_str(&id_node->value, &a);
        Zinc_error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", a);
        free(a);
        type_node->type = Ake_ast_type_error;
        /* test case: test_parse_error_duplicate_declarations */
    } else {
        struct Ake_symbol* sym2 = Ake_environment_get(ps->st->top, &id_node->value);
        if (sym2 && sym2->td) {
            char* a;
            Zinc_string_create_str(&id_node->value, &a);
            Zinc_error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", a);
            free(a);
            type_node->type = Ake_ast_type_error;
            /* test case: test_parse_types_reserved_type */
        } else {
            struct Ake_symbol* new_sym = NULL;
            Zinc_malloc_safe((void**)&new_sym, sizeof(struct Ake_symbol));
            Ake_symbol_init(new_sym);
            new_sym->type = Ake_symbol_type_variable;
            new_sym->tu = Ake_type_use_clone(type_node->tu);
            Ake_environment_put(ps->st->top, &id_node->value, new_sym);
            id_node->sym = new_sym;
            /* copy is_mut from id node to type use node */
            new_sym->tu->is_mut = id_node->is_mut;
            new_sym->tu->original_is_mut = id_node->is_mut;
        }
    }
}

void Ake_declare_type(struct Ake_parse_state* ps, Ake_ast* type_node, Ake_ast* id_node)
{
    if (type_node && type_node->type != Ake_ast_type_error) {
        if (id_node) {
            if (id_node->type == Ake_ast_type_id) {
                Ake_create_variable_symbol(ps, type_node, id_node);
            } else if (id_node->type == Ake_ast_type_let_lseq) {
                Ake_ast* p = id_node->head;
                while (p) {
                    Ake_create_variable_symbol(ps, type_node, p);
                    p = p->next;
                }
            } else {
                assert(false);
            }
        }
    }
}

Ake_type_use* Ake_proto2type_use(struct Ake_symbol_table* st, Ake_ast* proto, Ake_ast* struct_type) {
    Ake_type_use *func = NULL;
    Ake_type_use_create(&func);
    return Ake_Type_use_add_proto(st, func, proto, struct_type);
}

Ake_type_use* Ake_Type_use_add_proto(
    struct Ake_symbol_table* st,
    Ake_type_use* func,
    Ake_ast* proto,
    Ake_ast* struct_type)
{
    struct Zinc_string bf;
    Zinc_string_init(&bf);
    Zinc_string_add_str(&bf, "Function");
    struct Ake_symbol* sym = Ake_environment_get(st->top, &bf);
    Zinc_string_destroy(&bf);
    assert(sym);
    assert(sym->td);
    func->td = sym->td;

    Ake_ast* id = Ast_node_get(proto, 0);
    Ake_ast* dseq = Ast_node_get(proto, 1);
    Ake_ast* dret = Ast_node_get(proto, 2);

    Zinc_string_copy(&id->value, &func->name);

    if (dseq->head) {
        Ake_type_use* inputs = NULL;
        Ake_type_use_create(&inputs);
        inputs->type = Ake_type_use_function_inputs;
        Ake_type_use_add(func, inputs);

        Ake_ast* dec = dseq->head;
        while (dec) {
            Ake_ast* id_node = Ast_node_get(dec, 0);
            Ake_ast* type_node = Ast_node_get(dec, 1);
            if (dec->type == Ake_ast_type_self) {
                if (struct_type) {
                    type_node = struct_type;
                } else {
                    dec = dec->next;
                    continue;
                }
            }

            Ake_type_use* tu2;
            if (dec->type == Ake_ast_type_ellipsis) {
                Ake_type_use_create(&tu2);
                tu2->type = Ake_type_use_function_ellipsis;
            } else {
                tu2 = Ake_type_use_clone(type_node->tu);
                Zinc_string_copy(&id_node->value, &tu2->name);
            }

            Ake_type_use_add(inputs, tu2);

            dec = dec->next;
        }
    }

    Ake_ast* ret_type_node = Ast_node_get(dret, 0);
    if (ret_type_node) {
        Ake_type_use* outputs = NULL;
        Ake_type_use_create(&outputs);
        outputs->type = Ake_type_use_function_outputs;
        Ake_type_use* tu = Ake_type_use_clone(ret_type_node->tu);
        Ake_type_use_add(outputs, tu);
        Ake_type_use_add(func, outputs);
    }

    return func;
}

bool Ake_check_return_type(struct Ake_parse_state* ps, Ake_ast* proto, Ake_ast* stmts_node, struct Zinc_location* loc)
{
    bool valid = true;

    if (proto->type != Ake_ast_type_error && stmts_node->type != Ake_ast_type_error) {
        Ake_ast *dret = Ast_node_get(proto, 2);
        Ake_ast *ret_type = Ast_node_get(dret, 0);
        if (ret_type) {
            if (!Ake_type_use_can_cast(ret_type->tu, stmts_node->tu)) {
                valid = Zinc_error_list_set(ps->el, loc, "returned type does not match function return type");
            }
        }
    }

    return valid;
}

void Ake_get_function_children(Ake_type_use* func, Ake_type_use** inputs, Ake_type_use** outputs)
{
    *inputs = NULL;
    *outputs = NULL;

    Ake_type_use* p = func->head;
    while (p) {
        if (p->type == Ake_type_use_function_inputs) {
            *inputs = p;
        } else if (p->type == Ake_type_use_function_outputs) {
            *outputs = p;
        }
        p = p->next;
    }
}

Ake_type_use* Ake_get_function_input_type(Ake_type_use* func, int index)
{
	Ake_type_use* inputs = NULL;
	Ake_type_use* outputs = NULL;
	Ake_get_function_children(func, &inputs, &outputs);

	if (!inputs) return NULL;

	Ake_type_use* p = inputs->head;
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
    struct Ake_parse_state* ps,
    Ake_type_use* func,
    int index,
    Ake_ast* a)
{
	bool valid = true;

	if (func) {
		Ake_type_use* tu0 = Ake_get_function_input_type(func, index);
		if (tu0) {
			Ake_type_use* call_tu0 = a->tu;
			if (call_tu0) {
				if (!Ake_type_use_can_cast(tu0, call_tu0)) {
					valid = Zinc_error_list_set(ps->el, &a->loc, "parameter and aguments types do not match");
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
void Ake_Override_rhs(Ake_type_use* tu, Ake_ast* rhs)
{
    if (tu->td->type == Ake_type_integer || tu->td->type == Ake_type_float) {
        rhs->tu->td = tu->td;
        if (rhs->type == Ake_ast_type_sign) {
            Ake_ast* p = Ast_node_get(rhs, 1);
            Ake_Override_rhs(tu, p);
        } else if (tu->is_array && rhs->type == Ake_ast_type_array_literal) {
            Ake_ast* p = rhs->head;
            while (p) {
                Ake_Override_rhs(tu, p);
                p = p->next;
            }
        }
    }
}

bool Ake_is_lvalue(enum Ake_ast_type type)
{
    if (type == Ake_ast_type_id) {
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

bool Ake_check_lvalue(struct Ake_parse_state* ps, Ake_ast* n, struct Zinc_location* loc)
{
    Ake_ast* p = n;
    struct Ake_symbol* sym = NULL;
    Ake_ast* first = NULL;
    while (p) {
        if (!Ake_is_lvalue(p->type)) {
            Zinc_error_list_set(ps->el, loc, "invalid lvalue");
            return false;
        }
        if (!p->head) {
            assert(p->tu);
            if (p->type != Ake_ast_type_id) {
                Zinc_error_list_set(ps->el, loc, "invalid lvalue");
                return false;
            }
            sym = Ake_environment_get(ps->st->top, &p->value);
            first = p;
        }
        p = p->head;
    }

    if (!n->tu) {
        Zinc_error_list_set(ps->el, loc, "invalid lvalue");
        return false;
    }

    if (!sym) {
        Zinc_error_list_set(ps->el, loc, "invalid lvalue");
        return false;
    }

    if (n->type != Ake_ast_type_error) {
        if (n->type == Ake_ast_type_array_subscript) {
            Ake_ast* left = n->head;
            if (left->tu->is_array) {
                Ake_type_dimension* type_dim = (Ake_type_dimension*)VECTOR_PTR(&left->tu->dim, 0);
                if (type_dim->option == Ake_array_element_const) {
                    Zinc_error_list_set(ps->el, loc, "immutable variable changed in assignment");
                }
            }

        }
    }
    if (n->type != Ake_ast_type_error) {
        if (!first->tu->is_mut && sym->assign_count >= 1) {
            Zinc_error_list_set(ps->el, loc, "immutable variable changed in assignment");
        }
    }

    return true;
}

bool Ake_is_placeholder_node(Ake_ast* n)
{
    if (n->type == Ake_ast_type_id && Zinc_string_compare_str(&n->value, "_")) {
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