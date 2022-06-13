#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "result.h"
#include "ustring.h"
#include "memory.h"
#include "allocator.h"
#include "io.h"

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
