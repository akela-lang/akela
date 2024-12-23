#ifndef CENTIPEDE_LEX_DATA_H
#define CENTIPEDE_LEX_DATA_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"
#include "zinc/hash.h"
#include "token.h"

typedef struct Cent_lex_data {
    struct error_list* errors;
    void* input;
    InputUnicodeVTable* input_vtable;
    struct hash_table reserved;
} Cent_lex_data;

void Cent_lex_data_init(
    Cent_lex_data *ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable);
void Cent_lex_data_create(
    Cent_lex_data **ld,
    struct error_list* errors,
    void* input,
    InputUnicodeVTable* input_vtable);
void Cent_lex_data_destroy();
void Cent_lex_add_reserved_word(Cent_lex_data* ld, char* word, Cent_token_type type);
Cent_token_type* Cent_lex_get_reserved_word(Cent_lex_data* ld, struct buffer* word);

#endif