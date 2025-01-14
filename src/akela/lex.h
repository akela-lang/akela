#ifndef AKELA_LEX_H
#define AKELA_LEX_H

#include "api.h"
#include <stdbool.h>
#include "zinc/zstring.h"
#include "token.h"
#include "symbol_table.h"
#include "lex_tools.h"

AKELA_API bool Ake_lex(struct Ake_lex_state* ls, struct Ake_token** t);

#endif
