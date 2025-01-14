#include "comp_unit.h"
#include "zinc/memory.h"
#include "lex.h"
#include "parse_tools.h"
#include "parse.h"
#include <assert.h>

void Ake_comp_unit_init(struct Ake_comp_unit* cu)
{
	cu->valid = true;
	cu->root = NULL;
	error_list_init(&cu->el);
    buffer_list_init(&cu->extern_list);
	Zinc_string_init(&cu->path);
	Ake_symbol_table_init(&cu->st);
    cu->input_obj = NULL;
	cu->next = NULL;
	cu->prev = NULL;
}

void Ake_comp_unit_create(struct Ake_comp_unit** cu)
{
    malloc_safe((void**)cu, sizeof(struct Ake_comp_unit));
    Ake_comp_unit_init(*cu);
}

void Ake_comp_unit_destroy(struct Ake_comp_unit* cu)
{
    if (cu) {
        Ake_ast_destroy(cu->root);
        error_list_destroy(&cu->el);
        Zinc_string_destroy(&cu->path);
        Ake_symbol_table_destroy(&cu->st);
        buffer_list_destroy(&cu->extern_list);
    }
}

void Ake_comp_unit_setup(struct Ake_comp_unit* cu, void* input_obj, InputUnicodeVTable* input_vtable, struct Ake_parse_state** ps)
{
	*ps = NULL;

    Ake_ast_destroy(cu->root);
	cu->root = NULL;

	struct Ake_lex_state* ls = NULL;
	malloc_safe((void**)&ls, sizeof(struct Ake_lex_state));
    Ake_lex_state_init(ls, input_obj, input_vtable, &cu->el, &cu->st);

	malloc_safe((void**)ps, sizeof(struct Ake_parse_state));
	Ake_parse_state_init(*ps, ls, &cu->el, &cu->extern_list, &cu->st);
}

void Ake_comp_unit_teardown(struct Ake_comp_unit* cu, struct Ake_parse_state* ps)
{
	assert(ps);
	assert(ps->ls);

	free(ps->ls);

	Ake_parse_state_destroy(ps);
	free(ps);
}

bool Ake_comp_unit_compile(struct Ake_comp_unit* cu, void* input_obj, InputUnicodeVTable* input_vtable)
{
	bool valid = true;
	struct Ake_parse_state* ps = NULL;
    cu->input_obj = input_obj;
	Ake_comp_unit_setup(cu, input_obj, input_vtable, &ps);
    cu->root = Ast_parse(ps);
    if (cu->root->type == Ake_ast_type_error || cu->el.head) {
        valid = false;
    }

	Ake_comp_unit_teardown(cu, ps);
	cu->valid = valid;
	return valid;
}
