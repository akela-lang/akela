#ifndef AKELA_PARSE_TOOLS_H
#define AKELA_PARSE_TOOLS_H

#include "api.h"
#include <stdbool.h>
#include "token.h"
#include "symbol_table.h"
#include "zinc/zstring.h"
#include "zinc/string_list.h"
#include "ast.h"
#include "lex_tools.h"

typedef struct Ake_parse_context {
    bool is_subscript;
} Ake_parse_context;

typedef struct Ake_parse_state {
	Ake_lex_state* ls;
	Ake_token* lookahead;
	Zinc_error_list* el;
	Ake_symbol_table* st;
    Zinc_string_list* extern_list;
    Ake_parse_context context;
} Ake_parse_state;

AKELA_API void Ake_parse_state_init(
        struct Ake_parse_state* ps,
        struct Ake_lex_state* ls,
        struct Zinc_error_list* el,
        struct Zinc_string_list* extern_list,
        struct Ake_symbol_table* st);
AKELA_API void Ake_parse_state_create(
    Ake_parse_state** ps,
    Ake_lex_state* ls,
    Zinc_error_list* el,
    Zinc_string_list* extern_list,
    Ake_symbol_table* st);
AKELA_API void Ake_parse_state_destroy(struct Ake_parse_state* ps);
AKELA_API bool Ake_match(
    struct Ake_parse_state* ps,
    enum Ake_token_enum type,
    const char* reason,
    struct Ake_token** t,
    Ake_ast* n);
AKELA_API bool Ake_consume_newline(struct Ake_parse_state* ps, Ake_ast* n);
AKELA_API bool Ake_is_identity_comparison(enum Ake_ast_type type);
AKELA_API struct Zinc_location Ake_get_location(struct Ake_parse_state* ps);
AKELA_API struct Ake_token* Ake_get_lookahead(struct Ake_parse_state* ps);
AKELA_API bool Ake_check_assignment_value_count(Ake_ast* a, Ake_ast* b);
AKELA_API void Ake_parse_separator(
    struct Ake_parse_state* ps,
    Ake_ast* n,
    bool* has_separator);

#endif
