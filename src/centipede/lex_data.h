#ifndef CENTIPEDE_LEX_DATA_H
#define CENTIPEDE_LEX_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include "zinc/hash_map_string.h"
#include "token.h"

typedef struct Cent_lex_data {
    struct Zinc_error_list* errors;
    void* input;
    Zinc_input_unicode_vtable* input_vtable;
    struct Zinc_hash_table reserved;
    struct Zinc_hash_table builtin;
    size_t process_newline_count;
} Cent_lex_data;

void Cent_lex_data_init(
    Cent_lex_data *ld,
    struct Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* input_vtable);
void Cent_lex_data_create(
    Cent_lex_data **ld,
    struct Zinc_error_list* errors,
    void* input,
    Zinc_input_unicode_vtable* input_vtable);
void Cent_lex_data_destroy();

void Cent_lex_add_reserved_word(Cent_lex_data* ld, char* word, Cent_token_type type);
Cent_token_type* Cent_lex_get_reserved_word(Cent_lex_data* ld, struct Zinc_string* word);

void Cent_lex_add_builtin(Cent_lex_data* ld, char* name, Cent_builtin_type type);
Cent_builtin_type* Cent_lex_get_builtin(Cent_lex_data* ld, struct Zinc_string* name);

#endif