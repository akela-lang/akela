//
// Created by miguel on 6/6/23.
//

#ifndef COBBLE_TEST_COMPILE_TOOLS_H
#define COBBLE_TEST_COMPILE_TOOLS_H

#include "cobble/compile_data.h"
#include "cobble/Ast_node.h"

void setup_compile(Cob_compile_data** cd, const char* s);
void teardown_compile(Cob_compile_data* cd, Cob_re* cr);

#endif //COBBLE_TEST_COMPILE_TOOLS_H
