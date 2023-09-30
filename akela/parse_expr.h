#ifndef _EXPR_H
#define _EXPR_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_tools.h"
#include "source.h"

/* dynamic-output ps{} root root{} */
AKELA_API bool expr(struct parse_state* ps, struct ast_node** root, struct location* loc);
AKELA_API struct ast_node* parse_simple_expr(struct parse_state* ps, struct location* loc);

#endif
