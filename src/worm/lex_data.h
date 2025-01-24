#ifndef WORM_LEX_DATA_H
#define WORM_LEX_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include "zinc/hash_map_string.h"
#include "token.h"

typedef struct {
    void* input;
    Zinc_input_unicode_vtable* vtable;
    Zinc_error_list* errors;
    Zinc_hash_map_string reserved;
} Worm_lex_data;

void Worm_lex_data_init(
    Worm_lex_data* ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors);
void Worm_lex_data_create(
    Worm_lex_data** ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors);

void Worm_lex_data_init(
    Worm_lex_data* ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors);
void Worm_lex_data_create(
    Worm_lex_data** ld,
    void* input,
    Zinc_input_unicode_vtable* vtable,
    Zinc_error_list* errors);
void Worm_lex_data_add_reserved(Worm_lex_data* ld, char* word, Worm_token_type type);
Worm_token_type* Worm_lex_data_get_reserved(Worm_lex_data* ld, Zinc_string* word);

#endif