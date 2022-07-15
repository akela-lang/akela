#include <stdlib.h>
#include <stdio.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "zinc/memory.h"
#include "zinc/result.h"
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

enum result circular_uchar32_buffer_init(struct circular_uchar32_buffer* cc32, size_t buf_size)
{
	cc32->buf = NULL;
	cc32->buf_size = buf_size;
	cc32->start = 0;
	cc32->size = 0;

	return malloc_safe(&cc32->buf, sizeof(UChar32)*buf_size);
}

void circular_uchar32_buffer_destroy(struct circular_uchar32_buffer* cc32)
{
	free(cc32->buf);
}

enum result circular_uchar32_buffer_add(struct circular_uchar32_buffer* cc32, UChar32 c)
{
	if (cc32->size < cc32->buf_size) {
		cc32->buf[(cc32->start + cc32->size) % cc32->buf_size] = c;
		cc32->size++;
		return result_ok;
	}

	return set_error("No room in circular UChar32 buffer: add %d", c);
}

UChar32 circular_uchar32_buffer_pop(struct circular_uchar32_buffer* cc32)
{
	if (cc32->size > 0) {
		UChar32 c = cc32->buf[cc32->start];
		cc32->start = (cc32->start + 1) % cc32->buf_size;
		cc32->size--;
		return c;
	}

	return EOF;
}
