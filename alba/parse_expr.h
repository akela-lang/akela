#ifndef _EXPR_H
#define _EXPR_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "dag.h"
#include "parse_tools.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool expr(struct parse_state* ps, struct dag_node** root);

#endif
