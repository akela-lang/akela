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

bool not_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool literal_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool id_nt(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool sign(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool array_literal(struct parse_state* ps, struct ast_node** root, struct location* loc);
bool aseq(struct parse_state* ps, struct ast_node* parent, struct location* loc);
struct ast_node* parenthesis(struct parse_state* ps);

/*
* factor -> id(cseq) | number | string | id | + factor | - factor | (expr)
*		  | ! factor | array_literal | function(dseq) stmts end
* note: type system should catch incompatible sign or not factors
*/
/* dynamic-output ps{} root root{} */
bool factor(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} */
	int num;
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0;
	t0 = get_token(&ps->lookahead, 0);

	if (t0 && t0->type == token_function) {
        /* allocate ps{} f f{} */
        struct token* f = NULL;
        valid = match(ps, token_function, "expected function", &f) && valid;
        location_update_token(loc, f);
        /* test case: no test case needed */
        token_destroy(f);
        free(f);

        valid = consume_newline(ps) && valid;

        valid = anonymous_function(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_not) {
		valid = not_nt(ps, &n, loc) && valid;

	} else if (t0 && (t0->type == token_number || t0->type == token_string || t0->type == token_boolean)) {
		valid = literal_nt(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_id) {
		valid = id_nt(ps, &n, loc) && valid;

	} else if (t0 && (t0->type == token_plus || t0->type == token_minus)) {
		valid = sign(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_left_square_bracket) {
		/* allocate n n{} */
		valid = array_literal(ps, &n, loc) && valid;

	} else if (t0 && t0->type == token_left_paren) {
		n = parenthesis(ps);
        if (n->type == ast_type_error) {
            valid = false;
        }
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool anonymous_function(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	/* shared ps{top} -> saved */
	struct environment* saved = ps->st->top;

	/* allocate env env{} */
	struct environment* env = NULL;
	malloc_safe((void**)&env, sizeof(struct environment));
	environment_init(env, saved);

	/* transfer env -> ps{top} */
	ps->st->top = env;

	struct token* lp = NULL;
	valid = match(ps, token_left_paren, "expected left parenthesis", &lp) && valid;
	location_update_token(loc, lp);
	/* test case: no test case needed */

    valid = consume_newline(ps) && valid;

	/* allocate a a{} */
	struct ast_node* dseq_node = NULL;
	struct location loc_dseq;
	valid = dseq(ps, &dseq_node, &loc_dseq) && valid;
	location_update(loc, &loc_dseq);

    valid = consume_newline(ps) && valid;
	
	/* allocate n */
	/* allocate ps{} rp rp{} */
	struct token* rp = NULL;
	valid = match(ps, token_right_paren, "expected right parenthesis", &rp) && valid;
	location_update_token(loc, rp);
	/* test case: test_parse_anonymous_function_expected_right_paren */

    valid = consume_newline(ps) && valid;

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct ast_node* dret_type = NULL;
	struct location loc_ret;
	if (t0 && t0->type == token_double_colon) {
		struct token* dc = NULL;
		valid = match(ps, token_double_colon, "expecting double colon", &dc) && valid;
		location_update_token(loc, dc);
		token_destroy(dc);
		free(dc);
		/* test case: no test case needed */

        valid = consume_newline(ps) && valid;

		valid = parse_type(ps, NULL, &dret_type, &loc_ret) && valid;
		location_update(loc, &loc_ret);

        if (!dret_type) {
            valid = set_source_error(ps->el, &loc_ret, "expected a type");
        }
	}

	/* allocate b b{} */
	struct ast_node* stmts_node = NULL;
	struct location loc_stmts;
	valid = stmts(ps, true, &stmts_node, &loc_stmts) && valid;
	location_update(loc, &loc_stmts);

	/* allocate ps{} end end{} */
	struct token* end = NULL;
	valid = match(ps, token_end, "expected end", &end) && valid;
	location_update_token(loc, end);
	/* test case: test_parse_anonymous_function_expected_end */

	if (valid) {
		ast_node_create(&n);
		n->type = ast_type_anonymous_function;

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
	} else {
		ast_node_destroy(dseq_node);
		ast_node_destroy(dret_type);
		ast_node_destroy(stmts_node);
	}

	/* destroy f f{} lp lp{} rp rp{} end end{} */
	token_destroy(lp);
	free(lp);
	token_destroy(rp);
	free(rp);
	token_destroy(end);
	free(end);

	if (valid) {
		n->tu = function2type(ps->st, n);
		check_return_type(ps, n, stmts_node, &loc_ret, &valid);
	}

	/* transfer saved -> ps{top} */
	ps->st->top = saved;

	/* destroy env env{} */
	environment_destroy(env);

	if (valid) {
		/* transfer n -> root */
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool not_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} not not{} */
	struct token* not = NULL;
	valid = match(ps, token_not, "expecting not", &not) && valid;
	location_update_token(loc, not);
	/* test case: no test case needed */

    valid = consume_newline(ps) && valid;

	/* allocate a a{} */
	struct ast_node* a = NULL;
	struct location loc_factor;
	valid = expr(ps, &a, &loc_factor) && valid;
	location_update(loc, &loc_factor);

	if (!a) {
		/* allocate ps{} */
		valid = set_source_error(ps->el, &loc_factor, "expected factor after !");
		/* test case: test_parse_not_error_expected_factor */
	}

	if (valid) {
		/* allocate n */
		ast_node_create(&n);
		n->type = ast_type_not;

		if (a) {
			/* transfer a -> n{} */
			ast_node_add(n, a);
		}

	} else {
		ast_node_destroy(a);
	}

	if (valid) {
		assert(a);
		struct ast_node* tu = a->tu;
		if (!tu) {
			valid = set_source_error(ps->el, &not->loc, "! operator used on factor with no value");
			/* test case: test_parse_not_error_no_value */
		} else {
			assert(tu->td);
			if (tu->td->type != type_boolean) {
				valid = set_source_error(ps->el, &not->loc, "not operator used on non-boolean");
				/* test case: test_parse_not_error_not_boolean */
			} else {
				n->tu = ast_node_copy(tu);
			}
		}
	}

	/* destroy not not{} */
	token_destroy(not);
	free(not);

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool literal_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;
	char* type_name = NULL;

	location_init(loc);

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* allocate ps{} x x{} */
	struct token* x = NULL;
	valid = match(ps, t0->type, "expecting number, bool, or string", &x) && valid;
	location_update_token(loc, x);
	/* test case: no test case needed */

	if (valid) {
		/* allocate n */
		ast_node_create(&n);

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

	if (valid) {
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

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool id_nt(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* allocate ps{} x x{} */
	struct token* id = NULL;
	valid = match(ps, token_id, "expecting identifier", &id) && valid;
	location_update_token(loc, id);
	/* test case: no test case needed */

	if (valid) {
		/* allocate n */
		ast_node_create(&n);

		n->type = ast_type_id;

		/* allocate n{} */
		buffer_copy(&id->value, &n->value);
	}

	if (valid) {
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
				valid = set_source_error(ps->el, &id->loc, "variable not a field of struct: %s", id->value.buf);
			}
		} else {
			struct symbol* sym2 = environment_get(ps->st->top, &full_id);
			if (!sym2) {
                buffer_finish(&full_id);
                valid = set_source_error(ps->el, &id->loc, "variable not declared: %s", full_id.buf);
                /* test case: test_parse_types_missing_declaration */
            } else if (sym2->td && sym2->td->type != type_struct) {
                valid = set_source_error(ps->el, &id->loc, "using type as an identifier: %s", full_id.buf);
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

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

bool sign(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	struct ast_node* n = NULL;

	location_init(loc);

	int num;
	valid = get_lookahead(ps, 1, &num) && valid;
	struct token* t0 = get_token(&ps->lookahead, 0);

	/* allocate sign */
	struct token* sign = NULL;
	valid = match(ps, t0->type, "expecting unary plus or minus", &sign) && valid;
	location_update_token(loc, sign);
	/* test case: no test case needed */

    valid = consume_newline(ps) && valid;

	/* allocate right */
	struct ast_node* right = NULL;
	struct location loc_factor;
	valid = expr(ps, &right, &loc_factor) && valid;
	location_update(loc, &loc_factor);

	if (!right) {
		valid = set_source_error(ps->el, &loc_factor, "expected factor after sign");
	}

	if (valid) {
		ast_node_create(&n);
		n->type = ast_type_sign;

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

	if (valid) {
		assert(right);
		struct ast_node* tu = right->tu;
		if (!tu) {
			valid = set_source_error(ps->el, &sign->loc, "negative operator was used on expression with no value");
			/* test case: test_parse_sign_error */
		} else {
			n->tu = ast_node_copy(tu);
		}
	}

	/* destroy sign */
	token_destroy(sign);

	if (valid) {
		*root = n;
	} else {
		ast_node_destroy(n);
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

/*
* array_literal -> [aseq]
*/
bool array_literal(struct parse_state* ps, struct ast_node** root, struct location* loc)
{
	bool valid = true;
	int num;
	struct ast_node* n = NULL;

	location_init(loc);

	/* allocate ps{} */
	valid = get_lookahead(ps, 1, &num) && valid;

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_left_square_bracket) {
		/* allocate ps{} lsb lsb{} */
		struct token* lsb = NULL;
		valid = match(ps, token_left_square_bracket, "expected left square bracket", &lsb) && valid;
		location_update_token(loc, lsb);
		/* test case: no test case needed */

		/* destroy lsb lsb{} */
		token_destroy(lsb);
		free(lsb);

        valid = consume_newline(ps) && valid;

		/* allocate n */
		ast_node_create(&n);
		n->type = ast_type_array_literal;

		/* allocate ps{} n{} */
		struct location loc_aseq;
		valid = aseq(ps, n, &loc_aseq) && valid;
		location_update(loc, &loc_aseq);

        valid = consume_newline(ps) && valid;

		/* allocate ps{} rsb rsb{] */
		struct token* rsb = NULL;
		valid = match(ps, token_right_square_bracket, "expected right square bracket", &rsb) && valid;
		location_update_token(loc, rsb);
		/* test case: test_parse_array_literal_error_right_square_bracket */

		/* destroy rsb rsb{} */
		token_destroy(rsb);
		free(rsb);

		if (valid) {
			struct ast_node* first = n->head;

			if (!first) {
				valid = set_source_error(ps->el, &loc_aseq, "array literal has no elements");
				/* test case: test_parse_array_literal_empty_error */
			} else {
				struct ast_node* tu_first = ast_node_copy(first->tu);
				struct ast_node* x = first->next;
				struct ast_node* tu_x;
				while (x) {
					tu_x = x->tu;
					if (!type_find_whole(ps->st, tu_first, tu_x)) {
						valid = set_source_error(ps->el, &loc_aseq, "array elements not the same type");
						/* test case: test_parse_array_literal_mixed_error */
						break;
					}
					x = x->next;
				}
				n->tu = tu_first;
			}
		}
	}

	/* transfer n -> root */
	if (valid) {
		*root = n;
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

/* aseq -> expr aseq' | e */
/* aseq' = , expr aseq' | e */
/* dynamic-output ps{} parent{} */
bool aseq(struct parse_state* ps, struct ast_node* parent, struct location* loc)
{
	bool valid = true;

	location_init(loc);

	/* allocate ps{} a a{} */
	struct ast_node* a = NULL;
	struct location loc_expr;
	valid = simple_expr(ps, &a, &loc_expr) && valid;
	location_update(loc, &loc_expr);

	if (a) {
		/* a -> parent{} */
		ast_node_add(parent, a);

		while (true) {
			/* allocate ps{} */
			int num;
			valid = get_lookahead(ps, 1, &num) && valid;
			struct token* t0 = get_token(&ps->lookahead, 0);
			if (!t0 || t0->type != token_comma) {
				valid = location_default(ps, loc) && valid;
				break;
			}

			/* allocate ps{} comma comma{} */
			struct token* comma = NULL;
			valid = match(ps, token_comma, "expecting comma", &comma) && valid;
			location_update_token(loc, comma);
			/* test case: no test case needed */

			/* destroy comma comma{} */
			token_destroy(comma);
			free(comma);

            valid = consume_newline(ps) && valid;

			/* allocate ps{} a a{} */
			struct ast_node* a = NULL;
			valid = simple_expr(ps, &a, &loc_expr) && valid;
			location_update(loc, &loc_expr);

			if (!a) {
				valid = set_source_error(ps->el, &loc_expr, "expected expr after comma");
				/* test cases: test_parse_array_literal_error_expected_expr */
				break;
			}

			/* transfer a -> parent */
			ast_node_add(parent, a);
		}
	}

	valid = location_default(ps, loc) && valid;

	return valid;
}

struct ast_node* parenthesis(struct parse_state* ps)
{
	struct ast_node* n = NULL;
    ast_node_create(&n);
    n->type = ast_type_parenthesis;

	ast_node_location_init(n);

	/* allocate ps{} lp lp{} */
	struct token* lp = NULL;
	if (!match(ps, token_left_paren, "expecting left parenthesis", &lp)) {
        n->type = ast_type_error;
    }
	ast_node_location_update_token(n, lp);
	/* test case: no test case needed */

    if (!consume_newline(ps)) {
        n->type = ast_type_error;
    }

	/* allocate n n{} */
	struct ast_node* a = NULL;
	struct location loc_a;
	if (!expr(ps, &a, &loc_a)) {
        n->type == ast_type_error;
    }
	location_update(&n->loc, &loc_a);

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
	ast_node_location_update_token(n, rp);

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

	if (!ast_node_location_default(ps, n)) {
        n->type = ast_type_error;
    }

	return n;
}
