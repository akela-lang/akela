#ifndef _TYPES_H
#define _TYPES_H

#include "zinc/buffer.h"

int is_valid_type(struct buffer* b);
enum result dseq(struct parse_state* ps, struct dag_node** root);
enum result declaration(struct allocator* al, struct parse_state* ps, struct dag_node** root);

#endif
