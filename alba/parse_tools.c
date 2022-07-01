#include "input.h"
#include "parse_tools.h"
#include "token.h"
#include "result.h"
#include <stdarg.h>
#include "scan.h"
#include <string.h>

void token_state_init(struct input_state* is, struct token_state* ts)
{
	ts->is = is;
	token_list_init(&ts->lookahead);
}

/* get lookahead token */
enum parse_result get_lookahead(struct allocator* al, struct token_state* ts, int count, int* num)
{
	enum result r = result_ok;
	int gt;
	struct token* t;

	*num = token_list_count(&ts->lookahead);
	if (*num >= count) {
		return r;
	}

	for (; *num < count; (*num)++) {
		if (ts->is->done) {
			break;
		}

		r = scan_get_token(al, ts->is, &gt, &t);
		if (r == result_error) {
			return r;
		}
		if (!gt) {
			break;
		}

		r = token_list_add(al, &ts->lookahead, t);
		if (r == result_error) {
			return r;
		}
	}

	return r;
}

/* expecting specific token */
enum result match(struct allocator* al, struct token_state* ts, enum token_type type, char* reason)
{
	enum result r = result_ok;

	int num = token_list_count(&ts->lookahead);
	int got_token;
	struct token* t;
	if (num <= 0) {
		enum result r = scan_get_token(al, ts->is, &got_token, &t);
		if (r == result_error) {
			return r;
		}
		if (!got_token) {
			return set_error("%d, %d: %s", ts->is->line, ts->is->col, reason);
		}
		r = token_list_add(al, &ts->lookahead, t);
		if (r == result_error) {
			return r;
		}
	} else {
		t = ts->lookahead.head->t;
	}

	if (t->type == type) {
		token_list_pop(&ts->lookahead);
		return r;
	}

	return set_error("%d, %d: %s", t->line, t->col, reason);
}

enum result parse_set_error(struct token* t, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int len;
	char* p = error_message;
	int n = ERROR_SIZE;

	if (t) {
		len = snprintf(p, n, "%zu, %zu: ", t->line, t->col);
		p += len - 1;
		n -= len - 1;
	}

	char last = 0;
	int i = 0;
	while (*fmt != '\0') {
		if (last == '%' && *fmt == '%') {
			if (i < n) p[i++] = '%';
		} else if (*fmt == '%') {
			/* nothing */
		} else if (last == '%' && *fmt == 'd') {
			len = snprintf(p + i, n - i, "%d", va_arg(args, int));
			i += len - 1;
		} else if (last == '%' && *fmt == 's') {
			len = snprintf(p + i, n - i, "%s", va_arg(args, char*));
			i += len - 1;
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
