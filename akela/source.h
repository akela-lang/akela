#ifndef _SOURCE_H
#define _SOURCE_H

#include "akela_api.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "input.h"
#include "zinc/error.h"

/* dynamic-output-none */
AKELA_API void compile_error_init(struct error* e);

/* dynamic-output-none */
AKELA_API void compile_error_list_init(struct error_list* el);

/* dynamic-output-none */
AKELA_API void compile_error_list_add(struct error_list* el, struct error* e);

/* dynamic-destroy el{} */
AKELA_API void compile_error_list_destroy(struct error_list* el);

/* dynamic-output el{} */
AKELA_API bool set_source_error(struct error_list* el, struct location* loc, const char* fmt, ...);

/* dynamic-output bf{} */
/* resource-input d */
/* resource-use d */
AKELA_API enum result format_error(struct error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf);

AKELA_API const char* plural(int number);

AKELA_API void location_create(struct location** loc);

#endif
