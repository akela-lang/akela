#include <stdio.h>
#include <stdlib.h>
#include "result.h"
#include "scan.h"

#define BUFFER_SIZE 100

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

enum result_enum next_line(struct string* s, int is_utf8, int* last_line)
{
    *last_line = 0;

    int i = 0;
    int count;
    enum result_enum r;
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
            r = num_bytes(c, &count);
            if (r == error_result) {
                return r;
            }

            for (int j = 0; j < count; j++) {
                if (j >= 1) {
                    r = check_extra_byte(c);
                    if (r == error_result) {
                        return r;
                    }
                }
                r = string_add_char(s, c);
                if (r == error_result) {
                    return r;
                }
            }
        } else {
            r = string_add_char(s, c);
            if (r == error_result) {
                return r;
            }
        }
    }

    return ok_result;
}

enum result_enum malloc_safe(char** buf, size_t size)
{
    *buf = malloc(size);
    if (*buf == NULL) {
        return set_error("Out of memory");
    }
    return ok_result;
}

enum result_enum realloc_safe(char** buf, size_t size)
{
    char* new_buf;
    new_buf = realloc(*buf, size);
    if (new_buf == NULL) {
        return set_error("Out of memory");
    }
    *buf = new_buf;
    return ok_result;
}

void string_init(struct string* s)
{
    s->buf = NULL;
    s->buf_size = 0;
    s->size = 0;
}

enum result_enum string_add_char(struct string* s, char c)
{
    if (s == NULL) {
        return set_error("adding char to a string that is not allocated");
    }

    enum result_enum r;

    if (s->size <= s->buf_size) {
        if (s->buf == NULL) {
            r = malloc_safe(&s->buf, STRING_CHUNK);
        } else {
            r = realloc_safe(&s->buf, s->buf_size + STRING_CHUNK);
        }
        if (r == error_result) {
            return r;
        }
        s->buf_size += STRING_CHUNK;
    }
    s->buf[s->size++] = c;

    return ok_result;
}

void string_clear(struct string* s)
{
    if (s != NULL) {
        if (s->buf != NULL) {
            free(s->buf);
            string_init(s);
        }
    }
}

enum result_enum string2array(struct string* s, char** a)
{
    enum result_enum r = malloc_safe(a, s->size + 1);
    if (r == error_result) {
        return r;
    }
    for (int i = 0; i < s->size; i++) {
        (*a)[i] = s->buf[i];
    }
    (*a)[s->size] = '\0';
    return ok_result;
}
