#ifndef CENTIPEDE_PARSE_TOOLS_H
#define CENTIPEDE_PARSE_TOOLS_H

#include <stdbool.h>
#include "parse_data.h"
#include "ast.h"
#include "lex.h"

void Cent_lookahead(Cent_parse_data* pd);
bool Cent_match(Cent_parse_data* pd, Cent_token_type type, char* message, Cent_token** t, Cent_ast* n);
void Cent_ignore_newlines(Cent_parse_data* pd);
bool Cent_has_separator(Cent_parse_data* pd, Cent_ast* n);
Cent_environment* Cent_get_environment(Cent_ast* n);
Cent_namespace_result Cent_namespace_lookup(Cent_ast* n);

#endif