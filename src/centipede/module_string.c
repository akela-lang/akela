#include "module_string.h"
#include <stddef.h>
#include "zinc/input_unicode_string.h"
#include "zinc/memory.h"

Cent_module_vtable Cent_module_string_vtable = {
    .find_offset = offsetof(Cent_module_string, find),
};

void Cent_module_string_init(Cent_module_string* ms)
{
    hash_table_init(&ms->ht, 16);
    ms->find = (Cent_module_find)Cent_module_find_string;
    ms->vtable = &Cent_module_string_vtable;
}

void Cent_module_string_create(Cent_module_string** ms)
{
    malloc_safe((void**)ms, sizeof(Cent_module_string));
    Cent_module_string_init(*ms);
}

void Cent_module_string_destroy(Cent_module_string* ms)
{
    hash_table_map(&ms->ht, (hash_table_func)buffer_free);
    hash_table_destroy(&ms->ht);
}

void Cent_module_string_add_module(Cent_module_string* ms, struct buffer* name, struct buffer* text)
{
    struct buffer* text2 = NULL;
    buffer_create(&text2);
    buffer_copy(text, text2);
    hash_table_add(&ms->ht, name, text2);
}

void Cent_module_string_add_module_str(Cent_module_string* ms, char* name, struct buffer* text)
{
    struct buffer* text2 = NULL;
    buffer_create(&text2);
    buffer_copy(text, text2);
    hash_table_add_str(&ms->ht, name, text2);
}

void Cent_module_string_add_module_str_str(Cent_module_string* ms, char* name, char* text)
{
    struct buffer* text2 = NULL;
    buffer_create(&text2);
    buffer_copy_str(text2, text);
    hash_table_add_str(&ms->ht, name, text2);
}

Cent_comp_unit* Cent_module_find_string(Cent_module_string* ms, struct buffer* name)
{
    struct buffer* text = hash_table_get(&ms->ht, name);

    if (!text) {
        return NULL;
    }

    Cent_comp_unit* cu = NULL;
    Cent_comp_unit_create(&cu);
    buffer_copy(name, &cu->name);

    struct error_list* errors = NULL;
    error_list_create(&errors);

    Vector* v = NULL;
    VectorCreate(&v, sizeof(char));
    VectorAdd(v, text->buf, text->size);

    InputUnicodeString* input = NULL;
    InputUnicodeStringCreate(&input, v);

    cu->input = input;
    cu->input_vtable = input->input_vtable;
    cu->errors = errors;

    return cu;
}