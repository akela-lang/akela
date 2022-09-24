#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "zinc/buffer.h"
#include "token.h"
#include "input.h"
#include "lookahead_char.h"
#include "source.h"
#include "zinc/memory.h"

/* dynamic-output-none */
void compile_error_init(struct compile_error* e)
{
	location_init(&e->loc);
	e->next = NULL;
	e->prev = NULL;
}

/* dynamic-output-none */
void compile_error_list_init(struct compile_error_list* el)
{
	el->head = NULL;
	el->tail = NULL;
}

/* adding error to end of error list */
/* dynamic-output-none */
void compile_error_list_add(struct compile_error_list* el, struct compile_error* e)
{
	struct compile_error* prev = el->tail;

	e->prev = prev;

	if (prev) {
		prev->next = e;
	}

	if (!el->head) {
		el->head = e;
	}

	el->tail = e;
}

/* dynamic-destroy el{} */
void compile_error_list_destroy(struct compile_error_list* el)
{
	struct compile_error* p = el->head;
	while (p) {
		struct compile_error* temp = p;
		p = p->next;
		/* destroy el{} */
		free(temp);
	}
}

/* dynamic-output el{} */
bool set_source_error(struct compile_error_list* el, struct location* loc, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int len;
	size_t n = COMPILE_ERROR_MESSAGE_SIZE;

	struct compile_error* e;

	/* allocate e */
	malloc_safe((void**)&e, sizeof(struct compile_error));
	compile_error_init(e);

	char* p = e->message;

	if (loc) {
		e->loc = *loc;
	}

	char last = 0;
	size_t i = 0;
	while (*fmt != '\0') {
		if (last == '%' && *fmt == '%') {
			if (i < n) p[i++] = '%';
		} else if (*fmt == '%') {
			/* nothing */
		} else if (last == '%' && *fmt == 'd') {
			len = snprintf(p + i, n - i, "%d", va_arg(args, int));
			i += len;
		} else if (last == '%' && *fmt == 's') {
			len = snprintf(p + i, n - i, "%s", va_arg(args, char*));
			i += len;
		} else {
			if (i < n) p[i++] = *fmt;
		}
		last = *fmt;
		fmt++;
	}
	if (i > n - 1) {
		i = n - 1;
	}
	p[i++] = '\0';

	va_end(args);

	/* allocate e -> el{} */
	compile_error_list_add(el, e);

	return false;
}

/* dynamic-output bf{} */
/* resource-input d */
/* resource-use d */
enum result format_error(struct compile_error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf)
{
	/* allocate bf{} */
	buffer_copy_str(bf, e->message);
	
	/* allocate bf{} */
	buffer_add_char(bf, '\n');

	/* use d */
	int err = seek(d, e->loc.byte_pos);
	if (err) return set_error("seek error: seek %zu", e->loc.byte_pos);

	int c;
	while ((c = f(d))) {
		if (c == EOF) break;
		if (c == '\n') break;

		/* allocate bf{} */
		buffer_add_char(bf, c);
	}

	return result_ok;
}

const char* plural(int number)
{
	if (number == 1) return "";
	else return "s";
}

void location_init(struct location* loc)
{
	loc->line = 0;
	loc->col = 0;
	loc->byte_pos = 0;
	loc->byte_count = 0;
}

void location_create(struct location** loc)
{
	malloc_safe((void**)loc, sizeof(struct location));
	location_init(*loc);
}
