#ifndef _STMT_H
#define _STMT_H

#include "akela_api.h"
#include <stdbool.h>
#include "parse_tools.h"

AKELA_API Ast_node* parse_stmts(struct parse_state* ps, bool suppress_env);

#endif
