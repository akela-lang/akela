#ifndef AKELA_COMP_UNIT_H
#define AKELA_COMP_UNIT_H

#include "api.h"
#include "ast.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "zinc/input_unicode.h"
#include "zinc/string_list.h"

typedef struct Ake_comp_unit {
	bool valid;
	Ake_ast* root;
	struct Zinc_error_list el;
    struct Zinc_string_list extern_list;
	struct Ake_symbol_table st;
    void* input_obj;
	struct Zinc_string path;
	struct Ake_comp_unit* next;
	struct Ake_comp_unit* prev;
} Ake_comp_unit;

AKELA_API void Ake_comp_unit_init(struct Ake_comp_unit* cu);
AKELA_API void Ake_comp_unit_create(struct Ake_comp_unit** cu);
AKELA_API void Ake_comp_unit_destroy(struct Ake_comp_unit* cu);
AKELA_API bool Ake_comp_unit_compile(struct Ake_comp_unit* cu, void* input_obj, InputUnicodeVTable* input_vtable);

#endif
