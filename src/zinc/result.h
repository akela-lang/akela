#ifndef _RESULT_H
#define _RESULT_H

#include "api.h"

#define ERROR_SIZE 100
enum result {
    result_error = 0,
    result_ok = 1
};

ZINC_API extern char error_message[ERROR_SIZE];

/* static-output */
ZINC_API enum result set_error(const char* fmt, ...);

ZINC_API char* get_error_message();

#endif
