#ifndef _SOURCE_H
#define _SOURCE_H

#include "alba_api.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "input.h"
#include "zinc/error.h"

/* dynamic-output-none */
ALBA_API void compile_error_init(struct error* e);

/* dynamic-output-none */
ALBA_API void compile_error_list_init(struct error_list* el);

/* dynamic-output-none */
ALBA_API void compile_error_list_add(struct error_list* el, struct error* e);

/* dynamic-destroy el{} */
ALBA_API void compile_error_list_destroy(struct error_list* el);

/* dynamic-output el{} */
ALBA_API bool set_source_error(struct error_list* el, struct location* loc, const char* fmt, ...);

/* dynamic-output bf{} */
/* resource-input d */
/* resource-use d */
ALBA_API enum result format_error(struct error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf);

ALBA_API const char* plural(int number);

ALBA_API void location_init(struct location* loc);

ALBA_API void location_create(struct location** loc);

#endif
