#ifndef _PARSE_TOOLS_H
#define _PARSE_TOOLS_H

#include "akela_api.h"
#include <stdbool.h>
#include "token.h"
#include "source.h"
#include "symbol_table.h"
#include "zinc/buffer.h"
#include "ast.h"

struct parse_state {
	struct scan_state* sns;
	struct token_list lookahead;
	struct error_list* el;
	struct symbol_table* st;
	struct buffer qualifier;
};

/* dynamic-output-none */
/* initialize-output ps ps{}*/
AKELA_API void parse_state_init(struct parse_state* ps, struct scan_state* sns, struct error_list* el, struct symbol_table* st);

AKELA_API void parse_state_destroy(struct parse_state* ps);

/* get lookahead token */
/* dynamic ps{} */
AKELA_API bool get_lookahead(struct parse_state* ps, int count, int* num);

AKELA_API bool get_lookahead_one(struct parse_state* ps);

/* expecting specific token */
/* dynamic-output ps{} t t{} */
AKELA_API bool match(struct parse_state* ps, enum token_enum type, const char* reason, struct token** t);

AKELA_API bool consume_newline(struct parse_state* ps);

/* dynamic-output ps{} */
AKELA_API bool get_parse_location(struct parse_state* ps, struct location* loc);

AKELA_API bool is_identity_comparison(enum ast_type type);

AKELA_API void location_update_token(struct location* loc, struct token* t);

AKELA_API void location_update(struct location* loc, struct location* loc2);

AKELA_API bool location_default(struct parse_state* ps, struct location* loc);

AKELA_API bool get_location(struct parse_state* ps, struct location* loc);

#endif
