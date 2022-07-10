#ifndef _LOOKAHEAD_TRANSLATOR_H
#define _LOOKAHEAD_TRANSLATOR_H

#include <stdlib.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>

struct circular_byte_buffer {
	char* buf;
	size_t buf_size;
	size_t start;
	size_t size;
};

struct circular_char_buffer {
	UChar32* buf;
	size_t buf_size;
	size_t start;
	size_t size;
};

enum result circular_byte_buffer_init(struct circular_byte_buffer* cbyte, size_t buf_size);
void circular_byte_buffer_destroy(struct circular_byte_buffer* cbyte);
enum result circular_byte_buffer_add(struct circular_byte_buffer* cbyte, char c);
char circular_byte_buffer_pop(struct circular_byte_buffer* cbyte);

#endif
