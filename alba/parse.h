#ifndef _PARSE_H
#define _PARSE_H

#include "alba_api.h"
#include <stdbool.h>
#include "parse_tools.h"
#include "ast.h"
#include "zinc/result.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool parse(struct parse_state* ps, struct ast_node** root);

#endif
