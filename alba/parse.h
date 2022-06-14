#ifndef _PARSE_H
#define _PARSE_H

#include "allocator.h"
#include "token.h"
#include "dag.h"
#include "result.h"

struct token_state {
	struct input_state* is;
	struct token_list lookahead;
};

enum result parse(struct allocator* al, struct token_state* ts, struct dag_node** root);
void token_state_init(struct input_state* is, struct token_state* ts);
enum result get_lookahead(struct allocator* al, struct token_state* ts, int count, int* num);
enum result match(struct allocator* al, struct token_state* ts, enum token_type type, char* reason);

#endif
