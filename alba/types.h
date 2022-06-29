#ifndef _TYPES_H
#define _TYPES_H

#include "buffer.h"

int is_valid_type(struct buffer* b);
enum result dseq(struct allocator* al, struct token_state* ts, struct dag_node** root);
enum result declaration(struct allocator* al, struct token_state* ts, struct dag_node** root);

#endif
