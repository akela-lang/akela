#include "comp_unit.h"
#include "zinc/memory.h"
#include "lex.h"
#include "parse_tools.h"
#include "parse.h"
#include <assert.h>
#include <string.h>
#include "zinc/input_unicode_string.h"

void Ake_comp_unit_init(Ake_comp_unit* cu)
{
	cu->valid = true;
	cu->root = NULL;
	Zinc_error_list_init(&cu->errors);
    Zinc_string_list_init(&cu->extern_list);
	Zinc_string_init(&cu->path);
	Ake_symbol_table_init(&cu->st);
	cu->ps = NULL;
	cu->ls = NULL;
    cu->input_obj = NULL;
	cu->next = NULL;
	cu->prev = NULL;
}

void Ake_comp_unit_create(Ake_comp_unit** cu)
{
    Zinc_malloc_safe((void**)cu, sizeof(Ake_comp_unit));
    Ake_comp_unit_init(*cu);
}

void Ake_comp_unit_destroy(Ake_comp_unit* cu)
{
    if (cu) {
        Ake_ast_destroy(cu->root);
        Zinc_error_list_destroy(&cu->errors);
        Zinc_string_destroy(&cu->path);
        Ake_symbol_table_destroy(&cu->st);
        Zinc_string_list_destroy(&cu->extern_list);
    	if (cu->ps) {
    		free(cu->ps);
    	}
    	if (cu->ls) {
    		Zinc_input_unicode_string_destroy(cu->ls->input);
    		free(cu->ls->input);
    		free(cu->ls);
    	}
    }
}

void Ake_comp_unit_setup(Ake_comp_unit* cu, void* input, Zinc_input_unicode_vtable* vtable, Ake_parse_state** ps)
{
	*ps = NULL;

    Ake_ast_destroy(cu->root);
	cu->root = NULL;

	struct Ake_lex_state* ls = NULL;
	Zinc_malloc_safe((void**)&ls, sizeof(struct Ake_lex_state));
    Ake_lex_state_init(ls, input, vtable, &cu->errors, &cu->st);

	Zinc_malloc_safe((void**)ps, sizeof(struct Ake_parse_state));
	Ake_parse_state_init(*ps, ls, &cu->errors, &cu->extern_list, &cu->st);
}

void Ake_comp_unit_teardown(Ake_comp_unit* cu, Ake_parse_state* ps)
{
	assert(ps);
	assert(ps->ls);

	free(ps->ls);

	Ake_parse_state_destroy(ps);
	free(ps);
}

bool Ake_comp_unit_compile(Ake_comp_unit* cu, void* input_obj, Zinc_input_unicode_vtable* input_vtable)
{
	bool valid = true;
	Ake_parse_state* ps = NULL;
    cu->input_obj = input_obj;
	Ake_comp_unit_setup(cu, input_obj, input_vtable, &ps);
	Ake_parse_result pr = Ake_parse(ps);
    cu->root = pr.root;
    if (cu->root->type == Ake_ast_type_error || cu->errors.head) {
        valid = false;
    }

	Ake_comp_unit_teardown(cu, ps);
	cu->valid = valid;
	return valid;
}

Ake_parse_result Ake_comp_unit_parse(Ake_comp_unit* cu)
{
	Ake_parse_result pr = Ake_parse(cu->ps);
	cu->root = pr.root;
	return pr;
}