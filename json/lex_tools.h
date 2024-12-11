#ifndef JSON_LEX_TOOLS_H
#define JSON_LEX_TOOLS_H

#include "zinc/error.h"
#include "zinc/input_unicode.h"

typedef enum Json_lex_state {
    Json_lex_state_start,
    Json_lex_state_string,
} Json_lex_state;

typedef struct Json_lex_data {
    struct error_list* el;
    void* input_obj;
    InputUnicodeVTable* input_vtable;
} Json_lex_data;

void Json_lex_data_init(
    Json_lex_data* jld,
    struct error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable);
void Json_lex_data_create(
    Json_lex_data** jld,
    struct error_list* el,
    void* input_obj,
    InputUnicodeVTable* input_vtable);
bool Json_is_hex_digit(char c[4], int num);
int code_to_utf8(unsigned char *const buffer, const unsigned int code);
unsigned int char_to_hex(char c);
unsigned int char4_to_hex(char* src, int num);

#endif