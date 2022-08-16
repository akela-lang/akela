#ifndef _STMT_H
#define _STMT_H

#include "alba_api.h"
#include <stdbool.h>
#include "parse_tools.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool stmts(struct parse_state* ps, bool suppress_env, struct ast_node** root);

#endif
