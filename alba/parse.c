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
	enum result r = result_ok;
	int pos = -1;
	int add_pos = -1;
	int sub_pos = -1;
	enum dag_type op;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;
	struct dag_node* n = NULL;

	/* expr + term */
	/* expr - term */
	add_pos = token_find_last(tl->tail, token_plus);
	sub_pos = token_find_last(tl->tail, token_minus);
	if (add_pos >= sub_pos) {
		pos = add_pos;
		op = dag_type_plus;
	} else {
		pos = sub_pos;
		op = dag_type_minus;
	}
	if (pos >= 0) {
		struct token_list* before;
		struct token_list* after;

		r = token_list_slice(tl, -1, pos - 1, &before);
		if (r == result_error) {
			goto function_error;
		}
		r = defer(token_list_destroy, before, &stack_temp);
		if (r == result_error) {
			token_list_destroy(before);
			goto function_error;
		}

		r = token_list_slice(tl, pos + 1, -1, &after);
		if (r == result_error) {
			goto function_error;
		}
		r = defer(token_list_destroy, after, &stack_temp);
		if (r == result_error) {
			token_list_destroy(after);
			goto function_error;
		}

		// new node
		r = dag_create_node(&n);
		if (r == result_error) {
			goto function_error;
		}
		r = defer(dag_destroy, n, &stack_error);
		if (r == result_error) {
			goto function_error;
			return r;
		}

		// left child
		struct dag_node* left = NULL;
		r = expr(before, &left);
		if (r == result_error)
		{
			goto function_error;
		}
		if (left) {
			dag_add_child(n, left);

			// operator
			n->type = op;

			// right child
			struct dag_node* right = NULL;
			r = term(after, &right);
			if (r == result_error) {
				goto function_error;
			}
			if (right) {
				dag_add_child(n, right);
				goto function_success;
			} else {
				cleanup(stack_error);
				stack_error = NULL;
			}
		} else {
			cleanup(stack_error);
			stack_error = NULL;
		}
	}

	/* term */
	r = term(tl, &n);
	if (r == result_error) {
		goto function_error;
	} else {
		goto function_success;
	}

function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
}

enum result term(struct token_list* tl, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;

	if (token_list_count(tl) > 0) {
		r = factor(tl, &n);
		if (r == result_error) {
			goto function_error;
		}
		goto function_success;
	}

	goto function_error;

function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
}

enum result factor(struct token_list* tl, struct dag_node** root)
{
	enum result r = result_ok;
	struct dag_node* n = NULL;
	struct defer_node* stack_error = NULL;
	struct defer_node* stack_temp = NULL;

	if (token_list_count(tl) == 1) {
		struct token* t = get_token(tl->head, 0);
		if (t->type == token_number || t->type == token_word) {
			r = dag_create_node(&n);
			if (r == result_error) {
				goto function_error;
			}
			r = defer(dag_destroy, n, &stack_error);
			if (r == result_error) {
				dag_destroy(n);
				goto function_error;
			}
			if (t->type == token_word) {
				n->type = dag_type_word;
			} else {
				n->type = dag_type_number;
			}
			r = string_copy(&t->value, &n->value);
			if (r == result_error) {
				goto function_error;
			}
			goto function_success;
		}
	}

	goto function_error;

function_success:
	*root = n;
	cleanup(stack_temp);
	cleanup_stack(stack_error);
	return r;

function_error:
	cleanup(stack_error);
	cleanup(stack_temp);
	return r;
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
