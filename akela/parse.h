#ifndef _PARSE_H
#define _PARSE_H

#include "akela_api.h"
#include <stdbool.h>
#include "parse_tools.h"
#include "ast.h"
#include "zinc/result.h"

AKELA_API struct ast_node* parse(struct parse_state* ps);

#endif
