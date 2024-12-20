#ifndef REGEX_COMPILE_H
#define REGEX_COMPILE_H

#include <stdbool.h>
#include "compile_data.h"
#include "Ast_node.h"

Cob_re compile(Compile_data* cd);

#endif
