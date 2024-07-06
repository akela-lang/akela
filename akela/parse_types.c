#include <stdbool.h>
#include "zinc/buffer.h"
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

bool token_is_type(struct parse_state* ps, struct token* t);
Type_use* Type_use_add_proto(
        struct symbol_table* st,
        Type_use* func,
        Ast_node* proto,
        Ast_node* struct_type);

/**
 * Parse a function prototype
 * @param ps
 * @param has_id
 * @param loc
 * @return struct ast_node*
 */
/* NOLINTNEXTLINE(misc-no-recursion) */
Ast_node* parse_prototype(
        struct parse_state* ps,
        bool is_function,
        bool is_extern,
        bool is_method,
        bool require_param_name,
        bool* has_id)
{
    Ast_node* n = NULL;

    Ast_node_create(&n);
    n->type = Ast_type_prototype;

    /* 0 id */
    Ast_node* id_node = NULL;
    struct token* t0 = get_lookahead(ps);
    if (t0->type == token_id) {
        struct token* id = NULL;
        if (!match(ps, token_id, "expected identifier", &id, n)) {
            assert(false);
        }
        Ast_node_create(&id_node);
        id_node->type = Ast_type_id;
        buffer_copy(&id->value, &id_node->value);
        Ast_node_add(n, id_node);
        token_destroy(id);
        free(id);
        consume_newline(ps, n);
        *has_id = true;
    } else {
        Ast_node_create(&id_node);
        id_node->type = Ast_type_id;
        if (is_function) {
            buffer_add_format(
                    &id_node->value,
                    "__anonymous_function_%zu",
                    symbol_table_generate_id(ps->st)
            );
        }
        Ast_node_add(n, id_node);
        *has_id = false;
    }

    struct token* lp = NULL;
    if (!match(ps, token_left_paren, "expected left parenthesis", &lp, n)) {
        /* test case: no test case needed */
        n->type = Ast_type_error;
    }
    token_destroy(lp);
    free(lp);

    consume_newline(ps, n);

    /* 1 dseq */
    Ast_node* dseq_node = NULL;
    dseq_node = parse_dseq(ps, require_param_name, is_extern, is_method);
    if (dseq_node && dseq_node->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

    if (dseq_node) {
        Ast_node_add(n, dseq_node);
    }

    if (!consume_newline(ps, n)) {
        n->type = Ast_type_error;
    }

    struct token* rp = NULL;
    if (!match(ps, token_right_paren, "expected right parenthesis", &rp, n)) {
        /* test case: test_parse_anonymous_function_expected_right_paren */
        n->type = Ast_type_error;
    }
    token_destroy(rp);
    free(rp);

    /* 2 ret */
    t0 = get_lookahead(ps);
    Ast_node* ret_type = NULL;
    if (t0 && t0->type == token_arrow) {
        struct token* dc = NULL;
        if (!match(ps, token_arrow, "expecting ->", &dc, n)) {
            /* test case: no test case needed */
            n->type = Ast_type_error;
        }
        token_destroy(dc);
        free(dc);

        if (!consume_newline(ps, n)) {
            n->type = Ast_type_error;
        }

        ret_type = parse_type(ps);
        if (ret_type && ret_type->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (!ret_type) {
            struct location ret_loc = get_location(ps);
            error_list_set(ps->el, &ret_loc, "expected a type");
            n->type = Ast_type_error;
        }
    }

    Ast_node* ret = NULL;
    Ast_node_create(&ret);
    ret->type = Ast_type_dret;
    if (ret_type) {
        Ast_node_add(ret, ret_type);
    }

    Ast_node_add(n, ret);

    return n;
}

/**
 * Declare the parameters of a prototype
 * @param ps
 * @param proto
 */
void declare_params(struct parse_state* ps, Ast_node* proto, Ast_node* struct_type)
{
    Ast_node* dseq = Ast_node_get(proto, 1);
    Ast_node* dec = dseq->head;
    while (dec) {
        Ast_node* id_node = Ast_node_get(dec, 0);
        Ast_node* type_node = Ast_node_get(dec, 1);
        if (dec->type == Ast_type_self) {
            if (struct_type) {
                type_node = struct_type;
            } else {
                dec = dec->next;
                continue;
            }
        }
        if (dec->type != Ast_type_error && type_node->type != Ast_type_error) {
            declare_type(ps, type_node, id_node);
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
Ast_node* parse_dseq(
        struct parse_state* ps,
        bool require_param_name,
        bool is_extern,
        bool is_method)
{
	Ast_node* n = NULL;
    Ast_node_create(&n);
	n->type = Ast_type_dseq;

	Ast_node* dec = NULL;
	dec = parse_declaration(ps, false, is_method, require_param_name);
    if (dec && dec->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

	if (!dec) {
		return n;
	}

    Ast_node_add(n, dec);

	while (true)
	{
		struct token* t0 = get_lookahead(ps);
		if (!t0 || t0->type != token_comma) {
			break;
		}

		struct token* comma = NULL;
		if (!match(ps, token_comma, "expecting comma", &comma, n)) {
            assert(false);
            /* test case: no test case needed */
        }

		token_destroy(comma);
		free(comma);

        consume_newline(ps, n);

        struct token* t = get_lookahead(ps);
        if (t->type == token_ellipsis) {
            struct token* eps = NULL;
            if (!match(ps, token_ellipsis, "expected ellipsis", &eps, n)) {
                assert(false);
            }
            token_destroy(eps);
            free(eps);
            if (is_extern) {
                Ast_node* ellipsis = NULL;
                Ast_node_create(&ellipsis);
                ellipsis->type = Ast_type_ellipsis;
                Ast_node_add(n, ellipsis);
                break;
            } else {
                error_list_set(ps->el, &eps->loc,
                       "Found ellipsis but variadic functions are only supported in extern declarations");
                n->type = Ast_type_error;
                break;
            }
        }

		dec = parse_declaration(ps, false, is_method, require_param_name);
        if (dec && dec->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

		if (!dec) {
            struct location dec_loc = get_location(ps);
			error_list_set(ps->el, &dec_loc, "expected declaration after comma");
			/* test case: test_parse_error_dseq_comma */
            n->type = Ast_type_error;
			break;
		}

        Ast_node_add(n, dec);
	}

	return n;
}

/**
 * Is the token the start of a type
 * @param ps
 * @param t
 * @return true if a type, otherwise false
 */
bool token_is_type(struct parse_state* ps, struct token* t)
{
    /* array type */
    if (t->type == token_left_square_bracket) {
        return true;
    }

    /* function type */
    if (t->type == token_fn) {
        return true;
    }

    /* type id */
    if (t->type == token_id) {
        struct symbol* sym = environment_get(ps->st->top, &t->value);
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
Ast_node* parse_declaration(
        struct parse_state* ps,
        bool add_symbol,
        bool is_method,
        bool require_param_name)
{
	Ast_node* n = NULL;

	struct token* t0 = get_lookahead(ps);
    bool type_only = !require_param_name && token_is_type(ps, t0);
    bool is_self = is_method && t0->type == token_self;
    if (t0->type == token_id || type_only || is_self) {
        Ast_node_create(&n);
        n->type = Ast_type_declaration;

        if (is_self) {
            n->type = Ast_type_self;
            struct token *self = NULL;
            if (!match(ps, token_self, "expected self", &self, n)) {
                /* test case: no test case needed */
                assert(false);
            }

            Ast_node* id_node = NULL;
            Ast_node_create(&id_node);
            id_node->type = Ast_type_id;
            buffer_copy_str(&id_node->value, "self");
            Ast_node_add(n, id_node);

            token_destroy(self);
            free(self);

        } else if (type_only) {
            Ast_node* id_node = NULL;
            Ast_node_create(&id_node);
            id_node->type = Ast_type_id;
            Ast_node_add(n, id_node);

            Ast_node* type_use = NULL;
            type_use = parse_type(ps);
            if (type_use && type_use->type == Ast_type_error) {
                n->type = Ast_type_error;
            }

            if (!type_use) {
                struct location type_use_loc = get_location(ps);
                error_list_set(ps->el, &type_use_loc, "expected a type");
                /* test case: test_parse_error_declaration_type */
                n->type = Ast_type_error;
            }

            if (type_use) {
                Ast_node_add(n, type_use);
            }
        } else if (t0->type == token_id) {
            Ast_node* id_node = NULL;
            Ast_node_create(&id_node);
            id_node->type = Ast_type_id;
            Ast_node_add(n, id_node);

            struct token *id = NULL;
            if (!match(ps, token_id, "expected id", &id, n)) {
                assert(false);
                /* test case: no test case needed */
            }
            buffer_copy(&id->value, &id_node->value);
            token_destroy(id);
            free(id);

            consume_newline(ps, n);

            struct token *dc = NULL;
            if (!match(ps, token_colon, "expected colon", &dc, n)) {
                /* test case: test_parse_error_declaration_double_colon */
                n->type = Ast_type_error;
            }
            token_destroy(dc);
            free(dc);

            consume_newline(ps, n);

            Ast_node* type_use = NULL;
            type_use = parse_type(ps);
            if (type_use && type_use->type == Ast_type_error) {
                n->type = Ast_type_error;
            }
            if (add_symbol) {
                if (n->type != Ast_type_error) {
                    declare_type(ps, type_use, id_node);
                }
            }

            if (!type_use) {
                struct location type_use_loc = get_location(ps);
                error_list_set(ps->el, &type_use_loc, "expected a type");
                /* test case: test_parse_error_declaration_type */
                n->type = Ast_type_error;
            }

            if (type_use) {
                Ast_node_add(n, type_use);
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
Ast_node* parse_type(struct parse_state* ps)
{
	Ast_node* n = NULL;
    Ast_node_create(&n);
    n->type = Ast_type_type;

    Type_use* tu = NULL;
    Type_use_create(&tu);
    n->tu = tu;

    /* handle array dimensions */
	struct token* t0 = get_lookahead(ps);
    while (t0->type == token_left_square_bracket) {
        struct token *lsb = NULL;
        if (!match(ps, token_left_square_bracket, "expected left square bracket", &lsb, n)) {
            /* test case: no test case needed */
            assert(false);
        }

        token_destroy(lsb);
        free(lsb);

        bool has_number = false;
        bool has_const = false;
        size_t dim_size_number;
        t0 = get_lookahead(ps);

        if (t0->type == token_number) {
            struct token *dim_size = NULL;
            if (!match(ps, token_number, "expected number", &dim_size, n)) {
                /* test case: no test case needed */
                assert(false);
            }
            has_number = true;
            buffer_finish(&dim_size->value);
            dim_size_number = (size_t) strtol(dim_size->value.buf, NULL, 10);
            token_destroy(dim_size);
            free(dim_size);
            t0 = get_lookahead(ps);
        }

        if (t0->type == token_const) {
            struct token* const_token = NULL;
            if (!match(ps, token_const, "expected const", &const_token, n)) {
                /* test case: no test case needed */
                assert(false);
            }
            has_const = true;
            token_destroy(const_token);
            free(const_token);
        }

        struct token* rsb = NULL;
        if (!match(ps, token_right_square_bracket, "expected right square bracket", &rsb, n)) {
            n->type = Ast_type_error;
        }

        token_destroy(rsb);
        free(rsb);

        if (has_number) {
            n->tu->is_array = true;
            Type_dimension dim;
            dim.size =  dim_size_number;
            if (has_const) {
                dim.option = Array_element_const;
            } else {
                dim.option = Array_element_default;
            }
            VectorAdd(&n->tu->dim, &dim, 1);
        } else {
            n->tu->is_slice = true;
            Type_dimension dim;
            dim.size = 0;
            if (has_const) {
                dim.option = Array_element_const;
            } else {
                dim.option = Array_element_default;
            }
            VectorAdd(&n->tu->dim, &dim, 1);
        }

        t0 = get_lookahead(ps);
    }

    if (t0->type == token_fn) {
        struct token* fn = NULL;
        if (!match(ps, token_fn, "expected fn", &fn, n)) {
            assert(false);
        }

        token_destroy(fn);
        free(fn);

        bool has_id;
        Ast_node* proto = parse_prototype(ps, false, false, false, false, &has_id);
        if (has_id) {
            error_list_set(ps->el, &proto->loc, "function type has name");
        }
        if (proto->type == Ast_type_error) {
            n->type = Ast_type_error;
        } else {
            Type_use_add_proto(ps->st, n->tu, proto, NULL);
        }
        Ast_node_destroy(proto);

    } else if (t0->type == token_id) {
        /* handle type or array element */
        struct token* name = NULL;
        if (!match(ps, t0->type, "expected type identifier", &name, n)) {
            n->type = Ast_type_error;
        }

        struct symbol* sym = NULL;
        if (n->type != Ast_type_error) {
            sym = environment_get(ps->st->top, &name->value);
            if (!sym) {
                char* a;
                buffer2array(&name->value, &a);
                error_list_set(ps->el, &name->loc, "type not defined: %s", a);
                free(a);
                n->type = Ast_type_error;
                /* test case: test_parse_error_type_not_defined */
            } else if (!sym->td) {
                char *a;
                buffer2array(&name->value, &a);
                error_list_set(ps->el, &name->loc, "identifier is not a type: %s", a);
                free(a);
                n->type = Ast_type_error;
                /* test case: test_parse_error_not_a_type */
            } else if (sym->td == ps->st->function_type_def) {
                error_list_set(
                        ps->el,
                        &name->loc,
                        "can not directly use Function to declare a function; use fn syntax to declare a function");
                n->type = Ast_type_error;
            } else {
                if (n->type != Ast_type_error) {
                    n->tu->td = sym->td;
                }
            }
        }

        token_destroy(name);
        free(name);

    } else {
        t0 = get_lookahead(ps);
        if (t0->type != token_fn && t0->type != token_id) {
            error_list_set(ps->el, &t0->loc, "expected type identifier or fn");
            n->type = Ast_type_error;
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
void create_variable_symbol(struct parse_state* ps, Ast_node* type_node, Ast_node* id_node)
{
    struct symbol* dup = environment_get_local(ps->st->top, &id_node->value);
    if (dup) {
        char* a;
        buffer2array(&id_node->value, &a);
        error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", a);
        free(a);
        type_node->type = Ast_type_error;
        /* test case: test_parse_error_duplicate_declarations */
    } else {
        struct symbol* sym2 = environment_get(ps->st->top, &id_node->value);
        if (sym2 && sym2->td) {
            char* a;
            buffer2array(&id_node->value, &a);
            error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", a);
            free(a);
            type_node->type = Ast_type_error;
            /* test case: test_parse_types_reserved_type */
        } else {
            struct symbol* new_sym = NULL;
            malloc_safe((void**)&new_sym, sizeof(struct symbol));
            symbol_init(new_sym);
            new_sym->type = Symbol_type_variable;
            new_sym->tu = Type_use_clone(type_node->tu);
            environment_put(ps->st->top, &id_node->value, new_sym);
            id_node->sym = new_sym;
            /* copy is_mut from id node to type use node */
            new_sym->tu->is_mut = id_node->is_mut;
            new_sym->tu->original_is_mut = id_node->is_mut;
        }
    }
}

void declare_type(struct parse_state* ps, Ast_node* type_node, Ast_node* id_node)
{
    if (type_node && type_node->type != Ast_type_error) {
        if (id_node) {
            if (id_node->type == Ast_type_id) {
                create_variable_symbol(ps, type_node, id_node);
            } else if (id_node->type == Ast_type_let_lseq) {
                Ast_node* p = id_node->head;
                while (p) {
                    create_variable_symbol(ps, type_node, p);
                    p = p->next;
                }
            } else {
                assert(false);
            }
        }
    }
}

Type_use* proto2type_use(struct symbol_table* st, Ast_node* proto, Ast_node* struct_type) {
    Type_use *func = NULL;
    Type_use_create(&func);
    Type_use_add_proto(st, func, proto, struct_type);
}

Type_use* Type_use_add_proto(
    struct symbol_table* st,
    Type_use* func,
    Ast_node* proto,
    Ast_node* struct_type)
{
    struct buffer bf;
    buffer_init(&bf);
    buffer_copy_str(&bf, "Function");
    struct symbol* sym = environment_get(st->top, &bf);
    buffer_destroy(&bf);
    assert(sym);
    assert(sym->td);
    func->td = sym->td;

    Ast_node* id = Ast_node_get(proto, 0);
    Ast_node* dseq = Ast_node_get(proto, 1);
    Ast_node* dret = Ast_node_get(proto, 2);

    buffer_copy(&id->value, &func->name);

    if (dseq->head) {
        Type_use* inputs = NULL;
        Type_use_create(&inputs);
        inputs->type = Type_use_function_inputs;
        Type_use_add(func, inputs);

        Ast_node* dec = dseq->head;
        while (dec) {
            Ast_node* id_node = Ast_node_get(dec, 0);
            Ast_node* type_node = Ast_node_get(dec, 1);
            if (dec->type == Ast_type_self) {
                if (struct_type) {
                    type_node = struct_type;
                } else {
                    dec = dec->next;
                    continue;
                }
            }

            Type_use* tu2;
            if (dec->type == Ast_type_ellipsis) {
                Type_use_create(&tu2);
                tu2->type = Type_use_function_ellipsis;
            } else {
                tu2 = Type_use_clone(type_node->tu);
                buffer_copy(&id_node->value, &tu2->name);
            }

            Type_use_add(inputs, tu2);

            dec = dec->next;
        }
    }

    Ast_node* ret_type_node = Ast_node_get(dret, 0);
    if (ret_type_node) {
        Type_use* outputs = NULL;
        Type_use_create(&outputs);
        outputs->type = Type_use_function_outputs;
        Type_use* tu = Type_use_clone(ret_type_node->tu);
        Type_use_add(outputs, tu);
        Type_use_add(func, outputs);
    }

    return func;
}

bool check_return_type(struct parse_state* ps, Ast_node* proto, Ast_node* stmts_node, struct location* loc)
{
    bool valid = true;

    if (proto->type != Ast_type_error && stmts_node->type != Ast_type_error) {
        Ast_node *dret = Ast_node_get(proto, 2);
        Ast_node *ret_type = Ast_node_get(dret, 0);
        if (ret_type) {
            if (!type_use_can_cast(ret_type->tu, stmts_node->tu)) {
                valid = error_list_set(ps->el, loc, "returned type does not match function return type");
            }
        }
    }

    return valid;
}

void get_function_children(Type_use* func, Type_use** inputs, Type_use** outputs)
{
    *inputs = NULL;
    *outputs = NULL;

    Type_use* p = func->head;
    while (p) {
        if (p->type == Type_use_function_inputs) {
            *inputs = p;
        } else if (p->type == Type_use_function_outputs) {
            *outputs = p;
        }
        p = p->next;
    }
}

Type_use* get_function_input_type(Type_use* func, int index)
{
	Type_use* inputs = NULL;
	Type_use* outputs = NULL;
	get_function_children(func, &inputs, &outputs);

	if (!inputs) return NULL;

	Type_use* p = inputs->head;
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

bool check_input_type(
    struct parse_state* ps,
    Type_use* func,
    int index,
    Ast_node* a)
{
	bool valid = true;

	if (func) {
		Type_use* tu0 = get_function_input_type(func, index);
		if (tu0) {
			Type_use* call_tu0 = a->tu;
			if (call_tu0) {
				if (!type_use_can_cast(tu0, call_tu0)) {
					valid = error_list_set(ps->el, &a->loc, "parameter and aguments types do not match");
					/* test case: test_parse_types_error_param */
				}
			} else {
				valid = error_list_set(ps->el, &a->loc, "argument expression has no value");
				/* test case: test_parse_types_error_param_no_value */
			}
		}
	}

	return valid;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Override_rhs(Type_use* tu, Ast_node* rhs)
{
    if (tu->td->type == type_integer || tu->td->type == type_float) {
        rhs->tu->td = tu->td;
        if (rhs->type == Ast_type_sign) {
            Ast_node* p = Ast_node_get(rhs, 1);
            Override_rhs(tu, p);
        } else if (tu->is_array && rhs->type == Ast_type_array_literal) {
            Ast_node* p = rhs->head;
            while (p) {
                Override_rhs(tu, p);
                p = p->next;
            }
        }
    }
}

bool is_lvalue(enum Ast_type type)
{
    if (type == Ast_type_id) {
        return true;
    }

    if (type == Ast_type_array_subscript) {
        return true;
    }

    if (type == Ast_type_eseq) {
        return true;
    }

    if (type == Ast_type_dot) {
        return true;
    }

    return false;
}

bool check_lvalue(struct parse_state* ps, Ast_node* n, struct location* loc)
{
    Ast_node* p = n;
    struct symbol* sym = NULL;
    Ast_node* first = NULL;
    while (p) {
        if (!is_lvalue(p->type)) {
            error_list_set(ps->el, loc, "invalid lvalue");
            return false;
        }
        if (!p->head) {
            assert(p->tu);
            if (p->type != Ast_type_id) {
                error_list_set(ps->el, loc, "invalid lvalue");
                return false;
            }
            sym = environment_get(ps->st->top, &p->value);
            first = p;
        }
        p = p->head;
    }

    if (!n->tu) {
        error_list_set(ps->el, loc, "invalid lvalue");
        return false;
    }

    if (!sym) {
        error_list_set(ps->el, loc, "invalid lvalue");
        return false;
    }

    if (n->type != Ast_type_error) {
        if (n->type == Ast_type_array_subscript) {
            Ast_node* left = n->head;
            if (left->tu->is_array) {
                Type_dimension* type_dim = (Type_dimension*)VECTOR_PTR(&left->tu->dim, 0);
                if (type_dim->option == Array_element_const) {
                    error_list_set(ps->el, loc, "immutable variable changed in assignment");
                }
            }

        }
    }
    if (n->type != Ast_type_error) {
        if (!first->tu->is_mut && sym->assign_count >= 1) {
            error_list_set(ps->el, loc, "immutable variable changed in assignment");
        }
    }

    return true;
}

bool Is_placeholder_node(Ast_node* n)
{
    if (n->type == Ast_type_id && buffer_compare_str(&n->value, "_")) {
        return true;
    } else {
        return false;
    }
}

bool Is_placeholder_token(struct token* t)
{
    if (t->type == token_id && buffer_compare_str(&t->value, "_")) {
        return true;
    } else {
        return false;
    }
}