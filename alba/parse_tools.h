#ifndef _PARSE_TOOLS_H
#define _PARSE_TOOLS_H

#include "token.h"

struct parse_state {
	struct scan_state* sns;
	struct token_list lookahead;
	struct compile_error_list* el;
};

void parse_state_init(struct parse_state* ps, struct scan_state* sns, struct compile_error_list* el);
enum parse_result get_lookahead(struct allocator* al, struct parse_state* ps, int count, int* num);
enum result match(struct allocator* al, struct parse_state* ps, enum token_type type, char* reason, struct token** t);

#endif
