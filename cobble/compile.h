#ifndef REGEX_COMPILE_H
#define REGEX_COMPILE_H

#include <stdbool.h>
#include "compile_data.h"
#include "Ast_node.h"

bool compile(struct Compile_data* cd, Ast_node** root);

#endif
