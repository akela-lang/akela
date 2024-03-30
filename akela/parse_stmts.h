#ifndef _STMT_H
#define _STMT_H

#include "akela_api.h"
#include <stdbool.h>
#include "parse_tools.h"

/* dynamic-output ps{} root root{} */
AKELA_API struct ast_node* parse_stmts(struct parse_state* ps, bool suppress_env, struct location* loc);

#endif
