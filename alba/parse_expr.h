#ifndef _EXPR_H
#define _EXPR_H

#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "dag.h"

/* dynamic-output ps{} root root{} */
bool expr(struct parse_state* ps, struct dag_node** root);

#endif
