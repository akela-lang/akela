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

void parse_tseq(struct parse_state* ps, struct ast_node* parent, struct location* loc);

struct ast_node* parse_prototype(struct parse_state* ps, bool* has_id, struct location* loc)
{
    struct ast_node* n = NULL;

    get_location(ps, loc);

    ast_node_create(&n);
    n->type = ast_type_prototype;

    /* index 0: id */
    struct ast_node* id_node = NULL;
    struct token* t0 = get_lookahead(ps);
    if (t0->type == token_id) {
        struct token* id = NULL;
        if (!match(ps, token_id, "expected identifier", &id)) {
            assert(false);
        }
        ast_node_create(&id_node);
        id_node->type = ast_type_id;
        buffer_copy(&id->value, &id_node->value);
        ast_node_add(n, id_node);
        token_destroy(id);
        free(id);
        consume_newline(ps);
        *has_id = true;
    } else {
        ast_node_create(&id_node);
        id_node->type = ast_type_id;
        buffer_add_format(&id_node->value, "__anonymous_function_%zu", symbol_table_generate_id(ps->st));
        ast_node_add(n, id_node);
        *has_id = false;
    }

    struct token* lp = NULL;
    if (!match(ps, token_left_paren, "expected left parenthesis", &lp)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }
    token_destroy(lp);
    free(lp);

    consume_newline(ps);

    /* index 1: dseq */
    struct ast_node* dseq_node = NULL;
    struct location loc_dseq;
    dseq_node = parse_dseq(ps, &loc_dseq);
    if (dseq_node && dseq_node->type == ast_type_error) {
        n->type = ast_type_error;
    }

    if (dseq_node) {
        ast_node_add(n, dseq_node);
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

    struct token* rp = NULL;
    if (!match(ps, token_right_paren, "expected right parenthesis", &rp)) {
        /* test case: test_parse_anonymous_function_expected_right_paren */
        n->type = ast_type_error;
    }
    token_destroy(rp);
    free(rp);

    /* index 2: ret */
    t0 = get_lookahead(ps);
    struct ast_node* ret_type = NULL;
    struct location ret_loc;
    location_init(&ret_loc);
    if (t0 && t0->type == token_double_colon) {
        struct token* dc = NULL;
        if (!match(ps, token_double_colon, "expecting double colon", &dc)) {
            /* test case: no test case needed */
            n->type = ast_type_error;
        }
        token_destroy(dc);
        free(dc);

        if (!consume_newline(ps)) {
            n->type = ast_type_error;
        }

        ret_type = parse_type(ps, &ret_loc);
        if (ret_type && ret_type->type == ast_type_error) {
            n->type = ast_type_error;
        }

        if (!ret_type) {
            error_list_set(ps->el, &ret_loc, "expected a type");
            n->type = ast_type_error;
        }
    }

    struct ast_node* ret = NULL;
    ast_node_create(&ret);
    ret->type = ast_type_dret;
    ret->loc = ret_loc;
    ast_node_add(n, ret);

    if (ret_type) {
        ast_node_add(ret, ret_type);
    }

    /* index 3: type */
    struct ast_node* tu = function2type(ps->st, n);
    ast_node_add(n, tu);

    return n;
}

void declare_params(struct parse_state* ps, struct ast_node* proto)
{
    struct ast_node* dseq = ast_node_get(proto, 1);
    struct ast_node* dec = dseq->head;
    while (dec) {
        struct ast_node* id_node = ast_node_get(dec, 0);
        struct ast_node* type_node = ast_node_get(dec, 1);
        declare_type(ps, type_node, id_node);
        dec = dec->next;
    }
}

/* dseq -> declaration dseq' | e */
/* dseq' -> , declaration dseq' | e */
struct ast_node* parse_dseq(struct parse_state* ps, struct location* loc)
{
    get_location(ps, loc);

	struct ast_node* n = NULL;
	ast_node_create(&n);
	n->type = ast_type_dseq;

	struct ast_node* dec = NULL;
	struct location loc_dec;
	dec = parse_declaration(ps, false, &loc_dec);
    if (dec && dec->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (!dec) {
		return n;
	}

    ast_node_add(n, dec);

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

		struct ast_node* dec = NULL;
		struct location loc_dec;
		dec = parse_declaration(ps, false, &loc_dec);
        if (dec && dec->type == ast_type_error) {
            n->type = ast_type_error;
        }

		if (!dec) {
			error_list_set(ps->el, &loc_dec, "expected declaration after comma");
			/* test case: test_parse_error_dseq_comma */
            n->type = ast_type_error;
			break;
		}

		if (dec) {
			ast_node_add(n, dec);
		}
	}

	return n;
}

/* declaration -> id :: type | e */
struct ast_node* parse_declaration(struct parse_state* ps, bool add_symbol, struct location* loc)
{
	struct ast_node* n = NULL;

    get_location(ps, loc);

	struct token* t0 = get_lookahead(ps);

	if (t0 && t0->type == token_id) {
        /* id::type */
        ast_node_create(&n);
        n->type = ast_type_declaration;

		struct token* id = NULL;
		if (!match(ps, token_id, "expected id", &id)) {
            assert(false);
            /* test case: no test case needed */
        }

        struct ast_node* id_node = NULL;
        ast_node_create(&id_node);
        id_node->type = ast_type_id;
        if (id) {
            buffer_copy(&id->value, &id_node->value);
        }

        ast_node_add(n, id_node);

        consume_newline(ps);

		struct token* dc = NULL;
		if (!match(ps, token_double_colon, "expected double colon", &dc)) {
            /* test case: test_parse_error_declaration_double_colon */
            n->type = ast_type_error;
        }

        consume_newline(ps);

		struct ast_node* type_use = NULL;
		struct location loc_type;
        if (add_symbol) {
            type_use = parse_type(ps, &loc_type);
            declare_type(ps, type_use, id_node);
        } else {
            type_use = parse_type(ps, &loc_type);
        }
        if (type_use && type_use->type == ast_type_error) {
            n->type = ast_type_error;
        }

		if (!type_use) {
			error_list_set(ps->el, &loc_type, "expected a type");
			/* test case: test_parse_error_declaration_type */
            n->type = ast_type_error;
		}

        if (type_use) {
            ast_node_add(n, type_use);
        }

		token_destroy(id);
		free(id);

		token_destroy(dc);
		free(dc);
	}

	return n;
}

/* type -> id | id { tseq } */
struct ast_node* parse_type(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
	bool is_generic = false;

	get_location(ps, loc);

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
            ast_node_create(&n);
            n->type = ast_type_type;
        }

        t0 = get_lookahead(ps);
        if (t0->type != token_right_square_bracket) {
            if (t0->type == token_number || t0->type == token_mult) {
                struct token *dim_size = NULL;
                if (!match(ps, t0->type, "expected array size or asterisk", &dim_size)) {
                    /* test case: no test case needed */
                    assert(false);
                }

                if (t0->type == token_number) {
                    n->to.is_array = true;
                    buffer_finish(&dim_size->value);
                    size_t v = (size_t)strtol(dim_size->value.buf, NULL, 10);
                    VectorAdd(&n->to.dim, &v, 1);
                } else if (t0->type == token_mult) {
                    n->to.is_slice = true;
                }
            } else {
                error_list_set(ps->el,
                               &t0->loc,
                               "expected array size, asterisk, or right square_bracket");
                n->type = ast_type_error;
            }
        }

        struct token* rsb = NULL;
        if (!match(ps, token_right_square_bracket, "expected right square bracket", &rsb)) {
            n->type = ast_type_error;
        }

        t0 = get_lookahead(ps);
    }

	if (t0 && t0->type == token_id) {
        if (!n) {
            ast_node_create(&n);
            n->type = ast_type_type;
        }

		struct token* name = NULL;
		if (!match(ps, token_id, "expected type name", &name)) {
            /* test case: no test case needed */
            assert(false);
        }

		t0 = get_lookahead(ps);
		if (t0 && t0->type == token_left_curly_brace) {
			is_generic = true;
			struct token* lcb = NULL;
			if (!match(ps, token_left_curly_brace, "expected left curly brace", &lcb)) {
                /* test case: no test case needed */
                assert(false);
            }

            token_destroy(lcb);
            free(lcb);

            consume_newline(ps);

			struct location loc_tseq;
            parse_tseq(ps, n, &loc_tseq);

            consume_newline(ps);

			struct token* rcb = NULL;
			if (!match(ps, token_right_curly_brace, "expected right curly brace", &rcb)) {
                n->type = ast_type_error;
            }

            token_destroy(rcb);
            free(rcb);
		}

		struct symbol* sym = NULL;
		if (n->type != ast_type_error) {
			sym = environment_get(ps->st->top, &name->value);
			if (!sym) {
				char* a;
				buffer2array(&name->value, &a);
				error_list_set(ps->el, &name->loc, "type not defined: %s", a);
				free(a);
                n->type = ast_type_error;
				/* test case: test_parse_error_type_not_defined */
			} else if (!sym->td) {
				char* a;
				buffer2array(&name->value, &a);
				error_list_set(ps->el, &name->loc, "identifier is not a type: %s", a);
				free(a);
                n->type = ast_type_error;
				/* test case: test_parse_error_not_a_type */
			} else if (is_generic && !sym->td->is_generic) {
				char* a;
				buffer2array(&name->value, &a);
				error_list_set(ps->el, &name->loc, "subtype was specified for non-generic type: %s", a);
				free(a);
                n->type = ast_type_error;
				/* test case: test_parse_error_not_generic */
			} else {
				if (is_generic) {
					int count = ast_node_count_children(n);
					if (sym->td->generic_count > 0 && count != sym->td->generic_count) {
						char* a;
						buffer2array(&name->value, &a);
						error_list_set(
							ps->el, &name->loc, "generic type (%s) should have %d subtype%s but has %d subtype%s",
							a,
							sym->td->generic_count, plural(sym->td->generic_count),
							count, plural(count)
						);
						free(a);
                        n->type = ast_type_error;
						/* test case: test_parse_error_subtype_count */
					}
				}

				if (n->type != ast_type_error) {
					n->td = sym->td;
				}
			}
		}

		token_destroy(name);
		free(name);
	}

	return n;
}

/**
 * Check ID node and create symbol
 * @param ps parse state
 * @param n type node
 * @param id_node ID node
 */
void check_id_node(struct parse_state* ps, struct ast_node* n, struct ast_node* id_node)
{
    struct symbol* dup = environment_get_local(ps->st->top, &id_node->value);
    if (dup) {
        char* a;
        buffer2array(&id_node->value, &a);
        error_list_set(ps->el, &id_node->loc, "duplicate declaration in same scope: %s", a);
        free(a);
        n->type = ast_type_error;
        /* test case: test_parse_error_duplicate_declarations */
    } else {
        struct symbol* sym2 = environment_get(ps->st->top, &id_node->value);
        if (sym2 && sym2->td) {
            char* a;
            buffer2array(&id_node->value, &a);
            error_list_set(ps->el, &id_node->loc, "identifier reserved as a type: %s", a);
            free(a);
            n->type = ast_type_error;
            /* test case: test_parse_types_reserved_type */
        } else {
            struct symbol* new_sym = NULL;
            malloc_safe((void**)&new_sym, sizeof(struct symbol));
            symbol_init(new_sym);
            new_sym->tk_type = token_id;
            new_sym->tu = ast_node_copy(n);
            environment_put(ps->st->top, &id_node->value, new_sym);
            id_node->sym = new_sym;
        }
    }
}

void declare_type(struct parse_state* ps, struct ast_node* n, struct ast_node* id_node)
{
    if (n && n->type != ast_type_error) {
        if (id_node) {
            if (id_node->type == ast_type_id) {
                check_id_node(ps, n, id_node);
            } else if (id_node->type == ast_type_var_lseq) {
                struct ast_node* p = id_node->head;
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
void parse_tseq(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
	struct ast_node* tu = NULL;
	struct token* t0 = NULL;

	get_location(ps, loc);

	struct location loc_type;
    tu = parse_type(ps, &loc_type);
	if (tu && tu->type == ast_type_error) {
        parent->type = ast_type_error;
    }

	if (!tu) {
		error_list_set(ps->el, &loc_type, "expected a type name");
		/* test case: test_parse_error_type_name */
        parent->type = ast_type_error;
	}

	if (tu) {
		assert(tu);
		ast_node_add(parent, tu);
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
		if (tu && tu->type == ast_type_error) {
            parent->type = ast_type_error;
        }

		if (!tu) {
			error_list_set(ps->el, &loc_type, "expected a type name after comma");
			/* test case: test_parse_error_comma_type_name */
            parent->type = ast_type_error;
			break;
		}

		if (parent->type != ast_type_error) {
			assert(tu);
			ast_node_add(parent, tu);
			tu = NULL;
		}

	}
}

struct ast_node* function2type(struct symbol_table* st, struct ast_node* n)
{
	struct buffer bf;
	int current_node = 0;

    struct ast_node* id_node = ast_node_get(n, current_node++);
    assert(id_node->type == ast_type_id);

	/* function */
	struct ast_node* tu = NULL;
	ast_node_create(&tu);
	tu->type = ast_type_type;

	buffer_init(&bf);
	buffer_copy_str(&bf, "Function");
	struct symbol* sym = environment_get(st->top, &bf);
	assert(sym);
	assert(sym->td);
	tu->td = sym->td;

	/* input */
	struct ast_node* dseq = ast_node_get(n, current_node++);

	if (dseq->head) {
		struct ast_node* input_tu = NULL;
		ast_node_create(&input_tu);
		input_tu->type = ast_type_type;

		buffer_clear(&bf);
		buffer_copy_str(&bf, "Input");
		struct symbol* input_sym = environment_get(st->top, &bf);
		assert(input_sym);
		assert(input_sym->td);
		input_tu->td = input_sym->td;

		struct ast_node* dec = dseq->head;
		while (dec) {
			struct ast_node* type_node = ast_node_get(dec, 1);
			struct ast_node* element_tu = ast_node_copy(type_node);
			ast_node_add(input_tu, element_tu);
			dec = dec->next;
		}

		ast_node_add(tu, input_tu);
	}

	/* output */
	struct ast_node* dret = ast_node_get(n, current_node++);
	struct ast_node* dret_type = ast_node_get(dret, 0);

	if (dret_type) {
		buffer_clear(&bf);
		buffer_copy_str(&bf, "Output");
		struct symbol* output_sym = environment_get(st->top, &bf);
		assert(output_sym);
		assert(output_sym->td);
		struct ast_node* output_tu = NULL;
		ast_node_create(&output_tu);
		output_tu->type = ast_type_type;
		output_tu->td = output_sym->td;

		struct ast_node* element_tu = ast_node_copy(dret_type);
		ast_node_add(output_tu, element_tu);
		ast_node_add(tu, output_tu);
	}

	buffer_destroy(&bf);

	return tu;
}

bool check_return_type(struct parse_state* ps, struct ast_node* fd, struct ast_node* stmts_node, struct location* loc)
{
    bool valid = true;

	assert(fd);

    if (fd->type != ast_type_error && stmts_node->type != ast_type_error) {
        struct ast_node* tu = fd->tu;
        struct ast_node* p = tu->head;
        while (p) {
            struct type_def* p_td = p->td;
            if (p_td->type == type_function_output) {
                struct ast_node* ret = ast_node_get(p, 0);
                if (ret) {
                    if (!type_use_can_cast(ret, stmts_node->tu)) {
                        valid = error_list_set(ps->el, loc, "returned type does not match function return type");
                    }
                }
            }
            p = p->next;
        }
    }

    return valid;
}

void get_function_children(struct ast_node* tu, struct ast_node** input, struct ast_node** output)
{
	struct ast_node* p = tu->head;
	while (p) {
		if (p->td) {
			if (p->td->type == type_function_input) {
				*input = p;
			} else if (p->td->type == type_function_output) {
				*output = p;
			}
		}
		p = p->next;
	}
}

struct ast_node* get_function_type(struct symbol* sym)
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

struct ast_node* get_function_input_type(struct ast_node* tu, int index)
{
	struct ast_node* input = NULL;
	struct ast_node* output = NULL;
	get_function_children(tu, &input, &output);

	if (!input) return NULL;

	struct ast_node* p = input->head;
	int i = 0;
	while (p) {
		if (i == index) return p;
		p = p->next;
		i++;
	}
	return NULL;
}

bool check_input_type(struct parse_state* ps, struct ast_node* tu, int index, struct ast_node* a, struct location* loc_expr)
{
	bool valid = true;

	if (tu) {
		struct ast_node* tu0 = get_function_input_type(tu, index);
		if (tu0) {
			struct ast_node* call_tu0 = a->tu;
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

struct ast_node* make_constructor(struct type_def* td)
{
	struct ast_node* tu = td->composite;

	struct ast_node* n = NULL;
	ast_node_create(&n);
	n->type = ast_type_function;

	/* id */
	struct ast_node* id = NULL;
	ast_node_create(&id);
	id->type = ast_type_id;
	buffer_copy(&td->name, &id->value);
	ast_node_add(n, id);

	/* dseq */
	struct ast_node* dseq = NULL;
	ast_node_create(&dseq);
	dseq->type = ast_type_dseq;

	struct ast_node* p = tu->head;
	while (p) {
		struct ast_node* dec = ast_node_copy(p);
		ast_node_add(dseq, dec);
		p = p->next;
	}

	ast_node_add(n, dseq);

	/* dret */
	struct ast_node* dret = NULL;
	ast_node_create(&dret);
	dret->type = ast_type_dret;

	struct ast_node* dret_tu = NULL;
	ast_node_create(&dret_tu);
	dret_tu->type = ast_type_type;
	dret_tu->td = td;
	ast_node_add(dret, dret_tu);

	ast_node_add(n, dret);

	/* stmts */
	struct ast_node* stmts = NULL;
	ast_node_create(&stmts);
	stmts->type = ast_type_stmts;
	ast_node_add(n, stmts);

	/* var */
	struct ast_node* var = NULL;
	ast_node_create(&var);
	var->type = ast_type_var;
	ast_node_add(stmts, var);

	struct ast_node* dec = NULL;
	ast_node_create(&dec);
	dec->type = ast_type_declaration;
	ast_node_add(var, dec);

	struct ast_node* id2 = NULL;
	ast_node_create(&id2);
	id2->type = ast_type_id;
	buffer_copy_str(&id2->value, "__x");
	ast_node_add(dec, id2);

	struct ast_node* t = NULL;
	ast_node_create(&t);
	t->type = ast_type_type;
	t->td = td;
	ast_node_add(dec, t);

	/* assignments */
	p = tu->head;
	while (p) {
		struct ast_node* assign = NULL;
		ast_node_create(&assign);
		assign->type = ast_type_assign;
		ast_node_add(stmts, assign);

		struct ast_node* dot = NULL;
		ast_node_create(&dot);
		dot->type = ast_type_type;
		ast_node_add(assign, dot);

		struct ast_node* x = NULL;
		ast_node_create(&x);
		x->type = ast_type_id;
		buffer_copy_str(&x->value, "__x");
		ast_node_add(dot, x);

		struct ast_node* field = NULL;
		ast_node_create(&field);
		field->type = ast_type_id;
		struct ast_node* id3 = ast_node_get(p, 0);
		buffer_copy(&id3->value, &field->value);
		ast_node_add(dot, field);

		p = p->next;
	}

	/* return __x */
	struct ast_node* id4 = NULL;
	ast_node_create(&id4);
	id4->type = ast_type_id;
	buffer_copy_str(&id4->value, "__x");
	ast_node_add(stmts, id4);

	return n;
}
