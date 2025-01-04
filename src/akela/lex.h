#ifndef _SCAN_H
#define _SCAN_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "token.h"
#include "symbol_table.h"
#include "lex_tools.h"

AKELA_API bool lex(struct lex_state* ls, struct token** t);

#endif
