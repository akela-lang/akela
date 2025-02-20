#include "module_finder_string.h"
#include <stddef.h>
#include "zinc/input_unicode_string.h"
#include "zinc/memory.h"

Cent_module_finder_vtable Cent_module_finder_string_vtable = {
    .find_offset = offsetof(Cent_module_finder_string, find),
    .destroy_offset = offsetof(Cent_module_finder_string, destroy),
};

void Cent_module_finder_string_init(Cent_module_finder_string* ms)
{
    Zinc_hash_map_string_init(&ms->ht, 16);
    ms->find = (Cent_module_finder_find_interface)Cent_module_finder_string_find;
    ms->destroy = (Cent_module_finder_destroy_interface)Cent_module_finder_string_destroy;
    ms->vtable = &Cent_module_finder_string_vtable;
}

void Cent_module_finder_string_create(Cent_module_finder_string** ms)
{
    Zinc_malloc_safe((void**)ms, sizeof(Cent_module_finder_string));
    Cent_module_finder_string_init(*ms);
}

void Cent_module_finder_string_destroy(Cent_module_finder_string* ms)
{
    Zinc_hash_map_string_map(&ms->ht, (Zinc_hash_map_string_func)Zinc_string_free);
    Zinc_hash_map_string_destroy(&ms->ht);
}

void Cent_module_finder_string_add_module(
    Cent_module_finder_string* ms,
    Zinc_string* name,
    Zinc_string* text)
{
    Zinc_string* text2 = NULL;
    Zinc_string_create(&text2);
    Zinc_string_copy(text, text2);
    Zinc_hash_map_string_add(&ms->ht, name, text2);
}

void Cent_module_finder_string_add_module_str(
    Cent_module_finder_string* ms, char* name,
    Zinc_string* text)
{
    Zinc_string* text2 = NULL;
    Zinc_string_create(&text2);
    Zinc_string_copy(text, text2);
    Zinc_hash_map_string_add_str(&ms->ht, name, text2);
}

void Cent_module_finder_string_add_module_str_str(
    Cent_module_finder_string* ms,
    char* name,
    char* text)
{
    Zinc_string* text2 = NULL;
    Zinc_string_create(&text2);
    Zinc_string_add_str(text2, text);
    Zinc_hash_map_string_add_str(&ms->ht, name, text2);
}

Cent_input_data Cent_module_finder_string_find(
    Cent_module_finder_string* ms,
    Zinc_string* name)
{
    Zinc_string* text = Zinc_hash_map_string_get(&ms->ht, name);
    Cent_input_data data = {NULL, NULL};

    if (!text) {
        return data;
    }

    Zinc_vector* v = NULL;
    Zinc_vector_create(&v, sizeof(char));
    Zinc_vector_add(v, text->buf, text->size);

    Zinc_input_unicode_string* input = NULL;
    Zinc_input_unicode_string_create(&input, v);

    data.input = input;
    data.input_vtable = input->input_vtable;

    return data;
}