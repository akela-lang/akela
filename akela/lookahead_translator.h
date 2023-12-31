#ifndef _LOOKAHEAD_TRANSLATOR_H
#define _LOOKAHEAD_TRANSLATOR_H

#include "akela_api.h"
#include <stdlib.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/ustring.h>
#include "zinc/result.h"

struct circular_byte_buffer {
	char* buf;
	size_t buf_size;
	size_t start;
	size_t size;
};

struct circular_uchar32_buffer {
	UChar32* buf;
	size_t buf_size;
	size_t start;
	size_t size;
};

/* dynamic-output cbyte{} */
AKELA_API void circular_byte_buffer_init(struct circular_byte_buffer* cbyte, size_t buf_size);

/* dynamic-destroy cbyte{} */
AKELA_API void circular_byte_buffer_destroy(struct circular_byte_buffer* cbyte);

/* dynamic-output-none */
AKELA_API enum result circular_byte_buffer_add(struct circular_byte_buffer* cbyte, char c);

/* dynamic-output-none */
AKELA_API char circular_byte_buffer_pop(struct circular_byte_buffer* cbyte);

/* dynamic-output: cc32{} */
AKELA_API void circular_uchar32_buffer_init(struct circular_uchar32_buffer* cc32, size_t buf_size);

/* dynamic-destroy cc32{} */
AKELA_API void circular_uchar32_buffer_destroy(struct circular_uchar32_buffer* cc32);

/* dynamic-output-none */
AKELA_API enum result circular_uchar32_buffer_add(struct circular_uchar32_buffer* cc32, UChar32 c);

/* dynamic-output-none */
AKELA_API UChar32 circular_uchar32_buffer_pop(struct circular_uchar32_buffer* cc32);

#endif
