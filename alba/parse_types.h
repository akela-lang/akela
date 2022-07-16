#ifndef _PARSE_TYPES_H
#define _PARSE_TYPES_H

#include "zinc/buffer.h"

/* dynamic-output ps{} root root{} */
enum result dseq(struct parse_state* ps, struct dag_node** root);

/* static-output */
int is_valid_type(struct buffer* b);

/* dynamic-output ps{} root root{} */
enum result declaration(struct parse_state* ps, struct dag_node** root);

#endif
