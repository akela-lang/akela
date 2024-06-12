#ifndef _EXPR_H
#define _EXPR_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_tools.h"

/* dynamic-output ps{} root root{} */
AKELA_API Ast_node* parse_expr(struct parse_state* ps, struct location* loc);
AKELA_API Ast_node* parse_simple_expr(struct parse_state* ps, struct location* loc);

#endif
