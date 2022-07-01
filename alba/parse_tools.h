#ifndef _PARSE_TOOLS_H
#define _PARSE_TOOLS_H

#include "token.h"

struct token_state {
	struct input_state* is;
	struct token_list lookahead;
};

void token_state_init(struct input_state* is, struct token_state* ts);
enum result get_lookahead(struct allocator* al, struct token_state* ts, int count, int* num);
enum result match(struct allocator* al, struct token_state* ts, enum token_type type, char* reason, struct token** t);

#endif
