#ifndef ZINC_RESULT_H
#define ZINC_RESULT_H

#include "api.h"

#define ERROR_SIZE 100
typedef enum Zinc_result {
    Zinc_result_error = 0,
    Zinc_result_ok = 1
} Zinc_result;

ZINC_API extern char Zinc_error_message[ERROR_SIZE];

ZINC_API Zinc_result Zinc_set_error(const char* fmt, ...);
ZINC_API char* Zinc_get_error_message();

#endif
