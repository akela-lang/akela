#ifndef	_PARSE_FACTOR_H
#define _PARSE_FACTOR_H

#include "akela_api.h"

/* dynamic-output ps{} root root{} */
AKELA_API struct ast_node* factor(struct parse_state* ps, struct location* loc);
AKELA_API struct ast_node* parse_anonymous_function(struct parse_state* ps, struct location* loc);

#endif
