#ifndef	_PARSE_FACTOR_H
#define _PARSE_FACTOR_H

#include "akela_api.h"

/* dynamic-output ps{} root root{} */
AKELA_API bool factor(struct parse_state* ps, struct ast_node** root, struct location* loc);
AKELA_API bool anonymous_function(struct parse_state* ps, struct ast_node** root, struct location* loc);

#endif
