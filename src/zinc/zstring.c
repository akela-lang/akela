#include <stdlib.h>
#include <string.h>
#include "result.h"
#include "zstring.h"
#include "memory.h"
#include "utf8.h"
#include <stdarg.h>
#include <stdbool.h>

void Zinc_string_init(struct Zinc_string* bf)
{
    bf->buf = NULL;
    bf->buf_size = 0;
    bf->size = 0;
}

void Zinc_string_create(struct Zinc_string** bf)
{
    Zinc_malloc_safe((void*)bf, sizeof(struct Zinc_string));
    Zinc_string_init(*bf);
}

void Zinc_string_destroy(struct Zinc_string* bf)
{
    if (bf) {
        if (bf->buf) {
            free(bf->buf);
        }
    }
}

void Zinc_string_free(struct Zinc_string* bf)
{
    if (bf) {
        Zinc_string_destroy(bf);
        free(bf);
    }
}

void Zinc_string_expand(struct Zinc_string* bf, size_t new_size)
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
            Zinc_realloc_safe((void**)&bf->buf, new_buf_size);
            bf->buf_size = new_buf_size;
        }
    }
}

void Zinc_string_add(struct Zinc_string* bf, const char* s, size_t num)
{
    size_t new_size = bf->size + num;
    Zinc_string_expand(bf, new_size);
    memcpy(bf->buf + bf->size, s, num);
    bf->size = new_size;
}

void Zinc_string_add_char(struct Zinc_string* bf, char c)
{
    if (bf->size + 1 > bf->buf_size) {
        if (bf->buf == NULL) {
            Zinc_malloc_safe((void**)&bf->buf, BUFFER_CHUNK);
        } else {
            Zinc_realloc_safe((void**)&bf->buf, bf->buf_size + BUFFER_CHUNK);
        }
        bf->buf_size += BUFFER_CHUNK;
    }
    bf->buf[bf->size++] = c;
}

void Zinc_string_add_str(struct Zinc_string* bf, const char* str)
{
    while (*str) {
        Zinc_string_add_char(bf, *str);
        str++;
    }
}

void Zinc_string_add_string(Zinc_string* string, Zinc_string* string2)
{
    Zinc_string_add(string, string2->buf, string2->size);
}

void Zinc_string_finish(struct Zinc_string* bf)
{
    Zinc_string_add_char(bf, '\0');
    bf->size--;
}

void Zinc_string_reset(struct Zinc_string* bf)
{
    if (bf != NULL) {
        if (bf->buf != NULL) {
            /* destroy bf{} */
            free(bf->buf);
            Zinc_string_init(bf);
        }
    }
}

void Zinc_string_clear(struct Zinc_string* bf)
{
    if (bf != NULL) {
        bf->size = 0;
    }
}

void Zinc_string_copy(struct Zinc_string* src, struct Zinc_string* dest)
{
    for (int i = 0; i < src->size; i++) {
        Zinc_string_add_char(dest, src->buf[i]);
    }
}

void Zinc_string_create_str(struct Zinc_string* bf, char** a)
{
    Zinc_malloc_safe((void**)a, bf->size + 1);
    for (int i = 0; i < bf->size; i++) {
        (*a)[i] = bf->buf[i];
    }
    (*a)[bf->size] = '\0';
}

/* bf must be initialized */
/* dynamic-output bf{} */
void Zinc_string_add_str2(const char* a, struct Zinc_string* bf)
{
    const char* p = a;
    while (*p != '\0') {
        /* allocate b{} */
        Zinc_string_add_char(bf, *p);
        p++;
    }
}

enum Zinc_result Zinc_next_char(struct Zinc_string* bf, size_t* pos, struct Zinc_string* bf2)
{
    char c = bf->buf[(*pos)++];
    int count;
    enum Zinc_result r = Zinc_check_num_bytes(c, &count);
    if (r == Zinc_result_error) {
        return r;
    }
    Zinc_string_clear(bf2);

    Zinc_string_add_char(bf2, c);
    for (int i = 1; i < count; i++) {
        c = bf->buf[(*pos)++];
        r = Zinc_check_extra_byte(c);
        if (r == Zinc_result_error) {
            return r;
        }

        Zinc_string_add_char(bf2, c);
    }
    return Zinc_result_ok;
}

/*
* if strings are equal, return 1
* otherwise, return 0
*/
int Zinc_string_compare(struct Zinc_string* a, struct Zinc_string* b)
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

int Zinc_string_order(struct Zinc_string* a, struct Zinc_string* b)
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
int Zinc_string_compare_str(struct Zinc_string* a, const char* b)
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

enum Zinc_result Zinc_string_uslice(struct Zinc_string* src, struct Zinc_string* dest, size_t start, size_t end)
{
    enum Zinc_result r = Zinc_result_ok;
    char c;
    int count;

    size_t i = 0;
    size_t index = 0;
    while (i < src->size && index < end) {
        c = src->buf[i++];
        r = Zinc_check_num_bytes(c, &count);
        if (r == Zinc_result_error) return r;

        if (index >= start && index < end) {
            Zinc_string_add_char(dest, c);
        }


        for (int j = 1; j < count; j++) {
            c = src->buf[i++];
            r = Zinc_check_extra_byte(c);
            if (r == Zinc_result_error) return r;

            if (index >= start && index < end) {
                Zinc_string_add_char(dest, c);
            }
        }

        index++;
    }

    return r;
}

void Zinc_string_add_format(struct Zinc_string *bf, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char* buf = NULL;
    size_t buf_size = BUFFER_CHUNK;
    int len;

    Zinc_malloc_safe((void**)&buf, buf_size);

    char last_last_last = 0;
    char last_last = 0;
    char last = 0;
    while (*fmt != '\0') {
        if (last == '%' && *fmt == '%') {
            Zinc_string_add_char(bf, '%');
        } else if (*fmt == '%') {
        } else if (last == '%' && *fmt == 'l') {
        } else if (last_last == '%' && last == 'l' && *fmt == 'l') {
        } else if (last == '%' && *fmt == 'h') {
        } else if (last_last == '%' && last == 'h' && *fmt == 'h') {
        } else if (last == '%' && *fmt == '%') {
            Zinc_string_add_char(bf, '%');
        } else if (last_last == '%' && last == 'l' && *fmt == 'f') {
            double lf = va_arg(args, double);
            while (true) {
                len = snprintf(buf, buf_size, "%lf", lf);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'd') {
            int d = va_arg(args, int);
            while (true) {
                len = snprintf(buf, buf_size, "%d", d);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last_last == '%' && last == 'l' && *fmt == 'd') {
            long d = va_arg(args, int);
            while (true) {
                len = snprintf(buf, buf_size, "%ld", d);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last_last_last == '%' && last_last == 'l' && last == 'l' && *fmt == 'd') {
            long long lld = va_arg(args, long long);
            while (true) {
                len = snprintf(buf, buf_size, "%lld", lld);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'z') {
        } else if (last_last == '%' && last == 'z' && *fmt == 'u') {
            size_t zu = va_arg(args, size_t);
            while (true) {
                len = snprintf(buf, buf_size, "%zu", zu);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'b') {
            /* nothing */
        } else if (last_last == '%' && last == 'b' && *fmt == 'f') {
            struct Zinc_string* bf_in = va_arg(args, struct Zinc_string*);
            Zinc_string_copy(bf_in, bf);
        } else if (last == '%' && *fmt == 's') {
            char* s = va_arg(args, char*);
            while (true) {
                len = snprintf(buf, buf_size, "%s", s);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last == '%' && *fmt == 'c') {
            char c = va_arg(args, int);
            while (true) {
                len = snprintf(buf, buf_size, "%c", c);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last_last == '%' && last == 'l' && *fmt == 'x') {
            unsigned long x = va_arg(args, unsigned long);
            while (true) {
                len = snprintf(buf, buf_size, "%lx", x);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else if (last_last_last == '%' && last_last == 'h' && last == 'h' && *fmt == 'd') {
            char x = va_arg(args, int);
            while (true) {
                len = snprintf(buf, buf_size, "%hhd", x);
                if (len < buf_size) {
                    break;
                }
                buf_size *= 2;
                Zinc_realloc_safe((void**)&buf, buf_size);
            }
            for (int j = 0; j < len; j++) {
                Zinc_string_add_char(bf, buf[j]);
            }
        } else {
            Zinc_string_add_char(bf, *fmt);
        }
        last_last_last = last_last;
        last_last = last;
        last = *fmt;
        fmt++;
    }

    va_end(args);
    free(buf);
}

struct Zinc_string* Zinc_string_clone(struct Zinc_string* bf)
{
    struct Zinc_string* new_bf = NULL;
    Zinc_string_create(&new_bf);
    Zinc_string_copy(bf, new_bf);
    return new_bf;
}