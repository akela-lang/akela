#ifndef _SCAN_H
#define _SCAN_H

#include "api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "token.h"
#include "symbol_table.h"
#include "lex_tools.h"

AKELA_API bool Ake_lex(struct Ake_lex_state* ls, struct token** t);

#endif
