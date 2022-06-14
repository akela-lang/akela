#include "result.h"
#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "buffer.h"
#include "expr.h"
#include "input.h"
#include "scan.h"
#include "parse.h"

/*
* stmts -> stmt stmts'
*		 | e
*/
enum result stmts(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;

	/* stmt stmts' */
	r = dag_create_node(al, &n);
	if (r == result_error) {
		goto function_error;
	}

	n->type = dag_type_stmts;

	struct dag_node* a = NULL;
	r = stmt(al, ts, &a);
	if (r == result_error) {
		goto function_error;
	}

	struct dag_node* b = NULL;
	r = stmts_prime(al, ts, &b);
	if (r == result_error) {
		goto function_error;
	}

	if (a) {
		dag_add_child(n, a);
	}

	if (b && b->type == dag_type_stmts) {
		for (struct dag_node* x = b->head; x; x = x->next) {
			dag_add_child(n, b);
		}
	}	else if (b) {
		dag_add_child(n, b);
	}

	/* single statement */
	struct dag_node* n1 = dag_get_child(n, 1);
	if (!n1) {
		struct dag_node* n0 = dag_get_child(n, 0);
		n = n0;
	}

	/* e */

function_success:
	*root = n;
	return r;

function_error:
	return r;
}

/*
* stmts' -> \n stmts
*		  | e
*/
enum result stmts_prime(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r;
	struct dag_node* n = NULL;
	int num;

	r = get_lookahead(al, ts, 1, &num);
	if (r == result_error) {
		goto function_error;
	}

	/* e */
	if (num <= 0) {
		goto function_success;
	}

	/* \n stmts */
	struct token* t = get_token(ts->lookahead.head, 0);
	if (t->type == token_newline) {
		r = match(al, ts, token_newline, "expecting newline");
		if (r == result_error) {
			return r;
		}

		r = stmts(al, ts, &n);
		if (r == result_error) {
			goto function_error;
		}
		goto function_success;
	}

	r = set_error("expected stmts");
	goto function_error;

function_success:
	*root = n;
	return result_ok;

function_error:
	return r;
}

/*
* stmt -> word = expr
*       | expr
*       | e
*/
enum result stmt(struct allocator* al, struct token_state* ts, struct dag_node** root)
{
	enum result r;
	struct dag_node* n = NULL;
	int num;

	r = get_lookahead(al, ts, 2, &num);
	if (r == result_error) {
		goto function_error;
	}

	/* e */
	if (num <= 0) {
		goto function_success;
	}


	/* word = expr */
	struct token* t0 = get_token(ts->lookahead.head, 0);
	struct token* t1 = get_token(ts->lookahead.head, 1);
	if (t0 && t0->type == token_word && t1 && t1->type == token_equal) {
		r = match(al, ts, token_word, "expected word");
		if (r == result_error) {
			goto function_error;
		}
		r = match(al, ts, token_equal, "expected equal");
		if (r == result_error) {
			goto function_error;
		}

		r = dag_create_node(al, &n);
		if (r == result_error) {
			goto function_error;
		}
		n->type = dag_type_assign;

		struct dag_node* a;
		r = dag_create_node(al, &a);
		if (r == result_error) {
			goto function_error;
		}
		a->type = dag_type_word;
		r = buffer_copy(al, &t0->value, &a->value);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, a);

		struct dag_node* b;
		r = expr(al, ts, &b);
		if (r == result_error) {
			goto function_error;
		}
		if (b == NULL) {
			r = set_error("expect expression");
			goto function_error;
		}
		dag_add_child(n, b);
		goto function_success;

	/* expr */
	} else {
		r = expr(al, ts, &n);
		if (r == result_error) {
			goto function_error;
		}
		if (n == NULL) {
			r = set_error("expected expression");
			goto function_error;
		}

		

		goto function_success;
	}

	r = set_error("expected statement");
	goto function_error;

function_success:
	*root = n;
	return result_ok;

function_error:
	return r;
}
