#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "result.h"
#include "buffer.h"
#include "memory.h"
#include "allocator.h"
#include "io.h"
#include "uconv.h"

void buffer_init(struct buffer* bf)
{
    bf->buf = NULL;
    bf->buf_size = 0;
    bf->size = 0;
}

enum result buffer_add_char(struct allocator* al, struct buffer* bf, char c)
{
    if (bf == NULL) {
        return set_error("adding char to a string that is not allocated");
    }

    enum result r;

    if (bf->size + 1 > bf->buf_size) {
        if (bf->buf == NULL) {
            r = allocator_malloc(al, &bf->buf, BUFFER_CHUNK);
        } else {
            r = allocator_realloc(al, &bf->buf, bf->buf_size + BUFFER_CHUNK);
        }
        if (r == result_error) {
            return r;
        }
        bf->buf_size += BUFFER_CHUNK;
    }
    bf->buf[bf->size++] = c;

    return result_ok;
}

void buffer_reset(struct buffer* bf)
{
    if (bf != NULL) {
        if (bf->buf != NULL) {
            buffer_init(bf);
        }
    }
}

void buffer_clear(struct buffer* bf)
{
    if (bf != NULL) {
        bf->size = 0;
    }
}

/*
* assumes that a and b are initialized
*/
enum result buffer_copy(struct allocator* al, struct buffer* a, struct buffer* b)
{
    enum result r;
    for (int i = 0; i < a->size; i++) {
        r = buffer_add_char(al, b, a->buf[i]);
        if (r == result_error) {
            return r;
        }
    }
    return result_ok;
}

enum result buffer_copy_str(struct allocator* al, struct buffer* a, char* b)
{
    enum result r;
    while (*b) {
        r = buffer_add_char(al, a, *b);
        if (r == result_error) return r;
        b++;
    }

    return result_ok;
}

enum result buffer2array(struct allocator* al, struct buffer* bf, char** a)
{
    enum result r = allocator_malloc(al, a, bf->size + 1);
    if (r == result_error) {
        return r;
    }
    for (int i = 0; i < bf->size; i++) {
        (*a)[i] = bf->buf[i];
    }
    (*a)[bf->size] = '\0';
    return result_ok;
}

enum result array2buffer(struct allocator* al, char* a, struct buffer* bf)
{
    enum result r;
    char* p = a;
    while (*p != '\0') {
        r = buffer_add_char(al, bf, *p);
        if (r == result_error) {
            return r;
        }
        p++;
    }
    return result_ok;
}

enum result next_char(struct allocator* al, struct buffer* bf, size_t* pos, struct buffer* bf2)
{
    char c = bf->buf[(*pos)++];
    int count;
    enum result r = check_num_bytes(c, &count);
    if (r == result_error) {
        return r;
    }
    buffer_clear(bf2);
    r = buffer_add_char(al, bf2, c);
    if (r == result_error) {
        return r;
    }
    for (int i = 1; i < count; i++) {
        c = bf->buf[(*pos)++];
        r = check_extra_byte(c);
        if (r == result_error) {
            return r;
        }
        r = buffer_add_char(al, bf2, c);
        if (r == result_error) {
            return r;
        }
    }
    return result_ok;
}

enum result buffer_uslice(struct allocator* al, struct buffer* src, struct buffer* dest, size_t start, size_t end)
{
    enum result r = result_ok;
    char c;
    int count;

    size_t i = 0;
    size_t index = 0;
    while (i < src->size && index < end) {
        c = src->buf[i++];
        r = check_num_bytes(c, &count);
        if (r == result_error) return r;

        if (index >= start && index < end) {
            r = buffer_add_char(al, dest, c);
            if (r == result_error) return r;
        }


        for (int j = 1; j < count; j++) {
            c = src->buf[i++];
            r = check_extra_byte(c);
            if (r == result_error) return r;

            if (index >= start && index < end) {
                r = buffer_add_char(al, dest, c);
                if (r == result_error) return r;
            }
        }

        index++;
    }

    return r;
}

/*
* if strings are equal, return 1
* otherwise, return 0
*/
int buffer_compare(struct buffer* a, struct buffer* b)
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
int buffer_str_compare(struct buffer* a, char* b)
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
