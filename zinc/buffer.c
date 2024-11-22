#include <stdlib.h>
#include <string.h>
#include "result.h"
#include "buffer.h"
#include "memory.h"
#include "utf8.h"
#include <stdarg.h>
#include <stdbool.h>

/* static-output */
/* initialize-output bf{} */
void buffer_init(struct buffer* bf)
{
    bf->buf = NULL;
    bf->buf_size = 0;
    bf->size = 0;
}

void buffer_create(struct buffer** bf)
{
    malloc_safe((void*)bf, sizeof(struct buffer));
    buffer_init(*bf);
}

/* dynamic-destroy bf{} */
void buffer_destroy(struct buffer* bf)
{
    if (bf) {
        if (bf->buf) {
            /* destroy bf{buf} */
            free(bf->buf);
        }
    }
}

void buffer_expand(struct buffer* bf, size_t new_size)
{
    size_t num = new_size - bf->size;
    if (num > 0) {
        if (new_size > bf->buf_size) {
            size_t new_buf_size = bf->size;
            if (num > BUFFER_CHUNK) {
                new_buf_size += num;
            } else {
                new_buf_size += BUFFER_CHUNK;
            }
            realloc_safe((void**)&bf->buf, new_buf_size);
            bf->buf_size = new_buf_size;
        }
    }
}

void buffer_add(struct buffer* bf, char* s, size_t num)
{
    size_t new_size = bf->size + num;
    buffer_expand(bf, new_size);
    memcpy(bf->buf + bf->size, s, num);
    bf->size = new_size;
}

void buffer_add_char(struct buffer* bf, char c)
{
    if (bf->size + 1 > bf->buf_size) {
        if (bf->buf == NULL) {
            malloc_safe((void**)&bf->buf, BUFFER_CHUNK);
        } else {
            realloc_safe((void**)&bf->buf, bf->buf_size + BUFFER_CHUNK);
        }
        bf->buf_size += BUFFER_CHUNK;
    }
    bf->buf[bf->size++] = c;
}

void buffer_finish(struct buffer* bf)
{
    buffer_add_char(bf, '\0');
    bf->size--;
}

/* dynamic-destroy bf{} */
void buffer_reset(struct buffer* bf)
{
    if (bf != NULL) {
        if (bf->buf != NULL) {
            /* destroy bf{} */
            free(bf->buf);
            buffer_init(bf);
        }
    }
}

/* static-output */
void buffer_clear(struct buffer* bf)
{
    if (bf != NULL) {
        bf->size = 0;
    }
}

void buffer_copy(struct buffer* src, struct buffer* dest)
{
    for (int i = 0; i < src->size; i++) {
        buffer_add_char(dest, src->buf[i]);
    }
}

/* dynamic-output a{} */
void buffer_copy_str(struct buffer* a, const char* b)
{
    while (*b) {
        /* allocate a{} */
        buffer_add_char(a, *b);
        b++;
    }
}

/* dynamic-output a */
void buffer2array(struct buffer* bf, char** a)
{
    /* allocate a */
    malloc_safe((void**)a, bf->size + 1);
    for (int i = 0; i < bf->size; i++) {
        (*a)[i] = bf->buf[i];
    }
    (*a)[bf->size] = '\0';
}

/* bf must be initialized */
/* dynamic-output bf{} */
void array2buffer(const char* a, struct buffer* bf)
{
    const char* p = a;
    while (*p != '\0') {
        /* allocate b{} */
        buffer_add_char(bf, *p);
        p++;
    }
}

/* dynamic-output bf2{} */
enum result next_char(struct buffer* bf, size_t* pos, struct buffer* bf2)
{
    char c = bf->buf[(*pos)++];
    int count;
    enum result r = check_num_bytes(c, &count);
    if (r == result_error) {
        return r;
    }
    buffer_clear(bf2);

    /* allocate bf2{} */
    buffer_add_char(bf2, c);
    for (int i = 1; i < count; i++) {
        c = bf->buf[(*pos)++];
        r = check_extra_byte(c);
        if (r == result_error) {
            return r;
        }

        /* allocate bf2{} */
        buffer_add_char(bf2, c);
    }
    return result_ok;
}

/*
* if strings are equal, return 1
* otherwise, return 0
*/
/* static-output */
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

int buffer_order(struct buffer* a, struct buffer* b)
{
    size_t size = 0;
    if (a->size > size) size = a->size;
    if (b->size > size) size = b->size;

    for (size_t i = 0; i < size; i++) {
        if (i >= b->size) return 1;
        if (i >= a->size) return -1;
        if (a->buf[i] < b->buf[i]) return -1;
        if (a->buf[i] > b->buf[i]) return 1;
    }

    return 0;
}

/*
* if strings are equal, return 1
* otherwise, return 0
*/
/* static-output */
int buffer_compare_str(struct buffer* a, const char* b)
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

/* dynamic-output dest{} */
enum result buffer_uslice(struct buffer* src, struct buffer* dest, size_t start, size_t end)
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
            /* allocate dest{} */
            buffer_add_char(dest, c);
        }


        for (int j = 1; j < count; j++) {
            c = src->buf[i++];
            r = check_extra_byte(c);
            if (r == result_error) return r;

            if (index >= start && index < end) {
                /* allocate dest{} */
                buffer_add_char(dest, c);
            }
        }

        index++;
    }

    return r;
}

void buffer_add_format(struct buffer *bf, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char* buf = NULL;
    size_t buf_size = BUFFER_CHUNK;
    int len;

    malloc_safe((void**)&buf, buf_size);

    char last_last = 0;
    char last = 0;
    while (*fmt != '\0') {
        if (last == '%' && *fmt == '%') {
            buffer_add_char(bf, '%');
        } else if (*fmt == '%') {
            /* nothing */
        } else if (last == '%' && *fmt == 'l') {
            /* nothing */
        } else if (last_last == '%' && last == 'l' && *fmt == 'f') {
            double lf = va_arg(args, double);
            while (true) {
                len = snprintf(buf, buf_size, "%lf", lf);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                buffer_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'd') {
            int d = va_arg(args, int);
            while (true) {
                len = snprintf(buf, buf_size, "%d", d);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                buffer_add_char(bf, buf[j]);
            }
        } else if (last_last == '%' && last == 'l' && *fmt == 'd') {
            long d = va_arg(args, int);
            while (true) {
                len = snprintf(buf, buf_size, "%ld", d);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                buffer_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'z') {
            /* nothing */
        } else if (last_last == '%' && last == 'z' && *fmt == 'u') {
            size_t zu = va_arg(args, size_t);
            while (true) {
                len = snprintf(buf, buf_size, "%zu", zu);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                buffer_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'b') {
            /* nothing */
        } else if (last_last == '%' && last == 'b' && *fmt == 'f') {
            struct buffer* bf_in = va_arg(args, struct buffer*);
            buffer_copy(bf_in, bf);
        } else if (last == '%' && *fmt == 's') {
            char* s = va_arg(args, char*);
            while (true) {
                len = snprintf(buf, buf_size, "%s", s);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                buffer_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'c') {
            char c = va_arg(args, int);
            while (true) {
                len = snprintf(buf, buf_size, "%c", c);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                buffer_add_char(bf, buf[j]);
            }
        } else if (last_last == '%' && last == 'l' && *fmt == 'x') {
            unsigned long x = va_arg(args, unsigned long);
            while (true) {
                len = snprintf(buf, buf_size, "%lx", x);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                buffer_add_char(bf, buf[j]);
            }        } else {
            buffer_add_char(bf, *fmt);
        }
        last_last = last;
        last = *fmt;
        fmt++;
    }

    va_end(args);
    free(buf);
}