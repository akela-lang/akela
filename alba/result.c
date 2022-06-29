#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "result.h"

char error_message[ERROR_SIZE];

enum result set_error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buf[ERROR_SIZE] = "";
    int len;

    char last = 0;
    int i = 0;
    while (*fmt != '\0') {
        if (last == '%' && *fmt == '%') {
            if (i < ERROR_SIZE) error_message[i++] = '%';
        } else if (*fmt == '%') {
            /* nothing */
        } else if (last == '%' && *fmt == 'd') {
            len = snprintf(buf, ERROR_SIZE, "%d", va_arg(args, int));
            for (int j = 0; j < len; j++) {
                if (i < ERROR_SIZE) error_message[i++] = buf[j];
            }
        } else if (last == '%' && *fmt == 's') {
            len = snprintf(buf, ERROR_SIZE, "%s", va_arg(args, char*));
            for (int j = 0; j < len; j++) {
                if (i < ERROR_SIZE) error_message[i++] = buf[j];
            }
        } else {
            if (i < ERROR_SIZE) error_message[i++] = *fmt;
        }
        last = *fmt;
        fmt++;
    }
    error_message[i++] = '\0';

    va_end(args);

    return result_error;
}
