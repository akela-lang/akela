#ifndef	_PARSE_FACTOR_H
#define _PARSE_FACTOR_H

#include "akela_api.h"

AKELA_API Ake_ast* parse_factor(struct parse_state* ps);
AKELA_API Ake_ast* parse_function(struct parse_state* ps, bool is_method, Ake_ast* struct_type);

#endif
