#ifndef _PARSE_TOOLS_H
#define _PARSE_TOOLS_H

#include "alba_api.h"
#include <stdbool.h>
#include "token.h"
#include "source.h"
#include "symbol_table.h"

struct parse_state {
	struct scan_state* sns;
	struct token_list lookahead;
	struct compile_error_list* el;
	struct symbol_table* st;
};

/* dynamic-output-none */
/* initialize-output ps ps{}*/
ALBA_API void parse_state_init(struct parse_state* ps, struct scan_state* sns, struct compile_error_list* el, struct symbol_table* st);

/* get lookahead token */
/* dynamic ps{} */
ALBA_API bool get_lookahead(struct parse_state* ps, int count, int* num);

/* expecting specific token */
/* dynamic-output ps{} t t{} */
ALBA_API bool match(struct parse_state* ps, enum token_enum type, char* reason, struct token** t);

/* dynamic-output ps{} */
ALBA_API bool get_parse_location(struct parse_state* ps, struct location* loc);

ALBA_API bool is_identity_comparison(enum ast_type type);

ALBA_API void update_location_token(struct ast_node* n, struct token* t);

ALBA_API void update_location_ast_node(struct ast_node* n, struct ast_node* a);

ALBA_API void update_location(struct location* loc, struct location* loc2);

ALBA_API bool default_location(struct parse_state* ps, struct ast_node* n);

ALBA_API bool parse_ast_node_finish(struct parse_state* ps, struct ast_node* n);

#endif
