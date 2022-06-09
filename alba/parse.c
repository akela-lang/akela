#include <stdlib.h>
#include "result.h"
#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "expr.h"

enum result parse(struct allocator* al, struct token_list* tl, struct dag_node** root)
{
	*root = NULL;
	enum result r = expr(al, tl, root);
	if (r == result_error) {
		return r;
	}
	return result_ok;
}
