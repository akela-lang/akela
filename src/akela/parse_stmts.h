#ifndef AKELA_PARSE_STMTS_H
#define AKELA_PARSE_STMTS_H

#include "api.h"
#include <stdbool.h>
#include "parse_tools.h"

AKELA_API Ake_ast* Ake_parse_stmts(struct Ake_parse_state* ps, bool suppress_env);

#endif
