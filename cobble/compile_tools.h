//
// Created by miguel on 6/6/23.
//

#ifndef COBBLE_COMPILE_TOOLS_H
#define COBBLE_COMPILE_TOOLS_H

#include "compile_data.h"
#include "token.h"
#include "stdbool.h"
#include "ast.h"

void get_lookahead(struct Cob_compile_data* cd);
bool match(
    struct Cob_compile_data* cd,
    enum token_type type,
    const char* reason,
    struct token** t,
    struct Cob_ast* n);
void location_update(struct location* dest, struct location* src);
void location_update_token(struct location* dest, struct token* t);
void location_update_default(struct location* dest, struct Cob_compile_data* cd);

#endif //COBBLE_COMPILE_TOOLS_H
