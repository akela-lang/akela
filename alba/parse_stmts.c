#include "result.h"
#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "buffer.h"
#include "parse_expr.h"
#include "input.h"
#include "scan.h"
#include "parse_tools.h"
#include "types.h"
#include "source.h"

/*
* stmts -> stmt stmts'
*/
enum parse_result stmts(struct allocator* al, struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	struct dag_node* a = NULL;
	r = stmt(al, ps, &a);
	if (r == result_error) {
		goto function_error;
	}

	r = dag_create_node(al, &n);
	if (r == result_error) {
		goto function_error;
	}
	n->type = dag_type_stmts;

	if (a) {
		dag_add_child(n, a);
	}

	r = stmts_prime(al, ps, n);
	if (r == result_error) {
		goto function_error;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* stmts' -> separator stmt stmts'
*         | e
*/

enum parse_result stmts_prime(struct allocator* al, struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int has_separator;

	r = separator(al, ps, &has_separator);
	if (r == result_error) {
		return r;
	}

	if (!has_separator) {
		return r;
	}

	struct dag_node* a = NULL;
	r = stmt(al, ps, &a);
	if (r == result_error) {
		return r;
	}

	if (a) {
		dag_add_child(parent, a);
	}

	r = stmts_prime(al, ps, parent);
	if (r == result_error) {
		return r;
	}

	return r;
}

/*
* separator -> \n | ;
*/
enum parse_result separator(struct allocator* al, struct parse_state* ps, int* has_separator)
{
	enum result r = result_ok;
	enum token_type type;
	int num;
	*has_separator = 0;
	struct token* t;

	r = get_lookahead(al, ps, 1, &num);
	if (r == result_error) {
		return r;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_newline) {
		type = token_newline;
		*has_separator = 1;
	} else if (t0 && t0->type == token_semicolon) {
		type = token_semicolon;
		*has_separator = 1;
	} else {
		return r;
	}

	r = match(al, ps, type, "expecting newline or semicolon", &t);
	if (r == result_error) {
		return r;
	}

	return r;
}

/*
* stmt -> id = expr
*		| function id (dseq) stmts end
*       | expr
*       | e
*/
enum result stmt(struct allocator* al, struct parse_state* ps, struct dag_node** root)
{
	enum result r;
	struct dag_node* n = NULL;
	int num;
	struct token* t;

	r = get_lookahead(al, ps, 2, &num);
	if (r == result_error) {
		goto function_error;
	}

	/* e */
	if (num <= 0) {
		goto function_success;
	}


	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	/* while */
	if (t0 && t0->type == token_while) {
		r = match(al, ps, token_while, "expecting while", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_while;

		struct dag_node* a = NULL;
		r = expr(al, ps, &a);
		if (r == result_error) {
			goto function_error;
		}
		if (!a) {
			r = set_source_error(t0, ps->sns->lc, "expected expression after while");
			goto function_error;
		}
		dag_add_child(n, a);

		struct dag_node* b = NULL;
		r = stmts(al, ps, &b);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, b);

		r = match(al, ps, token_end, "expected end", &t);
		if (r == result_error) {
			goto function_error;
		}

		goto function_success;

	/* for */
	} else if (t0 && t0->type == token_for) {
		r = get_lookahead(al, ps, 3, &num);
		if (r == result_error) {
			goto function_error;
		}

		struct token* t2 = get_token(&ps->lookahead, 2);

		if (t1 && t1->type == token_id && t2 && t2->type == token_equal) {
			r = for_range(al, ps, &n);
			if (r == result_error) {
				goto function_error;
			}
		} else if (t1 && t1->type == token_id && t2 && t2->type == token_in) {
			r = for_iteration(al, ps, &n);
			if (r == result_error) {
				goto function_error;
			}
		} else {
			r = set_source_error(t0, ps->sns->lc, "expected = or in after for");
			goto function_error;
		}

		goto function_success;

		/* function word (seq) stmts end */
	} else if (t0 && t0->type == token_function) {
		r = match(al, ps, token_function, "expecting function", &t);
		if (r == result_error) {
			goto function_error;
		}
		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_function;

		r = match(al, ps, token_id, "expecting word", &t);
		if (r == result_error) {
			goto function_error;
		}
		struct dag_node* a;
		r = dag_create_node(al, &a);
		if (r == result_error) {
			goto function_error;
		}
		a->type = dag_type_id;
		buffer_copy(al, &t1->value, &a->value);
		dag_add_child(n, a);

		r = match(al, ps, token_left_paren, "expecting left parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* b;
		r = dseq(al, ps, &b);
		if (r == result_error) {
			goto function_error;
		}
		if (b) {
			dag_add_child(n, b);
		}

		r = match(al, ps, token_right_paren, "expecting right parenthesis", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* c;
		r = stmts(al, ps, &c);
		if (r == result_error) {
			goto function_error;
		}
		if (c) {
			dag_add_child(n, c);
		}

		r = match(al, ps, token_end, "expecting end", &t);
		if (r == result_error) {
			goto function_error;
		}

		goto function_success;

	} else if (t0 && t0->type == token_if) {
		/* if */
		r = match(al, ps, token_if, "expecting if", &t);
		if (r == result_error) {
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_if;

		struct dag_node* cb = NULL;
		r = dag_create_node(al, &cb);
		if (r == result_error) {
			goto function_error;
		}
		cb->type = dag_type_conditional_branch;
		dag_add_child(n, cb);

		/* condition */
		struct dag_node* cond = NULL;
		r = expr(al, ps, &cond);
		if (r == result_error) {
			goto function_error;
		}
		if (cond == NULL) {
			r = set_source_error(t0, ps->sns->lc, "expecting a condition after if");
			goto function_error;
		}
		dag_add_child(cb, cond);

		/* stmts */
		struct dag_node* body = NULL;
		r = stmts(al, ps, &body);
		if (r == result_error) {
			goto function_error;
		}

		if (body) {
			dag_add_child(cb, body);
		}

		/* elseif_stmts */
		r = elseif_stmts(al, ps, n);
		if (r == result_error) {
			goto function_error;
		}

		/* else_stmt */
		r = else_stmt(al, ps, n);
		if (r == result_error) {
			goto function_error;
		}

		/* end */
		r = match(al, ps, token_end, "expected end", &t);
		if (r == result_error) {
			goto function_error;
		}

		goto function_success;

	/* expr */
	} else {
		r = expr(al, ps, &n);
		if (r == result_error) {
			goto function_error;
		}

		goto function_success;
	}

	r = set_source_error(t0, ps->sns->lc, "expected statement");
	goto function_error;

function_success:
	*root = n;
	return result_ok;

function_error:
	return r;
}

/*
* for_range -> for id = expr:expr stmts end
*/
enum result for_range(struct allocator* al, struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;
	struct token* t;

	r = get_lookahead(al, ps, 3, &num);
	if (r == result_error) {
		goto function_error;
	}
	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	r = match(al, ps, token_for, "expected for", &t);
	if (r == result_error) {
		goto function_error;
	}

	r = match(al, ps, token_id, "expected id", &t);
	if (r == result_error) {
		goto function_error;
	}

	r = match(al, ps, token_equal, "expected equal", &t);
	if (r == result_error) {
		goto function_error;
	}

	r = dag_create_node(al, &n);
	if (r == result_error) {
		goto function_error;
	}

	/* for range */
	r = dag_create_node(al, &n);
	if (r == result_error) {
		goto function_error;
	}
	n->type = dag_type_for_range;

	/* id */
	struct dag_node* a = NULL;
	r = dag_create_node(al, &a);
	if (r == result_error) {
		goto function_error;
	}
	a->type = dag_type_id;
	r = buffer_copy(al, &t1->value, &a->value);
	if (r == result_error) {
		goto function_error;
	}
	dag_add_child(n, a);

	/* start expr */
	struct dag_node* b = NULL;
	r = expr(al, ps, &b);
	if (r == result_error) {
		goto function_error;
	}
	if (!b) {
		r = set_source_error(t0, ps->sns->lc, "expected range start after for-range");
		goto function_error;
	}
	dag_add_child(n, b);

	r = match(al, ps, token_colon, "expected colon", &t);
	if (r == result_error) {
		goto function_error;
	}

	/* end expr */
	struct dag_node* c = NULL;
	r = expr(al, ps, &c);
	if (r == result_error) {
		goto function_error;
	}
	if (!c) {
		r = set_source_error(t0, ps->sns->lc, "expected range end after for-range");
		goto function_error;
	}
	dag_add_child(n, c);

	/* stmts */
	struct dag_node* d = NULL;
	r = stmts(al, ps, &d);
	if (r == result_error) {
		goto function_error;
	}
	dag_add_child(n, d);

	r = match(al, ps, token_end, "expected end", &t);
	if (r == result_error) {
		goto function_error;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* for_iteration -> for id in expr stmts end
*/
enum result for_iteration(struct allocator* al, struct parse_state* ps, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	int num;
	struct token* t;

	r = get_lookahead(al, ps, 3, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	struct token* t1 = get_token(&ps->lookahead, 1);

	r = match(al, ps, token_for, "expecting for", &t);
	if (r == result_error) {
		goto function_error;
	}

	r = match(al, ps, token_id, "expecting id", &t);
	if (r == result_error) {
		goto function_error;
	}

	r = match(al, ps, token_in, "expecting in", &t);
	if (r == result_error) {
		goto function_error;
	}

	/* for */
	r = dag_create_node(al, &n);
	if (r == result_error) {
		goto function_error;
	}
	n->type = dag_type_for_iteration;

	/* id */
	struct dag_node* a = NULL;
	r = dag_create_node(al, &a);
	if (r == result_error) {
		goto function_error;
	}
	a->type = dag_type_id;
	r = buffer_copy(al, &t1->value, &a->value);
	if (r == result_error) {
		goto function_error;
	}
	dag_add_child(n, a);

	/* expr */
	struct dag_node* b = NULL;
	r = expr(al, ps, &b);
	if (r == result_error) {
		goto function_error;
	}
	if (!b) {
		r = set_source_error(t0, ps->sns->lc, "expected expression after for-iteration");
		goto function_error;
	}
	dag_add_child(n, b);

	/* stmts */
	struct dag_node* c = NULL;
	r = stmts(al, ps, &c);
	if (r == result_error) {
		goto function_error;
	}
	dag_add_child(n, c);

	r = match(al, ps, token_end, "expected end", &t);
	if (r == result_error) {
		goto function_error;
	}

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
*  elseif_stmts -> elseif expr \n stmts elseif_stmts
*				 | e
*/
enum result elseif_stmts(struct allocator* al, struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;
	struct token* t;

	r = get_lookahead(al, ps, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0->type == token_elseif) {
		r = match(al, ps, token_elseif, "expecting elseif", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* cb = NULL;
		r = dag_create_node(al, &cb);
		if (r == result_error) {
			goto function_error;
		}
		cb->type = dag_type_conditional_branch;

		struct dag_node* cond = NULL;
		r = expr(al, ps, &cond);
		if (r == result_error) {
			goto function_error;
		}
		if (cond == NULL) {
			r = set_source_error(t0, ps->sns->lc, "expecting condition after elseif");
			goto function_error;
		}
		dag_add_child(cb, cond);

		struct dag_node* node = NULL;
		r = stmts(al, ps, &node);
		if (r == result_error) {
			goto function_error;
		}
		if (node) {
			dag_add_child(cb, node);
		}

		dag_add_child(parent, cb);

		r = elseif_stmts(al, ps, parent);
		if (r == result_error) {
			goto function_error;
		}
	}

function_success:
	return r;

function_error:
	return r;
}

/*
* else_stmt -> else \n stmts
*			 | e
*/
enum result else_stmt(struct allocator* al, struct parse_state* ps, struct dag_node* parent)
{
	enum result r = result_ok;
	int num;
	struct token* t;

	r = get_lookahead(al, ps, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	struct token* t0 = get_token(&ps->lookahead, 0);
	if (t0 && t0->type == token_else) {
		/* else */
		r = match(al, ps, token_else, "expected else", &t);
		if (r == result_error) {
			goto function_error;
		}

		struct dag_node* cb = NULL;
		r = dag_create_node(al, &cb);
		if (r == result_error) {
			goto function_error;
		}
		cb->type = dag_type_default_branch;

		/* stmts */
		struct dag_node* node = NULL;
		r = stmts(al, ps, &node);
		if (r == result_error) {
			goto function_error;
		}
		if (node) {
			dag_add_child(cb, node);
		}

		dag_add_child(parent, cb);
	}

function_error:
	return r;

function_success:
	return r;
}
