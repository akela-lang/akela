#ifndef REGEX_COMPILE_H
#define REGEX_COMPILE_H

#include <stdbool.h>
#include "compile_data.h"
#include "ast.h"

Cob_re Cob_compile(Zinc_string* text);
Cob_re Cob_compile_str(char const* s);

#endif
