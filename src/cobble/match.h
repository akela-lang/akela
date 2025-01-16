#ifndef COBBLE_MATCH_H
#define COBBLE_MATCH_H

#include "match_tools.h"
#include "zinc/string_list.h"
#include "compile_tools.h"

Cob_result Cob_match(Cob_re* re, Zinc_string_slice slice);

#endif //COBBLE_MATCH_H
