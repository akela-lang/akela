#ifndef _SOURCE_H
#define _SOURCE_H

#include <stdarg.h>
#include <stdlib.h>
#include "result.h"

enum result set_source_error(size_t line, size_t col, const char* fmt, ...);

#endif
