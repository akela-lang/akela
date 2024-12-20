#ifndef _STMT_H
#define _STMT_H

#include "akela_api.h"
#include <stdbool.h>
#include "parse_tools.h"

AKELA_API Cob_ast* parse_stmts(struct parse_state* ps, bool suppress_env);

#endif
