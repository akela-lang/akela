#include "error.h"
#include "memory.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "zstring.h"

void Zinc_location_init(Zinc_location* loc)
{
    loc->start_pos = 0;
    loc->end_pos = 0;
    loc->line = 0;
    loc->col = 0;
}

void Zinc_error_init(Zinc_error* e)
{
    Zinc_string_init(&e->message);
    Zinc_location_init(&e->loc);
    e->next = NULL;
    e->prev = NULL;
}

void Zinc_error_create(Zinc_error** e)
{
    Zinc_malloc_safe((void**)e, sizeof(Zinc_error));
    Zinc_error_init(*e);
}

void Zinc_error_destroy(Zinc_error* e)
{
    Zinc_string_destroy(&e->message);
}

void Zinc_error_list_init(Zinc_error_list* el)
{
    el->head = NULL;
    el->tail = NULL;
}

void Zinc_error_list_create(Zinc_error_list** el)
{
    Zinc_malloc_safe((void**)el, sizeof(Zinc_error_list));
    Zinc_error_list_init(*el);
}

void Zinc_error_list_add(Zinc_error_list *el, Zinc_error* e)
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

void Zinc_error_list_destroy(Zinc_error_list* el)
{
    Zinc_error* p = el->head;
    while (p) {
        Zinc_error* temp = p;
        p = p->next;
        Zinc_error_destroy(temp);
        free(temp);
    }
}

bool Zinc_error_list_set(Zinc_error_list *el, Zinc_location* loc, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Zinc_string bf;
    Zinc_string_init(&bf);
    int len;
    Zinc_error* e = NULL;
    Zinc_error_create(&e);

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
            Zinc_string* src = va_arg(args, Zinc_string*);
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

    Zinc_error_list_add(el, e);
    if (loc) {
        e->loc = *loc;
    }

    Zinc_string_destroy(&bf);

    return false;
}

void Zinc_error_list_print(Zinc_error_list* el)
{
    Zinc_error* e = el->head;
    while (e) {
        Zinc_string_finish(&e->message);
        fprintf(stderr, "(%zu,%zu): %s\n", e->loc.line, e->loc.col, e->message.buf);
        e = e->next;
    }
}

void Zinc_location_create(Zinc_location** loc)
{
    Zinc_malloc_safe((void**)loc, sizeof(Zinc_location));
    Zinc_location_init(*loc);
}

const char* Zinc_plural(size_t number)
{
    if (number == 1) return "";
    else return "s";
}

void Zinc_location_combine(Zinc_location* p, Zinc_location* c)
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