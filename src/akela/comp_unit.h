#ifndef AKELA_COMP_UNIT_H
#define AKELA_COMP_UNIT_H

#include "api.h"
#include "ast.h"
#include "symbol_table.h"
#include <stdbool.h>
#include "zinc/input_unicode.h"
#include "zinc/string_list.h"
#include "parse_tools.h"
#include "parse_data.h"

typedef struct Ake_comp_unit {
	bool valid;
	Ake_ast* root;
	Zinc_error_list errors;
    Zinc_string_list extern_list;
	Ake_symbol_table st;
    void* input_obj;
	Zinc_string path;
	Ake_lex_state* ls;
	Ake_parse_state* ps;
	struct Ake_comp_unit* next;
	struct Ake_comp_unit* prev;
} Ake_comp_unit;

AKELA_API void Ake_comp_unit_init(Ake_comp_unit* cu);
AKELA_API void Ake_comp_unit_create(Ake_comp_unit** cu);
AKELA_API void Ake_comp_unit_destroy(Ake_comp_unit* cu);
AKELA_API bool Ake_comp_unit_compile(Ake_comp_unit* cu, void* input_obj, Zinc_input_unicode_vtable* input_vtable);
Ake_parse_result Ake_comp_unit_parse(Ake_comp_unit* cu);

#endif
