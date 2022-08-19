#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"

/* dynamic-output ps{} root root{} */
ALBA_API bool dseq(struct parse_state* ps, struct ast_node** root);

/* dynamic-output ps{} root root{} */
ALBA_API bool declaration(struct parse_state* ps, struct ast_node** root);

ALBA_API bool type(struct parse_state* ps, struct token* id, struct ast_node** root);

ALBA_API struct ast_node* af2etype(struct ast_node* n);

#endif
