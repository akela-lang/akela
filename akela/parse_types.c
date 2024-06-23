#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "lex.h"
#include "parse_tools.h"
#include "ast.h"
#include "symbol_table.h"
#include "zinc/memory.h"
#include "parse_types.h"
#include "type_def.h"
#include <assert.h>

void parse_tseq(struct parse_state* ps, Ast_node* parent, struct location* loc);
bool token_is_type(struct parse_state* ps, struct token* t);

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
        bool require_param_name,
        bool* has_id,
        struct location* loc)
{
    Ast_node* n = NULL;

    get_location(ps, loc);

    Ast_node_create(&n);
    n->type = Ast_type_prototype;

    /* 0 id */
    Ast_node* id_node = NULL;
    struct token* t0 = get_lookahead(ps);
    if (t0->type == token_id) {
        struct token* id = NULL;
        if (!match(ps, token_id, "expected identifier", &id)) {
            assert(false);
        }
        Ast_node_create(&id_node);
        id_node->type = Ast_type_id;
        buffer_copy(&id->value, &id_node->value);
        Ast_node_add(n, id_node);
        token_destroy(id);
        free(id);
        consume_newline(ps);
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
    if (!match(ps, token_left_paren, "expected left parenthesis", &lp)) {
        /* test case: no test case needed */
        n->type = Ast_type_error;
    }
    token_destroy(lp);
    free(lp);

    consume_newline(ps);

    /* 1 dseq */
    Ast_node* dseq_node = NULL;
    struct location loc_dseq;
    dseq_node = parse_dseq(ps, require_param_name, is_extern, &loc_dseq);
    if (dseq_node && dseq_node->type == Ast_type_error) {
        n->type = Ast_type_error;
    }

    if (dseq_node) {
        Ast_node_add(n, dseq_node);
    }

    if (!consume_newline(ps)) {
        n->type = Ast_type_error;
    }

    struct token* rp = NULL;
    if (!match(ps, token_right_paren, "expected right parenthesis", &rp)) {
        /* test case: test_parse_anonymous_function_expected_right_paren */
        n->type = Ast_type_error;
    }
    token_destroy(rp);
    free(rp);

    /* 2 ret */
    t0 = get_lookahead(ps);
    Ast_node* ret_type = NULL;
    struct location ret_loc;
    location_init(&ret_loc);
    if (t0 && t0->type == token_arrow) {
        struct token* dc = NULL;
        if (!match(ps, token_arrow, "expecting ->", &dc)) {
            /* test case: no test case needed */
            n->type = Ast_type_error;
        }
        token_destroy(dc);
        free(dc);

        if (!consume_newline(ps)) {
            n->type = Ast_type_error;
        }

        ret_type = parse_type(ps, &ret_loc);
        if (ret_type && ret_type->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (!ret_type) {
            error_list_set(ps->el, &ret_loc, "expected a type");
            n->type = Ast_type_error;
        }
    }

    Ast_node* ret = NULL;
    Ast_node_create(&ret);
    ret->type = Ast_type_dret;
    ret->loc = ret_loc;
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
void declare_params(struct parse_state* ps, Ast_node* proto)
{
    Ast_node* dseq = Ast_node_get(proto, 1);
    Ast_node* dec = dseq->head;
    while (dec) {
        Ast_node* id_node = Ast_node_get(dec, 0);
        Ast_node* type_node = Ast_node_get(dec, 1);
        declare_type(ps, type_node, id_node);
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
Ast_node* parse_dseq(
        struct parse_state* ps,
        bool require_param_name,
        bool is_extern,
        struct location* loc)
{
    get_location(ps, loc);

	Ast_node* n = NULL;
    Ast_node_create(&n);
	n->type = Ast_type_dseq;

	Ast_node* dec = NULL;
	struct location loc_dec;
	dec = parse_declaration(ps, false, require_param_name, &loc_dec);
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
		if (!match(ps, token_comma, "expecting comma", &comma)) {
            assert(false);
            /* test case: no test case needed */
        }

		token_destroy(comma);
		free(comma);

        consume_newline(ps);

        struct token* t = get_lookahead(ps);
        if (t->type == token_ellipsis) {
            struct token* eps = NULL;
            if (!match(ps, token_ellipsis, "expected ellipsis", &eps)) {
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

		dec = parse_declaration(ps, require_param_name, false, &loc_dec);
        if (dec && dec->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

		if (!dec) {
			error_list_set(ps->el, &loc_dec, "expected declaration after comma");
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
        bool require_param_name,
        struct location* loc)
{
	Ast_node* n = NULL;

    get_location(ps, loc);

	struct token* t0 = get_lookahead(ps);
    bool type_only = !require_param_name && token_is_type(ps, t0);
    if (t0->type == token_id || type_only) {
        Ast_node_create(&n);
        n->type = Ast_type_declaration;

        Ast_node* id_node = NULL;
        Ast_node_create(&id_node);
        id_node->type = Ast_type_id;
        Ast_node_add(n, id_node);

        if (!type_only) {
            struct token *id = NULL;
            if (!match(ps, token_id, "expected id", &id)) {
                assert(false);
                /* test case: no test case needed */
            }
            buffer_copy(&id->value, &id_node->value);
            token_destroy(id);
            free(id);

            consume_newline(ps);

            struct token *dc = NULL;
            if (!match(ps, token_colon, "expected colon", &dc)) {
                /* test case: test_parse_error_declaration_double_colon */
                n->type = Ast_type_error;
            }
            token_destroy(dc);
            free(dc);

            consume_newline(ps);
        }

        Ast_node* type_use = NULL;
        struct location loc_type;
        if (add_symbol) {
            type_use = parse_type(ps, &loc_type);
            declare_type(ps, type_use, id_node);
        } else {
            type_use = parse_type(ps, &loc_type);
        }
        if (type_use && type_use->type == Ast_type_error) {
            n->type = Ast_type_error;
        }

        if (!type_use) {
            error_list_set(ps->el, &loc_type, "expected a type");
            /* test case: test_parse_error_declaration_type */
            n->type = Ast_type_error;
        }

        if (type_use) {
            Ast_node_add(n, type_use);
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
Ast_node* parse_type(struct parse_state* ps, struct location* loc)
{
	Ast_node* n = NULL;

	get_location(ps, loc);

    /* handle array dimensions */
	struct token* t0 = get_lookahead(ps);
    while (t0->type == token_left_square_bracket) {
        struct token *lsb = NULL;
        if (!match(ps, token_left_square_bracket, "expected left square bracket", &lsb)) {
            /* test case: no test case needed */
            assert(false);
        }

        token_destroy(lsb);
        free(lsb);

        if (!n) {
            Ast_node_create(&n);
            n->type = Ast_type_type;
        }

        bool has_number = false;
        bool has_const = false;
        size_t dim_size_number;
        t0 = get_lookahead(ps);

        if (t0->type == token_number) {
            struct token *dim_size = NULL;
            if (!match(ps, token_number, "expected number", &dim_size)) {
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
            if (!match(ps, token_const, "expected const", &const_token)) {
                /* test case: no test case needed */
                assert(false);
            }
            has_const = true;
            token_destroy(const_token);
            free(const_token);
        }

        struct token* rsb = NULL;
        if (!match(ps, token_right_square_bracket, "expected right square bracket", &rsb)) {
            n->type = Ast_type_error;
        }

        token_destroy(rsb);
        free(rsb);

        if (has_number) {
            n->to.is_array = true;
            Type_dimension dim;
            dim.size =  dim_size_number;
            if (has_const) {
                dim.option = Array_element_const;
            } else {
                dim.option = Array_element_default;
            }
            VectorAdd(&n->to.dim, &dim, 1);
        } else {
            n->to.is_slice = true;
            Type_dimension dim;
            dim.size = 0;
            if (has_const) {
                dim.option = Array_element_const;
            } else {
                dim.option = Array_element_default;
            }
            VectorAdd(&n->to.dim, &dim, 1);
        }

        t0 = get_lookahead(ps);
    }

    if (!n) {
        Ast_node_create(&n);
        n->type = Ast_type_type;
    }

    if (t0->type == token_fn) {
        struct token* fn = NULL;
        if (!match(ps, token_fn, "expected fn", &fn)) {
            assert(false);
        }

        token_destroy(fn);
        free(fn);

        struct location fn_loc;
        bool has_id;
        Ast_node* proto = parse_prototype(ps, false, false, false, &has_id, &fn_loc);
        if (has_id) {
            error_list_set(ps->el, &fn_loc, "function type has name");
        }
        if (proto->type == Ast_type_error) {
            n->type = Ast_type_error;
        } else {
            struct buffer name;
            buffer_init(&name);
            buffer_copy_str(&name, "Function");
            struct symbol* sym = environment_get(ps->st->top, &name);
            assert(sym);
            n->td = sym->td;
            Ast_node_add(n, proto);

            buffer_destroy(&name);
        }

    } else if (t0->type == token_id) {
        /* handle type or array element */
        struct token* name = NULL;
        if (!match(ps, t0->type, "expected type identifier", &name)) {
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
                    n->td = sym->td;
                }
            }
        }

        token_destroy(name);
        free(name);

    } else {
        t0 = get_lookahead(ps);
        if (t0->type != token_fn && t0->type != token_id) {
            error_list_set(ps->el, &t0->loc, "expected type identifier or fn");
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
void check_id_node(struct parse_state* ps, Ast_node* n, Ast_node* id_node)
{
    struct symbol* dup = environment_get_local(ps->st->top, &id_node->value);
    if (dup) {
        char* a;
        buffer2array(&id_node->value, &a);
        error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", a);
        free(a);
        n->type = Ast_type_error;
        /* test case: test_parse_error_duplicate_declarations */
    } else {
        struct symbol* sym2 = environment_get(ps->st->top, &id_node->value);
        if (sym2 && sym2->td) {
            char* a;
            buffer2array(&id_node->value, &a);
            error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", a);
            free(a);
            n->type = Ast_type_error;
            /* test case: test_parse_types_reserved_type */
        } else {
            struct symbol* new_sym = NULL;
            malloc_safe((void**)&new_sym, sizeof(struct symbol));
            symbol_init(new_sym);
            new_sym->tk_type = token_id;
            new_sym->tu = Ast_node_copy(n);
            environment_put(ps->st->top, &id_node->value, new_sym);
            id_node->sym = new_sym;
            /* copy is_mut from id node to type use node */
            new_sym->tu->to.is_mut = id_node->to.is_mut;
            new_sym->tu->to.original_is_mut = id_node->to.is_mut;
        }
    }
}

void declare_type(struct parse_state* ps, Ast_node* n, Ast_node* id_node)
{
    if (n && n->type != Ast_type_error) {
        if (id_node) {
            if (id_node->type == Ast_type_id) {
                check_id_node(ps, n, id_node);
            } else if (id_node->type == Ast_type_let_lseq) {
                Ast_node* p = id_node->head;
                while (p) {
                    check_id_node(ps, n, p);
                    p = p->next;
                }
            } else {
                assert(false);
            }
        }
    }
}

/* tseq -> type tseq' */
/* tseq' -> , tseq' */
void parse_tseq(struct parse_state* ps, Ast_node* parent, struct location* loc)
{
	Ast_node* tu = NULL;
	struct token* t0 = NULL;

	get_location(ps, loc);

	struct location loc_type;
    tu = parse_type(ps, &loc_type);
	if (tu && tu->type == Ast_type_error) {
        parent->type = Ast_type_error;
    }

	if (!tu) {
		error_list_set(ps->el, &loc_type, "expected a type name");
		/* test case: test_parse_error_type_name */
        parent->type = Ast_type_error;
	}

	if (tu) {
		assert(tu);
        Ast_node_add(parent, tu);
		tu = NULL;
	}

	while (true) {
		t0 = get_lookahead(ps);

		if (!t0 || t0->type != token_comma) {
			break;
		}

		struct token* comma = NULL;
		if (!match(ps, token_comma, "expected comma", &comma)) {
            /* test case: test case not needed */
            assert(false);
        }

		token_destroy(comma);
		free(comma);

        consume_newline(ps);

        tu = parse_type(ps, &loc_type);
		if (tu && tu->type == Ast_type_error) {
            parent->type = Ast_type_error;
        }

		if (!tu) {
			error_list_set(ps->el, &loc_type, "expected a type name after comma");
			/* test case: test_parse_error_comma_type_name */
            parent->type = Ast_type_error;
			break;
		}

		if (parent->type != Ast_type_error) {
			assert(tu);
            Ast_node_add(parent, tu);
			tu = NULL;
		}

	}
}

Ast_node* proto2type(struct symbol_table* st, Ast_node* proto)
{
	Ast_node* tu = NULL;
    Ast_node_create(&tu);
	tu->type = Ast_type_type;

    struct buffer bf;
	buffer_init(&bf);
	buffer_copy_str(&bf, "Function");
	struct symbol* sym = environment_get(st->top, &bf);
	assert(sym);
	assert(sym->td);
	tu->td = sym->td;

    Ast_node_add(tu, Ast_node_copy(proto));

	buffer_destroy(&bf);

	return tu;
}

bool check_return_type(struct parse_state* ps, Ast_node* proto, Ast_node* stmts_node, struct location* loc)
{
    bool valid = true;

    if (proto->type != Ast_type_error && stmts_node->type != Ast_type_error) {
        Ast_node *dret = Ast_node_get(proto, 2);
        Ast_node *ret_type = Ast_node_get(dret, 0);
        if (ret_type) {
            if (!type_use_can_cast(ret_type, stmts_node->tu)) {
                valid = error_list_set(ps->el, loc, "returned type does not match function return type");
            }
        }
    }

    return valid;
}

void get_function_children(Ast_node* tu, Ast_node** dseq, Ast_node** dret)
{
    Ast_node* proto = Ast_node_get(tu, 0);
    *dseq = Ast_node_get(proto, 1);
    *dret = Ast_node_get(proto, 2);
}

Ast_node* get_function_type(struct symbol* sym)
{
	if (sym) {
		if (sym->tu) {
			if (sym->tu->td) {
				if (sym->tu->td->type == type_function) {
					return sym->tu;
				}
			}
		}
	}

	return NULL;
}

Ast_node* get_function_input_type(Ast_node* tu, int index)
{
	Ast_node* dseq = NULL;
	Ast_node* dret = NULL;
	get_function_children(tu, &dseq, &dret);

	if (!dseq) return NULL;

	Ast_node* p = dseq->head;
	int i = 0;
	while (p) {
		if (i == index) {
            Ast_node* dec = p;
            Ast_node* dec_tu = Ast_node_get(dec, 1);
            return dec_tu;
        }
		p = p->next;
		i++;
	}
	return NULL;
}

bool check_input_type(struct parse_state* ps, Ast_node* tu, int index, Ast_node* a, struct location* loc_expr)
{
	bool valid = true;

	if (tu) {
		Ast_node* tu0 = get_function_input_type(tu, index);
		if (tu0) {
			Ast_node* call_tu0 = a->tu;
			if (call_tu0) {
				if (!type_use_can_cast(tu0, call_tu0)) {
					valid = error_list_set(ps->el, loc_expr, "parameter and aguments types do not match");
					/* test case: test_parse_types_error_param */
				}
			} else {
				valid = error_list_set(ps->el, loc_expr, "argument expression has no value");
				/* test case: test_parse_types_error_param_no_value */
			}
		}
	}

	return valid;
}

Ast_node* make_constructor(struct type_def* td)
{
	Ast_node* tu = td->composite;

    /* function */
	Ast_node* n = NULL;
    Ast_node_create(&n);
	n->type = Ast_type_function;

    Ast_node* proto = NULL;
    Ast_node_create(&proto);
    proto->type = Ast_type_prototype;

	/* id */
	Ast_node* id = NULL;
    Ast_node_create(&id);
	id->type = Ast_type_id;
	buffer_copy(&td->name, &id->value);
    Ast_node_add(proto, id);

	/* dseq */
	Ast_node* dseq = NULL;
    Ast_node_create(&dseq);
	dseq->type = Ast_type_dseq;
	Ast_node* p = tu->head;
	while (p) {
		Ast_node* dec = Ast_node_copy(p);
        Ast_node_add(dseq, dec);
		p = p->next;
	}
    Ast_node_add(proto, dseq);

	/* dret */
	Ast_node* dret = NULL;
    Ast_node_create(&dret);
	dret->type = Ast_type_dret;
	Ast_node* dret_tu = NULL;
    Ast_node_create(&dret_tu);
	dret_tu->type = Ast_type_type;
	dret_tu->td = td;
    Ast_node_add(dret, dret_tu);
    Ast_node_add(proto, dret);

    /* proto */
    Ast_node_add(n, proto);

	/* stmts */
	Ast_node* stmts = NULL;
    Ast_node_create(&stmts);
	stmts->type = Ast_type_stmts;
    Ast_node_add(n, stmts);

	/* let */
	Ast_node* let = NULL;
    Ast_node_create(&let);
	let->type = Ast_type_let;
    Ast_node_add(stmts, let);

	Ast_node* dec = NULL;
    Ast_node_create(&dec);
	dec->type = Ast_type_declaration;
    Ast_node_add(let, dec);

	Ast_node* id2 = NULL;
    Ast_node_create(&id2);
	id2->type = Ast_type_id;
	buffer_copy_str(&id2->value, "__x");
    Ast_node_add(dec, id2);

	Ast_node* t = NULL;
    Ast_node_create(&t);
	t->type = Ast_type_type;
	t->td = td;
    Ast_node_add(dec, t);

	/* assignments */
	p = tu->head;
	while (p) {
		Ast_node* assign = NULL;
        Ast_node_create(&assign);
		assign->type = Ast_type_assign;
        Ast_node_add(stmts, assign);

		Ast_node* dot = NULL;
        Ast_node_create(&dot);
		dot->type = Ast_type_type;
        Ast_node_add(assign, dot);

		Ast_node* x = NULL;
        Ast_node_create(&x);
		x->type = Ast_type_id;
		buffer_copy_str(&x->value, "__x");
        Ast_node_add(dot, x);

		Ast_node* field = NULL;
        Ast_node_create(&field);
		field->type = Ast_type_id;
		Ast_node* id3 = Ast_node_get(p, 0);
		buffer_copy(&id3->value, &field->value);
        Ast_node_add(dot, field);

		p = p->next;
	}

	/* return __x */
	Ast_node* id4 = NULL;
    Ast_node_create(&id4);
	id4->type = Ast_type_id;
	buffer_copy_str(&id4->value, "__x");
    Ast_node_add(stmts, id4);

	return n;
}

/* NOLINTNEXTLINE(misc-no-recursion) */
void Override_rhs(Ast_node* tu, Ast_node* rhs)
{
    if (tu->td->type == type_integer || tu->td->type == type_float) {
        rhs->tu->td = tu->td;
        if (rhs->type == Ast_type_sign) {
            Ast_node* p = Ast_node_get(rhs, 1);
            Override_rhs(tu, p);
        } else if (tu->to.is_array && rhs->type == Ast_type_array_literal) {
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

    if (!n->tu->to.is_mut && sym->assign_count >= 1) {
        error_list_set(ps->el, loc, "immutable variable changed in assignment");
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