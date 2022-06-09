#include "result.h"
#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "ustring.h"
#include "expr.h"

/*
* stmt -> word = expr
*       | expr
*       | e
*/
enum result stmt(struct allocator* al, struct token_list* tl, struct dag_node** root)
{
	enum result r;
	struct dag_node* n = NULL;
	struct token* t0 = get_token(tl->head, 0);

	/* e */
	if (t0 == NULL) {
		goto function_success;
	}

	/* word = expr */
	struct token* t1 = get_token(tl->head, 1);
	if (t0 && t0->type == token_word && t1 && t1->type == token_equal) {
		token_list_pop(tl);
		token_list_pop(tl);

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
		r = string_copy(al, &t0->value, &a->value);
		if (r == result_error) {
			goto function_error;
		}
		dag_add_child(n, a);

		struct dag_node* b;
		r = expr(al, tl, &b);
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
		r = expr(al, tl, &n);
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
