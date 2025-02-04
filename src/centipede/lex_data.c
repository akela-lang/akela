#include "lex_data.h"
#include "zinc/memory.h"
#include "token.h"

void Cent_lex_data_init(
    Cent_lex_data *ld,
    struct Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* input_vtable)
{
    ld->errors = errors;
    ld->input = input;
    ld->input_vtable = input_vtable;

    Zinc_hash_map_string_init(&ld->reserved, 16);
    Cent_lex_add_reserved_word(ld, "element", Cent_token_element);
    Cent_lex_add_reserved_word(ld, "properties", Cent_token_properties);
    Cent_lex_add_reserved_word(ld, "children", Cent_token_children);
    Cent_lex_add_reserved_word(ld, "end", Cent_token_end);
    Cent_lex_add_reserved_word(ld, "enum", Cent_token_enum);
    Cent_lex_add_reserved_word(ld, "true", Cent_token_true);
    Cent_lex_add_reserved_word(ld, "false", Cent_token_false);
    Cent_lex_add_reserved_word(ld, "use", Cent_token_use);
    Cent_lex_add_reserved_word(ld, "const", Cent_token_const);

    Zinc_hash_map_string_init(&ld->builtin, 8);
    Cent_lex_add_builtin(ld, "@child_of", Cent_builtin_type_child_of);
    Cent_lex_add_builtin(ld, "@property_of", Cent_builtin_type_property_of);
    Cent_lex_add_builtin(ld, "@file_name", Cent_builtin_type_file_name);
}

void Cent_lex_data_create(
    Cent_lex_data **ld,
    struct Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* input_vtable)
{
    Zinc_malloc_safe((void**)ld, sizeof(Cent_lex_data));
    Cent_lex_data_init(*ld, errors, input, input_vtable);
}

void Cent_lex_data_destroy(Cent_lex_data* ld)
{
    Zinc_hash_map_string_map(&ld->reserved, free);
    Zinc_hash_map_string_destroy(&ld->reserved);

    Zinc_hash_map_string_map(&ld->builtin, free);
    Zinc_hash_map_string_destroy(&ld->builtin);
}

void Cent_lex_add_reserved_word(Cent_lex_data* ld, char* word, Cent_token_type type)
{
    Cent_token_type* p = NULL;
    Zinc_malloc_safe((void**)&p, sizeof(Cent_token_type));
    *p = type;
    Zinc_hash_map_string_add_str(&ld->reserved, word, p);
}

Cent_token_type* Cent_lex_get_reserved_word(Cent_lex_data* ld, struct Zinc_string* word)
{
    return Zinc_hash_map_string_get(&ld->reserved, word);
}

void Cent_lex_add_builtin(Cent_lex_data* ld, char* name, Cent_builtin_type type)
{
    Cent_builtin_type* bt = NULL;
    Zinc_malloc_safe((void**)&bt, sizeof(Cent_builtin_type));
    *bt = type;
    Zinc_hash_map_string_add_str(&ld->builtin, name, bt);
}

Cent_builtin_type* Cent_lex_get_builtin(Cent_lex_data* ld, struct Zinc_string* name)
{
    return Zinc_hash_map_string_get(&ld->builtin, name);
}
