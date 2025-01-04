//
// Created by miguel on 6/6/23.
//

#ifndef COBBLE_COMPILE_TOOLS_H
#define COBBLE_COMPILE_TOOLS_H

#include "compile_data.h"
#include "token.h"
#include "stdbool.h"
#include "ast.h"

void Cob_lookahead(Cob_compile_data* cd);
bool Cob_match_token(
    Cob_compile_data* cd,
    Cob_token_type type,
    const char* reason,
    Cob_token** t,
    Cob_ast* n);
void Cob_location_update(struct location* dest, struct location* src);
void Cob_location_update_token(struct location* dest, Cob_token* t);

#endif //COBBLE_COMPILE_TOOLS_H
