#include "error.h"
#include "memory.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "buffer.h"

void location_init(struct location* loc)
{
    loc->start_pos = 0;
    loc->end_pos = 0;
    loc->line = 0;
    loc->col = 0;
}

void error_init(struct error* e)
{
    Zinc_string_init(&e->message);
    location_init(&e->loc);
    e->next = NULL;
    e->prev = NULL;
}

void error_create(struct error** e)
{
    malloc_safe((void**)e, sizeof(struct error));
    error_init(*e);
}

void error_destroy(struct error* e)
{
    Zinc_string_destroy(&e->message);
}

void error_list_init(struct error_list* el)
{
    el->head = NULL;
    el->tail = NULL;
}

void error_list_create(struct error_list** el)
{
    malloc_safe((void**)el, sizeof(struct error_list));
    error_list_init(*el);
}

void error_list_add(struct error_list *el, struct error* e)
{
    if (el->head && el->tail) {
        el->tail->next = e;
        e->prev = el->tail;
        el->tail = e;
    } else {
        el->head = e;
        el->tail = e;
    }
}

void error_list_destroy(struct error_list* el)
{
    struct error* p = el->head;
    while (p) {
        struct error* temp = p;
        p = p->next;
        error_destroy(temp);
        free(temp);
    }
}

bool error_list_set(struct error_list *el, struct location* loc, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    struct Zinc_string bf;
    Zinc_string_init(&bf);
    int len;
    struct error* e = NULL;
    error_create(&e);

    char last_last = 0;
    char last = 0;
    while (*fmt != '\0') {
        if (last == '%' && *fmt == '%') {
            Zinc_string_add_char(&e->message, '%');
        } else if (*fmt == '%') {
            /* nothing */
        } else if (last == '%' && *fmt == 'z') {
            /* nothing */
        } else if (last == '%' && *fmt == 'd') {
            int x = va_arg(args, int);
            while (true) {
                Zinc_string_clear(&bf);
                len = snprintf(bf.buf, bf.buf_size, "%d", x);
                if (len < bf.buf_size) {
                    bf.size = len;
                    break;
                } else {
                    Zinc_string_expand(&bf, len + BUFFER_CHUNK);
                }
            }
            Zinc_string_add(&e->message, bf.buf, bf.size);
        } else if (last_last == '%' && last == 'z' && *fmt == 'u') {
            size_t x = va_arg(args, size_t);
            while (true) {
                Zinc_string_clear(&bf);
                len = snprintf(bf.buf, bf.buf_size, "%zu", x);
                if (len < bf.buf_size) {
                    bf.size = len;
                    break;
                } else {
                    Zinc_string_expand(&bf, len + BUFFER_CHUNK);
                }
            }
            Zinc_string_add(&e->message, bf.buf, bf.size);
        } else if (last == '%' && *fmt == 's') {
            char* x = va_arg(args, char*);
            while (true) {
                Zinc_string_clear(&bf);
                len = snprintf(bf.buf, bf.buf_size, "%s", x);
                if (len + 1 < bf.buf_size) {
                    bf.size = len;
                    break;
                } else {
                    Zinc_string_expand(&bf, len + 1 + BUFFER_CHUNK);
                }
            }
            Zinc_string_add(&e->message, bf.buf, bf.size);
        } else if (last == '%' && *fmt == 'b') {
            struct Zinc_string* src = va_arg(args, struct Zinc_string*);
            Zinc_string_finish(src);
            while (true) {
                Zinc_string_clear(&bf);
                len = snprintf(bf.buf, bf.buf_size, "%s", src->buf);
                if (len < bf.buf_size) {
                    bf.size = len;
                    break;
                } else {
                    Zinc_string_expand(&bf, len + BUFFER_CHUNK);
                }
            }
            Zinc_string_add(&e->message, bf.buf, bf.size);
        } else if (last == '%' && *fmt == 'c') {
            char c = (char)va_arg(args, int);
            Zinc_string_add_char(&e->message, c);
        } else {
            Zinc_string_add_char(&e->message, *fmt);
        }
        last_last = last;
        last = *fmt;
        fmt++;
    }

    va_end(args);

    error_list_add(el, e);
    if (loc) {
        e->loc = *loc;
    }

    Zinc_string_destroy(&bf);

    return false;
}

void error_list_print(struct error_list* el)
{
    struct error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        fprintf(stderr, "(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
        e = e->next;
    }
}

void location_create(struct location** loc)
{
    malloc_safe((void**)loc, sizeof(struct location));
    location_init(*loc);
}

const char* plural(size_t number)
{
    if (number == 1) return "";
    else return "s";
}

void location_combine(struct location* p, struct location* c)
{
    if (!p->line && !p->col) {
        *p = *c;
    } else {
        if (c->line && c->col) {
            if (c->start_pos < p->start_pos) {
                p->start_pos = c->start_pos;
            }
            if (c->end_pos > p->end_pos) {
                p->end_pos = c->end_pos;
            }
            if (c->line < p->line) {
                p->line = c->line;
            }
            if (c->col < p->col) {
                p->col = c->col;
            }
        }
    }
}