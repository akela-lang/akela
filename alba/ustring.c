#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "result.h"
#include "ustring.h"
#include "memory.h"
#include "allocator.h"
#include "io.h"

enum result num_bytes(unsigned char c, int* count)
{
    /* 1 byte: 0xxx xxxx */
    if ((c & 0x80) == 0x00) {
        *count = 1;
        return result_ok;
    }

    /* 2 byte: 110x xxxx */
    if ((c & 0xe0) == 0xc0) {
        *count = 2;
        return result_ok;
    }

    /* 3 byte: 1110 xxxx */
    if ((c & 0xf0) == 0xe0) {
        *count = 3;
        return result_ok;
    }

    /* 4 byte: 1111 0xxx */
    if ((c & 0xf8) == 0xf0) {
        *count = 3;
        return result_ok;
    }

    *count = 0;
    return set_error("Not utf8: could not detect number bytes in character");
}

enum result check_extra_byte(char c)
{
    /* 10xx xxxx */
    if ((c & 0xc0) == 0x80) {
        return result_ok;
    }

    return set_error("Not utf8: extra byte in character not encoded as utf8");
}

enum result next_line(struct allocator* al, FILE* f, struct string* s, int is_utf8, int* last_line)
{
    *last_line = 0;

    int i = 0;
    int count;
    enum result r;
    while (1) {
        int c = getc(f);

        if (c == EOF) {
            *last_line = 1;
            break;
        }

        if (c == '\n' || c == '\r') {
            break;
        }

        if (is_utf8) {
            r = num_bytes(c, &count);
            if (r == result_error) {
                return r;
            }
            r = string_add_char(al, s, c);
            if (r == result_error) {
                return r;
            }

            for (int j = 1; j < count; j++) {
                c = getc(f);
                r = check_extra_byte(c);
                if (r == result_error) {
                    return r;
                }
                r = string_add_char(al, s, c);
                if (r == result_error) {
                    return r;
                }
            }
        } else {
            r = string_add_char(al, s, c);
            if (r == result_error) {
                return r;
            }
        }
    }

    return result_ok;
}

void string_init(struct string* s)
{
    s->buf = NULL;
    s->buf_size = 0;
    s->size = 0;
}

enum result string_add_char(struct allocator* al, struct string* s, char c)
{
    if (s == NULL) {
        return set_error("adding char to a string that is not allocated");
    }

    enum result r;

    if (s->size + 1 > s->buf_size) {
        if (s->buf == NULL) {
            r = allocator_malloc(al, &s->buf, STRING_CHUNK);
        } else {
            r = allocator_realloc(al, &s->buf, s->buf_size + STRING_CHUNK);
        }
        if (r == result_error) {
            return r;
        }
        s->buf_size += STRING_CHUNK;
    }
    s->buf[s->size++] = c;

    return result_ok;
}

void string_reset(struct string* s)
{
    if (s != NULL) {
        if (s->buf != NULL) {
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

/*
* assumes that a and b are initialized
*/
enum result string_copy(struct allocator* al, struct string* a, struct string* b)
{
    enum result r;
    for (int i = 0; i < a->size; i++) {
        r = string_add_char(al, b, a->buf[i]);
        if (r == result_error) {
            return r;
        }
    }
    return result_ok;
}

enum result string2array(struct allocator* al, struct string* s, char** a)
{
    enum result r = allocator_malloc(al, a, s->size + 1);
    if (r == result_error) {
        return r;
    }
    for (int i = 0; i < s->size; i++) {
        (*a)[i] = s->buf[i];
    }
    (*a)[s->size] = '\0';
    return result_ok;
}

enum result array2string(struct allocator* al, char* a, struct string* s)
{
    enum result r;
    char* p = a;
    while (*p != '\0') {
        r = string_add_char(al, s, *p);
        if (r == result_error) {
            return r;
        }
        p++;
    }
    return result_ok;
}

enum result next_char(struct allocator* al, struct string* s, size_t* pos, struct string* s2)
{
    char c = s->buf[(*pos)++];
    int count;
    enum result r = num_bytes(c, &count);
    if (r == result_error) {
        return r;
    }
    string_clear(s2);
    r = string_add_char(al, s2, c);
    if (r == result_error) {
        return r;
    }
    for (int i = 1; i < count; i++) {
        c = s->buf[(*pos)++];
        r = check_extra_byte(c);
        if (r == result_error) {
            return r;
        }
        r = string_add_char(al, s2, c);
        if (r == result_error) {
            return r;
        }
    }
    return result_ok;
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

/*
* if strings are equal, return 1
* otherwise, return 0
*/
int str_compare(struct string* a, char* b)
{
    size_t size = strlen(b);
    if (a->size != size) {
        return 0;
    }

    for (int i = 0; i < a->size; i++) {
        if (a->buf[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}
