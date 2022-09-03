#ifndef _SOURCE_H
#define _SOURCE_H

#include "alba_api.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "input.h"

#define COMPILE_ERROR_MESSAGE_SIZE 100

struct compile_error {
	char message[COMPILE_ERROR_MESSAGE_SIZE];
	size_t line;
	size_t col;
	size_t byte_pos;
	size_t byte_count;
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
	size_t byte_count;
};

/* dynamic-output-none */
ALBA_API void compile_error_init(struct compile_error* e);

/* dynamic-output-none */
ALBA_API void compile_error_list_init(struct compile_error_list* el);

/* dynamic-output-none */
ALBA_API void compile_error_list_add(struct compile_error_list* el, struct compile_error* e);

/* dynamic-destroy el{} */
ALBA_API void compile_error_list_destroy(struct compile_error_list* el);

/* dynamic-output el{} */
ALBA_API bool set_source_error(struct compile_error_list* el, struct location* loc, const char* fmt, ...);

/* dynamic-output bf{} */
/* resource-input d */
/* resource-use d */
ALBA_API enum result format_error(struct compile_error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf);

ALBA_API char* plural(int number);

ALBA_API void location_init(struct location* loc);

ALBA_API void location_create(struct location** loc);

#endif
