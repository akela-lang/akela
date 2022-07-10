#include <stdlib.h>
#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "memory.h"
#include "result.h"
#include "input.h"
#include "lookahead_translator.h"

enum result circular_byte_buffer_init(struct circular_byte_buffer* cbyte, size_t buf_size)
{
	cbyte->buf = NULL;
	cbyte->buf_size = buf_size;
	cbyte->start = 0;
	cbyte->size = 0;

	enum result r = malloc_safe(&cbyte->buf, buf_size);
	return r;
}

void circular_byte_buffer_destroy(struct circular_byte_buffer* cbyte)
{
	free(cbyte->buf);
}

enum result circular_byte_buffer_add(struct circular_byte_buffer* cbyte, char c)
{
	if (cbyte->size < cbyte->buf_size) {
		cbyte->buf[(cbyte->start + cbyte->size) % cbyte->buf_size] = c;
		cbyte->size++;
		return result_ok;
	}

	return set_error("No room in circular byte buffer: add %c", c);
}

char circular_byte_buffer_pop(struct circular_byte_buffer* cbyte)
{
	if (cbyte->size > 0) {
		char c = cbyte->buf[cbyte->start];
		cbyte->start = (cbyte->start + 1) % cbyte->buf_size;
		cbyte->size--;
		return c;
	}

	return EOF;
}
