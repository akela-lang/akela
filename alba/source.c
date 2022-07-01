#include <stdarg.h>
#include <stdlib.h>
#include "result.h"

enum result set_source_error(size_t line, size_t col, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int len;
	char* p = error_message;
	int n = ERROR_SIZE;

	len = snprintf(p, n, "%zu, %zu: ", line, col);
	p += len;
	n -= len;

	char last = 0;
	int i = 0;
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
