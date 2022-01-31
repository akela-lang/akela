#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
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
    if ((c & 0xe0) == 0xc0) {
        *count = 2;
        return ok_result;
    }

    /* 3 byte: 1110 xxxx */
    if ((c & 0xf0) == 0xe0) {
        *count = 3;
        return ok_result;
    }

    /* 4 byte: 1111 0xxx */
    if ((c & 0xf8) == 0xf0) {
        *count = 3;
        return ok_result;
    }

    *count = 0;
    return set_error("Not utf8: could not detect number bytes in character");
}

enum result_enum check_extra_byte(char c)
{
    /* 10xx xxxx */
    if ((c & 0xc0) == 0x80) {
        return ok_result;
    }

    return set_error("Not utf8: extra byte in character not encoded as utf8");
}

enum result_enum next_line(FILE* f, struct string* s, int is_utf8, int* last_line)
{
    *last_line = 0;

    int i = 0;
    int count;
    enum result_enum r;
    while (1) {
        int c = getc(f);

        if (c == EOF) {
            printf("EOF\n");
            *last_line = 1;
            break;
        }

        if (c == '\n' || c == '\r') {
            printf("line break\n");
            break;
        }

        if (is_utf8) {
            r = num_bytes(c, &count);
            if (r == error_result) {
                fprintf(stderr, "%s\n", error_message);
                return r;
            }
            printf("%c - %0x - %d byte\n", c, c, count);
            r = string_add_char(s, c);
            if (r == error_result) {
                return r;
            }

            for (int j = 1; j < count; j++) {
                c = getc(f);
                printf("%c - %0x\n", c, c);
                r = check_extra_byte(c);
                if (r == error_result) {
                    return r;
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

void string_reset(struct string* s)
{
    if (s != NULL) {
        if (s->buf != NULL) {
            free(s->buf);
            string_init(s);
        }
    }
}

void string_clear(struct string* s)
{
    if (s != NULL) {
        s->size = 0;
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

enum result_enum array2string(char* a, struct string* s)
{
    enum result_enum r;
    char* p = a;
    while (*p != '\0') {
        r = string_add_char(s, *p);
        if (r == error_result) {
            return r;
        }
        p++;
    }
    return ok_result;
}

enum result_enum next_char(struct string* s, size_t* pos, struct string* s2)
{
    char c = s->buf[(*pos)++];
    int count;
    enum result_enum r = num_bytes(c, &count);
    if (r == error_result) {
        return r;
    }
    string_clear(s2);
    r = string_add_char(s2, c);
    if (r == error_result) {
        return r;
    }
    for (int i = 1; i < count; i++) {
        c = s->buf[(*pos)++];
        r = check_extra_byte(c);
        if (r == error_result) {
            return r;
        }
        r = string_add_char(s2, c);
        if (r == error_result) {
            return r;
        }
    }
    return ok_result;
}

/*
* if strings are equal, return 1
* otherwise, return 0
*/
int string_compare(struct string* a, struct string* b)
{
    if (a->size != b->size) {
        return 0;
    }

    for (int i = 0; i < a->size; i++) {
        if (a->buf[i] != b->buf[i]) {
            return 0;
        }
    }
    return 1;
}

enum result_enum scan(struct string* line)
{
    enum result_enum r;
    size_t pos = 0;
    struct string s;
    string_init(&s);

    UErrorCode err;
    UConverter* conv = ucnv_open("utf8", &err);
    if (U_FAILURE(err)) {
        return set_error("utf error");
    }

    while (pos < line->size) {
        r = next_char(line, &pos, &s);
        char c = s.buf[0];
        size_t size = s.size;
        if (r == error_result) {
            return r;
        }

        char* a;
        r = string2array(&s, &a);
        if (r == error_result) {
            return r;
        }
        UChar32 c2;
        UChar dest[10];
        int32_t len = ucnv_toUChars(conv, dest, 10, a, s.size, &err);
        int i = 0;
        U16_NEXT(dest, i, len, c2);
        free(a);

        if (size > 1 && u_isalpha(c2)) {
            printf("found alpha\n");
        } else if (size == 1 && isalpha(c)) {
            printf("found alpha\n");
        } else if (size == 1 && isdigit(c)) {
            printf("found digit\n");
        } else if (size == 1 && c == '+') {
            printf("found plus\n");
        } else if (size == 1 && c == ' ') {
            printf("found space\n");
        } else {
            char* a;
            r = string2array(&s, &a);
            if (r == error_result) {
                return r;
            }
            printf("unrecogized: %s\n", a);
            free(a);
        }
    }

    string_reset(&s);
    ucnv_close(conv);

    return ok_result;
}
