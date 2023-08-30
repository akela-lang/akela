#ifndef _STMT_H
#define _STMT_H

#include "akela_api.h"
#include <stdbool.h>
#include "parse_tools.h"

/* dynamic-output ps{} root root{} */
AKELA_API bool stmts(struct parse_state* ps, bool suppress_env, struct ast_node** root, struct location* loc);

#endif
