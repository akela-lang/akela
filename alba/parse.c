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
	struct defer_node* cleanup_stack = NULL;
	char* token_name[token_count];
	token_name_init(token_name);

	/* expr + term */
	i = token_find_last(tl->tail, token_plus);
	if (i >= 0) {
		struct token_list* before;
		struct token_list* after;
		printf("plus: %d\n", i);

		r = token_list_slice(tl, -1, i - 1, &before);
		if (r == result_error) {
			return r;
		}
		r = defer(token_list_destroy, before, &cleanup_stack);
		if (r == result_error) {
			token_list_destroy(before);
			return r;
		}

		r = token_list_slice(tl, i + 1, -1, &after);
		if (r == result_error) {
			cleanup(cleanup_stack);
			return r;
		}
		r = defer(token_list_destroy, after, &cleanup_stack);
		if (r == result_error) {
			token_list_destroy(after);
			return r;
		}

		r = token_list_print(before, token_name);
		if (r == result_error) {
			cleanup(cleanup_stack);
			return r;
		}
		r = token_list_print(after, token_name);
		if (r == result_error) {
			cleanup(cleanup_stack);
			return r;
		}

		cleanup(cleanup_stack);
		return result_ok;
	}

	/* expr - term */
	i = token_find_last(tl->tail, token_minus);
	if (i >= 0) {
		printf("minus: %d\n", i);
		return result_ok;
	}

	/* term */
	printf("term: %d\n", i);
	r = term(tl, root);
	return r;
}

enum result term(struct token_list* tl, struct dag_node** root)
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
