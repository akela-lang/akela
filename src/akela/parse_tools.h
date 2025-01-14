#ifndef AKELA_PARSE_TOOLS_H
#define AKELA_PARSE_TOOLS_H

#include "api.h"
#include <stdbool.h>
#include "token.h"
#include "symbol_table.h"
#include "zinc/zstring.h"
#include "ast.h"

typedef struct Ake_parse_state {
	struct Ake_lex_state* ls;
	struct Ake_token* lookahead;
	struct error_list* el;
	struct Ake_symbol_table* st;
    struct Zinc_string_list* extern_list;
} Ake_parse_state;

AKELA_API void Ake_parse_state_init(
        struct Ake_parse_state* ps,
        struct Ake_lex_state* ls,
        struct error_list* el,
        struct Zinc_string_list* extern_list,
        struct Ake_symbol_table* st);
AKELA_API void Ake_parse_state_destroy(struct Ake_parse_state* ps);
AKELA_API bool Ake_match(
    struct Ake_parse_state* ps,
    enum Ake_token_enum type,
    const char* reason,
    struct Ake_token** t,
    Ake_ast* n);
AKELA_API bool Ake_consume_newline(struct Ake_parse_state* ps, Ake_ast* n);
AKELA_API bool Ake_is_identity_comparison(enum Ake_ast_type type);
AKELA_API struct location Ake_get_location(struct Ake_parse_state* ps);
AKELA_API struct Ake_token* Ake_get_lookahead(struct Ake_parse_state* ps);
AKELA_API bool Ake_check_assignment_value_count(Ake_ast* a, Ake_ast* b);
AKELA_API void Ake_parse_separator(
    struct Ake_parse_state* ps,
    Ake_ast* n,
    bool* has_separator);

#endif
