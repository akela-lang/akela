#include <stdlib.h>
#include "dag.h"
#include "result.h"
#include "defer.h"
#include "scan.h"

/*
* expr ->
*	expr + term
*	expr - term
*	term
*/
enum result expr(struct token_list* tl, struct dag_node** root)
{
	enum result r;
	int i;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;

	/* expr + term */
	i = token_find_last(tl->tail, token_plus);
	if (i >= 0) {
		struct token_list* before;
		struct token_list* after;

		r = token_list_slice(tl, -1, i - 1, &before);
		if (r == result_error) {
			return r;
		}
		r = defer(token_list_destroy, before, &stack_temp);
		if (r == result_error) {
			token_list_destroy(before);
			return r;
		}

		r = token_list_slice(tl, i + 1, -1, &after);
		if (r == result_error) {
			cleanup(stack_error);
			return r;
		}
		r = defer(token_list_destroy, after, &stack_temp);
		if (r == result_error) {
			token_list_destroy(after);
			cleanup(stack_error);
			return r;
		}

		// new node
		struct dag_node* n;
		r = dag_create_node(&n);
		if (r == result_error) {
			cleanup(stack_error);
			cleanup_stack(stack_temp);
			return r;
		}
		r = defer(dag_destroy, n, &stack_error);
		if (r == result_error) {
			cleanup(stack_error);
			cleanup_stack(stack_temp);
			return r;
		}

		// left child
		struct dag_node* left = NULL;
		r = expr(before, &left);
		if (r == result_error)
		{
			cleanup(stack_error);
			cleanup_stack(stack_temp);
			return r;
		}
		if (left) {
			dag_add_child(n, left);

			// operator
			struct dag_node* op;
			r = dag_create_node(&op);
			if (r == result_error) {
				cleanup(stack_error);
				cleanup_stack(stack_temp);
				return r;
			}
			op->type = dag_type_plus;
			dag_add_child(n, op);

			// right child
			struct dag_node* right = NULL;
			r = term(after, &n);
			if (r == result_error) {
				cleanup(stack_error);
				cleanup_stack(stack_temp);
				return r;
			}
			if (right) {
				dag_add_child(n, right);
				*root = n;
				cleanup(stack_temp);
				cleanup_stack(stack_error);
				return result_ok;
			} else {
				dag_destroy(n);
			}
		} else {
			dag_destroy(n);
		}
	}

	/* expr - term */
	i = token_find_last(tl->tail, token_minus);
	if (i >= 0) {
		printf("minus: %d\n", i);
		return result_ok;
	}

	/* term */
	r = term(tl, root);
	return r;
}

enum result term(struct token_list* tl, struct dag_node** root)
{
	enum result r;
	return result_ok;
}

enum result factor(struct token_list* tl, struct dag_node** root)
{
	enum result r;
	return result_ok;
}

enum result parse(struct token_list* tl, struct dag_node** root)
{
	*root = NULL;
	enum result r = expr(tl, root);
	if (r == result_error) {
		return r;
	}
	return result_ok;
}
