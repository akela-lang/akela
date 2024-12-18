//
// Created by miguel on 6/6/23.
//

#ifndef COBBLE_COMPILE_TOOLS_H
#define COBBLE_COMPILE_TOOLS_H

#include "compile_data.h"
#include "token.h"
#include "stdbool.h"
#include "Ast_node.h"

void get_lookahead(struct Compile_data* cd);
bool match(
    struct Compile_data* cd,
    enum token_type type,
    const char* reason,
    struct token** t,
    struct Ast_node* n);
void location_update(struct location* dest, struct location* src);
void location_update_token(struct location* dest, struct token* t);
void location_update_default(struct location* dest, struct Compile_data* cd);

#endif //COBBLE_COMPILE_TOOLS_H
