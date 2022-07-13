#ifndef _SOURCE_H
#define _SOURCE_H

#include <stdarg.h>
#include <stdlib.h>
#include "result.h"
#include "input.h"
#include "token.h"

enum result set_source_error(struct token* t, struct lookahead_char* lc, const char* fmt, ...);

#endif
