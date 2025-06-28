#ifndef	AKELA_PARSE_FACTOR_H
#define AKELA_PARSE_FACTOR_H

#include "api.h"

AKELA_API Ake_Ast* Ake_parse_factor(struct Ake_parse_state* ps);
AKELA_API Ake_Ast* Ake_parse_function(struct Ake_parse_state* ps, bool is_method, Ake_Type* struct_type);

#endif
