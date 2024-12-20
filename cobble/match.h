#ifndef COBBLE_MATCH_H
#define COBBLE_MATCH_H

#include "match_tools.h"
#include "zinc/buffer_list.h"

bool Cob_match(Cob_ast* root, String_slice slice, struct buffer_list* groups);

#endif //COBBLE_MATCH_H
