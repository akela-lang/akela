#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "result.h"

char Zinc_error_message[ERROR_SIZE];

Zinc_result Zinc_set_error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[ERROR_SIZE] = "";
    int len;

    char last_last = 0;
    char last = 0;
    int i = 0;
    while (*fmt != '\0') {
        if (last == '%' && *fmt == '%') {
            if (i < ERROR_SIZE) Zinc_error_message[i++] = '%';
        } else if (*fmt == '%') {
            /* nothing */
        } else if (last == '%' && *fmt == 'z') {
            /* nothing */
        } else if (last == '%' && *fmt == 'd') {
            len = snprintf(buf, ERROR_SIZE, "%d", va_arg(args, int));
            for (int j = 0; j < len; j++) {
                if (i < ERROR_SIZE) Zinc_error_message[i++] = buf[j];
            }
        } else if (last_last == '%' && last == 'z' && *fmt == 'u') {
            len = snprintf(buf, ERROR_SIZE, "%zu", va_arg(args, size_t));
            for (int j = 0; j < len; j++) {
                if (i < ERROR_SIZE) Zinc_error_message[i++] = buf[j];
            }
        } else if (last == '%' && *fmt == 's') {
            len = snprintf(buf, ERROR_SIZE, "%s", va_arg(args, char*));
            for (int j = 0; j < len; j++) {
                if (i < ERROR_SIZE) Zinc_error_message[i++] = buf[j];
            }
        } else if (last == '%' && *fmt == 'c') {
            len = snprintf(buf, ERROR_SIZE, "%c", va_arg(args, int));
            for (int j = 0; j < len; j++) {
                if (i < ERROR_SIZE) Zinc_error_message[i++] = buf[j];
            }
        } else {
            if (i < ERROR_SIZE) Zinc_error_message[i++] = *fmt;
        }
        last_last = last;
        last = *fmt;
        fmt++;
    }
    if (i >= ERROR_SIZE) {
        i = ERROR_SIZE - 1;
    }
    Zinc_error_message[i++] = '\0';

    va_end(args);

    return Zinc_result_error;
}

char* Zinc_get_error_message()
{
    return Zinc_error_message;
}
