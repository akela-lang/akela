#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include <stdbool.h>
#include "zinc/buffer.h"

/* dynamic-output ps{} root root{} */
bool dseq(struct parse_state* ps, struct dag_node** root);

/* dynamic-output-none */
bool is_valid_type(struct buffer* b);

/* dynamic-output ps{} root root{} */
bool declaration(struct parse_state* ps, struct dag_node** root);

bool type(struct parse_state* ps, struct token* id, struct dag_node** root);

#endif
