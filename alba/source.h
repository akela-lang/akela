#ifndef _SOURCE_H
#define _SOURCE_H

#include <stdarg.h>
#include <stdlib.h>
#include "zinc/result.h"
#include "input.h"
#include "token.h"

#define COMPILE_ERROR_MESSAGE_SIZE 100

struct compile_error {
	char message[COMPILE_ERROR_MESSAGE_SIZE];
	size_t line;
	size_t col;
	size_t byte_pos;
	struct compile_error* next;
	struct compile_error* prev;
};

struct compile_error_list {
	struct compile_error* head;
	struct compile_error* tail;
};

/* static-output */
void compile_error_init(struct compile_error* e);

/* static-output */
void compile_error_list_init(struct compile_error_list* el);

/* static-output */
void compile_error_list_add(struct compile_error_list* el, struct compile_error* e);

/* dynamic-destroy el{} */
void compile_error_list_destroy(struct compile_error_list* el);

/* dynamic-output el{} */
enum result set_source_error(struct compile_error_list* el, struct token* t, struct lookahead_char* lc, const char* fmt, ...);

/* dynamic-output bf{} */
/* resource-input d */
/* resource-use d */
enum result format_error(struct compile_error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf);

#endif
