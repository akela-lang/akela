#ifndef _PARSE_TOOLS_H
#define _PARSE_TOOLS_H

#include <stdbool.h>
#include "token.h"
#include "source.h"
#include "symbol_table.h"

struct parse_state {
	struct scan_state* sns;
	struct token_list lookahead;
	struct compile_error_list* el;
	struct environment* top;
};

/* dynamic-output-none */
/* initialize-output ps ps{}*/
void parse_state_init(struct parse_state* ps, struct scan_state* sns, struct compile_error_list* el);

/* get lookahead token */
/* dynamic ps{} */
bool get_lookahead(struct parse_state* ps, int count, int* num);

/* expecting specific token */
/* dynamic-output ps{} t t{} */
bool match(struct parse_state* ps, enum token_type type, char* reason, struct token** t);

/* dynamic-output ps{} */
bool get_parse_location(struct parse_state* ps, struct location* loc);

#endif
