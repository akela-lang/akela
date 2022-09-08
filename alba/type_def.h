#ifndef _TYPE_DEF_H
#define _TYPE_DEF_H

#include "alba_api.h"
#include <stdbool.h>
#include "zinc/buffer.h"
#include "zinc/result.h"
#include "token.h"
#include "source.h"
#include "symbol_table.h"

enum type {
	type_none,
	type_integer,
	type_float,
	type_array,
	type_struct,
	type_boolean,
	type_string,
	type_function,
	type_function_input,
	type_function_output,
	type_module,
};

struct type_def {
	enum type type;
	struct buffer name;
	bool is_signed;
	int bit_count;
	bool is_generic;
	int generic_count;
};

/* dynamic-output n */
ALBA_API void type_def_create(struct type_def** n);

/* dynamic-destroy n n{} */
ALBA_API void type_def_destroy(struct type_def* n);

/* dynamic-output-none */
ALBA_API void type_def_init(struct type_def* n);

ALBA_API struct type_def* type_def_copy(struct type_def* n);

ALBA_API bool type_def_match(struct type_def* a, struct type_def* b);

#endif
