#ifndef _EXPR_H
#define _EXPR_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_tools.h"
#include "source.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool expr(struct parse_state* ps, struct ast_node** root, struct location* loc);
ALBA_API bool boolean(struct parse_state* ps, struct ast_node** root, struct location* loc);

#endif
