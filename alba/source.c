#include <stdarg.h>
#include <stdlib.h>
#include "result.h"
#include "token.h"
#include "input.h"

enum result set_source_error(struct token* t, struct input_state* is, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int len;
	char* p = error_message;
	size_t n = ERROR_SIZE;

	if (t) {
		len = snprintf(p, n, "%zu, %zu: ", t->line, t->col);
		p += len;
		n -= len;
	} else if (is) {
		len = snprintf(p, n, "%zu, %zu: ", is->line, is->col);
		p += len;
		n -= len;
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

	return result_error;
}
