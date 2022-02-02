#include <stdlib.h>
#include "dag.h"
#include "result.h"
#include "defer.h"
#include "scan.h"

enum result_enum parse(struct token_list* tl, struct dag_node** root)
{
	struct defer_node* ds = NULL;
	enum result_enum r = dag_create_node(root);
	if (r == error_result) {
		return r;
	}
	dag_init_node(*root);

	cleanup(ds);
	return ok_result;
}
