#ifndef WORM_PARSE_TOOLS_H
#define WORM_PARSE_TOOLS_H

#include "parse_data.h"
#include "token.h"
#include "ast.h"

void Worm_lookahead(Worm_parse_data* pd);
bool Worm_match(Worm_parse_data* pd,
    Worm_token_type type,
    char const* message,
    Worm_token** t,
    Worm_ast* n);

#endif
