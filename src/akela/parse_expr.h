#ifndef AKELA_PARSE_EXPR_H
#define AKELA_PARSE_EXPR_H

#include "api.h"
#include <stdbool.h>
#include "zinc/result.h"
#include "token.h"
#include "ast.h"
#include "parse_tools.h"

AKELA_API Ake_ast* Ake_parse_expr(struct Ake_parse_state* ps);
AKELA_API Ake_ast* Ake_parse_simple_expr(struct Ake_parse_state* ps);

#endif
