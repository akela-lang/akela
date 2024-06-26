#ifndef	_PARSE_FACTOR_H
#define _PARSE_FACTOR_H

#include "akela_api.h"

AKELA_API Ast_node* parse_factor(struct parse_state* ps);
AKELA_API Ast_node* parse_function(struct parse_state* ps, bool is_method);

#endif
