#include "lex_data.h"
#include "zinc/memory.h"
#include "token.h"

void Cent_lex_data_init(
    Cent_lex_data *ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable)
{
    ld->errors = errors;
    ld->input = input;
    ld->input_vtable = input_vtable;

    hash_table_init(&ld->reserved, 16);
    Cent_lex_add_reserved_word(ld, "element", Cent_token_element);
    Cent_lex_add_reserved_word(ld, "properties", Cent_token_properties);
    Cent_lex_add_reserved_word(ld, "children", Cent_token_children);
    Cent_lex_add_reserved_word(ld, "end", Cent_token_end);
    Cent_lex_add_reserved_word(ld, "enum", Cent_token_enum);
    Cent_lex_add_reserved_word(ld, "true", Cent_token_true);
    Cent_lex_add_reserved_word(ld, "false", Cent_token_false);

    hash_table_init(&ld->builtin, 8);
    Cent_lex_add_builtin(ld, "@child_of", Cent_builtin_type_child_of);
    Cent_lex_add_builtin(ld, "@property_of", Cent_builtin_type_property_of);
    Cent_lex_add_builtin(ld, "@file_name", Cent_builtin_type_file_name);
}

void Cent_lex_data_create(
    Cent_lex_data **ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable)
{
    malloc_safe((void**)ld, sizeof(Cent_lex_data));
    Cent_lex_data_init(*ld, errors, input, input_vtable);
}

void Cent_lex_data_destroy(Cent_lex_data* ld)
{
    hash_table_map(&ld->reserved, free);
    hash_table_destroy(&ld->reserved);
}

void Cent_lex_add_reserved_word(Cent_lex_data* ld, char* word, Cent_token_type type)
{
    Cent_token_type* p = NULL;
    malloc_safe((void**)&p, sizeof(Cent_token_type));
    *p = type;
    hash_table_add_str(&ld->reserved, word, p);
}

Cent_token_type* Cent_lex_get_reserved_word(Cent_lex_data* ld, struct buffer* word)
{
    return hash_table_get(&ld->reserved, word);
}

void Cent_lex_add_builtin(Cent_lex_data* ld, char* name, Cent_builtin_type type)
{
    Cent_builtin_type* bt = NULL;
    malloc_safe((void**)&bt, sizeof(Cent_builtin_type));
    *bt = type;
    hash_table_add_str(&ld->builtin, name, bt);
}

Cent_builtin_type* Cent_lex_get_builtin(Cent_lex_data* ld, struct buffer* name)
{
    return hash_table_get(&ld->builtin, name);
}