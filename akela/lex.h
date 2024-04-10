#ifndef _SCAN_H
#define _SCAN_H

#include "akela_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "token.h"
#include "lookahead_char.h"
#include "source.h"
#include "symbol_table.h"
#include "zinc/input_char.h"
#include "lex_tools.h"

AKELA_API bool lex(struct scan_state* sns, struct token** t);

#endif
