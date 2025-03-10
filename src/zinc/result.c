#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "result.h"

#include "zstring.h"

char Zinc_error_message[ERROR_SIZE];

Zinc_result Zinc_set_error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Zinc_string s;
    Zinc_string_init(&s);
    Zinc_string_add_vformat(&s, fmt, args);
    size_t n = s.size;
    if (n >= ERROR_SIZE) {
        n = ERROR_SIZE - 1;
    }
    memcpy(Zinc_error_message, Zinc_string_c_str(&s), n);
    Zinc_error_message[n] = '\0';
    va_end(args);

    return Zinc_result_error;
}

char* Zinc_get_error_message()
{
    return Zinc_error_message;
}
