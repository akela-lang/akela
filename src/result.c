#include <string.h>
#include "result.h"

char error_message[ERROR_SIZE];

enum result_enum set_error(char* error)
{
    strncpy(error_message, error, ERROR_SIZE);
    return error_result;
}
