#ifndef _TEST_CODE_GEN_TOOLS_H
#define _TEST_CODE_GEN_TOOLS_H

#include <stdbool.h>
#include "zinc/zstring.h"
#include "akela/code_gen.h"

bool AkeLlvmUnit_cg_setup(const char* text, Ake_code_gen_result* result);

#endif
