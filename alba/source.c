#include <stdarg.h>
#include <stdlib.h>
#include "zinc/result.h"
#include "zinc/buffer.h"
#include "token.h"
#include "input.h"
#include "lookahead_char.h"
#include "source.h"
#include "zinc/memory.h"

/* static-output */
void compile_error_init(struct compile_error* e)
{
	e->line = 0;
	e->col = 0;
	e->byte_pos = 0;
	e->next = NULL;
	e->prev = NULL;
}

/* static-output */
void compile_error_list_init(struct compile_error_list* el)
{
	el->head = NULL;
	el->tail = NULL;
}

/* adding error to end of error list */
/* static-output */
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
enum result set_source_error(struct compile_error_list* el, struct token* t, struct lookahead_char* lc, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int len;
	size_t n = COMPILE_ERROR_MESSAGE_SIZE;

	struct compile_error* e;

	/* allocate e */
	malloc_safe(&e, sizeof(struct compile_error));
	compile_error_init(e);

	char* p = e->message;

	if (t) {
		e->line = t->line;
		e->col = t->col;
		e->byte_pos = t->byte_pos;
	} else if (lc) {
		e->line = lc->line;
		e->col = lc->col;
		e->byte_pos = lc->byte_pos;
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

	return result_ok;
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
	int err = seek(d, e->byte_pos);
	if (err) return set_error("seek error: seek %zu", e->byte_pos);

	int c;
	while (c = f(d)) {
		if (c == EOF) break;
		if (c == '\n') break;

		/* allocate bf{} */
		buffer_add_char(bf, c);
	}

	return result_ok;
}
