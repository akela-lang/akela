#ifndef _PARSE_H
#define _PARSE_H

#include "api.h"
#include <stdbool.h>
#include "parse_tools.h"
#include "ast.h"
#include "zinc/result.h"

AKELA_API Ake_ast* Ast_parse(struct Ake_parse_state* ps);

#endif
