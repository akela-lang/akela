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

struct location {
	size_t line;
	size_t col;
	size_t byte_pos;
};

/* dynamic-output-none */
void compile_error_init(struct compile_error* e);

/* dynamic-output-none */
void compile_error_list_init(struct compile_error_list* el);

/* dynamic-output-none */
void compile_error_list_add(struct compile_error_list* el, struct compile_error* e);

/* dynamic-destroy el{} */
void compile_error_list_destroy(struct compile_error_list* el);

/* dynamic-output el{} */
void set_source_error(struct compile_error_list* el, struct location* loc, const char* fmt, ...);

/* dynamic-output bf{} */
/* resource-input d */
/* resource-use d */
enum result format_error(struct compile_error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf);

/* dynamic-output-none */
void get_token_location(struct token* t, struct location* loc);

#endif
