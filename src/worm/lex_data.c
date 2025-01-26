#include "lex_data.h"
#include "zinc/memory.h"
#include "token.h"

void Worm_lex_data_init(
    Worm_lex_data* ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors)
{
    ld->input = input;
    ld->vtable = vtable;
    ld->errors = errors;
    Zinc_hash_map_string_init(&ld->reserved, 32);

    Worm_lex_data_add_reserved(ld, "true", Worm_token_type_true);
    Worm_lex_data_add_reserved(ld, "false", Worm_token_type_false);
}

void Worm_lex_data_create(
    Worm_lex_data** ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors)
{
    Zinc_malloc_safe((void**)ld, sizeof(Worm_lex_data));
    Worm_lex_data_init(*ld, input, vtable, errors);
}

void Worm_lex_data_add_reserved(Worm_lex_data* ld, char* word, Worm_token_type type)
{
    Worm_token_type* type2 = NULL;
    Zinc_malloc_safe((void**)&type2, sizeof(Worm_token_type));
    *type2 = type;
    Zinc_hash_map_string_add_str(&ld->reserved, word, type2);
}

Worm_token_type* Worm_lex_data_get_reserved(Worm_lex_data* ld, Zinc_string* word)
{
    return Zinc_hash_map_string_get(&ld->reserved, word);
}

void Worm_lex_data_destroy(Worm_lex_data* ld)
{
    Zinc_hash_map_string_map(&ld->reserved, free);
    Zinc_hash_map_string_destroy(&ld->reserved);
}
