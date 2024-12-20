#ifndef REGEX_COMPILE_H
#define REGEX_COMPILE_H

#include <stdbool.h>
#include "compile_data.h"
#include "Ast_node.h"

Cob_re Cob_compile(Cob_compile_data* cd);

#endif
