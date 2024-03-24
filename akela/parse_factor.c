#include <assert.h>
#include "zinc/result.h"
#include "parse_tools.h"
#include "ast.h"
#include "token.h"
#include "parse_types.h"
#include "parse_stmts.h"
#include "parse_expr.h"
#include "source.h"
#include "scan.h"
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

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! parse_factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
struct ast_node* parse_factor(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
	get_lookahead_one(ps);

	struct token* t0;
	t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_function) {
        struct token* f = NULL;
        match(ps, token_function, "expected function", &f);
        consume_newline(ps);
        token_destroy(f);
        free(f);
        n = parse_anonymous_function(ps, NULL, loc);

	} else if (t0 && t0->type == token_not) {
		n = parse_not(ps, loc);

	} else if (t0 && (t0->type == token_number || t0->type == token_string || t0->type == token_boolean)) {
		n = parse_literal(ps, loc);

	} else if (t0 && t0->type == token_id) {
		n = parse_id(ps, loc);

	} else if (t0 && (t0->type == token_plus || t0->type == token_minus)) {
		n = parse_sign(ps, loc);

	} else if (t0 && t0->type == token_left_square_bracket) {
		n = parse_array_literal(ps, loc);

	} else if (t0 && t0->type == token_left_paren) {
		n = parse_parenthesis(ps, loc);
	} else {
        if (!get_location(ps, loc)) {
            ast_node_create(&n);
            n->type = ast_type_error;
        }
    }

	return n;
}

struct ast_node* parse_anonymous_function(struct parse_state* ps, struct ast_node* n, struct location* loc)
{
    if (!n) {
        ast_node_create(&n);
    }
    n->type = ast_type_anonymous_function;

    if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

    /* shared ps{top} -> saved */
	struct environment* saved = ps->st->top;

	/* allocate env env{} */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);

	/* transfer env -> ps{top} */
	ps->st->top = env;

	struct token* lp = NULL;
	if (!match(ps, token_left_paren, "expected left parenthesis", &lp)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

	/* allocate a a{} */
	struct ast_node* dseq_node = NULL;
	struct location loc_dseq;
	if (!dseq(ps, &dseq_node, &loc_dseq)) {
        n->type = ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }
	
	/* allocate n */
	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	if (!match(ps, token_right_paren, "expected right parenthesis", &rp)) {
        /* test case: test_parse_anonymous_function_expected_right_paren */
        n->type = ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

	int num;
	if (!get_lookahead(ps, 1, &num)) {
        n->type = ast_type_error;
    }
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct ast_node* dret_type = NULL;
	struct location loc_ret;
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

		if (!parse_type(ps, NULL, &dret_type, &loc_ret)) {
            n->type = ast_type_error;
        }

        if (!dret_type) {
            set_source_error(ps->el, &loc_ret, "expected a type");
            n->type = ast_type_error;
        }
	}

	/* allocate b b{} */
	struct ast_node* stmts_node = NULL;
	struct location loc_stmts;
	if (!stmts(ps, true, &stmts_node, &loc_stmts)) {
        n->type = ast_type_error;
    }

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	if (!match(ps, token_end, "expected end", &end)) {
        /* test case: test_parse_anonymous_function_expected_end */
        n->type = ast_type_error;
    }

	if (n->type != ast_type_error) {
		/* transfer dseq_node dseq_node{} -> n{} */
		ast_node_add(n, dseq_node);

		struct ast_node* dret = NULL;
		ast_node_create(&dret);
		dret->type = ast_type_dret;

		if (dret_type) {
			ast_node_add(dret, dret_type);
		}
		ast_node_add(n, dret);

		/* transfer stmts_node stmts_node{} -> n */
		ast_node_add(n, stmts_node);
	}

	/* destroy f f{} lp lp{} rp rp{} end end{} */
	token_destroy(lp);
	free(lp);
	token_destroy(rp);
	free(rp);
	token_destroy(end);
	free(end);

	if (n->type != ast_type_error) {
		n->tu = function2type(ps->st, n);
        bool valid = true;
		check_return_type(ps, n, stmts_node, &loc_ret, &valid);
        if (!valid) {
            n->type = ast_type_error;
        }
	}

	/* transfer saved -> ps{top} */
	ps->st->top = saved;

	/* destroy env env{} */
	environment_destroy(env);

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
		set_source_error(ps->el, &loc_factor, "expected parse_factor after !");
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
			set_source_error(ps->el, &not->loc, "! operator used on parse_factor with no value");
			/* test case: test_parse_not_error_no_value */
            n->type = ast_type_error;
		} else {
			assert(tu->td);
			if (tu->td->type != type_boolean) {
				set_source_error(ps->el, &not->loc, "not operator used on non-boolean");
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

    int num;
	if (!get_lookahead(ps, 1, &num)) {
        n->type = ast_type_error;
    }
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* allocate ps{} x x{} */
	struct token* x = NULL;
	if (!match(ps, t0->type, "expecting number, bool, or string", &x)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

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
			type_name = "String";
		} else if (x->type == token_boolean) {
			n->type = ast_type_boolean;
			type_name = "Bool";
		}
		/* allocate n{} */
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
				set_source_error(ps->el, &id->loc, "variable not a field of struct: %s", id->value.buf);
                n->type = ast_type_error;
			}
		} else {
			struct symbol* sym2 = environment_get(ps->st->top, &full_id);
			if (!sym2) {
                buffer_finish(&full_id);
                set_source_error(ps->el, &id->loc, "variable not declared: %s", full_id.buf);
                /* test case: test_parse_types_missing_declaration */
                n->type = ast_type_error;
            } else if (sym2->td && sym2->td->type != type_struct) {
                set_source_error(ps->el, &id->loc, "using type as an identifier: %s", full_id.buf);
                n->type = ast_type_error;
			} else {
                struct symbol* sym3 = sym2;
				if (!sym3->tu) {
					sym3 = sym3->constructor;
				}
                assert(sym3);
				assert(sym3->tu);
				n->tu = ast_node_copy(sym3->tu);
			}
		}
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

	int num;
	if (!get_lookahead(ps, 1, &num)) {
        n->type = ast_type_error;
    }
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* allocate sign */
	struct token* sign = NULL;
	if (!match(ps, t0->type, "expecting unary plus or minus", &sign)) {
        /* test case: no test case needed */
        n->type = ast_type_error;
    }

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

	/* allocate right */
	struct ast_node* right = NULL;
	struct location loc_factor;
    right = parse_expr(ps, &loc_factor);
	if (right && right->type == ast_type_error) {
        n->type = ast_type_error;
    }

	if (!right) {
		set_source_error(ps->el, &loc_factor, "expected parse_factor after sign");
        n->type = ast_type_error;
	}

	if (n->type != ast_type_error) {
		/* allocate left */
		struct ast_node* left;
		ast_node_create(&left);

		if (t0->type == token_plus) {
			left->type = ast_type_plus;
		} else {
			left->type = ast_type_minus;
		}

		/* transfer left -> n{} */
		ast_node_add(n, left);

		/* transfer right -> n{} */
		ast_node_add(n, right);

	}

	if (n->type != ast_type_error) {
		assert(right);
		struct ast_node* tu = right->tu;
		if (!tu) {
			set_source_error(ps->el, &sign->loc, "negative operator was used on expression with no value");
			/* test case: test_parse_sign_error */
            n->type = ast_type_error;
		} else {
			n->tu = ast_node_copy(tu);
		}
	}

	/* destroy sign */
	token_destroy(sign);

	return n;
}

/*
* array_literal -> [aseq]
*/
struct ast_node* parse_array_literal(struct parse_state* ps, struct location* loc)
{
	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_array_literal;

    if (!get_location(ps, loc)) {
        n->type = ast_type_error;
    }

    struct token* lsb = NULL;
    if (!match(ps, token_left_square_bracket, "expected left square bracket", &lsb)) {
        n->type = ast_type_error;
        /* test case: no test case needed */
    }

    /* destroy lsb lsb{} */
    token_destroy(lsb);
    free(lsb);

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

    struct location first_loc;
    parse_aseq(ps, n, &first_loc);

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

    /* allocate ps{} rsb rsb{] */
    struct token* rsb = NULL;
    if (!match(ps, token_right_square_bracket, "expected right square bracket", &rsb)) {
        /* test case: test_parse_array_literal_error_right_square_bracket */
        n->type = ast_type_error;
    }

    if (n->type != ast_type_error) {
        struct ast_node* first = n->head;

        if (!first) {
            set_source_error(ps->el, &rsb->loc, "array literal has no elements");
            /* test case: test_parse_array_literal_empty_error */
            n->type = ast_type_error;
        } else {
            struct ast_node* tu_first = ast_node_copy(first->tu);
            struct ast_node* x = first->next;
            struct ast_node* tu_x;
            while (x) {
                tu_x = x->tu;
                if (!type_find_whole(ps->st, tu_first, tu_x)) {
                    set_source_error(ps->el, &first_loc, "array elements not the same type");
                    /* test case: test_parse_array_literal_mixed_error */
                    n->type = ast_type_error;
                    break;
                }
                x = x->next;
            }
            n->tu = tu_first;
        }
    }

    /* destroy rsb rsb{} */
    token_destroy(rsb);
    free(rsb);

	return n;
}

/* aseq -> expr aseq' | e */
/* aseq' = , expr aseq' | e */
/* dynamic-output ps{} parent{} */
void parse_aseq(struct parse_state* ps, struct ast_node* parent, struct location *loc)
{
    if (!get_location(ps, loc)) {
        parent->type = ast_type_error;
    }

	struct ast_node* a = NULL;
	struct location loc_expr;
    a = parse_simple_expr(ps, &loc_expr);
	if (a && a->type == ast_type_error) {
        parent->type = ast_type_error;
    }

	if (a) {
		/* a -> parent{} */
		ast_node_add(parent, a);

		while (true) {
			/* allocate ps{} */
			int num;
			if (!get_lookahead(ps, 1, &num)) {
                parent->type = ast_type_error;
            }
			struct token* t0 = get_token(&ps->lookahead, 0);
			if (!t0 || t0->type != token_comma) {
				break;
			}

			/* allocate ps{} comma comma{} */
			struct token* comma = NULL;
			if (!match(ps, token_comma, "expecting comma", &comma)) {
                /* test case: no test case needed */
                parent->type = ast_type_error;
            }

			/* destroy comma comma{} */
			token_destroy(comma);
			free(comma);

            if (!consume_newline(ps)) {
                parent->type = ast_type_error;
            }

			/* allocate ps{} a a{} */
			a = parse_simple_expr(ps, &loc_expr);
			if (a && a->type == ast_type_error) {
                parent->type = ast_type_error;
            }

			if (!a) {
				set_source_error(ps->el, &loc_expr, "expected expr after comma");
                parent->type = ast_type_error;
				/* test cases: test_parse_array_literal_error_expected_expr */
				break;
			}

			/* transfer a -> parent */
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
		set_source_error(ps->el, &loc_a, "empty parenthesis");
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
			set_source_error(ps->el, &loc_a, "parenthesis on expression that has no value");
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
