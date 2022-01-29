#include <stdio.h>
#include <stdlib.h>
#include "result.h"

#define BUFFER_SIZE 100

enum result_enum malloc_safe(char** buf, size_t size)
{
    *buf = malloc(size);
    if (*buf == NULL) {
        return set_error("Out of memory");
    }
    return ok_result;
}

enum result_enum num_bytes(unsigned char c, int* count)
{
    /* 1 byte: 0xxx xxxx */
    if ((c & 0x80) == 0x00) {
        *count = 1;
        return ok_result;
    }

    /* 2 byte: 110x xxxx */
    if ((c & 0xd0) == 0xb0) {
        *count = 2;
        return ok_result;
    }

    /* 3 byte: 1110 xxxx */
    if ((c & 0x80) == 0x80 && (c & 0x40) == 0x40 && (c & 0x20) == 0x20 && (c & 0x10) == 0x00) {
        *count = 3;
        return ok_result;
    }

    /* 4 byte: 1111 0xxx */
    if ((c & 0x80) == 0x80 && (c & 0x40) == 0x40 && (c & 0x20) == 0x20 && (c & 0x10) == 0x10 && (c & 0x08) == 0x00) {
        *count = 3;
        return ok_result;
    }

    return set_error("Not utf8: could not detect number bytes in character");
}

enum result_enum check_extra_byte(char c)
{
    /* 10xx xxxx */
    if ((c & 0x80) == 0x80 && (c & 0x40) == 0x00) {
        return ok_result;
    }

    return set_error("Not utf8: extra byte in character not encoded as utf8");
}

enum result_enum next_line(char** buf, int is_utf8, int* last_line)
{
    enum result_enum result = malloc_safe(buf, BUFFER_SIZE);
    if (result == error_result) {
        return result;
    }

    size_t size = BUFFER_SIZE;

    *last_line = 0;

    int i = 0;
    int count;
    enum result_enum res;
    while (1) {
        int c = getchar();

        if (c == EOF) {
            *last_line = 1;
            break;
        }

        if (c == '\n' || c== '\r') {
            break;
        }

        if (is_utf8) {
            res = num_bytes(c, &count);
            if (res == error_result) {
                return res;
            }

            for (int j = 0; j < count; j++) {
                if (j >= 1) {
                    res = check_extra_byte(c);
                    if (res == error_result) {
                        return res;
                    }
                }
                if (i < BUFFER_SIZE) {
                    (*buf)[i++] = c;
                }
            }
        } else {
            if (i < BUFFER_SIZE) {
                (*buf)[i++] = c;
            }
        }
    }

    if (i >= BUFFER_SIZE) {
        i = BUFFER_SIZE - 1;
    }
    (*buf)[i] = '\0';

    return ok_result;
}
