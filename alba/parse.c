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

	/* expr + term */
	i = token_find_last(tl->tail, token_plus);
	if (i >= 0) {
		printf("plus: %d\n", i);
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
