#ifndef _SOURCE_H
#define _SOURCE_H

#include "akela_api.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include "zinc/result.h"
#include "input.h"
#include "zinc/error.h"

#define set_source_error error_list_set

AKELA_API enum result format_error(struct error* e, input_getchar f, input_seek seek, input_data d, struct buffer* bf);

#endif
