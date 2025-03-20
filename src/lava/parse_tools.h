#ifndef LAVA_PARSE_TOOLS_H
#define LAVA_PARSE_TOOLS_H

#include "parse_data.h"

void Lava_lookahead(Lava_parse_data* pd);
bool Lava_match(
    Lava_parse_data* pd,
    Lava_token_kind kind,
    const char* message,
    Lava_token** t,
    Lava_dom* dom);

#endif