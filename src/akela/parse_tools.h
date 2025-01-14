#ifndef _PARSE_TOOLS_H
#define _PARSE_TOOLS_H

#include "api.h"
#include <stdbool.h>
#include "token.h"
#include "symbol_table.h"
#include "zinc/buffer.h"
#include "ast.h"

struct parse_state {
	struct Ake_lex_state* ls;
	struct token* lookahead;
	struct error_list* el;
	struct symbol_table* st;
    struct buffer_list* extern_list;
};

AKELA_API void parse_state_init(
        struct parse_state* ps,
        struct Ake_lex_state* ls,
        struct error_list* el,
        struct buffer_list* extern_list,
        struct symbol_table* st);
AKELA_API void parse_state_destroy(struct parse_state* ps);
AKELA_API bool match(
    struct parse_state* ps,
    enum token_enum type,
    const char* reason,
    struct token** t,
    Ake_ast* n);
AKELA_API bool consume_newline(struct parse_state* ps, Ake_ast* n);
AKELA_API bool is_identity_comparison(enum Ake_ast_type type);
AKELA_API struct location get_location(struct parse_state* ps);
AKELA_API struct token* get_lookahead(struct parse_state* ps);
AKELA_API bool check_assignment_value_count(Ake_ast* a, Ake_ast* b);
AKELA_API void parse_separator(
    struct parse_state* ps,
    Ake_ast* n,
    bool* has_separator);

#endif
